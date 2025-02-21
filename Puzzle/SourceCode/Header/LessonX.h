///////////////////////////////////////////////////////////////////
// NAME:               LessonX.h
//
// PURPOSE:            游戏主控制头文件。CGameMain的定义。
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
#define BLOCK_COUNT 4	// 定义游戏方块的数量 (4x4 矩阵)

//===================================================================
// NAME        : CGameMain
//
// DESCRIPTION : 游戏总管类，负责游戏循环、初始化、运行及结束等逻辑。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
class CGameMain
{
// methods
//-------------------------------
public :
    // 构造函数
    CGameMain();
	// 析构函数
    ~CGameMain();

    // 获取游戏状态
    int GetGameState() { return m_iGameState; }
    // 设置游戏状态
    void SetGameState(const int iState) { m_iGameState = iState; }

    // 游戏主循环
    void GameMainLoop(float fDeltaTime);
	// 游戏初始化
    void GameInit();
	// 游戏进行
    void GameRun(float fDeltaTime);
	// 游戏结束
    void GameEnd();

    // 处理键盘按键
    void OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress);
	// 处理鼠标点击
    void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY);

    // 索引转换 (二维坐标 -> 一维索引)
    int XYToOneIndex(const int iIndexX, const int iIndexY);
    // 索引转换 (一维索引 -> 二维X坐标)
    int OneIndexToX(const int iIndex);
	// 索引转换 (一维索引 -> Y坐标)
    int OneIndexToY(const int iIndex);
    // 移动精灵到指定的方块位置
    void MoveSpriteToBlock(CSprite* tmpSprite, const int iIndexX, const int iIndexY);
    // 判断游戏是否获胜
    bool IsGameWin();
protected :
private :
// attributes
//-------------------------------
public :
protected :
private :
	// 游戏状态: 0=结束或等待开始, 1=初始化, 2=游戏进行中
    int m_iGameState; 
    // "空格开始"精灵
    CSprite *m_spGameBegin;
    // 4x4 位置参数
	static const float m_fBlockSize;   // 方块大小
    static const float m_fBlockStartX; // 方块起始X坐标
    static const float m_fBlockStartY; // 方块起始Y坐标
    // 方块状态矩阵
    int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
    // 方块精灵数组
    CSprite *m_spBlock[BLOCK_COUNT * BLOCK_COUNT];
};

extern CGameMain g_GameMain;	// 全局游戏管理对象

#endif // _LESSON_X_H