#pragma once
#include <Windows.h>
#include <string>
#include <tlhelp32.h>
#include <vector>

class Figure_Data {
public:
	float life;
	int camp;
	float x;
	float y;
	float z;
};

namespace data {
	extern int pid;
	extern HANDLE handle;
	extern  HWND Game_handle;
	extern uintptr_t module;
	extern uintptr_t module2;

	//Æ«ÒÆ
	extern std::vector<uintptr_t> gamer_quantity;
	extern std::vector<uintptr_t> w;
	extern std::vector<uintptr_t> Matrix_address;

	//¿ª¹Ø
	extern bool line;
	extern bool life;
	extern bool pane;
	extern bool lock;
	extern bool round_range;
	extern bool teammate;
	extern bool Crosshairs;

	extern float round;
}