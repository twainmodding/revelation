#include "stdafx.h"
#include "MW2.h"
#include "Utils.h"
#include "MW2_Stubs.h"
#include "MW2_Addresses.h"
#include "MW2_Functions.h"

hook xam_input_get_state_h;
hook sv_execute_client_command_h;

unsigned long xam_input_get_state_hook(const int user_index, const int flags, const PXINPUT_STATE p_state)
{
	if (user_index != 0)
		return XInputGetStateEx(user_index, flags, p_state);

	if (dvar_get_bool("cl_ingame"))
	{
	}
	else
	{
	}

	return XInputGetStateEx(user_index, flags, p_state);
}

void sv_execute_client_command_hook(const unsigned long cl, const char* s, const bool client_ok)
{
	const auto client_index = get_client_index_from_client_s(cl);

	sv_cmd_tokenize_string(s);
	client_command(client_index);
	sv_cmd_end_tokenized_string();

	sv_execute_client_command_stub(cl, s, client_ok);
}

void mw2::load_menu()
{
	debug_print("Initializing Modern Warfare 2");

	xam_input_get_state_h = hook(xam_input_get_state_address,
	                             reinterpret_cast<unsigned long>(xam_input_get_state_hook));
	sv_execute_client_command_h = hook(sv_execute_client_command_address,
	                                   reinterpret_cast<unsigned long*>(sv_execute_client_command_stub),
	                                   reinterpret_cast<unsigned long>(sv_execute_client_command_hook));

	xam_input_get_state_h.start();
	sv_execute_client_command_h.start();

	debug_print("Initialized Modern Warfare 2");

	x_notify(L"Revelation - Modern Warfare 2 Loaded");
}

void mw2::unload_menu()
{
	debug_print("Stopping Modern Warfare 2");

	xam_input_get_state_h.stop();
	sv_execute_client_command_h.stop();

	debug_print("Stopped Modern Warfare 2");
}
