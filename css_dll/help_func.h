#pragma once
#include <Windows.h>

//��麯���ķ���ֵ
void check_error(bool state, const char* str = nullptr)
{
	if (state) return;

	char buffer[1024 * 2];
	wsprintf(buffer, "�������� : %s", str);
	MessageBox(nullptr, buffer, nullptr, MB_OK | MB_ICONHAND);
	exit(-1);
}
