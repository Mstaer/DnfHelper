#include "pch.h"

#include "Driver.h"


Driver::Driver() //���캯��
{
	//��ʼ������
	m_pSysPath = NULL;
	m_pServiceName = NULL;
	m_pDisplayName = NULL;
	m_hSCManager = NULL;
	m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;
}

Driver::~Driver() //��������
{
	//�ͷ���Դ
	CloseServiceHandle(m_hService);
	CloseServiceHandle(m_hSCManager);
	CloseHandle(m_hDriver);
}
//-----------��ȡ������
BOOL Driver::GetSvcHandle(LPCWSTR pServiceName)
{
	m_pServiceName = pServiceName;
	m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == m_hSCManager)
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	m_hService = OpenServiceW(m_hSCManager, m_pServiceName, SERVICE_ALL_ACCESS);
	if (NULL == m_hService)
	{
		CloseServiceHandle(m_hSCManager);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//-------------��װ��������
BOOL Driver::Install(LPCWSTR pSysPath, LPCWSTR pServiceName, LPCWSTR pDisplayName)
{
	m_pSysPath = pSysPath;
	m_pServiceName = pServiceName;
	m_pDisplayName = pDisplayName;
	m_hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == m_hSCManager)
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	m_hService = CreateServiceW(m_hSCManager,
		m_pServiceName,//����������ע����е�����
		m_pDisplayName,// ע������������ DisplayName ֵ
		SERVICE_ALL_ACCESS,// ������������ķ���Ȩ��
		SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������
		SERVICE_DEMAND_START,// ע������������ Start ֵ
		SERVICE_ERROR_NORMAL,// ע������������ ErrorControl ֵ
		m_pSysPath,// ע������������ ImagePath ֵ
		NULL, NULL, NULL, NULL, NULL);
	if (NULL == m_hService)
	{
		m_dwLastError = GetLastError();
		if (ERROR_SERVICE_EXISTS == m_dwLastError)
		{
			m_hService = OpenServiceW(m_hSCManager, m_pServiceName, SERVICE_ALL_ACCESS);
			if (NULL == m_hService)
			{
				CloseServiceHandle(m_hSCManager);
				return FALSE;
			}
		}
		else
		{
			CloseServiceHandle(m_hSCManager);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL Driver::Start()
{
	if (!StartServiceW(m_hService, NULL, NULL))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL Driver::Stop()
{
	SERVICE_STATUS ss;
	GetSvcHandle(m_pServiceName);
	if (!ControlService(m_hService, SERVICE_CONTROL_STOP, &ss))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;

}

BOOL Driver::Remove()
{
	GetSvcHandle(m_pServiceName);
	if (!DeleteService(m_hService))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

BOOL Driver::Open(LPCWSTR pLinkName)//example: \\\\.\\xxoo
{
	if (m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;
	m_hDriver = CreateFileW(pLinkName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;
	else
		return FALSE;
}

BOOL Driver::IoControl(DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen, DWORD* RealRetBytes)
{
	return DeviceIoControl(m_hDriver, dwIoCode, InBuff, InBuffLen, OutBuff, OutBuffLen, RealRetBytes, NULL);
}

DWORD Driver::CTL_CODE_GEN(DWORD lngFunction)
{
	return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
}