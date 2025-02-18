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
#define BLOCK_COUNT 4 // 4x4 ���󷽿�

/////////////////////////////////////////////////////////////////////////////////
//
// ��Ϸ�ܹ��ࡣ��������Ϸ��ѭ������Ϸ��ʼ���������ȹ���
class	CGameMain
{
private:
	int				m_iGameState;				// ��Ϸ״̬��0���������ߵȴ���ʼ��1����ʼ����2����Ϸ������

public:
	CGameMain();            //���캯��
	~CGameMain();           //��������  

	// Get����
	int				GetGameState()											{ return m_iGameState; }
	
	// Set����
	void			SetGameState( const int iState )				{ m_iGameState	=	iState; }
	
	// ��Ϸ��ѭ����
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();

	// s1
	CSprite* m_spGameBegin; // "�ո�ʼ"����
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