#pragma once

#include "framework.h"


class Driver
{
public:
	Driver();
	~Driver();

public:
	DWORD m_dwLastError; //最后的错误
	LPCWSTR m_pSysPath;  //驱动路径
	LPCWSTR m_pServiceName; //服务名
	LPCWSTR m_pDisplayName;  //显示名
	HANDLE m_hDriver;   //驱动句柄
	SC_HANDLE m_hSCManager;  //SCM句柄
	SC_HANDLE m_hService;  //服务句柄

public:
	BOOL Install(LPCWSTR pSysPath, LPCWSTR pServiceName, LPCWSTR pDisplayName); //安装驱动服务
	BOOL Start(); //启动驱动服务
	BOOL Stop();  //停止驱动服务
	BOOL Remove();  //卸载驱动服务
	BOOL Open(LPCWSTR pLinkName); //打开驱动句柄
	BOOL IoControl(DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen, DWORD* RealRetBytes); //IO控制
private:
	BOOL GetSvcHandle(LPCWSTR pServiceName);
	DWORD CTL_CODE_GEN(DWORD lngFunction);
};