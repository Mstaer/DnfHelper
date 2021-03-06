#pragma once

#include "Helper.h"

class ReadWrite
{
public:

	template<class T>
	inline T 万能读(DWORD 进程ID, ULONG 进程地址)
	{
		T buffer;
		VU_内存_读字节集(进程ID, &进程地址, (PVOID)&buffer, sizeof(T));
		return buffer;
	}

	template <class T>
	BOOL 万能写(DWORD 进程ID, ULONG 进程地址, T 写入数据) {
		return VU_内存_写字节集(进程ID, &进程地址, 写入数据, sizeof(T));
	}

	//vector<int> 内存读字节集(ULONG 进程ID, ULONG 进程地址, INT32 读取长度);

	//BYTE 内存读字节集(ULONG 进程ID, ULONG 进程地址);

	//FLOAT 内存读小数(ULONG 进程ID, ULONG 进程地址);

	//INT 内存读整数(ULONG 进程ID, ULONG 进程地址);

	//INT64 内存读长整数(ULONG 进程ID, ULONG 进程地址);

	//BOOL 内存写字节(ULONG 进程ID, ULONG 进程地址, BYTE 写入数值);

	//BOOL 内存写字节集(ULONG 进程ID, ULONG 进程地址, vector<byte> 写入数据);

	//BOOL 内存写小数(ULONG 进程ID, ULONG 进程地址, FLOAT 写入数值);

	//BOOL 内存写整数(ULONG 进程ID, ULONG 进程地址, DWORD 写入数值);

	//BOOL 内存写长整数(ULONG 进程ID, ULONG 进程地址, DWORD64 写入数值);

	//INT64 内存申请(ULONG 进程ID, ULONG 进程地址, DWORD64 申请长度);
};
