#pragma once
class game
{
public:
	virtual void load_menu() = 0;

	virtual void unload_menu() = 0;
};

enum xbox_titles : unsigned int
{
	boot_up = 0x00000000,
	dashboard = 0xFFFE07D1,
	cod_modern_warfare = 0x415607E6,
	cod_modern_warfare_2 = 0x41560817,
	cod_modern_warfare_3 = 0x415608CB,
	cod_black_ops_1 = 0x41560855,
	cod_black_ops_2 = 0x415608C3,
	cod_black_ops_3 = 0x4156091D,
	cod_ghosts = 0x415608fC,
	cod_advanced_warfare = 0x41560914,
};
