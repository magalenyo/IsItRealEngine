#pragma once
// Class to detect leaks. Must be included in every file and will dump in the main function.
#ifdef _DEBUG
	#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
	#define new MYDEBUG_NEW
#endif

