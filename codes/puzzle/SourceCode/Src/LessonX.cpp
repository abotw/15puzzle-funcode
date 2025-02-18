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
// ����ĳ�������Ϊ��GameMainLoop����Ϊ��ѭ��������������ÿ֡ˢ����Ļͼ��֮�󣬶��ᱻ����һ�Ρ�

//==============================================================================
//
// ���캯��
CGameMain::CGameMain()
{
	m_iGameState			=	0;
	// s1
	m_spGameBegin = new CSprite("GameBegin");
}
//==============================================================================
//
// ��������
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// ��Ϸ��ѭ�����˺���������ͣ�ĵ��ã�����ÿˢ��һ����Ļ���˺�����������һ��
// ���Դ�����Ϸ�Ŀ�ʼ�������С������ȸ���״̬. 
// ��������fDeltaTime : �ϴε��ñ��������˴ε��ñ�������ʱ��������λ����
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// ��ʼ����Ϸ�������һ���������
	case 1:
		{
			GameInit();
			SetGameState(2); // ��ʼ��֮�󣬽���Ϸ״̬����Ϊ������
		}
		break;

		// ��Ϸ�����У����������Ϸ�߼�
	case 2:
		{
			// TODO �޸Ĵ˴���Ϸѭ�������������ȷ��Ϸ�߼�
			if( !IsGameWin() ) // s4
			{
				GameRun( fDeltaTime );
			}
			else // ��Ϸ������������Ϸ���㺯����������Ϸ״̬�޸�Ϊ����״̬
			{				
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// ��Ϸ����/�ȴ����ո����ʼ
	case 0:
	default:
		break;
	};
}
//=============================================================================
//
// ÿ�ֿ�ʼǰ���г�ʼ���������һ���������
void CGameMain::GameInit()
{
	// s2
	int iLoopX = 0, iLoopY = 0, iLoop = 0;
	int iOneIndex = 0, iRandIndex = 0;
	int iDataCount = BLOCK_COUNT * BLOCK_COUNT - 1;
	int iRandData[BLOCK_COUNT * BLOCK_COUNT - 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	for (iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++) {
    for (iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++) {
        iOneIndex = XYToOneIndex(iLoopX, iLoopY);
        
        // �趨��λ
        if (iLoopX == BLOCK_COUNT - 1 && iLoopY == BLOCK_COUNT - 1) {
            m_iBlockState[iLoopY][iLoopX] = 0;
            m_spBlock[iOneIndex] = new CSprite("NULL");
        } else {
            // ���ѡ��һ��δʹ�õ���ֵ
            iRandIndex = CSystem::RandomRange(0, iDataCount - 1);
            m_iBlockState[iLoopY][iLoopX] = iRandData[iRandIndex];
            char* tmpName = CSystem::MakeSpriteName("PictureBlock", m_iBlockState[iLoopY][iLoopX]);
            m_spBlock[iOneIndex] = new CSprite(tmpName);
            MoveSpriteToBlock(m_spBlock[iOneIndex], iLoopX, iLoopY);
            
            // �ƶ�ʣ�����ݣ���ֹ�ظ�
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
// ÿ����Ϸ������
void CGameMain::GameRun( float fDeltaTime )
{
}
//=============================================================================
//
// ������Ϸ����
void CGameMain::GameEnd()
{
	// ��ʾ��ʾ��ʼ����
	m_spGameBegin->SetSpriteVisible(1);
}

// s1
void CGameMain::OnKeyDown(const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress) {
	if (iKey == KEY_SPACE && m_iGameState == 0) {
		m_iGameState = 1; // ������Ϸ״̬
		m_spGameBegin->SetSpriteVisible(false); // ���� "�ո�ʼ" ����
	}
}

// s2
int CGameMain::XYToOneIndex(const int iIndexX, const int iIndexY) {
    return (iIndexY * BLOCK_COUNT + iIndexX);
}

void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, const int iIndexX, const int iIndexY) {
   float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
   float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
   tmpSprite->SetSpritePosition(fPosX, fPosY);
}

// s3
void CGameMain::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY) {
    // ֻ������Ϸ�����е������Ӧ
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

    // �ж��ĸ������Ƿ��п�λ
    if (iIndexX > 0 && m_iBlockState[iIndexY][iIndexX - 1] == 0) {
        iEmptyIndexX = iIndexX - 1;
        iEmptyIndexY = iIndexY;
    }
    else if (iIndexX < BLOCK_COUNT - 1 && m_iBlockState[iIndexY][iIndexX + 1] == 0) {
        iEmptyIndexX = iIndexX + 1;
        iEmptyIndexY = iIndexY;
    }
    else if (iIndexY > 0 && m_iBlockState[iIndexY - 1][iIndexX] == 0) {
        iEmptyIndexX = iIndexX;
        iEmptyIndexY = iIndexY - 1;
    }
    else if (iIndexY < BLOCK_COUNT - 1 && m_iBlockState[iIndexY + 1][iIndexX] == 0) {
        iEmptyIndexX = iIndexX;
        iEmptyIndexY = iIndexY + 1;
    }

    if (iEmptyIndexX == -1 || iEmptyIndexY == -1) return;

    // ��������λ�ã���ʹ�� std::swap��
    int tempState = m_iBlockState[iIndexY][iIndexX];
    m_iBlockState[iIndexY][iIndexX] = m_iBlockState[iEmptyIndexY][iEmptyIndexX];
    m_iBlockState[iEmptyIndexY][iEmptyIndexX] = tempState;

    int iOneIndex = XYToOneIndex(iEmptyIndexX, iEmptyIndexY);
    CSprite* tempBlock = m_spBlock[iClickIndex];
    m_spBlock[iClickIndex] = m_spBlock[iOneIndex];
    m_spBlock[iOneIndex] = tempBlock;

    // �ƶ���������λ��
    MoveSpriteToBlock(m_spBlock[iOneIndex], iEmptyIndexX, iEmptyIndexY);
}

int CGameMain::OneIndexToX(const int iIndex) {
    return iIndex % BLOCK_COUNT;
}

int CGameMain::OneIndexToY(const int iIndex) {
    return iIndex / BLOCK_COUNT;
}

// s4
int CGameMain::IsGameWin() {
	int iLoopX = 0, iLoopY = 0;
	int iResult = 1;

	for (iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++) {
		for (iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++) {
			// �ж��Ƿ�Ϊ���һ��λ��
			if (iLoopX == BLOCK_COUNT - 1 && iLoopY == BLOCK_COUNT - 1) {
				return (m_iBlockState[iLoopY][iLoopX] == 0) ? 1 : 0;
			}
        
			// ��ֵ������Ԥ�ڣ�����Ϸδʤ��
			if (m_iBlockState[iLoopY][iLoopX] != iResult) {
				return 0;
			}
        
			iResult++;
		}
	}

	return 1;
}
