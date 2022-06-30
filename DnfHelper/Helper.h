#pragma once

#include "framework.h"
#include "tlhelp32.h"

#include<vector>
using namespace std;

typedef int 整数型;
typedef __int64 长整数型;
typedef float 小数型;
typedef double 双精度浮点型;
typedef bool 逻辑型;
typedef char 文本型;
typedef void(*子程序指针)();

#define 注册热键 RegisterHotKey 
// RegisterHotKey(NULL, 1, 0, VK_HOME);
// RegisterHotKey(NULL, 1, MOD_ALT, VK_LEFT);
// RegisterHotKey(NULL, 0xC005, MOD_CONTROL, VK_LEFT);

#define 信息框 MessageBoxA
// MessageBoxA(NULL, "激活按钮事件", "激活", MB_OK);

#define 关闭对象 CloseHandle

#define 创建线程 CreateThread
// 创建线程(NULL, 0, (LPTHREAD_START_ROUTINE)函数名, NULL, 0, &线程id);


#define 创建时钟 SetTimer
// HWND hWnd = FindWindow(TEXT("地下城与勇士"), TEXT("地下城与勇士"));
// SetTimer(hWnd, 1, 频率, (TIMERPROC)函数名);
#define 销毁时钟 KillTimer
// KillTimer(hWnd, 1);

#define 字节集 vector<int>
#define 取数组成员数 sizeof
#define 结构 struct
#define 判断 switch
#define 输出 printf

#define 写配置 WritePrivateProfileStringW
#define 读配置 GetPrivateProfileIntA
// 写配置(L"代码全屏", L"代码", L"48102", L"C:\\情歌.ini");
// 读配置("代码全屏", "代码", 0, "c:\\情歌.ini");

CString 整数转字符(DWORD num);
int 取进程ID(TCHAR 进程[]);