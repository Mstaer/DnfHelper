#include "pch.h"
#include "VoouerDrv.h"

static HANDLE g_hDrv = INVALID_HANDLE_VALUE;

VOID �����������(HANDLE hDrv)
{
	g_hDrv = hDrv;
	return;
}

LONG Ч����Ч��(const char* key)
{
	IOCTL_BUFFER		pInfo = { 0 };
	pInfo.Buf = (ULONG64)key;
	ULONG64 �������� = 0;
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_FILE_CHECK, &pInfo, sizeof(pInfo), &��������, sizeof(��������), &returnLegth, NULL);

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


LONG VU_�ڴ�_�ö�дģʽ(DWORD ģʽ, DWORD ����)
{
	BOOL				bResult = FALSE;
	ULONG64             returnCode = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER		pInfo = { 0 };
	pInfo.Addr = ģʽ;
	pInfo.Size = ����;
	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_MOD, &pInfo, sizeof(pInfo), &returnCode, sizeof(returnCode), &returnLegth, NULL);
	return returnCode;
}

//ö��ָ������ģ�� 
DWORD64	VU_�ڴ�_ȡģ���ַ(DWORD pid, const char* moduleName)
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER_GetBase		pInfo = { 0 };
	ALLOCATE_FREE_MEMORY_RESULT result = { 0 };

	pInfo.ModelAddr = 0;
	pInfo.PID = (ULONG)pid;
	sprintf_s(pInfo.ModelName, "%s", moduleName);

	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_GETMODULEBASE, &pInfo, sizeof(pInfo), &result, sizeof(result), &returnLegth, NULL);

	return result.address;

}


//���ڴ溯�����Լ���װ
BOOL VU_�ڴ�_���ֽڼ�(DWORD pid, PVOID addr, PVOID pBuf, INT32 size)
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER		pInfo = { 0 };

	pInfo.PID = pid;
	pInfo.Addr = (DWORD)addr;
	pInfo.Size = size;

	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_READ, &pInfo, sizeof(pInfo), pBuf, size, &returnLegth, NULL);

	return bResult;
}

//д�ڴ溯�����Լ���װ
BOOL VU_�ڴ�_д�ֽڼ�(DWORD pid, PVOID addr, PVOID pBuf, INT32 size)
{

	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;


	DWORD len = sizeof(IOCTL_BUFFER_Write) + size;
	IOCTL_BUFFER_Write* pInfo = (IOCTL_BUFFER_Write*)malloc(len);
	if (!pInfo)
		return FALSE;

	ZeroMemory(pInfo, len);


	pInfo->PID = pid;
	pInfo->Addr = (DWORD)addr;
	pInfo->Size = size;
	memcpy(pInfo->Buf, pBuf, size);

	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_WRITE, pInfo, len, 0, 0, &returnLegth, NULL);

	free(pInfo);
	return bResult;
}


DWORD64 VU_�ڴ�_����(DWORD ����ID, DWORD64 ���̵�ַ, DWORD64 ���볤��, DWORD64 �ڴ�����, BOOL �Ƿ�����)
{


	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	ALLOCATE_FREE_MEMORY_RESULT result = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ���̵�ַ;
	msgInfo.size = ���볤��;
	msgInfo.protection = �ڴ�����;
	msgInfo.allocate = TRUE;
	msgInfo.physical = �Ƿ�����;
	msgInfo.type = MEM_COMMIT;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &result, sizeof(result), &returnLegth, NULL);
	return result.address;
}


DWORD64 VU_�ڴ�_�ͷ�(DWORD ����ID, DWORD64 ���̵�ַ, DWORD64 �ڴ泤��, BOOL �Ƿ�����)
{


	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	ALLOCATE_FREE_MEMORY_RESULT result = { 0 };
	msgInfo.pid = ����ID;
	msgInfo.base = ���̵�ַ;
	msgInfo.size = �ڴ泤��;
	msgInfo.protection = 0;
	msgInfo.allocate = FALSE;
	msgInfo.physical = �Ƿ�����;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &result, sizeof(result), &returnLegth, NULL);
	return result.address;
}


DWORD64 VU_�ڴ�_�޸�����(DWORD ����ID, DWORD64 ��ַ, DWORD64 ����, DWORD64 �ڴ�����)
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


DWORD64 VU_�ڴ�_��ѯ����(DWORD ����ID, DWORD64 ��ַ)
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






DWORD64 VU_HOOK_����(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 Ŀ�ĵ�ַ, BOOL Ӳ������ִ��)
{

	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.pĿ���ַ = Ŀ���ַ;
	msgInfo.pĿ�ĵ�ַ = Ŀ�ĵ�ַ;
	msgInfo.n����ģʽ = 1;
	msgInfo.b�Ƿ����� = Ӳ������ִ��;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 VU_HOOK_����(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.pĿ���ַ = Ŀ���ַ;
	msgInfo.pĿ�ĵ�ַ = 0;
	msgInfo.n����ģʽ = 2;
	msgInfo.b�Ƿ����� = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_��ͣ(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.pĿ���ַ = Ŀ���ַ;
	msgInfo.pĿ�ĵ�ַ = 0;
	msgInfo.n����ģʽ = 3;
	msgInfo.b�Ƿ����� = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_�Ƴ�(DWORD ����ID, DWORD64 Ŀ���ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.pĿ���ַ = Ŀ���ַ;
	msgInfo.pĿ�ĵ�ַ = 0;
	msgInfo.n����ģʽ = 4;
	msgInfo.b�Ƿ����� = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_�ٳ�(DWORD ����ID, DWORD64 Ŀ���ַ, PVOID �ٳִ���, DWORD ���볤��, BOOL Ӳ������ִ��, DWORD ִ���ܴ���, BOOL �Ƿ񱣴��ջ)
{
	DWORD len = sizeof(IOCTL_BUFFER_Hook) + ���볤��;
	IOCTL_BUFFER_Hook* pInfo = (IOCTL_BUFFER_Hook*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;

	pInfo->PID = ����ID;
	pInfo->pĿ���ַ = Ŀ���ַ;
	pInfo->pĿ�ĵ�ַ = �Ƿ񱣴��ջ;
	pInfo->n����ģʽ = 5;
	pInfo->b�Ƿ����� = Ӳ������ִ��;
	pInfo->n���� = ִ���ܴ���;
	pInfo->Size = ���볤��;
	memcpy(pInfo->Buf, �ٳִ���, ���볤��);
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;
}


DWORD64 VU_ע��dll(DWORD ����ID, CHAR* �ļ�·��, DWORD ע��ģʽ, BOOL ����Ӳ��ִ��, DWORD �����ļ���ʽ, BOOL �����ٳ��߳�,
	BOOL �����ӳ��, BOOL ���޸��쳣����, PVOID �ļ�����, DWORD �ļ���С)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectFile) + �ļ���С;
	IOCTL_BUFFER_InjectFile* pInfo = (IOCTL_BUFFER_InjectFile*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "%s", �ļ�·��);
	if (ע��ģʽ == 4)
		sprintf_s(strPath, "\\??\\%s", �ļ�·��);





	BOOL				bResult = FALSE;
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;

	DWORD64 flages = 0;
	if (ע��ģʽ < 4)
		flages = 2;


	if (ע��ģʽ == 4)
	{
		sprintf_s(strPath, "\\??\\%s", �ļ�·��);
		flages = 0;
		flages |= 4;
		if (�����ӳ��)
			flages |= 1;
		if (�����ٳ��߳�)
			flages |= 8;
		if (����Ӳ��ִ��)
			flages |= 16;
		if (�����ļ���ʽ == 1)
			flages |= 128;
		if (�����ļ���ʽ == 2)
			flages |= 256;
		if (���޸��쳣����)
			flages |= 4096;

		flages |= 32768;
		flages |= 65536;
	}



	pInfo->PID = ����ID;
	pInfo->Flages = flages;
	pInfo->Type = ע��ģʽ;
	pInfo->str·�� = (DWORD64)&strPath;

	pInfo->Size = �ļ���С;
	if (�ļ����� && �ļ���С)
		memcpy(pInfo->Buf, �ļ�����, �ļ���С);

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_DLL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;
}




DWORD64 VU_����ע��CALL_�����̻߳�ȡģʽ(DWORD64 ģʽ)
{

	BOOL				bResult = FALSE;
	DWORD64				pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Addr = ģʽ;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SET_THREADMOD, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_Զ��ע��CALL(DWORD ����ID, PBYTE CALL����, DWORD CALL���볤��, BOOL Ӳ������ִ��)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectCode) + CALL���볤��;
	IOCTL_BUFFER_InjectCode* pInfo = (IOCTL_BUFFER_InjectCode*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);
	pInfo->PID = ����ID;
	pInfo->isPhyMem = Ӳ������ִ��;
	pInfo->Size = CALL���볤��;

	memcpy(pInfo->Buf, CALL����, CALL���볤��);

	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;

}


DWORD64 VU_Զ��ִ��CALL(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 ������ַ)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_InjectCode	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.Size = ������ַ;
	msgInfo.Addr = Ŀ���ַ;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_����ע��CALL(DWORD ����ID, PBYTE CALL����, DWORD CALL���볤��, DWORD �߳�ID, BOOL Ӳ������ִ��)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectCode) + CALL���볤��;
	IOCTL_BUFFER_InjectCode* pInfo = (IOCTL_BUFFER_InjectCode*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	pInfo->PID = ����ID;
	pInfo->isPhyMem = Ӳ������ִ��;
	pInfo->Size = CALL���볤��;

	memcpy(pInfo->Buf, CALL����, CALL���볤��);
	pInfo->Addr = �߳�ID;


	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;


	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;

}


DWORD64 VU_����ִ��CALL(DWORD ����ID, DWORD64 Ŀ���ַ, DWORD64 ������ַ, DWORD �߳�ID)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_InjectCode	msgInfo = { 0 };
	msgInfo.PID = ����ID;
	msgInfo.Addr = Ŀ���ַ;
	msgInfo.Size = ������ַ;
	msgInfo.isPhyMem = �߳�ID;


	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


BOOL VU_����_��װ()
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_INIT, 0, 0, 0, 0, &returnLegth, NULL);

}


//����������ο�������ģ��Դ��,�����о����װ��������������������
//��һ���ַ�װ��C��,�ɵ���������,����͵����Ū��,˭���Ū��,ϣ���л��Ὺ��Դ,������ʹ��
VOID VU_����_������(MOUSE_INPUT_DATA ���)
{
	DWORD				returnLegth = 0;
	if (���.Flags == 1)
	{
		���.LastX = ���.LastX * 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
		���.LastY = ���.LastY * 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);
	}
	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_MOUSE, &���, sizeof(���), 0, 0, &returnLegth, NULL);
}

VOID VU_����_���̲���(KEYBOARD_INPUT_DATA ����)
{
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_KEYBOARD, &����, sizeof(����), 0, 0, &returnLegth, NULL);
}



BOOL VU_�����ļ�_��װ()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_INIT, 0, 0, 0, 0, &returnLegth, NULL);
}

BOOL VU_�����ļ�_ж��()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_UNINIT, 0, 0, 0, 0, &returnLegth, NULL);
}


DWORD64 VU_�����ļ�_�ļ�(CHAR* �ļ�·��)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", �ļ�·��);
	msgInfo.Buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_FILE, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_�����ļ�_Ŀ¼(CHAR* Ŀ¼·��)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", Ŀ¼·��);
	msgInfo.Buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_DIR, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}





BOOL VU_����_��װ()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 1;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_ж��()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_����_��ʼ(DWORD64 ����ID, BOOL �Ƿ񱣻�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = ����ID;
	if (�Ƿ񱣻�����)
		msgInfo.Size = 100;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_����_����(DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_����_����_��ʼ(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = ���ھ��;
	msgInfo.PID = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_����_����(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = ���ھ��;
	msgInfo.PID = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_��Ϸ_��ʼ(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = ���ھ��;
	msgInfo.PID = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_����_��Ϸ_����(DWORD64 ���ھ��, DWORD64 ����ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = ���ھ��;
	msgInfo.PID = ����ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}



BOOL VU_Ӳ��_�޸�Ӳ�����(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)���;
	msgInfo.Size = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_DISK, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)���;
	msgInfo.Size = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_VOLUMES, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)���;
	msgInfo.Size = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_MAC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_Ӳ��_�޸��������(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)���;
	msgInfo.Size = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_SMBOIS, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_Ӳ��_�޸��Կ����(CHAR* ���, DWORD64 �ı�����)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER		msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)���;
	msgInfo.Size = �ı�����;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_GPU, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}