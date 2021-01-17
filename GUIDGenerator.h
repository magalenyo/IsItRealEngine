#pragma once

#include <string>
#include "combaseapi.h"


inline std::string GenerateUID() {
	GUID guid;
	CoCreateGuid(&guid);

	char guid_cstr[37];
	snprintf(guid_cstr, sizeof(guid_cstr),
		"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);


	return std::string(guid_cstr);
}