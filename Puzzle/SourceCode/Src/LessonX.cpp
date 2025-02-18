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
			if( true )
			{
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
	for (iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++) {
		for (iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++) {
			iOneIndex = XYToOneIndex(iLoopX, iLoopY);
        
			// 设定空位
			if (iLoopX == BLOCK_COUNT - 1 && iLoopY == BLOCK_COUNT - 1) {
				m_iBlockState[iLoopY][iLoopX] = 0;
				m_spBlock[iOneIndex] = new CSprite("NULL");
			} else {
				// 随机选择一个未使用的数值
				iRandIndex = CSystem::RandomRange(0, iDataCount - 1);
				m_iBlockState[iLoopY][iLoopX] = iRandData[iRandIndex];
				char* tmpName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[iLoopY][iLoopX]);
				m_spBlock[iOneIndex] = new CSprite(tmpName);
				MoveSpriteToBlock(m_spBlock[iOneIndex], iLoopX, iLoopY);
            
				// 移动剩余数据，防止重复
				for (iLoop = iRandIndex; iLoop < iDataCount - 1; iLoop++) {
					iRandData[iLoop] = iRandData[iLoop + 1];
				}
				iDataCount--;
			}
		}
	}
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
// s2 移动精灵到指定位置
void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, const int iIndexX, const int iIndexY) {
   float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
   float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
   tmpSprite->SetSpritePosition(fPosX, fPosY);
}