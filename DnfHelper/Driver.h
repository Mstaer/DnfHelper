#pragma once

#include "framework.h"


class Driver
{
public:
	Driver();
	~Driver();

public:
	DWORD m_dwLastError; //���Ĵ���
	LPCWSTR m_pSysPath;  //����·��
	LPCWSTR m_pServiceName; //������
	LPCWSTR m_pDisplayName;  //��ʾ��
	HANDLE m_hDriver;   //�������
	SC_HANDLE m_hSCManager;  //SCM���
	SC_HANDLE m_hService;  //������

public:
	BOOL Install(LPCWSTR pSysPath, LPCWSTR pServiceName, LPCWSTR pDisplayName); //��װ��������
	BOOL Start(); //������������
	BOOL Stop();  //ֹͣ��������
	BOOL Remove();  //ж����������
	BOOL Open(LPCWSTR pLinkName); //���������
	BOOL IoControl(DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen, DWORD* RealRetBytes); //IO����
private:
	BOOL GetSvcHandle(LPCWSTR pServiceName);
	DWORD CTL_CODE_GEN(DWORD lngFunction);
};