#pragma once

typedef enum
{
	XNOTIFYUI_TYPE_SONGPLAYING = 13,
	XNOTIFYUI_TYPE_PREFERRED_REVIEW = 14,
	XNOTIFYUI_TYPE_AVOID_REVIEW = 15,
	XNOTIFYUI_TYPE_COMPLAINT = 16
} XNOTIFYQUEUEUI_TYPE;

void debug_print(const string& value);

bool is_tray_open();

unsigned long xam_get_current_title_id();

void x_notify(const wstring& value, XNOTIFYQUEUEUI_TYPE type = XNOTIFYUI_TYPE_PREFERRED_REVIEW);

unsigned long resolve_function(const char* module_name, unsigned long ordinal);

class hook
{
	unsigned long *address_,
	              *save_stub_,
	              destination_,
	              original_stub_[4];

	static unsigned long relink_gplr(unsigned long address, const unsigned long* save_stub,
	                                 const unsigned long* original_address);

public:
	hook();

	hook(unsigned long address, unsigned long destination);

	hook(unsigned long address, unsigned long* save_stub, unsigned long destination);

	void start() const;
	void stop() const;
};

template <typename Variable>
class address
{
	unsigned int address_;

public:

	explicit address(const unsigned int address)
	{
		address_ = address;
	}

	void set(Variable value)
	{
		*static_cast<Variable*>(address_) = value;
	}

	Variable get()
	{
		return *static_cast<Variable*>(address_);
	}
};
