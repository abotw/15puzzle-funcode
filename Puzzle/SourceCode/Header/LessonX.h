///////////////////////////////////////////////////////////////////
// NAME:               LessonX.h
//
// PURPOSE:            ��Ϸ������ͷ�ļ���CGameMain�Ķ��塣
// FUNCTIONS/OBJECTS:  CGameMain
//
// AUTHOR:             mql
///////////////////////////////////////////////////////////////////

// include guards
//------------------------------
#ifndef _LESSON_X_H
#define _LESSON_X_H

// includes
//------------------------------
#include <Windows.h>

// defines
//------------------------------
#define BLOCK_COUNT 4	// ������Ϸ��������� (4x4 ����)

//===================================================================
// NAME        : CGameMain
//
// DESCRIPTION : ��Ϸ�ܹ��࣬������Ϸѭ������ʼ�������м��������߼���
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
class CGameMain
{
// methods
//-------------------------------
public :
    // ���캯��
    CGameMain();
	// ��������
    ~CGameMain();

    // ��ȡ��Ϸ״̬
    int GetGameState() { return m_iGameState; }
    // ������Ϸ״̬
    void SetGameState(const int iState) { m_iGameState = iState; }

    // ��Ϸ��ѭ��
    void GameMainLoop(float fDeltaTime);
	// ��Ϸ��ʼ��
    void GameInit();
	// ��Ϸ����
    void GameRun(float fDeltaTime);
	// ��Ϸ����
    void GameEnd();

    // ������̰���
    void OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress);
	// ���������
    void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY);

    // ����ת�� (��ά���� -> һά����)
    int XYToOneIndex(const int iIndexX, const int iIndexY);
    // ����ת�� (һά���� -> ��άX����)
    int OneIndexToX(const int iIndex);
	// ����ת�� (һά���� -> Y����)
    int OneIndexToY(const int iIndex);
    // �ƶ����鵽ָ���ķ���λ��
    void MoveSpriteToBlock(CSprite* tmpSprite, const int iIndexX, const int iIndexY);
    // �ж���Ϸ�Ƿ��ʤ
    bool IsGameWin();
protected :
private :
// attributes
//-------------------------------
public :
protected :
private :
	// ��Ϸ״̬: 0=������ȴ���ʼ, 1=��ʼ��, 2=��Ϸ������
    int m_iGameState; 
    // "�ո�ʼ"����
    CSprite *m_spGameBegin;
    // 4x4 λ�ò���
	static const float m_fBlockSize;   // �����С
    static const float m_fBlockStartX; // ������ʼX����
    static const float m_fBlockStartY; // ������ʼY����
    // ����״̬����
    int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
    // ���龫������
    CSprite *m_spBlock[BLOCK_COUNT * BLOCK_COUNT];
};

extern CGameMain g_GameMain;	// ȫ����Ϸ�������

#endif // _LESSON_X_H