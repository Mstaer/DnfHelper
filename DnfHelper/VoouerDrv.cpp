#include "pch.h"
#include "VoouerDrv.h"

static HANDLE g_hDrv = INVALID_HANDLE_VALUE;

VOID 设置驱动句柄(HANDLE hDrv)
{
	g_hDrv = hDrv;
	return;
}

LONG 效验有效性(CHAR* key)
{
	MsgInfo			msgInfo = { 0 };
	msgInfo.m_buf = (ULONG64)key;
	LONG 返回数据 = 0;
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_FILE_CHECK, &msgInfo, sizeof(msgInfo), &返回数据, sizeof(返回数据), &returnLegth, NULL);

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

LONG 内存_置读写模式(DWORD 模式, DWORD 类型)
{
	BOOL				bResult = FALSE;
	LONG				returnCode = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_pRWAddr = 模式;
	msgInfo.m_nRWSize = 类型;
	bResult = DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_MOD, &msgInfo, sizeof(msgInfo), &returnCode, sizeof(returnCode), &returnLegth, NULL);
	return returnCode;
}

//枚举指定进程模块 
DWORD64	内存_取模块基址(DWORD pid, const char* moduleName)
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


//读内存函数，自己封装
BOOL 内存_读字节集(ULONG pid, ULONG addr, PVOID pBuf, INT32 size)
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

//写内存函数，自己封装
BOOL 内存_写字节集(ULONG pid, ULONG addr, ULONG pBuf, ULONG size)
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


DWORD64 内存_申请(DWORD 进程ID, DWORD64 进程地址, DWORD64 申请长度, DWORD64 内存属性, BOOL 是否物理)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 进程地址;
	msgInfo.size = 申请长度;
	msgInfo.protection = 内存属性;
	msgInfo.allocate = TRUE;
	msgInfo.physical = 是否物理;
	msgInfo.type = MEM_COMMIT;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 内存_释放(DWORD 进程ID, DWORD64 进程地址, DWORD64 内存长度, BOOL 是否物理)
{
	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	ALLOCATE_FREE_MEMORY	msgInfo = { 0 };
	msgInfo.pid = 进程ID;
	msgInfo.base = 进程地址;
	msgInfo.size = 内存长度;
	msgInfo.protection = 0;
	msgInfo.allocate = FALSE;
	msgInfo.physical = 是否物理;
	msgInfo.type = MEM_RELEASE;

	DeviceIoControl(g_hDrv, IOCTL_IO_MEMORY_ALLOC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 内存_修改属性(DWORD 进程ID, DWORD64 地址, DWORD64 长度, DWORD64 内存属性)
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


DWORD64 内存_查询属性(DWORD 进程ID, DWORD64 地址)
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






DWORD64 HOOK_创建(DWORD 进程ID, DWORD64 目标地址, DWORD64 目的地址, BOOL 硬件物理执行)
{

	DWORD64 pAddr = 0;

	DWORD					returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_pRWAddr = 目标地址;
	msgInfo.m_buf = 目的地址;
	msgInfo.m_nRWSize = 1;
	msgInfo.m_isPhyMem = 硬件物理执行;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 HOOK_启动(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_pRWAddr = 目标地址;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 2;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 HOOK_暂停(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_pRWAddr = 目标地址;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 3;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 HOOK_移除(DWORD 进程ID, DWORD64 目标地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_pRWAddr = 目标地址;
	msgInfo.m_buf = 0;
	msgInfo.m_nRWSize = 4;
	msgInfo.m_isPhyMem = 0;
	DeviceIoControl(g_hDrv, IOCTL_IO_HOOK_CTL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


DWORD64 注入dll(DWORD 进程ID, CHAR* dll路径, DWORD 注入模式, BOOL 断链, BOOL 擦除PE头, BOOL 导入表映射,
	BOOL 创建劫持线程, BOOL 分配内核内存, BOOL 劫持映射, BOOL 不修复异常处理, BOOL 不激活上下文, BOOL 不初始化回调)
{
	BOOL				bResult = FALSE;
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo			msgInfo = { 0 };
	DWORD64 flages = 0;
	if (注入模式 < 4)
	{
		if (断链)
			flages = 1;
		if (擦除PE头)
			flages = 2;
	}
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "%s", dll路径);
	if (注入模式 == 4)
	{
		sprintf_s(strPath, "\\??\\%s", dll路径);
		flages = 0;
		if (导入表映射)
			flages |= 1;
		if (擦除PE头)
			flages |= 4;
		if (创建劫持线程)
			flages |= 8;
		if (分配内核内存)
			flages |= 16;
		if (劫持映射)
			flages |= 128;
		if (不修复异常处理)
			flages |= 4096;
		if (不激活上下文)
			flages |= 32768;
		if (不初始化回调)
			flages |= 65536;
	}



	msgInfo.m_ProcessId = (ULONG)进程ID;
	msgInfo.m_pRWAddr = flages;
	msgInfo.m_nRWSize = 注入模式;
	msgInfo.m_buf = (DWORD64)&strPath;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_DLL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}




DWORD64 超级注入CALL_置主线程获取模式(DWORD64 模式)
{

	BOOL				bResult = FALSE;
	DWORD64				pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_pRWAddr = 模式;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SET_THREADMOD, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 远程注入CALL(DWORD 进程ID, PBYTE CALL代码, DWORD CALL代码长度, BOOL 硬件物理执行)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_buf = (DWORD64)CALL代码;
	msgInfo.m_nRWSize = CALL代码长度;
	msgInfo.m_isPhyMem = 硬件物理执行;
	msgInfo.m_pRWAddr = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;

}


DWORD64 远程执行CALL(DWORD 进程ID, DWORD64 目标地址, DWORD64 参数地址)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	MsgInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_nRWSize = 参数地址;
	msgInfo.m_pRWAddr = 目标地址;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_REMOTE_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 超级注入CALL(DWORD 进程ID, PBYTE CALL代码, DWORD CALL代码长度, DWORD 线程ID, BOOL 硬件物理执行)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_buf = (DWORD64)CALL代码;
	msgInfo.m_nRWSize = CALL代码长度;
	msgInfo.m_isPhyMem = 硬件物理执行;
	msgInfo.m_pRWAddr = 线程ID;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALL, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;

}


DWORD64 超级执行CALL(DWORD 进程ID, DWORD64 目标地址, DWORD64 参数地址, DWORD 线程ID)
{
	DWORD64 pAddr = 0;
	DWORD	returnLegth = 0;
	InjectInfo	msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;
	msgInfo.m_buf = 目标地址;
	msgInfo.m_nRWSize = 参数地址;
	msgInfo.m_pRWAddr = 线程ID;
	msgInfo.m_isPhyMem = 1;

	DeviceIoControl(g_hDrv, IOCTL_IO_INJECT_SUPER_CALLEX, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}


BOOL 键鼠_安装()
{
	BOOL				bResult = FALSE;
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_INIT, 0, 0, 0, 0, &returnLegth, NULL);

}


//其他代码请参考易语言模块源码,里面有具体封装键盘鼠标操作的完整代码
//这一部分封装成C的,可得累死人了,本人偷懒不弄了,谁如果弄了,希望有机会开个源,方便大家使用
VOID 键鼠_鼠标操作(MOUSE_INPUT_DATA 鼠标)
{
	DWORD				returnLegth = 0;
	if (鼠标.Flags == 1)
	{
		鼠标.LastX = 鼠标.LastX * 65535 / (GetSystemMetrics(SM_CXSCREEN) - 1);
		鼠标.LastY = 鼠标.LastY * 65535 / (GetSystemMetrics(SM_CYSCREEN) - 1);
	}
	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_MOUSE, &鼠标, sizeof(鼠标), 0, 0, &returnLegth, NULL);
}

VOID 键鼠_键盘操作(KEYBOARD_INPUT_DATA 键盘)
{
	DWORD				returnLegth = 0;

	DeviceIoControl(g_hDrv, IOCTL_IO_INPUT_KEYBOARD, &键盘, sizeof(键盘), 0, 0, &returnLegth, NULL);
}



BOOL 隐藏文件_安装()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_INIT, 0, 0, 0, 0, &returnLegth, NULL);
}

BOOL 隐藏文件_卸载()
{
	DWORD				returnLegth = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_UNINIT, 0, 0, 0, 0, &returnLegth, NULL);
}


DWORD64 隐藏文件_文件(CHAR* 文件路径)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", 文件路径);
	msgInfo.m_buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_FILE, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}

DWORD64 隐藏文件_目录(CHAR* 目录路径)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };

	CHAR strPath[MAX_PATH] = { 0 };
	sprintf_s(strPath, "\\??\\%s", 目录路径);
	msgInfo.m_buf = (DWORD64)&strPath;
	DeviceIoControl(g_hDrv, IOCTL_IO_FILTER_DIR, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
	return pAddr;
}





BOOL 保护_安装()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 1;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_卸载()
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 0;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_BEGIN_OR_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_进程_开始(DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_进程_结束(DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_PROCESS_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL 保护_窗口_开始(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = 窗口句柄;
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_窗口_结束(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = 窗口句柄;
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_WINDOW_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_游戏_开始(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = 窗口句柄;
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_BEGIN, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}

BOOL 保护_游戏_结束(DWORD64 窗口句柄, DWORD64 进程ID)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = 窗口句柄;
	msgInfo.m_ProcessId = 进程ID;

	return DeviceIoControl(g_hDrv, IOCTL_IO_PROTECT_GAME_END, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}



BOOL 硬件_修改硬盘序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)序号;
	msgInfo.m_nRWSize = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_DISK, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL 硬件_修改声卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)序号;
	msgInfo.m_nRWSize = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_VOLUMES, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL 硬件_修改网卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)序号;
	msgInfo.m_nRWSize = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_MAC, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL 硬件_修改主板序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)序号;
	msgInfo.m_nRWSize = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_SMBOIS, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}


BOOL 硬件_修改显卡序号(CHAR* 序号, DWORD64 文本长度)
{
	DWORD64 pAddr = 0;
	DWORD				returnLegth = 0;
	MsgInfo				msgInfo = { 0 };
	msgInfo.m_buf = (DWORD64)序号;
	msgInfo.m_nRWSize = 文本长度;

	return DeviceIoControl(g_hDrv, IOCTL_IO_HARDWARE_GPU, &msgInfo, sizeof(msgInfo), &pAddr, sizeof(pAddr), &returnLegth, NULL);
}