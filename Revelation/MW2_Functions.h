#pragma once
void sv_cmd_end_tokenized_string();

void sv_cmd_tokenize_string(const char* cmd);

void client_command(int client_index);

int get_client_index_from_client_s(int client_s);

char* dvar_get_string(const char* dvar);

bool dvar_get_bool(const char* dvar);

int dvar_get_int(const char* dvar);
