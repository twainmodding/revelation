#include "stdafx.h"

void* xex_load_executable_stub(...)
{
	__asm
	{
		li r3, 100
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}
