#include "pch.h"
#include "ReadWrite.h"

#include "VoouerDrv.h"

INT64 ReadWrite::内存申请(DWORD 进程ID, DWORD64 进程地址, DWORD64 申请长度) {
	return 内存_申请(进程ID, 进程地址, 申请长度,64,false);
}