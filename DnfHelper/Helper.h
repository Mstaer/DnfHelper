#pragma once

#include "framework.h"
#include "tlhelp32.h"

#include<vector>
using namespace std;

typedef int ������;
typedef __int64 ��������;
typedef float С����;
typedef double ˫���ȸ�����;
typedef bool �߼���;
typedef char �ı���;
typedef void(*�ӳ���ָ��)();

#define ע���ȼ� RegisterHotKey 
// RegisterHotKey(NULL, 1, 0, VK_HOME);
// RegisterHotKey(NULL, 1, MOD_ALT, VK_LEFT);
// RegisterHotKey(NULL, 0xC005, MOD_CONTROL, VK_LEFT);

#define ��Ϣ�� MessageBoxA
// MessageBoxA(NULL, "���ť�¼�", "����", MB_OK);

#define �رն��� CloseHandle

#define �����߳� CreateThread
// �����߳�(NULL, 0, (LPTHREAD_START_ROUTINE)������, NULL, 0, &�߳�id);


#define ����ʱ�� SetTimer
// HWND hWnd = FindWindow(TEXT("���³�����ʿ"), TEXT("���³�����ʿ"));
// SetTimer(hWnd, 1, Ƶ��, (TIMERPROC)������);
#define ����ʱ�� KillTimer
// KillTimer(hWnd, 1);

#define �ֽڼ� vector<int>
#define ȡ�����Ա�� sizeof
#define �ṹ struct
#define �ж� switch
#define ��� printf

#define д���� WritePrivateProfileStringW
#define ������ GetPrivateProfileIntA
// д����(L"����ȫ��", L"����", L"48102", L"C:\\���.ini");
// ������("����ȫ��", "����", 0, "c:\\���.ini");

CString ����ת�ַ�(DWORD num);
int ȡ����ID(TCHAR ����[]);