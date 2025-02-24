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
// DATE        : 250221
//===================================================================
void CGameMain::GameInit()
{
	int iDataCount = BLOCK_COUNT*BLOCK_COUNT - 1;
	// 初始化随机数据
	int iRandData[BLOCK_COUNT*BLOCK_COUNT-1];
	for (int i = 0; i < BLOCK_COUNT*BLOCK_COUNT-1; ++i) {
		iRandData[i] = i + 1;
	}

	// 遍历状态矩阵并随机打乱拼图
	for (int y = 0; y < BLOCK_COUNT; ++y) {
		for (int x = 0; x < BLOCK_COUNT; ++x) {
			// 获取一维索引
			int iIdx = XYToOneIndex(x, y);
			// 配置空白方块
			if (x == BLOCK_COUNT - 1 && y == BLOCK_COUNT - 1) {
				m_iBlockState[y][x] = 0;
				m_spBlock[iIdx] = new CSprite("NULL");
			} else {
				int randIdx = CSystem::RandomRange(0, iDataCount - 1);  // 随机选择编号
				m_iBlockState[y][x] = iRandData[randIdx];               // 绑定随机编号
				char *szSpName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[y][x]);
				m_spBlock[iIdx] = new CSprite(szSpName);                // 绑定对应拼图
				MoveSpriteToBlock(m_spBlock[iIdx], x, y);               // 重新渲染拼图
				// 移动剩余数据，防止重复赋值
				for (int i = randIdx; i < iDataCount - 1; ++i)
					iRandData[i] = iRandData[i + 1];
				iDataCount--;
			}
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