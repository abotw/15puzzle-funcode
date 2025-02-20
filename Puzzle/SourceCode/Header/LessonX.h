/////////////////////////////////////////////////////////////////////////////////
//
// LessonX.h - ��Ϸ������ͷ�ļ�
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_

#include <Windows.h>

// ������Ϸ��������� (4x4 ����)
#define BLOCK_COUNT 4

/////////////////////////////////////////////////////////////////////////////////
//
// CGameMain - ��Ϸ�ܹ��࣬������Ϸѭ������ʼ�������м��������߼�
//
/////////////////////////////////////////////////////////////////////////////////
class CGameMain
{
private:
    int m_iGameState; // ��Ϸ״̬: 0=������ȴ���ʼ, 1=��ʼ��, 2=��Ϸ������
    
    // "�ո�ʼ"����
    CSprite* m_spGameBegin;
    
    // 4x4 λ�ò���
    static const float m_fBlockStartX; // ������ʼX����
    static const float m_fBlockStartY; // ������ʼY����
    static const float m_fBlockSize;   // �����С
    
    // ����״̬����
    int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
    
    // �洢���з���ľ���
    CSprite* m_spBlock[BLOCK_COUNT * BLOCK_COUNT];
    
public:
    // ���� & ��������
    CGameMain();  
    ~CGameMain(); 

    // ��ȡ��Ϸ״̬
    int GetGameState() { return m_iGameState; }
    
    // ������Ϸ״̬
    void SetGameState(const int iState) { m_iGameState = iState; }
    
    // ��Ϸ��ѭ������
    void GameMainLoop(float fDeltaTime);
    void GameInit();
    void GameRun(float fDeltaTime);
    void GameEnd();

    // ������̰�������
    void OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress);
    
    // ����ת�� (��ά���� -> һά����)
    int XYToOneIndex(const int iIndexX, const int iIndexY);
    
    // �ƶ����鵽ָ���ķ���λ��
    void MoveSpriteToBlock(CSprite* tmpSprite, const int iIndexX, const int iIndexY);
    
    // ���������
    void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY);
    
    // һά����ת����X��Y����
    int OneIndexToX(const int iIndex);
    int OneIndexToY(const int iIndex);
    
    // �ж��Ƿ��ʤ
    bool IsGameWin();
};

/////////////////////////////////////////////////////////////////////////////////
//
// ȫ����Ϸ�������
//
extern CGameMain g_GameMain;

#endif // _LESSON_X_H_