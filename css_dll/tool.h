#pragma once
#include "data.h"

template<class T>
T read_memory(uintptr_t address) {
	T t;
	ReadProcessMemory(data::handle, reinterpret_cast<LPVOID>(address), &t, sizeof(T), nullptr);
	return t;
}

template<class T>
void read_list(uintptr_t address, void* t, int length) {
	ReadProcessMemory(data::handle, reinterpret_cast<LPVOID>(address), t, length * (sizeof(T)), nullptr);
}

template<class T>
T read_multilevel(uintptr_t address, std::vector<uintptr_t> Offset_array) {
	for (int i = 0; i < Offset_array.size(); i++) {
		if (i + 1 == Offset_array.size()) {
			return (read_memory<T>(address + Offset_array[i]));
		}
		else {
			address = read_memory<int>(address + Offset_array[i]);
		}
	}
}

template<class T>
void write_memory(uintptr_t address, T write) {
	WriteProcessMemory(data::handle, reinterpret_cast<LPVOID>(address), &write, sizeof(T), NULL);
}

template<class T>
void write_multilevel(uintptr_t address, std::vector<uintptr_t> Offset_array, T write) {
	for (int i = 0; i < Offset_array.size(); i++) {
		if (i + 1 == Offset_array.size()) {
			write_memory<T>(address + Offset_array[i], write);
		}
		else {
			address = read_memory<int>(address + Offset_array[i]);
		}
	}
}