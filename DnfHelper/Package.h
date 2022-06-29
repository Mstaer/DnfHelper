#pragma once

// 游戏组包
class Package
{
public:
	VOID 发包CALL();
	VOID 缓冲CALL(int 包头);
	VOID 加密CALL(INT64 参数,INT 长度);
};

