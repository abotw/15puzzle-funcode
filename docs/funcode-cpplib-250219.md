# FunCode C++ Library

## 1 EWorldLimit - 精灵与世界边界碰撞响应类型

```cpp
// 定义精灵与世界边界碰撞的不同响应模式
// 例如，精灵碰撞后可以弹回、停止或被删除

enum EWorldLimit {
    WORLD_LIMIT_OFF,      // 关闭世界边界碰撞
    WORLD_LIMIT_NULL,     // 碰撞后无默认处理，由游戏逻辑决定
    WORLD_LIMIT_RIGID,    // 发生刚性物理碰撞
    WORLD_LIMIT_BOUNCE,   // 反弹效果
    WORLD_LIMIT_CLAMP,    // 小幅反弹，最终静止
    WORLD_LIMIT_STICKY,   // 碰撞后固定不动
    WORLD_LIMIT_KILL,     // 碰撞后删除精灵
    WORLD_LIMIT_INVALID   // 无效值（用于错误检测）
};
```

## 2 ECollisionResponse - 精灵与精灵之间的碰撞响应

```cpp
// 定义精灵与其他精灵碰撞时的响应方式
// 例如，两者可以弹开、合并或保持静止

enum ECollisionResponse {
    COL_RESPONSE_OFF,     // 关闭碰撞响应
    COL_RESPONSE_RIGID,   // 刚性物理碰撞响应
    COL_RESPONSE_BOUNCE,  // 反弹模式
    COL_RESPONSE_CLAMP,   // 小幅反弹后逐渐停止
    COL_RESPONSE_STICKY,  // 碰撞后保持静止
    COL_RESPONSE_KILL,    // 碰撞后删除精灵
    COL_RESPONSE_CUSTOM,  // 由游戏逻辑决定碰撞处理方式
    COL_RESPONSE_INVALID  // 无效值（用于错误检测）
};
```

## *3 MouseTypes - 鼠标按键值定义

```cpp
// 枚举鼠标按钮类型，方便识别不同的鼠标操作

enum MouseTypes {
    MOUSE_LEFT = 0,   // 左键
    MOUSE_RIGHT = 1,  // 右键
    MOUSE_MIDDLE = 2  // 中键（通常用于滚轮点击）
};
```

## *4 KeyCodes - 键盘KEY值定义

```cpp
// 定义键盘按键代码，便于在程序中处理用户输入
// 例如，键盘事件可检测按键并执行相应的功能

enum KeyCodes {
    KEY_NULL = 0x000,     // 无效按键
    KEY_BACKSPACE = 0x001,
    KEY_TAB = 0x002,
    KEY_ENTER = 0x003,   // 回车键
    KEY_CONTROL = 0x004,
    KEY_ALT = 0x005,
    KEY_SHIFT = 0x006,
    // 更多按键定义...
    KEY_ANYKEY = 0xfffe  // 任意按键（通配）
};
```

## *5 CSprite 类

```cpp
// 精灵基类，所有游戏对象（如玩家、敌人、道具）均继承自该类
// 提供基本的精灵操作，如移动、缩放、可见性等

class CSprite {
private:
    char m_szName[MAX_NAME_LEN]; // 精灵名称

public:
    CSprite(const char *szName);  // 构造函数，创建精灵
    virtual ~CSprite();  // 析构函数，释放资源
    const char *GetName();  // 获取精灵名称
    bool CloneSprite(const char *szSrcName);  // 复制精灵
    void DeleteSprite();  // 删除精灵
    void SetSpriteVisible(bool bVisible);  // 设置可见性
    bool IsSpriteVisible();  // 检查是否可见
    void SetSpriteEnable(bool bEnable);  // 启用/禁用精灵
    void SetSpriteScale(float fScale);  // 调整缩放比例
    bool IsPointInSprite(float fPosX, float fPosY);  // 检测点是否在精灵范围内
    void SetSpritePosition(float fPosX, float fPosY);  // 设置位置
    float GetSpritePositionX();  // 获取X坐标
    float GetSpritePositionY();  // 获取Y坐标
};
```

## 6 CStaticSprite 类

```cpp
// 静态精灵类，继承自 CSprite，适用于不需要动画的静态对象
// 主要用于显示静态图片，如背景、地图元素等

class CStaticSprite : public CSprite {
public:
    CStaticSprite(const char *szName); // 构造函数，创建静态精灵
    ~CStaticSprite(); // 析构函数，释放资源
    void SetStaticSpriteImage(const char *szImageName, int iFrame); // 设置静态精灵的图片
    void SetStaticSpriteFrame(int iFrame); // 设置当前图片帧数
    const char* GetStaticSpriteImage(); // 获取当前显示的图片名称
    int GetStaticSpriteFrame(); // 获取当前显示的帧数
};
```

## 7 CAnimateSprite 类

```cpp
// 动画精灵类，继承自 CSprite，适用于具有动画效果的精灵
// 例如角色、怪物、特效等

class CAnimateSprite : public CSprite {
public:
    CAnimateSprite(const char *szName); // 构造函数
    ~CAnimateSprite(); // 析构函数
    void SetAnimateSpriteFrame(int iFrame); // 设置动画帧数
    const char* GetAnimateSpriteAnimationName(); // 获取当前动画名称
    float GetAnimateSpriteAnimationTime(); // 获取当前动画播放时间
    bool IsAnimateSpriteAnimationFinished(); // 判断动画是否播放完毕
    bool AnimateSpritePlayAnimation(const char *szAnim, bool bRestore); // 播放动画
};
```

## 8 CTextSprite 类

```cpp
// 文字精灵类，继承自 CSprite，用于显示游戏内文本信息
// 例如计分、对话框等

class CTextSprite : public CSprite {
public:
    CTextSprite(const char *szName); // 构造函数
    ~CTextSprite(); // 析构函数
    void SetTextValue(int iValue); // 设置显示的整数值
    void SetTextValueFloat(float fValue); // 设置显示的浮点数值
    void SetTextString(const char *szStr); // 设置显示的字符串
};
```

## 9 CEffect 类

```cpp
// 特效精灵类，继承自 CSprite，用于管理游戏特效
// 例如爆炸、火焰等

class CEffect : public CSprite {
private:
    char m_szCloneName[MAX_NAME_LEN]; // 用于克隆的特效名称
    float m_fTime; // 特效的持续时间

public:
    CEffect(const char *szCloneName, const char *szMyName, float fTime); // 构造函数
    ~CEffect(); // 析构函数
    const char* GetCloneName(); // 获取克隆特效的名称
    float GetTime(); // 获取特效的持续时间
    void PlayEffect(float fPosX, float fPosY, float fRotation); // 播放特效
    void PlayLoopEffect(float fPosX, float fPosY, float fRotation); // 播放循环特效
    void DeleteEffect(); // 删除特效
};
```

## 10 CSound 类

```cpp
// 声音播放类，用于播放游戏音效和背景音乐

class CSound {
private:
    char m_szName[MAX_NAME_LEN]; // 声音名称
    int m_iSoundId; // 声音 ID
    bool m_bLoop; // 是否循环播放
    float m_fVolume; // 音量大小（0-1）

public:
    CSound(const char *szName, bool bLoop, float fVolume); // 构造函数
    ~CSound(); // 析构函数
    const char* GetName(); // 获取声音名称
    void PlaySound(); // 播放声音
    void StopSound(); // 停止播放
    static void StopAllSound(); // 停止所有声音播放
};
```

## *11 CSystem 类

```cpp
// 系统管理类，提供窗口管理和输入处理等功能

class CSystem {
public:
    static void OnMouseMove(float fMouseX, float fMouseY); // 处理鼠标移动事件
    static void OnMouseClick(int iMouseType, float fMouseX, float fMouseY); // 处理鼠标点击事件
    static void OnKeyDown(int iKey, bool bAltPress, bool bShiftPress, bool bCtrlPress); // 处理按键按下事件
    static void OnKeyUp(int iKey); // 处理按键释放事件
    static void SetWindowTitle(const char *szTitle); // 设置窗口标题
    static void ResizeWindow(int iWidth, int iHeight); // 调整窗口大小
};
```