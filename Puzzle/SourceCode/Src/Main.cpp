///////////////////////////////////////////////////////////////////
// NAME:               Main.cpp
//
// PURPOSE:            游戏主入口文件。
// FUNCTIONS/OBJECTS:  
//
// AUTHOR:             mql
///////////////////////////////////////////////////////////////////

#include "CommonClass.h"
#include "LessonX.h"

//=================================================================================
//
// WinMain - Windows 程序入口函数
//
//=================================================================================
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    // 初始化游戏引擎
    if (!CSystem::InitGameEngine(hInstance, lpCmdLine))
        return 0;

    // 设置窗口标题
    CSystem::SetWindowTitle("15 Puzzle @mql");

    // 引擎主循环，处理屏幕图像刷新等工作
    while (CSystem::EngineMainLoop())
    {
        // 获取两次调用之间的时间差，传递给游戏逻辑处理
        float fTimeDelta = CSystem::GetTimeDelta();

        // 执行游戏主循环
        g_GameMain.GameMainLoop(fTimeDelta);
    }

    // 关闭游戏引擎
    CSystem::ShutdownGameEngine();
    return 0;
}

//=================================================================================
//
// 鼠标输入处理
//
//=================================================================================
void CSystem::OnMouseMove(const float fMouseX, const float fMouseY)
{
    // 可在此添加游戏需要的响应函数
}

void CSystem::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY)
{
    // 处理鼠标点击
    g_GameMain.OnMouseClick(iMouseType, fMouseX, fMouseY);
}

void CSystem::OnMouseUp(const int iMouseType, const float fMouseX, const float fMouseY)
{
    // 可在此添加游戏需要的响应函数
}

//=================================================================================
//
// 键盘输入处理
//
//=================================================================================
void CSystem::OnKeyDown(const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress)
{
    // 处理键盘按下事件
    g_GameMain.OnKeyDown(iKey, bAltPress, bShiftPress, bCtrlPress);
}

void CSystem::OnKeyUp(const int iKey)
{
    // 可在此添加游戏需要的响应函数
}

//=================================================================================
//
// 精灵碰撞检测
//
//=================================================================================
void CSystem::OnSpriteColSprite(const char* szSrcName, const char* szTarName)
{
    // 可在此添加游戏需要的响应函数
}

void CSystem::OnSpriteColWorldLimit(const char* szName, const int iColSide)
{
    // 可在此添加游戏需要的响应函数
}