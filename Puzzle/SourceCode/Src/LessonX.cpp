///////////////////////////////////////////////////////////////////
// NAME:               LessonX.cpp
//
// PURPOSE:            ��Ϸ�����߼�ʵ���ļ���
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
CGameMain g_GameMain;	// ��Ϸ����

// constants
//------------------------------
// ���㷽�����ʼλ�úʹ�С
const float CGameMain::m_fBlockSize = 18.75f;
const float CGameMain::m_fBlockStartX = -40.625f;
const float CGameMain::m_fBlockStartY = -28.125f;

//===================================================================
// NAME        : CGameMain
//
// DESCRIPTION : ���캯������ʼ����Ϸ״̬�͡��ո�ʼ�����顣
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
// DESCRIPTION : ����������
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
CGameMain::~CGameMain()
{

}

//===================================================================
// NAME        : GameMainLoop
//
// DESCRIPTION : ��Ϸ��ѭ����������Ϸ��ʼ�������кͽ���״̬��
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
// DESCRIPTION : ��Ϸ��ʼ�������ɲ�����ƴͼ���顣
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
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

//===================================================================
// NAME        : GameRun
//
// DESCRIPTION : ������Ϸ��
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::GameRun(float fDeltaTime)
{

}

//===================================================================
// NAME        : GameEnd
//
// DESCRIPTION : ������Ϸ��
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::GameEnd()
{
    m_spGameBegin->SetSpriteVisible(1);
}

//===================================================================
// NAME        : OnKeyDown
//
// DESCRIPTION : ��Ӧ���̰�����
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnKeyDown(int iKey, bool iAltPress, bool iShiftPress, bool iCtrlPress)
{
    if (iKey == KEY_SPACE && m_iGameState == 0)
    {
        m_iGameState = 1;
        m_spGameBegin->SetSpriteVisible(false);
    }
}

//===================================================================
// NAME        : OnMouseClick
//
// DESCRIPTION : ��Ӧ�������
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // ֻ������Ϸ�����е������Ӧ
    if (m_iGameState != 2) return;

    int iClickIndex = -1;

  	// ��ȡ������ķǿհ׸񷽿�����
    for (int iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++) {
        if (m_spBlock[iLoop]->GetName() == "NULL") continue;
        if (m_spBlock[iLoop]->IsPointInSprite(fMouseX, fMouseY)) {
            iClickIndex = iLoop;
            break;
        }
    }

    if (iClickIndex == -1) return;

  	// ��ȡһά������Ӧ�Ķ�ά����
    int iIndexX = OneIndexToX(iClickIndex);
    int iIndexY = OneIndexToY(iClickIndex);
    int iEmptyIndexX = -1, iEmptyIndexY = -1;

    // �ж��ĸ������Ƿ��п�λ
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
		// �ĸ�������޿�λ
    if (iEmptyIndexX == -1 || iEmptyIndexY == -1) return;

    // ���¾���״̬
    int tempState = m_iBlockState[iIndexY][iIndexX];
    m_iBlockState[iIndexY][iIndexX] = m_iBlockState[iEmptyIndexY][iEmptyIndexX];
    m_iBlockState[iEmptyIndexY][iEmptyIndexX] = tempState;
		// ���·�������
    int iOneIndex = XYToOneIndex(iEmptyIndexX, iEmptyIndexY);
    CSprite *tempBlock = m_spBlock[iClickIndex];
    m_spBlock[iClickIndex] = m_spBlock[iOneIndex];
    m_spBlock[iOneIndex] = tempBlock;
    // �ƶ���������λ�ã�������Ⱦ
    MoveSpriteToBlock(m_spBlock[iOneIndex], iEmptyIndexX, iEmptyIndexY);
}

//===================================================================
// NAME        : XYToOneIndex
//
// DESCRIPTION : ����ת�� (��ά���� -> һά����)��
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
// DESCRIPTION : ����ת�� (һά���� -> ��άX����)��
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int CGameMain::OneIndexToX(int iIndex)
{
    return iIndex % BLOCK_COUNT;
}

//===================================================================
// NAME        : OneIndexToY
//
// DESCRIPTION : ����ת�� (һά���� -> ��άY����)��
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int CGameMain::OneIndexToY(int iIndex)
{
    return iIndex / BLOCK_COUNT;
}

//===================================================================
// NAME        : MoveSpriteToBlock
//
// DESCRIPTION : �ƶ����鵽ָ������λ�á�
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, int iIndexX, int iIndexY)
{
    float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
    float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
    tmpSprite->SetSpritePosition(fPosX, fPosY);
}

//===================================================================
// NAME        : IsGameWin
//
// DESCRIPTION : �ж���Ϸ�Ƿ�ʤ����
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
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