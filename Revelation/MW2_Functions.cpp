#include "stdafx.h"
#include "MW2_Functions.h"
#include "MW2_Addresses.h"

void sv_cmd_end_tokenized_string()
{
	reinterpret_cast<void(*)(...)>(sv_cmd_end_tokenized_string_address)();
}

void sv_cmd_tokenize_string(const char* cmd)
{
	reinterpret_cast<void(*)(...)>(sv_cmd_tokenize_string_address)(cmd);
}

void client_command(const int client_index)
{
	reinterpret_cast<void(*)(...)>(client_command_address)(client_index);
}

int get_client_index_from_client_s(const int client_s)
{
	return (client_s - *reinterpret_cast<int*>(client_s_address)) / 0x97F80;
}

char* dvar_get_string(const char* dvar)
{
	return reinterpret_cast<char* (*)(...)>(dvar_get_string_address)(dvar);
}

bool dvar_get_bool(const char* dvar)
{
	return reinterpret_cast<bool(*)(...)>(dvar_get_bool_address)(dvar);
}

int dvar_get_int(const char* dvar)
{
	return reinterpret_cast<int(*)(...)>(dvar_get_int_address)(dvar);
}
