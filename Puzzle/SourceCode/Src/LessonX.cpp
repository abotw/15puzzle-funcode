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
			if( true )
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
	// s2 ��ʼ����������
	int iLoopX = 0, iLoopY = 0, iLoop = 0;
	int iOneIndex = 0, iRandIndex = 0;
	int iDataCount = BLOCK_COUNT * BLOCK_COUNT - 1;
	int iRandData[BLOCK_COUNT * BLOCK_COUNT - 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	// s2 ������ά���鲢�����ʼ������
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
}
//=============================================================================
//
// s1
void CGameMain::OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress) {
  if (iKey == KEY_SPACE && m_iGameState == 0) {
      m_iGameState = 1; // ������Ϸ״̬
      m_spGameBegin->SetSpriteVisible(false); // ���� "�ո�ʼ" ����
  }
}
//=============================================================================
//
// s2 ����ת��
int CGameMain::XYToOneIndex(const int iIndexX, const int iIndexY) {
    return (iIndexY * BLOCK_COUNT + iIndexX);
}
//=============================================================================
//
// s2 �ƶ����鵽ָ��λ��
void CGameMain::MoveSpriteToBlock(CSprite *tmpSprite, const int iIndexX, const int iIndexY) {
   float fPosX = m_fBlockStartX + iIndexX * m_fBlockSize;
   float fPosY = m_fBlockStartY + iIndexY * m_fBlockSize;
   tmpSprite->SetSpritePosition(fPosX, fPosY);
}