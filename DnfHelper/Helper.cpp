#include "pch.h"
#include "framework.h"
#include "helper.h"

#include<string.h>
#include<Windows.h>
#include<time.h>

CString ����ת�ַ�(int num) 
{
	char tmp[50];
	CString tmpData;
	sprintf_s(tmp, "%d", num);
	tmpData = tmp;
	return tmpData;
}

BYTE ConvertHexChar(BYTE ch)//ת��16����
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

int ȡ�����(int min, int max)//���������Сֵ-���ֵ
{
	int m;
	m = (max - min);
	m = min + (int)double(rand()) / RAND_MAX * m;
	return m;
}

CString ȡ����ʱ��()
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

	CString װ��ʱ�� = _T("װ��ʱ��");
	CString stryear = ����ת�ַ�(year) + "��";
	CString strmonth = ����ת�ַ�(month) + "��";
	CString strday = ����ת�ַ�(day) + "��";
	CString strhour = ����ת�ַ�(hour) + "ʱ";
	CString strminute = ����ת�ַ�(minute) + "��";
	CString strsecond = ����ת�ַ�(second) + "��";
	//CString strdayofweek = "����" + (����ת�ַ�(dayofweek);
	now = stryear + strmonth + strday + strhour + strminute + strsecond;

	////����((�ı���*)stryear.data());

	return now;
}

HANDLE �̴߳����߳�(�ӳ���ָ�� �߳��ӳ���, int �߳�ID)
{
	HANDLE �߳̾�� = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)�߳��ӳ���, NULL, 0, (LPDWORD)&�߳�ID);
	return �߳̾��;
}
BOOL �߳������߳�(HANDLE �߳̾��)
{
	BOOL res = TerminateThread(�߳̾��, 0);
	�رն���(�߳̾��);
	return res;
}

//void ���ܿ���() {
//
//	HWND hWnd = FindWindow(TEXT("���³�����ʿ"), TEXT("���³�����ʿ"));
//	static bool _switch = false;
//	if (_switch == false)
//	{
//		SetTimer(hWnd, 1, Ƶ��, (TIMERPROC)����ȫ������);
//		����("����ȫ�� - ����");
//	}
//	else
//	{
//		KillTimer(hWnd, 1);
//		����("����ȫ�� - �ر�");
//	}
//	_switch = !_switch;
//
//}



int ȡ����ID(TCHAR ����[]) {
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnap) {
		return 0;
	}
	
	if (Process32First(hSnap, &pe))
	{
		while (Process32Next(hSnap, &pe)) {
			if (lstrcmpi(����, pe.szExeFile) == 0) {
				return pe.th32ProcessID;
			}
		}
	}
	CloseHandle(hSnap);
	return 0;
}