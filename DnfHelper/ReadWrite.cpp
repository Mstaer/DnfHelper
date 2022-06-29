#include "pch.h"
#include "ReadWrite.h"
#include "Helper.h"
#include "VoouerDrv.h"

INT64 ReadWrite::�ڴ�����(ULONG ����ID, ULONG ���̵�ַ, DWORD64 ���볤��) {
	return �ڴ�_����(����ID, ���̵�ַ, ���볤��,64,false);
}

vector<int> ReadWrite::�ڴ���ֽڼ�(ULONG ����ID, ULONG ���̵�ַ, INT32 ��ȡ����) {
	vector<int> result;
	�ڴ�_���ֽڼ�(����ID, ���̵�ַ, &result, ��ȡ����);
	return result;
}

FLOAT ReadWrite::�ڴ��С��(ULONG ����ID, ULONG ���̵�ַ) {
	FLOAT result;
	�ڴ�_���ֽڼ�(����ID, ���̵�ַ, &result, (INT32)4);
	return result;
}

INT ReadWrite::�ڴ������(ULONG ����ID, ULONG ���̵�ַ) {
	INT result;
	�ڴ�_���ֽڼ�(����ID, ���̵�ַ, &result, (INT32)4);
	return result;
}

BOOL ReadWrite::�ڴ�д�ֽڼ�(ULONG ����ID, ULONG ���̵�ַ, vector<byte> д������) {
	vector<byte> *result = &д������;
	return �ڴ�_д�ֽڼ�(����ID, ���̵�ַ, (ULONG)result, sizeof(result));
}

INT64 ReadWrite::�ڴ��������(ULONG ����ID, ULONG ���̵�ַ) {
	INT64 result;
	�ڴ�_���ֽڼ�(����ID, ���̵�ַ, &result, (INT32)8);
	return result;
}

BOOL ReadWrite::�ڴ�дС��(ULONG ����ID, ULONG ���̵�ַ, FLOAT д������) {
	byte result = (byte)д������;
	return �ڴ�_д�ֽڼ�(����ID, ���̵�ַ, result, sizeof(result));
}

BOOL ReadWrite::�ڴ�д����(ULONG ����ID, ULONG ���̵�ַ, DWORD д������) {
	byte result = (byte)д������;
	return �ڴ�_д�ֽڼ�(����ID, ���̵�ַ, result, sizeof(result));
}

BOOL ReadWrite::�ڴ�д������(ULONG ����ID, ULONG ���̵�ַ, DWORD64 д������) {
	byte result = (byte)д������;
	return �ڴ�_д�ֽڼ�(����ID, ���̵�ַ, result, sizeof(result));
}