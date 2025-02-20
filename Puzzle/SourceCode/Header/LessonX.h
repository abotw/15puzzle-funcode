/////////////////////////////////////////////////////////////////////////////////
//
// LessonX.h - 游戏主控制头文件
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_

#include <Windows.h>

// 定义游戏方块的数量 (4x4 矩阵)
#define BLOCK_COUNT 4

/////////////////////////////////////////////////////////////////////////////////
//
// CGameMain - 游戏总管类，负责游戏循环、初始化、运行及结束等逻辑
//
/////////////////////////////////////////////////////////////////////////////////
class CGameMain
{
private:
    int m_iGameState; // 游戏状态: 0=结束或等待开始, 1=初始化, 2=游戏进行中
    
    // "空格开始"精灵
    CSprite* m_spGameBegin;
    
    // 4x4 位置参数
    static const float m_fBlockStartX; // 方块起始X坐标
    static const float m_fBlockStartY; // 方块起始Y坐标
    static const float m_fBlockSize;   // 方块大小
    
    // 方块状态数组
    int m_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
    
    // 存储所有方块的精灵
    CSprite* m_spBlock[BLOCK_COUNT * BLOCK_COUNT];
    
public:
    // 构造 & 析构函数
    CGameMain();  
    ~CGameMain(); 

    // 获取游戏状态
    int GetGameState() { return m_iGameState; }
    
    // 设置游戏状态
    void SetGameState(const int iState) { m_iGameState = iState; }
    
    // 游戏主循环函数
    void GameMainLoop(float fDeltaTime);
    void GameInit();
    void GameRun(float fDeltaTime);
    void GameEnd();

    // 处理键盘按键输入
    void OnKeyDown(const int iKey, const bool iAltPress, const bool iShiftPress, const bool iCtrlPress);
    
    // 索引转换 (二维坐标 -> 一维索引)
    int XYToOneIndex(const int iIndexX, const int iIndexY);
    
    // 移动精灵到指定的方块位置
    void MoveSpriteToBlock(CSprite* tmpSprite, const int iIndexX, const int iIndexY);
    
    // 处理鼠标点击
    void OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY);
    
    // 一维索引转换回X或Y索引
    int OneIndexToX(const int iIndex);
    int OneIndexToY(const int iIndex);
    
    // 判断是否获胜
    bool IsGameWin();
};

/////////////////////////////////////////////////////////////////////////////////
//
// 全局游戏管理对象
//
extern CGameMain g_GameMain;

#endif // _LESSON_X_H_
