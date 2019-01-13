
#include "rom.h"

Rom::Rom() { }
Rom::~Rom() { }

Rom::Rom(const string& name_, const string& rom_name_, const string& search_name_)
	: name(name_), rom_name(rom_name_), search_name(search_name_)
{ }

