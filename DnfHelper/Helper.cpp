#include "pch.h"
#include "framework.h"
#include "helper.h"

#include<string.h>
#include<Windows.h>
#include<time.h>

CString 整数转字符(int num) 
{
	char tmp[50];
	CString tmpData;
	sprintf_s(tmp, "%d", num);
	tmpData = tmp;
	return tmpData;
}

BYTE ConvertHexChar(BYTE ch)//转换16进制
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else
		return -1;
}

int 取随机数(int min, int max)//随机数，最小值-最大值
{
	int m;
	m = (max - min);
	m = min + (int)double(rand()) / RAND_MAX * m;
	return m;
}

CString 取现行时间()
{
	CString now;
	SYSTEMTIME sys;
	GetLocalTime(&sys);

	int year = sys.wYear;
	int month = sys.wMonth;
	int day = sys.wDay;
	int hour = sys.wHour;
	int minute = sys.wMinute;
	int second = sys.wSecond;
	int msecond = sys.wMilliseconds;
	int dayofweek = sys.wDayOfWeek;

	CString 装逼时间 = _T("装逼时间");
	CString stryear = 整数转字符(year) + "年";
	CString strmonth = 整数转字符(month) + "月";
	CString strday = 整数转字符(day) + "日";
	CString strhour = 整数转字符(hour) + "时";
	CString strminute = 整数转字符(minute) + "分";
	CString strsecond = 整数转字符(second) + "秒";
	//CString strdayofweek = "星期" + (整数转字符(dayofweek);
	now = stryear + strmonth + strday + strhour + strminute + strsecond;

	////公告((文本型*)stryear.data());

	return now;
}

HANDLE 线程创建线程(子程序指针 线程子程序, int 线程ID)
{
	HANDLE 线程句柄 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)线程子程序, NULL, 0, (LPDWORD)&线程ID);
	return 线程句柄;
}
BOOL 线程销毁线程(HANDLE 线程句柄)
{
	BOOL res = TerminateThread(线程句柄, 0);
	关闭对象(线程句柄);
	return res;
}

//void 技能开关() {
//
//	HWND hWnd = FindWindow(TEXT("地下城与勇士"), TEXT("地下城与勇士"));
//	static bool _switch = false;
//	if (_switch == false)
//	{
//		SetTimer(hWnd, 1, 频率, (TIMERPROC)遍历全屏技能);
//		公告("技能全屏 - 开启");
//	}
//	else
//	{
//		KillTimer(hWnd, 1);
//		公告("技能全屏 - 关闭");
//	}
//	_switch = !_switch;
//
//}



int 取进程ID(TCHAR 进程[]) {
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnap) {
		return 0;
	}
	
	if (Process32First(hSnap, &pe))
	{
		while (Process32Next(hSnap, &pe)) {
			if (lstrcmpi(进程, pe.szExeFile) == 0) {
				return pe.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}