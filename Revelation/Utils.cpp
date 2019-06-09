#include "stdafx.h"
#include "Utils.h"

void debug_print(const string& value)
{
#ifndef NDEBUG
	auto t = time(nullptr);
	auto tm = *localtime(&t);
	cout << put_time(&tm, "[%H:%M:%S] ") << "[Revelation] " << value << endl;
#endif
}

void hal_send_smc_message(void* p_command_buffer, void* p_recv_buffer)
{
	reinterpret_cast<void(*)(...)>(resolve_function("xboxkrnl.exe", 0x29))(p_command_buffer, p_recv_buffer);
}

bool is_tray_open()
{
	unsigned char msg[0x10];
	unsigned char resp[0x10];
	memset(msg, 0x0, 0x10);
	msg[0] = 0xa;

	// HalSendSMCMessage(msg, resp);
	hal_send_smc_message(msg, resp);

	return resp[1] == 0x60;
}

unsigned long xam_get_current_title_id()
{
	return reinterpret_cast<unsigned long(*)(...)>(resolve_function("xam.xex", 0x1CF))();
}

void x_notify_queue_ui(const XNOTIFYQUEUEUI_TYPE type, const unsigned long dw_user_index,
                       const unsigned long long qw_areas, wchar_t* display_text, void* context_data)
{
	reinterpret_cast<void(*)(...)>(resolve_function("xam.xex", 0x290))(type, dw_user_index, qw_areas, display_text,
	                                                                   context_data);
}

void x_notify(const wstring& value, const XNOTIFYQUEUEUI_TYPE type)
{
	x_notify_queue_ui(type, 0xFF, XNOTIFY_SYSTEM, const_cast<wchar_t*>(value.c_str()), nullptr);
}

unsigned long resolve_function(const char* module_name, const unsigned long ordinal)
{
	const auto m_handle = GetModuleHandle(module_name);

	return (m_handle == nullptr)
		       ? NULL
		       : reinterpret_cast<unsigned long>(GetProcAddress(m_handle, reinterpret_cast<LPCSTR>(ordinal)));
}

void __declspec(naked) gplr()
{
	__asm
	{
		std r14, -0x98(sp)
		std r15, -0x90(sp)
		std r16, -0x88(sp)
		std r17, -0x80(sp)
		std r18, -0x78(sp)
		std r19, -0x70(sp)
		std r20, -0x68(sp)
		std r21, -0x60(sp)
		std r22, -0x58(sp)
		std r23, -0x50(sp)
		std r24, -0x48(sp)
		std r25, -0x40(sp)
		std r26, -0x38(sp)
		std r27, -0x30(sp)
		std r28, -0x28(sp)
		std r29, -0x20(sp)
		std r30, -0x18(sp)
		std r31, -0x10(sp)
		stw r12, -0x8(sp)
		blr
	}
}

unsigned long hook::relink_gplr(unsigned long address, const unsigned long* save_stub,
                                const unsigned long* original_address)
{
	unsigned long instruction = 0;
	auto* saver = reinterpret_cast<unsigned long*>(gplr);

	if (address & 0x2000000)
		address = address | 0xFC000000;

	const auto replacing = original_address[address / 4];

	for (auto i = 0; i < 20; i++)
		if (replacing == saver[i])
		{
			const auto NewOffset = reinterpret_cast<int>(& saver[i]) - reinterpret_cast<int>(save_stub);
			instruction = 0x48000001 | (NewOffset & 0x3FFFFFC);
		}

	return instruction;
}

hook::hook()
{
}

hook::hook(const unsigned long address, const unsigned long destination)
{
	this->address_ = reinterpret_cast<unsigned long*>(address);
	this->save_stub_ = nullptr;
	this->destination_ = destination;
	memcpy(this->original_stub_, reinterpret_cast<void*>(address), 0x10);
}

hook::hook(const unsigned long address, unsigned long* save_stub, const unsigned long destination)
{
	this->address_ = reinterpret_cast<unsigned long*>(address);
	this->save_stub_ = save_stub;
	this->destination_ = destination;
	memcpy(this->original_stub_, reinterpret_cast<void*>(address), 0x10);
}

void hook::start() const
{
	if (this->address_ == nullptr)
		return;

	if (this->save_stub_ != nullptr)
	{
		const auto address_relocation = reinterpret_cast<DWORD>(&this->address_[4]);

		this->save_stub_[0] = ((address_relocation & 0x8000)
			                       ? (0x3D600000 + (((address_relocation >> 16) & 0xFFFF) + 1))
			                       : (0x3D600000 + ((address_relocation >> 16) & 0xFFFF)));

		this->save_stub_[1] = 0x396B0000 + (address_relocation & 0xFFFF);
		this->save_stub_[2] = 0x7D6903A6;

		for (auto i = 0; i < 4; i++)
			if ((this->address_[i] & 0x48000003) == 0x48000001)
				this->save_stub_[i + 3] = relink_gplr((this->address_[i] & ~0x48000003), &this->save_stub_[i + 3],
				                                      &this->address_[i]);
			else
				this->save_stub_[i + 3] = this->address_[i];

		this->save_stub_[7] = 0x4E800420;
		__dcbst(0, this->save_stub_);
		__emit(0x7C0004AC);
		__emit(0x4C00012C);
	}

	this->address_[0] = ((this->destination_ & 0x8000)
		                     ? 0x3D600000 + (((this->destination_ >> 16) & 0xFFFF) + 1)
		                     : 0x3D600000 + ((this->destination_ >> 16) & 0xFFFF));
	this->address_[1] = 0x396B0000 + (this->destination_ & 0xFFFF);
	this->address_[2] = 0x7D6903A6;
	this->address_[3] = 0x4E800420;
}

void hook::stop() const
{
	if (!this->address_)
		return;

	memcpy(this->address_, this->original_stub_, 0x10);
}
