#pragma once

#include "Helper.h"

class ReadWrite
{
public:
	vector<int> �ڴ���ֽڼ�(ULONG ����ID, ULONG ���̵�ַ, INT32 ��ȡ����);

	BYTE �ڴ���ֽڼ�(ULONG ����ID, ULONG ���̵�ַ);

	FLOAT �ڴ��С��(ULONG ����ID, ULONG ���̵�ַ);

	INT �ڴ������(ULONG ����ID, ULONG ���̵�ַ);

	INT64 �ڴ��������(ULONG ����ID, ULONG ���̵�ַ);

	BOOL �ڴ�д�ֽ�(ULONG ����ID, ULONG ���̵�ַ, BYTE д����ֵ);

	BOOL �ڴ�д�ֽڼ�(ULONG ����ID, ULONG ���̵�ַ, vector<byte> д������);

	BOOL �ڴ�дС��(ULONG ����ID, ULONG ���̵�ַ, FLOAT д����ֵ);

	BOOL �ڴ�д����(ULONG ����ID, ULONG ���̵�ַ, DWORD д����ֵ);

	BOOL �ڴ�д������(ULONG ����ID, ULONG ���̵�ַ, DWORD64 д����ֵ);

	INT64 �ڴ�����(ULONG ����ID, ULONG ���̵�ַ, DWORD64 ���볤��);
};
