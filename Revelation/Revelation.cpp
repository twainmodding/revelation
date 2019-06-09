#include "stdafx.h"
#include "Stubs.h"
#include "MW2.h"
#include "Utils.h"
#include "MW3.h"

hook xex_load_executable_h;
game* current_game;

void handle_title_change(const DWORD title_id)
{
	if (current_game)
	{
		debug_print("Title changed - Unloading Menu");

		//CurrentGame->UnloadMenu();

		current_game = nullptr;
	}

	switch (title_id)
	{
	case cod_modern_warfare_2:
		debug_print("Modern Warfare 2 launched");

		current_game = new mw2();
		break;
	case cod_modern_warfare_3:
		debug_print("Modern Warfare 3 launched");

		current_game = new mw3();
		break;
	default:
		return;
	}

	if (current_game)
	{
		debug_print("Title changed - Loading Menu");

		current_game->load_menu();
	}
}

void* xex_load_executable_hook(char* sz_xex_name, const void* p_handle, const unsigned long dw_module_type_flags,
                               const unsigned long dw_minimum_version)
{
	const auto result = xex_load_executable_stub(sz_xex_name, p_handle, dw_module_type_flags, dw_minimum_version);

	if (result == nullptr)
	{
		handle_title_change(xam_get_current_title_id());
	}

	return result;
}

bool APIENTRY DllMain(HANDLE h_module, const DWORD dw_reason, LPVOID lp_reserved)
{
	if (dw_reason == DLL_PROCESS_ATTACH)
	{
		if (is_tray_open())
		{
			debug_print("Plugin blocked (Disc Tray open)");

			return false;
		}

		debug_print("Attaching");

		handle_title_change(xam_get_current_title_id());

		xex_load_executable_h = hook(resolve_function("xboxkrnl.exe", 0x198),
		                             reinterpret_cast<unsigned long*>(xex_load_executable_stub),
		                             reinterpret_cast<unsigned long>(xex_load_executable_hook));
		xex_load_executable_h.start();

		debug_print("Attached");
	}

	if (dw_reason == DLL_PROCESS_DETACH)
	{
		debug_print("Detaching");

		xex_load_executable_h.stop();

		if (current_game)
		{
			current_game->unload_menu();
		}

		debug_print("Detached");
	}

	return true;
}
