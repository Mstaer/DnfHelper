#include "pch.h"
#include "ReadWrite.h"
#include "Helper.h"
#include "VoouerDrv.h"

INT64 ReadWrite::内存申请(ULONG 进程ID, ULONG 进程地址, DWORD64 申请长度) {
	return 内存_申请(进程ID, 进程地址, 申请长度,64,false);
}

vector<int> ReadWrite::内存读字节集(ULONG 进程ID, ULONG 进程地址, INT32 读取长度) {
	vector<int> result;
	内存_读字节集(进程ID, 进程地址, &result, 读取长度);
	return result;
}

FLOAT ReadWrite::内存读小数(ULONG 进程ID, ULONG 进程地址) {
	FLOAT result;
	内存_读字节集(进程ID, 进程地址, &result, (INT32)4);
	return result;
}

INT ReadWrite::内存读整数(ULONG 进程ID, ULONG 进程地址) {
	INT result;
	内存_读字节集(进程ID, 进程地址, &result, (INT32)4);
	return result;
}

BOOL ReadWrite::内存写字节集(ULONG 进程ID, ULONG 进程地址, vector<byte> 写入数据) {
	vector<byte> *result = &写入数据;
	return 内存_写字节集(进程ID, 进程地址, (ULONG)result, sizeof(result));
}

INT64 ReadWrite::内存读长整数(ULONG 进程ID, ULONG 进程地址) {
	INT64 result;
	内存_读字节集(进程ID, 进程地址, &result, (INT32)8);
	return result;
}

BOOL ReadWrite::内存写小数(ULONG 进程ID, ULONG 进程地址, FLOAT 写入数据) {
	byte result = (byte)写入数据;
	return 内存_写字节集(进程ID, 进程地址, result, sizeof(result));
}

BOOL ReadWrite::内存写整数(ULONG 进程ID, ULONG 进程地址, DWORD 写入数据) {
	byte result = (byte)写入数据;
	return 内存_写字节集(进程ID, 进程地址, result, sizeof(result));
}

BOOL ReadWrite::内存写长整数(ULONG 进程ID, ULONG 进程地址, DWORD64 写入数据) {
	byte result = (byte)写入数据;
	return 内存_写字节集(进程ID, 进程地址, result, sizeof(result));
}