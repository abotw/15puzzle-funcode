///////////////////////////////////////////////////////////////////
// NAME:               Main.cpp
//
// PURPOSE:            游戏主入口文件。
// FUNCTIONS/OBJECTS:  
//
// AUTHOR:             mql
///////////////////////////////////////////////////////////////////

// includes
//------------------------------
#include "CommonClass.h"
#include "LessonX.h"

//===================================================================
// NAME        : WinMain
//
// DESCRIPTION : Windows 程序入口函数。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
    // 初始化游戏引擎
    if (!CSystem::InitGameEngine(hInstance, lpCmdLine)) return 0;
    // 设置窗口标题
    CSystem::SetWindowTitle("【15 Puzzle】by FunCode @mql");
    // 引擎主循环，处理屏幕图像刷新等工作

    while (CSystem::EngineMainLoop()) {
        // 获取两次调用之间的时间差，传递给游戏逻辑处理
        float fTimeDelta = CSystem::GetTimeDelta();
        // 执行游戏主循环
        g_GameMain.GameMainLoop(fTimeDelta);
    }

    // 关闭游戏引擎
    CSystem::ShutdownGameEngine();

    return 0;
}

//===================================================================
// NAME        : OnMouseMove
//
// DESCRIPTION : 监听鼠标移动。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnMouseMove(const float fMouseX, const float fMouseY)
{

}

//===================================================================
// NAME        : OnMouseClick
//
// DESCRIPTION : 监听鼠标点击。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnMouseClick(const int iMouseType, const float fMouseX, const float fMouseY)
{
    // 响应鼠标点击事件
    g_GameMain.OnMouseClick(iMouseType, fMouseX, fMouseY);
}

//===================================================================
// NAME        : OnMouseUp
//
// DESCRIPTION : 监听鼠标抬起。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnMouseUp(const int iMouseType, const float fMouseX, const float fMouseY)
{

}

//===================================================================
// NAME        : OnKeyDown
//
// DESCRIPTION : 监听键盘按键。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnKeyDown(const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress)
{
    // 响应键盘按键事件
	g_GameMain.OnKeyDown(iKey, bAltPress, bShiftPress, bCtrlPress);
}

//===================================================================
// NAME        : OnKeyUp
//
// DESCRIPTION : 监听键盘抬起。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnKeyUp(const int iKey)
{

}

//===================================================================
// NAME        : OnSpriteColSprite
//
// DESCRIPTION : 监听精灵碰撞。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnSpriteColSprite(const char* szSrcName, const char* szTarName)
{

}

//===================================================================
// NAME        : OnSpriteColWorldLimit
//
// DESCRIPTION : 监听精灵碰撞世界边界。
// AUTHOR      : mql
// DATE        : 250221
//===================================================================
void CSystem::OnSpriteColWorldLimit(const char* szName, const int iColSide)
{

}