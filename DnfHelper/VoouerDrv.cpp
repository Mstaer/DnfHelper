#include "pch.h"
#include "VoouerDrv.h"

static HANDLE g_hDrv = INVALID_HANDLE_VALUE;

VOID 设置驱动句柄(HANDLE hDrv)
{
	g_hDrv = hDrv;
	return;
}

LONG 效验有效性(const char* key)
{
	IOCTL_BUFFER		pInfo = { 0 };
	pInfo.Buf = (ULONG64)key;
	ULONG64 返回数据 = 0;
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_FILE_CHECK, &pInfo, sizeof(pInfo), &返回数据, sizeof(返回数据), &returnLegth, NULL);

	return 返回数据;

	/*
	返回数据 > 0 = (“载入驱动成功!”)
	0 = (“参数错误 - 请检查DLL路径是否真实存在以及您的激活码是否正确”)
	-1= (“激活码不存在”)
	-2=(“账户余额不足”)
	-3=(“扣费失败”)
	-4=(“网络错误”)
	-5=(“驱动安装失败”)
	-6=(“有效性效验失败”)
	其他(“未知错误”)
	*/

}


LONG VU_内存_置读写模式(DWORD 模式, DWORD 类型)
{
	BOOL				bResult = FALSE;
	ULONG64             returnCode = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER		pInfo = { 0 };
	pInfo.Addr = 模式;
	pInfo.Size = 类型;
	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_MOD, &pInfo, sizeof(pInfo), &returnCode, sizeof(returnCode), &returnLegth, NULL);
	return returnCode;
}

//枚举指定进程模块 
DWORD64	VU_内存_取模块基址(DWORD pid, const char* moduleName)
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


//读内存函数，自己封装
BOOL VU_内存_读字节集(DWORD pid, PVOID addr, PVOID pBuf, INT32 size)
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

//写内存函数，自己封装
BOOL VU_内存_写字节集(DWORD pid, PVOID addr, PVOID pBuf, INT32 size)
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


DWORD64 VU_内存_申请(DWORD 进程ID, DWORD64 进程地址, DWORD64 申请长度, DWORD64 内存属性, BOOL 是否物理)
{


	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	ALLOCATE_FREE_MEMORY_RESULT result = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 进程地址;
	msgInfo.size = 申请长度;
	msgInfo.protection = 内存属性;
	msgInfo.allocate = TRUE;
	msgInfo.physical = 是否物理;
	msgInfo.type = MEM_COMMIT;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &result, sizeof(result), &returnLegth, NULL);
	return result.address;
}


DWORD64 VU_内存_释放(DWORD 进程ID, DWORD64 进程地址, DWORD64 内存长度, BOOL 是否物理)
{


	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	ALLOCATE_FREE_MEMORY_RESULT result = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 进程地址;
	msgInfo.size = 内存长度;
	msgInfo.protection = 0;
	msgInfo.allocate = FALSE;
	msgInfo.physical = 是否物理;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &result, sizeof(result), &returnLegth, NULL);
	return result.address;
}


DWORD64 VU_内存_修改属性(DWORD 进程ID, DWORD64 地址, DWORD64 长度, DWORD64 内存属性)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 地址;
	msgInfo.size = 长度;
	msgInfo.protection = 内存属性;
	msgInfo.allocate = TRUE;
	msgInfo.physical = 1;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_PROTECT, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 VU_内存_查询属性(DWORD 进程ID, DWORD64 地址)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 地址;
	msgInfo.size = 4096;
	msgInfo.protection = 64;
	msgInfo.allocate = FALSE;
	msgInfo.physical = 1;
	msgInfo.type = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_QUERY, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}






DWORD64 VU_HOOK_创建(DWORD 进程ID, DWORD64 目标地址, DWORD64 目的地址, BOOL 硬件物理执行)
{

	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.p目标地址 = 目标地址;
	msgInfo.p目的地址 = 目的地址;
	msgInfo.n命令模式 = 1;
	msgInfo.b是否物理 = 硬件物理执行;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 VU_HOOK_启动(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.p目标地址 = 目标地址;
	msgInfo.p目的地址 = 0;
	msgInfo.n命令模式 = 2;
	msgInfo.b是否物理 = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_暂停(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.p目标地址 = 目标地址;
	msgInfo.p目的地址 = 0;
	msgInfo.n命令模式 = 3;
	msgInfo.b是否物理 = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_移除(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_Hook	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.p目标地址 = 目标地址;
	msgInfo.p目的地址 = 0;
	msgInfo.n命令模式 = 4;
	msgInfo.b是否物理 = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_HOOK_劫持(DWORD 进程ID, DWORD64 目标地址, PVOID 劫持代码, DWORD 代码长度, BOOL 硬件物理执行, DWORD 执行总次数, BOOL 是否保存堆栈)
{
	DWORD len = sizeof(IOCTL_BUFFER_Hook) + 代码长度;
	IOCTL_BUFFER_Hook* pInfo = (IOCTL_BUFFER_Hook*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;

	pInfo->PID = 进程ID;
	pInfo->p目标地址 = 目标地址;
	pInfo->p目的地址 = 是否保存堆栈;
	pInfo->n命令模式 = 5;
	pInfo->b是否物理 = 硬件物理执行;
	pInfo->n次数 = 执行总次数;
	pInfo->Size = 代码长度;
	memcpy(pInfo->Buf, 劫持代码, 代码长度);
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;
}


DWORD64 VU_注入dll(DWORD 进程ID, CHAR* 文件路径, DWORD 注入模式, BOOL 物理硬件执行, DWORD 激活文件方式, BOOL 创建劫持线程,
	BOOL 导入表映射, BOOL 不修复异常处理, PVOID 文件数据, DWORD 文件大小)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectFile) + 文件大小;
	IOCTL_BUFFER_InjectFile* pInfo = (IOCTL_BUFFER_InjectFile*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "%s", 文件路径);
	if (注入模式 == 4)
		sprintf_s(strPath, "\\??\\%s", 文件路径);





	BOOL				bResult = FALSE;
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;

	DWORD64 flages = 0;
	if (注入模式 < 4)
		flages = 2;


	if (注入模式 == 4)
	{
		sprintf_s(strPath, "\\??\\%s", 文件路径);
		flages = 0;
		flages |= 4;
		if (导入表映射)
			flages |= 1;
		if (创建劫持线程)
			flages |= 8;
		if (物理硬件执行)
			flages |= 16;
		if (激活文件方式 == 1)
			flages |= 128;
		if (激活文件方式 == 2)
			flages |= 256;
		if (不修复异常处理)
			flages |= 4096;

		flages |= 32768;
		flages |= 65536;
	}



	pInfo->PID = 进程ID;
	pInfo->Flages = flages;
	pInfo->Type = 注入模式;
	pInfo->str路径 = (DWORD64)&strPath;

	pInfo->Size = 文件大小;
	if (文件数据 && 文件大小)
		memcpy(pInfo->Buf, 文件数据, 文件大小);

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_DLL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;
}




DWORD64 VU_超级注入CALL_置主线程获取模式(DWORD64 模式)
{

	BOOL				bResult = FALSE;
	DWORD64				pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Addr = 模式;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SET_THREADMOD, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_远程注入CALL(DWORD 进程ID, PBYTE CALL代码, DWORD CALL代码长度, BOOL 硬件物理执行)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectCode) + CALL代码长度;
	IOCTL_BUFFER_InjectCode* pInfo = (IOCTL_BUFFER_InjectCode*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);
	pInfo->PID = 进程ID;
	pInfo->isPhyMem = 硬件物理执行;
	pInfo->Size = CALL代码长度;

	memcpy(pInfo->Buf, CALL代码, CALL代码长度);

	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;

}


DWORD64 VU_远程执行CALL(DWORD 进程ID, DWORD64 目标地址, DWORD64 参数地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_InjectCode	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.Size = 参数地址;
	msgInfo.Addr = 目标地址;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_超级注入CALL(DWORD 进程ID, PBYTE CALL代码, DWORD CALL代码长度, DWORD 线程ID, BOOL 硬件物理执行)
{
	DWORD len = sizeof(IOCTL_BUFFER_InjectCode) + CALL代码长度;
	IOCTL_BUFFER_InjectCode* pInfo = (IOCTL_BUFFER_InjectCode*)malloc(len);
	if (!pInfo)
		return FALSE;
	ZeroMemory(pInfo, len);

	pInfo->PID = 进程ID;
	pInfo->isPhyMem = 硬件物理执行;
	pInfo->Size = CALL代码长度;

	memcpy(pInfo->Buf, CALL代码, CALL代码长度);
	pInfo->Addr = 线程ID;


	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;


	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALL, pInfo, len, &pAddr, sizeof(pAddr), &returnLegth, NULL);
	free(pInfo);
	return pAddr;

}


DWORD64 VU_超级执行CALL(DWORD 进程ID, DWORD64 目标地址, DWORD64 参数地址, DWORD 线程ID)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	IOCTL_BUFFER_InjectCode	msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	msgInfo.Addr = 目标地址;
	msgInfo.Size = 参数地址;
	msgInfo.isPhyMem = 线程ID;


	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


BOOL VU_键鼠_安装()
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_INIT, 0, 0, 0, 0, &returnLegth, NULL);

}


//其他代码请参考易语言模块源码,里面有具体封装键盘鼠标操作的完整代码
//这一部分封装成C的,可得累死人了,本人偷懒不弄了,谁如果弄了,希望有机会开个源,方便大家使用
VOID VU_键鼠_鼠标操作(MOUSE_INPUT_DATA 鼠标)
{
	DWORD				returnLegth = 0;
	if (鼠标.Flags == 1)
	{
		鼠标.LastX = 鼠标.LastX * 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
		鼠标.LastY = 鼠标.LastY * 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);
	}
	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_MOUSE, &鼠标, sizeof(鼠标), 0, 0, &returnLegth, NULL);
}

VOID VU_键鼠_键盘操作(KEYBOARD_INPUT_DATA 键盘)
{
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_KEYBOARD, &键盘, sizeof(键盘), 0, 0, &returnLegth, NULL);
}



BOOL VU_隐藏文件_安装()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_INIT, 0, 0, 0, 0, &returnLegth, NULL);
}

BOOL VU_隐藏文件_卸载()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_UNINIT, 0, 0, 0, 0, &returnLegth, NULL);
}


DWORD64 VU_隐藏文件_文件(CHAR* 文件路径)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", 文件路径);
	msgInfo.Buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_FILE, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 VU_隐藏文件_目录(CHAR* 目录路径)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", 目录路径);
	msgInfo.Buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_DIR, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}





BOOL VU_保护_安装()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 1;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_卸载()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_进程_开始(DWORD64 进程ID, BOOL 是否保护驱动)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 进程ID;
	if (是否保护驱动)
		msgInfo.Size = 100;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_进程_结束(DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.PID = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_保护_窗口_开始(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = 窗口句柄;
	msgInfo.PID = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_窗口_结束(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = 窗口句柄;
	msgInfo.PID = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_游戏_开始(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = 窗口句柄;
	msgInfo.PID = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL VU_保护_游戏_结束(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = 窗口句柄;
	msgInfo.PID = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}



BOOL VU_硬件_修改硬盘序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)序号;
	msgInfo.Size = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_DISK, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_硬件_修改声卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)序号;
	msgInfo.Size = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_VOLUMES, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_硬件_修改网卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)序号;
	msgInfo.Size = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_MAC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_硬件_修改主板序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER				msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)序号;
	msgInfo.Size = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_SMBOIS, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL VU_硬件_修改显卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	IOCTL_BUFFER		msgInfo = { 0 };
	msgInfo.Buf = (DWORD64)序号;
	msgInfo.Size = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_GPU, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}