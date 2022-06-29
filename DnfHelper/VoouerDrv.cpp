#include "pch.h"
#include "VoouerDrv.h"

static HANDLE g_hDrv = INVALID_HANDLE_VALUE;

VOID �����������(HANDLE hDrv)
{
	g_hDrv = hDrv;
	return;
}

LONG Ч����Ч��(CHAR* key)
{
	MsgInfo			msgInfo = { 0 };
	msgInfo.m_buf = (ULONG64)key;
	LONG �������� = 0;
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_FILE_CHECK, &msgInfo, sizeof(msgInfo), &��������, sizeof(��������), &returnLegth, NULL);

	return ��������;

	/*
	�������� > 0 = (�����������ɹ�!��)
	0 = (���������� - ����DLL·���Ƿ���ʵ�����Լ����ļ������Ƿ���ȷ��)
	-1= (�������벻���ڡ�)
	-2=(���˻����㡱)
	-3=(���۷�ʧ�ܡ�)
	-4=(���������)
	-5=(��������װʧ�ܡ�)
	-6=(����Ч��Ч��ʧ�ܡ�)
	����(��δ֪����)
	*/

}

LONG �ڴ�_�ö�дģʽ(DWORD ģʽ, DWORD ����)
{
	BOOL				bResult = FALSE;
	LONG				returnCode = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_pRWAddr = ģʽ;
	msgInfo.m_nRWSize = ����;
	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_MOD, &msgInfo, sizeof(msgInfo), &returnCode, sizeof(returnCode), &returnLegth, NULL);
	return returnCode;
}

//ö��ָ������ģ�� 
DWORD64	�ڴ�_ȡģ���ַ(DWORD pid, const char* moduleName)
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	DWORD64				moduleBase = 0;

	msgInfo.m_ProcessId = (ULONG)pid;
	msgInfo.m_buf = (DWORD64)moduleName;
	msgInfo.m_nRWSize = strlen(moduleName);

	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_GETMODULEBASE, &msgInfo, sizeof(msgInfo), &moduleBase, sizeof(moduleBase), &returnLegth, NULL);

	return moduleBase;

}


//���ڴ溯�����Լ���װ
BOOL �ڴ�_���ֽڼ�(ULONG pid, ULONG addr, PVOID pBuf, INT32 size)
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;
	MsgInfo			msgInfo = { 0 };

	msgInfo.m_ProcessId = pid;
	msgInfo.m_pRWAddr = addr;
	msgInfo.m_nRWSize = size;

	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_READ_EX, &msgInfo, sizeof(msgInfo), pBuf, size, &returnLegth, NULL);

	return bResult;
}

//д�ڴ溯�����Լ���װ
BOOL �ڴ�_д�ֽڼ�(ULONG pid, ULONG addr, ULONG pBuf, ULONG size)
{

	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };

	msgInfo.m_ProcessId = pid;
	msgInfo.m_pRWAddr = addr;
	msgInfo.m_nRWSize = size;
	msgInfo.m_buf = pBuf;
	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_WRITE_EX, &msgInfo, sizeof(msgInfo), 0, 0, &returnLegth, NULL);
	return bResult;
}


DWORD64 �ڴ�_����(DWORD ����ID, DWORD64 ���̵�ַ, DWORD64 ���볤��, DWORD64 �ڴ�����, BOOL �Ƿ�����)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ���̵�ַ;
	msgInfo.size = ���볤��;
	msgInfo.protection = �ڴ�����;
	msgInfo.allocate = TRUE;
	msgInfo.physical = �Ƿ�����;
	msgInfo.type = MEM_COMMIT;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 �ڴ�_�ͷ�(DWORD ����ID, DWORD64 ���̵�ַ, DWORD64 �ڴ泤��, BOOL �Ƿ�����)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ���̵�ַ;
	msgInfo.size = �ڴ泤��;
	msgInfo.protection = 0;
	msgInfo.allocate = FALSE;
	msgInfo.physical = �Ƿ�����;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 �ڴ�_�޸�����(DWORD ����ID, DWORD64 ��ַ, DWORD64 ����, DWORD64 �ڴ�����)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ��ַ;
	msgInfo.size = ����;
	msgInfo.protection = �ڴ�����;
	msgInfo.allocate = TRUE;
	msgInfo.physical = 1;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_PROTECT, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 �ڴ�_��ѯ����(DWORD ����ID, DWORD64 ��ַ)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ��ַ;
	msgInfo.size = 4096;
	msgInfo.protection = 64;
	msgInfo.allocate = FALSE;
	msgInfo.physical = 1;
	msgInfo.type = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_QUERY, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}






DWORD64 HOOK_����(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 Ŀ�ĵ�ַ, BOOL Ӳ������ִ��)
{

	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_pRWAddr = Ŀ���ַ;
	msgInfo.m_buf = Ŀ�ĵ�ַ;
	msgInfo.m_nRWSize = 1;
	msgInfo.m_isPhyMem = Ӳ������ִ��;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 HOOK_����(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_pRWAddr = Ŀ���ַ;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 2;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 HOOK_��ͣ(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_pRWAddr = Ŀ���ַ;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 3;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 HOOK_�Ƴ�(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_pRWAddr = Ŀ���ַ;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 4;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 ע��dll(DWORD ����ID, CHAR* dll·��, DWORD ע��ģʽ, BOOL ����, BOOL ����PEͷ, BOOL �����ӳ��,
	BOOL �����ٳ��߳�, BOOL �����ں��ڴ�, BOOL �ٳ�ӳ��, BOOL ���޸��쳣����, BOOL ������������, BOOL ����ʼ���ص�)
{
	BOOL				bResult = FALSE;
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo			msgInfo = { 0 };
	DWORD64 flages = 0;
	if (ע��ģʽ < 4)
	{
		if (����)
			flages = 1;
		if (����PEͷ)
			flages = 2;
	}
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "%s", dll·��);
	if (ע��ģʽ == 4)
	{
		sprintf_s(strPath, "\\??\\%s", dll·��);
		flages = 0;
		if (�����ӳ��)
			flages |= 1;
		if (����PEͷ)
			flages |= 4;
		if (�����ٳ��߳�)
			flages |= 8;
		if (�����ں��ڴ�)
			flages |= 16;
		if (�ٳ�ӳ��)
			flages |= 128;
		if (���޸��쳣����)
			flages |= 4096;
		if (������������)
			flages |= 32768;
		if (����ʼ���ص�)
			flages |= 65536;
	}



	msgInfo.m_ProcessId = (ULONG)����ID;
	msgInfo.m_pRWAddr = flages;
	msgInfo.m_nRWSize = ע��ģʽ;
	msgInfo.m_buf = (DWORD64)&strPath;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_DLL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}




DWORD64 ����ע��CALL_�����̻߳�ȡģʽ(DWORD64 ģʽ)
{

	BOOL				bResult = FALSE;
	DWORD64				pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_pRWAddr = ģʽ;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SET_THREADMOD, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 Զ��ע��CALL(DWORD ����ID, PBYTE CALL����, DWORD CALL���볤��, BOOL Ӳ������ִ��)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_buf = (DWORD64)CALL����;
	msgInfo.m_nRWSize = CALL���볤��;
	msgInfo.m_isPhyMem = Ӳ������ִ��;
	msgInfo.m_pRWAddr = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;

}


DWORD64 Զ��ִ��CALL(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 ������ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	MsgInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_nRWSize = ������ַ;
	msgInfo.m_pRWAddr = Ŀ���ַ;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 ����ע��CALL(DWORD ����ID, PBYTE CALL����, DWORD CALL���볤��, DWORD �߳�ID, BOOL Ӳ������ִ��)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_buf = (DWORD64)CALL����;
	msgInfo.m_nRWSize = CALL���볤��;
	msgInfo.m_isPhyMem = Ӳ������ִ��;
	msgInfo.m_pRWAddr = �߳�ID;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;

}


DWORD64 ����ִ��CALL(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 ������ַ, DWORD �߳�ID)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;
	msgInfo.m_buf = Ŀ���ַ;
	msgInfo.m_nRWSize = ������ַ;
	msgInfo.m_pRWAddr = �߳�ID;
	msgInfo.m_isPhyMem = 1;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


BOOL ����_��װ()
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_INIT, 0, 0, 0, 0, &returnLegth, NULL);

}


//����������ο�������ģ��Դ��,�����о����װ��������������������
//��һ���ַ�װ��C��,�ɵ���������,����͵����Ū��,˭���Ū��,ϣ���л��Ὺ��Դ,������ʹ��
VOID ����_������(MOUSE_INPUT_DATA ���)
{
	DWORD				returnLegth = 0;
	if (���.Flags == 1)
	{
		���.LastX = ���.LastX * 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
		���.LastY = ���.LastY * 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);
	}
	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_MOUSE, &���, sizeof(���), 0, 0, &returnLegth, NULL);
}

VOID ����_���̲���(KEYBOARD_INPUT_DATA ����)
{
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_KEYBOARD, &����, sizeof(����), 0, 0, &returnLegth, NULL);
}



BOOL �����ļ�_��װ()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_INIT, 0, 0, 0, 0, &returnLegth, NULL);
}

BOOL �����ļ�_ж��()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_UNINIT, 0, 0, 0, 0, &returnLegth, NULL);
}


DWORD64 �����ļ�_�ļ�(CHAR* �ļ�·��)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", �ļ�·��);
	msgInfo.m_buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_FILE, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 �����ļ�_Ŀ¼(CHAR* Ŀ¼·��)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", Ŀ¼·��);
	msgInfo.m_buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_DIR, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}





BOOL ����_��װ()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 1;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_ж��()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_����_��ʼ(DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_����_����(DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL ����_����_��ʼ(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = ���ھ��;
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_����_����(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = ���ھ��;
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_��Ϸ_��ʼ(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = ���ھ��;
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL ����_��Ϸ_����(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = ���ھ��;
	msgInfo.m_ProcessId = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}



BOOL Ӳ��_�޸�Ӳ�����(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)���;
	msgInfo.m_nRWSize = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_DISK, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)���;
	msgInfo.m_nRWSize = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_VOLUMES, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)���;
	msgInfo.m_nRWSize = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_MAC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)���;
	msgInfo.m_nRWSize = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_SMBOIS, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL Ӳ��_�޸��Կ����(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)���;
	msgInfo.m_nRWSize = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_GPU, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}