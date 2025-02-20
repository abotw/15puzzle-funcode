/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonClass.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sstream>
//
//
CGameMain		g_GameMain;	
// s2
const float CGameMain::m_fBlockSize = 18.75f;
const float CGameMain::m_fBlockStartX = -40.625f;
const float CGameMain::m_fBlockStartY = -28.125f;

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。

//==============================================================================
//
// 构造函数
CGameMain::CGameMain()
{
	m_iGameState			=	0;
	// s1
	m_spGameBegin = new CSprite("GameBegin");
}
//==============================================================================
//
// 析构函数
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			SetGameState(2); // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if (!IsGameWin()) {
				GameRun( fDeltaTime );
			}
			else // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			{				
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void CGameMain::GameInit()
{
	// s2 初始化方块数据
	int iLoopX = 0, iLoopY = 0, iLoop = 0;
	int iOneIndex = 0, iRandIndex = 0;
	int iDataCount = BLOCK_COUNT * BLOCK_COUNT - 1;
	int iRandData[BLOCK_COUNT * BLOCK_COUNT - 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// s2 遍历二维数组并随机初始化方块
    std::vector<int> tiles;
    for (int i = 1; i <= 15; i++) {
        tiles.push_back(i);
    }

    // 随机种子
    std::srand(std::time(0));

    // 生成可解的拼图
    do {
        std::random_shuffle(tiles.begin(), tiles.end());
    } while (GetInversionCount(tiles) % 2 != 0);  // 逆序数必须是偶数

    // 填充拼图数组（空位固定在[3][3]）
    int index = 0;
    for (int i = 0; i < BLOCK_COUNT; i++) {
        for (int j = 0; j < BLOCK_COUNT; j++) {
            if (i == BLOCK_COUNT - 1 && j == BLOCK_COUNT - 1) {
                m_iBlockState[i][j] = 0;
                m_spBlock[XYToOneIndex(j, i)] = new CSprite("NULL");
            } else {
                m_iBlockState[i][j] = tiles[index++];
                char* tmpName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[i][j]);
                m_spBlock[XYToOneIndex(j, i)] = new CSprite(tmpName);
                MoveSpriteToBlock(m_spBlock[XYToOneIndex(j, i)], j, i);
            }
        }
    }
	//
	std::stringstream ss;
	for (size_t i = 0; i < tiles.size(); ++i) {
		ss << tiles[i];
		if (i != tiles.size()-1) {
			ss << " ";
		}
	}
	std::string str = ss.str();
	CSystem::SetWindowTitle(&str[0]);
}
//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun( float fDeltaTime )
{
}
//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()
{
	// 显示提示开始文字
	m_spGameBegin->SetSpriteVisible(1);
}
//=============================================================================
//
// s1
void CGameMain::OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress) {
  if (iKey == KEY_SPACE && m_iGameState == 0) {
      m_iGameState = 1; // 进入游戏状态
      m_spGameBegin->SetSpriteVisible(false); // 隐藏 "空格开始" 精灵
  }
}
//=============================================================================
//
// s2 索引转换
int CGameMain::XYToOneIndex(const int iIndexX, const int iIndexY) {
    return (iIndexY * BLOCK_COUNT + iIndexX);
}
//=============================================================================
//
// s2 计算逆序数（Inversion Count）
int CGameMain::GetInversionCount(const std::vector<int>& tiles) {
    int invCount = 0;
    for (size_t i = 0; i < tiles.size(); i++) {
        for (size_t j = i + 1; j < tiles.size(); j++) {
            if (tiles[i] > tiles[j]) invCount++;
        }
    }
    return invCount;
}
//=============================================================================
//
// s2 移动精灵到指定位置
void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, const int iIndexX, const int iIndexY) {
   float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
   float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
   tmpSprite->SetSpritePosition(fPosX, fPosY);
}
//=============================================================================
//
// s3
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // 只处理游戏进行中的鼠标响应
    if (m_iGameState != 2) return;

    int iClickIndex = -1;

    for (int iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++) {
        if (m_spBlock[iLoop]->GetName() == "NULL") continue;

        if (m_spBlock[iLoop]->IsPointInSprite(fMouseX, fMouseY)) {
            iClickIndex = iLoop;
            break;
        }
    }

    if (iClickIndex == -1) return;

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

    if (iEmptyIndexX == -1 || iEmptyIndexY == -1) return;

    // 交换方块位置
    int tempState = m_iBlockState[iIndexY][iIndexX];
    m_iBlockState[iIndexY][iIndexX] = m_iBlockState[iEmptyIndexY][iEmptyIndexX];
    m_iBlockState[iEmptyIndexY][iEmptyIndexX] = tempState;

    int iOneIndex = XYToOneIndex(iEmptyIndexX, iEmptyIndexY);
    CSprite *tempBlock = m_spBlock[iClickIndex];
    m_spBlock[iClickIndex] = m_spBlock[iOneIndex];
    m_spBlock[iOneIndex] = tempBlock;

    // 移动方块至新位置
    MoveSpriteToBlock(m_spBlock[iOneIndex], iEmptyIndexX, iEmptyIndexY);
}
//=============================================================================
//
// s3
int CGameMain::OneIndexToX(const int iIndex) {
    return iIndex % BLOCK_COUNT;
}

int CGameMain::OneIndexToY(const int iIndex) {
    return iIndex / BLOCK_COUNT;
}
//=============================================================================
//
// s4
bool CGameMain::IsGameWin() {
	int iLoopX = 0, iLoopY = 0;
	int iResult = 1;

	for (iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++) {
		for (iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++) {
			// 判断是否为最后一个位置
			if (iLoopX == BLOCK_COUNT - 1 && iLoopY == BLOCK_COUNT - 1) {
				return (m_iBlockState[iLoopY][iLoopX] == 0) ? 1 : 0;
			}
        
			// 若值不符合预期，则游戏未胜利
			if (m_iBlockState[iLoopY][iLoopX] != iResult) {
				return 0;
			}
        
			iResult++;
		}
	}

	return 1;
}