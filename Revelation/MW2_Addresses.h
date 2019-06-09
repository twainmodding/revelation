#pragma once
enum mw2_addresses : unsigned int
{
	xam_input_get_state_address = 0x823B64C4,
	sv_execute_client_command_address = 0x82253140,
	sv_cmd_end_tokenized_string_address = 0x822252D0,
	sv_cmd_tokenize_string_address = 0x822252B0,
	client_command_address = 0x821DC5A0,
	client_s_address = 0x83623B98,
	dvar_get_string_address = 0x8229F0A8,
	dvar_get_bool_address = 0x8229EEE8,
	dvar_get_int_address = 0x8229EF50
};
