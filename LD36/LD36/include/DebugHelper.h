#pragma once

#include "stdafx.h"
#include <fstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif 
#include <Windows.h>


#ifndef PLAYABLE_BUILD
static void PrintToDebug(const sf::String& LogName, const sf::String& DebugMessage)
{
	std::wcout << LogName.toWideString() << L" : " << DebugMessage.toWideString() << std::endl;
}

int VDebugPrintF(const char* Format, va_list ArgList)
{
	const Uint32 MAX_CHARS = 1024;
	static char s_buffer[MAX_CHARS];

	int charsWritten = vsnprintf(s_buffer, MAX_CHARS, Format, ArgList);


}

#endif