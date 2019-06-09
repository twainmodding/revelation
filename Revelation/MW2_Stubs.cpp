#include "stdafx.h"

void sv_execute_client_command_stub(...)
{
	__asm
	{
		li r3, 2
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
