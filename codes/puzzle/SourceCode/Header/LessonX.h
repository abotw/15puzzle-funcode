/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
// s2
#define BLOCK_COUNT 4 // 4x4 矩阵方块

/////////////////////////////////////////////////////////////////////////////////
//
// 游戏总管类。负责处理游戏主循环、游戏初始化、结束等工作
class	CGameMain
{
private:
	int				m_iGameState;				// 游戏状态，0：结束或者等待开始；1：初始化；2：游戏进行中

public:
	CGameMain();            //构造函数
	~CGameMain();           //析构函数  

	// Get方法
	int				GetGameState()											{ return m_iGameState; }
	
	// Set方法
	void			SetGameState( const int iState )				{ m_iGameState	=	iState; }
	
	// 游戏主循环等
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();

	// s1
	CSprite* m_spGameBegin; // "空格开始"精灵
	void OnKeyDown(const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress);
	// s2
	static const float m_fBlockStartX;
	static const float m_fBlockStartY;
	static const float m_fBlockSize;
	int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
	CSprite* m_spBlock[BLOCK_COUNT * BLOCK_COUNT];

	int XYToOneIndex(const int iIndexX, const int iIndexY);
	void MoveSpriteToBlock(CSprite *tmpSprite, const int iIndexX, const int iIndexY);

	// s3
	void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY);

	int OneIndexToX(const int iIndex);
	int OneIndexToY(const int iIndex);

	// s4
	int IsGameWin();
};

/////////////////////////////////////////////////////////////////////////////////
// 
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_