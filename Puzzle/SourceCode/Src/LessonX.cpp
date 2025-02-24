///////////////////////////////////////////////////////////////////
// NAME:               LessonX.cpp
//
// PURPOSE:            游戏核心逻辑实现文件。
// FUNCTIONS/OBJECTS:  
//
// AUTHOR:             mql
///////////////////////////////////////////////////////////////////

// includes
//------------------------------
#include <stdio.h>
#include <algorithm>
#include "CommonClass.h"
#include "LessonX.h"

// global vars
//------------------------------
CGameMain g_GameMain;	// 游戏管理

// constants
//------------------------------
// 定义拼图的大小和起始坐标
const float CGameMain::m_fBlockSize = 18.75f;
const float CGameMain::m_fBlockStartX = -40.625f;
const float CGameMain::m_fBlockStartY = -28.125f;

//===================================================================
// NAME        : CGameMain
//
// DESCRIPTION : 构造函数。初始化游戏状态和“空格开始”精灵。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
CGameMain::CGameMain()
{
    m_iGameState = 0;
    m_spGameBegin = new CSprite("GameBegin");
}

//===================================================================
// NAME        : ~CGameMain
//
// DESCRIPTION : 析构函数。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
CGameMain::~CGameMain()
{

}

//===================================================================
// NAME        : GameMainLoop
//
// DESCRIPTION : 游戏主循环。处理游戏初始化、运行和结束状态。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::GameMainLoop(float fDeltaTime)
{
    switch (GetGameState()) {
        case 1:
            GameInit();
            SetGameState(2);
            break;
        case 2:
            if (!IsGameWin()) {
                GameRun(fDeltaTime);
            } else {
                SetGameState(0);
                GameEnd();
            }
            break;
        default:
            break;
    }
}

//===================================================================
// NAME        : GameInit
//
// DESCRIPTION : 游戏初始化。生成并打乱拼图方块。
// AUTHOR      : mql
// DATE        : 250224
//===================================================================
void CGameMain::GameInit()
{
    // 生成可解的拼图
    GenerateSolvablePuzzle();
    // 绑定拼图到精灵
    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            int iBlockValue = m_iBlockState[y][x];

            int iIdx = XYToOneIndex(x, y); // 获取一维索引

            if (iBlockValue == 0) {
                m_spBlock[iIdx] = new CSprite("NULL"); // 空白块
            } else {
                char *szSpName = CSystem::MakeSpriteName("PictureBlock", iBlockValue);
                m_spBlock[iIdx] = new CSprite(szSpName);
            }

            // 重新渲染拼图
            MoveSpriteToBlock(m_spBlock[iIdx], x, y);
        }
    }
}

//===================================================================
// NAME        : GameRun
//
// DESCRIPTION : 运行游戏。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::GameRun(float fDeltaTime)
{

}

//===================================================================
// NAME        : GameEnd
//
// DESCRIPTION : 结束游戏。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::GameEnd()
{
	// 显示提示开始的“空格开始”精灵
	m_spGameBegin->SetSpriteVisible(true);
}

//===================================================================
// NAME        : OnKeyDown
//
// DESCRIPTION : 处理键盘按键事件。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnKeyDown(int iKey, bool iAltPress, bool iShiftPress, bool iCtrlPress)
{
	// 按下空格键且游戏未开始
	if (iKey == KEY_SPACE && m_iGameState == 0) {
		SetGameState(1);												// 切换成游戏开始状态
		m_spGameBegin->SetSpriteVisible(false);	// 隐藏"空格开始"精灵
	}  
}

//===================================================================
// NAME        : OnMouseClick
//
// DESCRIPTION : 处理鼠标点击事件。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // 只处理游戏进行中的鼠标点击事件
    if (m_iGameState != 2) return;
    // 初始化鼠标点击的拼图索引
    int iClickIdx = -1;
    // 遍历拼图数组获取鼠标点击的拼图索引
    for (int iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++) {
        if (m_spBlock[iLoop]->GetName() == "NULL") continue;
      	// 使用`IsPointInSprite`判断鼠标是否点击了某块拼图，若是，则`iClickIdx`记录下该索引
        if (m_spBlock[iLoop]->IsPointInSprite(fMouseX, fMouseY)) {
            iClickIdx = iLoop;
            break;
        }
    }
    // 未获取到具体索引
    if (iClickIdx == -1) return;
    // 获取一维索引对应的二维坐标
    int iIdxX = OneIndexToX(iClickIdx);
    int iIdxY = OneIndexToY(iClickIdx);
    // 初始化空白格坐标
    int iEmptyIdxX = -1, iEmptyIdxY = -1;
    // 判断点击的拼图周围是否有相邻的空白格
    if (iIdxX > 0 && m_iBlockState[iIdxY][iIdxX - 1] == 0) {
        // left
        iEmptyIdxX = iIdxX - 1;
        iEmptyIdxY = iIdxY;
    }
    else if (iIdxX < BLOCK_COUNT - 1 && m_iBlockState[iIdxY][iIdxX + 1] == 0) {
        // right
        iEmptyIdxX = iIdxX + 1;
        iEmptyIdxY = iIdxY;
    }
    else if (iIdxY > 0 && m_iBlockState[iIdxY - 1][iIdxX] == 0) {
        // top
        iEmptyIdxX = iIdxX;
        iEmptyIdxY = iIdxY - 1;
    }
    else if (iIdxY < BLOCK_COUNT - 1 && m_iBlockState[iIdxY + 1][iIdxX] == 0) {
        // bottom
        iEmptyIdxX = iIdxX;
        iEmptyIdxY = iIdxY + 1;
    }
    // 无法找到相邻的空白格
    if (iEmptyIdxX == -1 || iEmptyIdxY == -1) return;
    // 更新矩阵状态
    int tempState = m_iBlockState[iIdxY][iIdxX];
    m_iBlockState[iIdxY][iIdxX] = m_iBlockState[iEmptyIdxY][iEmptyIdxX];
    m_iBlockState[iEmptyIdxY][iEmptyIdxX] = tempState;
    // 更新拼图数组
    int iOneIdx = XYToOneIndex(iEmptyIdxX, iEmptyIdxY);
    CSprite *spTmp = m_spBlock[iClickIdx];
    m_spBlock[iClickIdx] = m_spBlock[iOneIdx];
    m_spBlock[iOneIdx] = spTmp;
    // 重新渲染拼图
    MoveSpriteToBlock(m_spBlock[iOneIdx], iEmptyIdxX, iEmptyIdxY);
}

//===================================================================
// NAME        : XYToOneIndex
//
// DESCRIPTION : 索引转换 (二维坐标 -> 一维索引)。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int CGameMain::XYToOneIndex(int iIndexX, int iIndexY)
{
    return (iIndexY * BLOCK_COUNT + iIndexX);
}

//===================================================================
// NAME        : OneIndexToX
//
// DESCRIPTION : 索引转换 (一维索引 -> 二维X坐标)。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int CGameMain::OneIndexToX(int iIndex)
{
    return (iIndex % BLOCK_COUNT);
}

//===================================================================
// NAME        : OneIndexToY
//
// DESCRIPTION : 索引转换 (一维索引 -> 二维Y坐标)。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int CGameMain::OneIndexToY(int iIndex)
{
    return (iIndex / BLOCK_COUNT);
}

//===================================================================
// NAME        : MoveSpriteToBlock
//
// DESCRIPTION : 移动拼图到指定位置。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::MoveSpriteToBlock(CSprite *sp, int iIndexX, int iIndexY)
{
    float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
    float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
    sp->SetSpritePosition(fPosX, fPosY);
}

//===================================================================
// NAME        : IsGameWin
//
// DESCRIPTION : 判断游戏是否获胜。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
bool CGameMain::IsGameWin()
{
    int iSN = 1;	// 拼图索引

    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            // 判断最后一个位置是否是空白格
            if (x == BLOCK_COUNT - 1 && y == BLOCK_COUNT - 1)
                return (m_iBlockState[y][x] == 0);
            // 判断排列，若值不符合预期，则游戏未胜利
            if (m_iBlockState[y][x] != iSN++)
                return false;
        }
    }

    return true;
}

//===================================================================
// NAME        : IsSolvable
//
// DESCRIPTION : 判断当前拼图是否可解。
// AUTHOR      : mql
// DATE        : 250224
//===================================================================
bool CGameMain::IsSolvable(int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT])
{
    int iInversionCount = 0;
    int iFlatPuzzle[BLOCK_COUNT * BLOCK_COUNT]; // 将 4x4 转换成一维数组
    int iIdx = 0;
    int iEmptyRow = 0;

    // 转换拼图为一维数组，并记录空白块所在行号（从上往下数）
    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            iFlatPuzzle[iIdx] = m_iBlockState[y][x];
            if (m_iBlockState[y][x] == 0) {
                iEmptyRow = y; // 记录空白块行号
            }
            iIdx++;
        }
    }

    // 计算逆序数
    for (int i = 0; i < BLOCK_COUNT * BLOCK_COUNT - 1; ++i) {
        for (int j = i + 1; j < BLOCK_COUNT * BLOCK_COUNT; ++j) {
            if (iFlatPuzzle[i] > iFlatPuzzle[j] && iFlatPuzzle[i] != 0 && iFlatPuzzle[j] != 0) {
                iInversionCount++;
            }
        }
    }

    // 计算空白格的行数（从底部往上数）
    int iBlankRowFromBottom = BLOCK_COUNT - iEmptyRow;

    // 15拼图可解性规则
    return (iInversionCount % 2 == 0) == (iBlankRowFromBottom % 2 == 1);
}

//===================================================================
// NAME        : GenerateSolvablePuzzle
//
// DESCRIPTION : 生成一个可解的 15 拼图，并确保空白格在 (3,3)。
// AUTHOR      : mql
// DATE        : 250224
//===================================================================
void CGameMain::GenerateSolvablePuzzle()
{
    int iRandData[BLOCK_COUNT * BLOCK_COUNT - 1];  // 存储1-15的编号
    for (int i = 0; i < BLOCK_COUNT * BLOCK_COUNT - 1; ++i) {
        iRandData[i] = i + 1;
    }

    do {
        // 使用 Fisher-Yates 洗牌算法随机打乱拼图
        for (int i = BLOCK_COUNT * BLOCK_COUNT - 2; i > 0; --i) {
            int j = CSystem::RandomRange(0, i); // 生成 0~i 之间的随机索引
            std::swap(iRandData[i], iRandData[j]);
        }
        // 将打乱的拼图填充到 4x4 矩阵中
        int iIdx = 0;
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int x = 0; x < BLOCK_COUNT; ++x) {
                if (y == BLOCK_COUNT - 1 && x == BLOCK_COUNT - 1) {
                    m_iBlockState[y][x] = 0; // 预留最后一格为空白格
                } else {
                    m_iBlockState[y][x] = iRandData[iIdx++];
                }
            }
        }
    } while (!IsSolvable(m_iBlockState)); // 重新生成直到满足可解性
}