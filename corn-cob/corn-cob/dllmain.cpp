// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "hook.h"
#include <detours.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include <string>
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>

std::mutex log_lock;
std::vector<std::string> sLog;

void async_log(const std::string& str)
{
	std::scoped_lock lock(log_lock);
	sLog.push_back(str);
}

#pragma optimize("", off)
typedef int(__cdecl* rand_t)();
rand_t original_rand;
int hk_rand() {
	int ret_addr = 0;
	__asm {
		mov ret_addr, esp
	}
	static int rng = original_rand();
	std::stringstream out;
	out //<< std::chrono::system_clock::now() 
		<< ",RAND," 
		<< std::hex << rng << std::dec << "," 
		<< std::this_thread::get_id() << ","
		<< std::hex << ret_addr << std::dec << "\n";
	async_log(out.str());
	return rng;
}

typedef void(__cdecl* srand_t)(unsigned seed);
srand_t original_srand;
void hk_srand(unsigned seed) {
	std::stringstream out;
	original_srand(seed);
	
	int ret_addr = 0;
	__asm {
		mov ret_addr, esp
	}

	out //<< std::chrono::system_clock::now() 
		<< ",SRAND," 
		<< std::hex << seed << std::dec << "," 
		<< std::this_thread::get_id() << ","
		<< std::hex << ret_addr << std::dec << "\n";
	async_log(out.str());
}

typedef int(*oblivion_rng_t)(int new_seed);
oblivion_rng_t original_oblivion_rng;
int hk_oblivion_rng(int new_seed) {
	int ret_addr = 0;
	__asm {
		mov ret_addr, esp
	}

	std::stringstream out;
	out //<< std::chrono::system_clock::now()
		<< ",OBVRAND," 
		<< std::hex << new_seed << std::dec << ","
		<< std::this_thread::get_id() << ","
		<< std::hex << ret_addr << std::dec << "\n";
	async_log(out.str());
	//std::ofstream out("test.log", std::ios::app);

	//auto now = std::chrono::system_clock::now();

	//if (new_seed) {
		//out << now <<  "| RNG Seed : " << new_seed << "\n";
	//}
	
	int rng = original_oblivion_rng(new_seed);
	//out << now << "| RNG Val  : " << rng << "\n";

	//out.close();
	return rng;
}

#pragma optimize("", on)

void RealMain() {
	using namespace std::chrono_literals;

	std::vector<hook> hooks;

	std::string exe_name;
	char buf[MAX_PATH];
	if (GetModuleFileNameA(nullptr, buf, MAX_PATH)) {
		std::string exe_full_path(buf);
		auto last = exe_full_path.find_last_of('\\');
		if (last != std::string::npos) {
			exe_name = std::string(exe_full_path, last + 1);
		}
	}

	// OblivionLauncher.exe also uses these functions but we don't want to mess with that
	if (exe_name == "Oblivion.exe") {

		hook GlobalHook_rand(L"hk_rand", L"Oblivion.exe", 0x9859D0 - 0x400000, (PVOID**)&original_rand, hk_rand);
		//hook GlobalHook_rand(L"hk_rand", (PVOID**)&original_rand, hk_rand);
		hook GlobalHook_srand(L"hk_srand", L"Oblivion.exe", 0x9859DD - 0x400000, (PVOID**)&original_srand, hk_srand);
		//hook GlobalHook_srand(L"hk_srand", (PVOID**)&original_srand, hk_srand);
		//hook GlobalHook_oblivion_rng_func(L"oblivion_rng", L"Oblivion.exe", 0x47DF80 - 0x400000, (PVOID**)&original_oblivion_rng, hk_oblivion_rng);

		original_rand = rand;
		original_srand = srand;

		//hooks.push_back(GlobalHook_oblivion_rng_func);
		hooks.push_back(GlobalHook_rand);
		hooks.push_back(GlobalHook_srand);

		for (auto& hk : hooks) {
			if (!hk.is_installed()) {
				hk.attach();
			}
		}
	}

	// Stall
	while (true) {
		std::this_thread::sleep_for(100ms); 
		std::ofstream out("rng.csv", std::ios::app);
		{
			std::scoped_lock lock(log_lock);
			for (auto& str : sLog)
			{
				out << str;
			}
			sLog.clear();
		}
		out.close();
	}

	for (auto& hk : hooks) {
		//hk.detach();
	}
}

DWORD WINAPI MainThread(HMODULE hDLL) {
	RealMain();

	Sleep(200);
	FreeLibraryAndExitThread(hDLL, NULL);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DWORD dwThreadID;
		CreateThread(0, 0x1000, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, &dwThreadID);
	}
	break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	break;
	}
	return TRUE;
}

