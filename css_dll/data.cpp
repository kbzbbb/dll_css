#include "data.h"

namespace data {
	 int pid = 0;
	HANDLE handle = nullptr;
	HWND Game_handle = 0x0;
	uintptr_t module = 0x0;
	uintptr_t module2 = 0x0;

	//ƫ��
	std::vector<uintptr_t> gamer_quantity = { 0x0058D758,0x500,0x4,0xC,0x4 };
	std::vector<uintptr_t> w = { 0x004F615C};
	std::vector<uintptr_t> Matrix_address = { 0x005AAAA4 };

	//����
	bool line = false;
	bool life = false;
	bool pane = false;
	bool lock = false;
	bool round_range = false;
	bool teammate = false;
	bool Crosshairs = false;

	float round = 100.0f;
}