/////////////////////////////////////////////////////////////////////////////////
//
// LessonX.cpp - 游戏核心逻辑实现文件
//
/////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "CommonClass.h"
#include "LessonX.h"

//=================================================================================
//
// 全局游戏管理对象
//
//=================================================================================
CGameMain g_GameMain;

//=================================================================================
//
// 常量定义 - 计算方块的起始位置和大小
//
//=================================================================================
const float CGameMain::m_fBlockSize = 18.75f;
const float CGameMain::m_fBlockStartX = -40.625f;
const float CGameMain::m_fBlockStartY = -28.125f;

//=================================================================================
//
// 构造函数 - 初始化游戏状态和精灵
//
//=================================================================================
CGameMain::CGameMain()
{
    m_iGameState = 0;
    m_spGameBegin = new CSprite("GameBegin");
}

//=================================================================================
//
// 析构函数
//
//=================================================================================
CGameMain::~CGameMain()
{
}

//=================================================================================
//
// 游戏主循环
//  - 处理游戏初始化、运行和结束状态
//
//=================================================================================
void CGameMain::GameMainLoop(float fDeltaTime)
{
    switch (GetGameState())
    {
        case 1:
            GameInit();
            SetGameState(2);
            break;
        case 2:
            if (!IsGameWin())
            {
                GameRun(fDeltaTime);
            }
            else
            {
                SetGameState(0);
                GameEnd();
            }
            break;
        default:
            break;
    }
}

//=================================================================================
//
// 游戏初始化
//  - 生成并打乱拼图方块
//
//=================================================================================
void CGameMain::GameInit()
{
    int iDataCount = BLOCK_COUNT * BLOCK_COUNT - 1;
    int iRandData[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    for (int y = 0; y < BLOCK_COUNT; y++)
    {
        for (int x = 0; x < BLOCK_COUNT; x++)
        {
            int index = XYToOneIndex(x, y);
            if (x == BLOCK_COUNT - 1 && y == BLOCK_COUNT - 1)
            {
                m_iBlockState[y][x] = 0;
                m_spBlock[index] = new CSprite("NULL");
            }
            else
            {
                int randIdx = CSystem::RandomRange(0, iDataCount - 1);
                m_iBlockState[y][x] = iRandData[randIdx];
                char* spriteName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[y][x]);
                m_spBlock[index] = new CSprite(spriteName);
                MoveSpriteToBlock(m_spBlock[index], x, y);
                
                for (int i = randIdx; i < iDataCount - 1; i++)
                    iRandData[i] = iRandData[i + 1];
                iDataCount--;
            }
        }
    }
}

//=================================================================================
//
// 运行游戏
//
//=================================================================================
void CGameMain::GameRun(float fDeltaTime)
{
}

//=================================================================================
//
// 结束游戏
//
//=================================================================================
void CGameMain::GameEnd()
{
    m_spGameBegin->SetSpriteVisible(1);
}

//=================================================================================
//
// 处理键盘按键
//
//=================================================================================
void CGameMain::OnKeyDown(int iKey, bool iAltPress, bool iShiftPress, bool iCtrlPress)
{
    if (iKey == KEY_SPACE && m_iGameState == 0)
    {
        m_iGameState = 1;
        m_spGameBegin->SetSpriteVisible(false);
    }
}

//=================================================================================
//
// 索引转换
//
//=================================================================================
int CGameMain::XYToOneIndex(int iIndexX, int iIndexY)
{
    return (iIndexY * BLOCK_COUNT + iIndexX);
}

int CGameMain::OneIndexToX(int iIndex)
{
    return iIndex % BLOCK_COUNT;
}

int CGameMain::OneIndexToY(int iIndex)
{
    return iIndex / BLOCK_COUNT;
}

//=================================================================================
//
// 移动方块到指定位置
//
//=================================================================================
void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, int iIndexX, int iIndexY)
{
    float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
    float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
    tmpSprite->SetSpritePosition(fPosX, fPosY);
}

//=================================================================================
//
// 判断游戏是否胜利
//
//=================================================================================
bool CGameMain::IsGameWin()
{
    int expectedValue = 1;
    for (int y = 0; y < BLOCK_COUNT; y++)
    {
        for (int x = 0; x < BLOCK_COUNT; x++)
        {
            if (x == BLOCK_COUNT - 1 && y == BLOCK_COUNT - 1)
                return (m_iBlockState[y][x] == 0);
            if (m_iBlockState[y][x] != expectedValue++)
                return false;
        }
    }
    return true;
}

//
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // 只处理游戏进行中的鼠标响应
    if (m_iGameState != 2) return;

    int iClickIndex = -1;

  	// 获取鼠标点击的非空白格方块索引
    for (int iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++) {
        if (m_spBlock[iLoop]->GetName() == "NULL") continue;
        if (m_spBlock[iLoop]->IsPointInSprite(fMouseX, fMouseY)) {
            iClickIndex = iLoop;
            break;
        }
    }

    if (iClickIndex == -1) return;

  	// 获取一维索引对应的二维索引
    int iIndexX = OneIndexToX(iClickIndex);
    int iIndexY = OneIndexToY(iClickIndex);
    int iEmptyIndexX = -1, iEmptyIndexY = -1;

    // 判断四个方向是否有空位
    if (iIndexX > 0 && m_iBlockState[iIndexY][iIndexX - 1] == 0) {    // left
        iEmptyIndexX = iIndexX - 1;
        iEmptyIndexY = iIndexY;
    }
    else if (iIndexX < BLOCK_COUNT - 1 && m_iBlockState[iIndexY][iIndexX + 1] == 0) {    // right
        iEmptyIndexX = iIndexX + 1;
        iEmptyIndexY = iIndexY;
    }
    else if (iIndexY > 0 && m_iBlockState[iIndexY - 1][iIndexX] == 0) {    // top
        iEmptyIndexX = iIndexX;
        iEmptyIndexY = iIndexY - 1;
    }
    else if (iIndexY < BLOCK_COUNT - 1 && m_iBlockState[iIndexY + 1][iIndexX] == 0) {    // bottom
        iEmptyIndexX = iIndexX;
        iEmptyIndexY = iIndexY + 1;
    }
		// 四个方向均无空位
    if (iEmptyIndexX == -1 || iEmptyIndexY == -1) return;

    // 更新矩阵状态
    int tempState = m_iBlockState[iIndexY][iIndexX];
    m_iBlockState[iIndexY][iIndexX] = m_iBlockState[iEmptyIndexY][iEmptyIndexX];
    m_iBlockState[iEmptyIndexY][iEmptyIndexX] = tempState;
		// 更新方块索引
    int iOneIndex = XYToOneIndex(iEmptyIndexX, iEmptyIndexY);
    CSprite *tempBlock = m_spBlock[iClickIndex];
    m_spBlock[iClickIndex] = m_spBlock[iOneIndex];
    m_spBlock[iOneIndex] = tempBlock;
    // 移动方块至新位置，重新渲染
    MoveSpriteToBlock(m_spBlock[iOneIndex], iEmptyIndexX, iEmptyIndexY);
}