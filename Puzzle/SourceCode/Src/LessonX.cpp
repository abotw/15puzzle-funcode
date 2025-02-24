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
#include <algorithm>
#include "CommonClass.h"
#include "LessonX.h"

// global vars
//------------------------------
CGameMain g_GameMain;	// ��Ϸ����

// constants
//------------------------------
// ����ƴͼ�Ĵ�С����ʼ����
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
// DATE        : 250224
//===================================================================
void CGameMain::GameInit()
{
    // ���ɿɽ��ƴͼ
    GenerateSolvablePuzzle();
    // ��ƴͼ������
    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            int iBlockValue = m_iBlockState[y][x];

            int iIdx = XYToOneIndex(x, y); // ��ȡһά����

            if (iBlockValue == 0) {
                m_spBlock[iIdx] = new CSprite("NULL"); // �հ׿�
            } else {
                char *szSpName = CSystem::MakeSpriteName("PictureBlock", iBlockValue);
                m_spBlock[iIdx] = new CSprite(szSpName);
            }

            // ������Ⱦƴͼ
            MoveSpriteToBlock(m_spBlock[iIdx], x, y);
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
	// ��ʾ��ʾ��ʼ�ġ��ո�ʼ������
	m_spGameBegin->SetSpriteVisible(true);
}

//===================================================================
// NAME        : OnKeyDown
//
// DESCRIPTION : ������̰����¼���
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnKeyDown(int iKey, bool iAltPress, bool iShiftPress, bool iCtrlPress)
{
	// ���¿ո������Ϸδ��ʼ
	if (iKey == KEY_SPACE && m_iGameState == 0) {
		SetGameState(1);												// �л�����Ϸ��ʼ״̬
		m_spGameBegin->SetSpriteVisible(false);	// ����"�ո�ʼ"����
	}  
}

//===================================================================
// NAME        : OnMouseClick
//
// DESCRIPTION : ����������¼���
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // ֻ������Ϸ�����е�������¼�
    if (m_iGameState != 2) return;
    // ��ʼ���������ƴͼ����
    int iClickIdx = -1;
    // ����ƴͼ�����ȡ�������ƴͼ����
    for (int iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++) {
        if (m_spBlock[iLoop]->GetName() == "NULL") continue;
      	// ʹ��`IsPointInSprite`�ж�����Ƿ�����ĳ��ƴͼ�����ǣ���`iClickIdx`��¼�¸�����
        if (m_spBlock[iLoop]->IsPointInSprite(fMouseX, fMouseY)) {
            iClickIdx = iLoop;
            break;
        }
    }
    // δ��ȡ����������
    if (iClickIdx == -1) return;
    // ��ȡһά������Ӧ�Ķ�ά����
    int iIdxX = OneIndexToX(iClickIdx);
    int iIdxY = OneIndexToY(iClickIdx);
    // ��ʼ���հ׸�����
    int iEmptyIdxX = -1, iEmptyIdxY = -1;
    // �жϵ����ƴͼ��Χ�Ƿ������ڵĿհ׸�
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
    // �޷��ҵ����ڵĿհ׸�
    if (iEmptyIdxX == -1 || iEmptyIdxY == -1) return;
    // ���¾���״̬
    int tempState = m_iBlockState[iIdxY][iIdxX];
    m_iBlockState[iIdxY][iIdxX] = m_iBlockState[iEmptyIdxY][iEmptyIdxX];
    m_iBlockState[iEmptyIdxY][iEmptyIdxX] = tempState;
    // ����ƴͼ����
    int iOneIdx = XYToOneIndex(iEmptyIdxX, iEmptyIdxY);
    CSprite *spTmp = m_spBlock[iClickIdx];
    m_spBlock[iClickIdx] = m_spBlock[iOneIdx];
    m_spBlock[iOneIdx] = spTmp;
    // ������Ⱦƴͼ
    MoveSpriteToBlock(m_spBlock[iOneIdx], iEmptyIdxX, iEmptyIdxY);
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
    return (iIndex % BLOCK_COUNT);
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
    return (iIndex / BLOCK_COUNT);
}

//===================================================================
// NAME        : MoveSpriteToBlock
//
// DESCRIPTION : �ƶ�ƴͼ��ָ��λ�á�
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
// DESCRIPTION : �ж���Ϸ�Ƿ��ʤ��
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
bool CGameMain::IsGameWin()
{
    int iSN = 1;	// ƴͼ����

    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            // �ж����һ��λ���Ƿ��ǿհ׸�
            if (x == BLOCK_COUNT - 1 && y == BLOCK_COUNT - 1)
                return (m_iBlockState[y][x] == 0);
            // �ж����У���ֵ������Ԥ�ڣ�����Ϸδʤ��
            if (m_iBlockState[y][x] != iSN++)
                return false;
        }
    }

    return true;
}

//===================================================================
// NAME        : IsSolvable
//
// DESCRIPTION : �жϵ�ǰƴͼ�Ƿ�ɽ⡣
// AUTHOR      : mql
// DATE        : 250224
//===================================================================
bool CGameMain::IsSolvable(int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT])
{
    int iInversionCount = 0;
    int iFlatPuzzle[BLOCK_COUNT * BLOCK_COUNT]; // �� 4x4 ת����һά����
    int iIdx = 0;
    int iEmptyRow = 0;

    // ת��ƴͼΪһά���飬����¼�հ׿������кţ�������������
    for (int y = 0; y < BLOCK_COUNT; ++y) {
        for (int x = 0; x < BLOCK_COUNT; ++x) {
            iFlatPuzzle[iIdx] = m_iBlockState[y][x];
            if (m_iBlockState[y][x] == 0) {
                iEmptyRow = y; // ��¼�հ׿��к�
            }
            iIdx++;
        }
    }

    // ����������
    for (int i = 0; i < BLOCK_COUNT * BLOCK_COUNT - 1; ++i) {
        for (int j = i + 1; j < BLOCK_COUNT * BLOCK_COUNT; ++j) {
            if (iFlatPuzzle[i] > iFlatPuzzle[j] && iFlatPuzzle[i] != 0 && iFlatPuzzle[j] != 0) {
                iInversionCount++;
            }
        }
    }

    // ����հ׸���������ӵײ���������
    int iBlankRowFromBottom = BLOCK_COUNT - iEmptyRow;

    // 15ƴͼ�ɽ��Թ���
    return (iInversionCount % 2 == 0) == (iBlankRowFromBottom % 2 == 1);
}

//===================================================================
// NAME        : GenerateSolvablePuzzle
//
// DESCRIPTION : ����һ���ɽ�� 15 ƴͼ����ȷ���հ׸��� (3,3)��
// AUTHOR      : mql
// DATE        : 250224
//===================================================================
void CGameMain::GenerateSolvablePuzzle()
{
    int iRandData[BLOCK_COUNT * BLOCK_COUNT - 1];  // �洢1-15�ı��
    for (int i = 0; i < BLOCK_COUNT * BLOCK_COUNT - 1; ++i) {
        iRandData[i] = i + 1;
    }

    do {
        // ʹ�� Fisher-Yates ϴ���㷨�������ƴͼ
        for (int i = BLOCK_COUNT * BLOCK_COUNT - 2; i > 0; --i) {
            int j = CSystem::RandomRange(0, i); // ���� 0~i ֮����������
            std::swap(iRandData[i], iRandData[j]);
        }
        // �����ҵ�ƴͼ��䵽 4x4 ������
        int iIdx = 0;
        for (int y = 0; y < BLOCK_COUNT; ++y) {
            for (int x = 0; x < BLOCK_COUNT; ++x) {
                if (y == BLOCK_COUNT - 1 && x == BLOCK_COUNT - 1) {
                    m_iBlockState[y][x] = 0; // Ԥ�����һ��Ϊ�հ׸�
                } else {
                    m_iBlockState[y][x] = iRandData[iIdx++];
                }
            }
        }
    } while (!IsSolvable(m_iBlockState)); // ��������ֱ������ɽ���
}