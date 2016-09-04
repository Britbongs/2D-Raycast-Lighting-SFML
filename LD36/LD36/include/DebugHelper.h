#pragma once

#include "stdafx.h"
#include <fstream>

static void PrintToDebug(const sf::String& LogName, const sf::String& DebugMessage)
{
#if IN_DEVELOPMENT_BUILD
	std::wcout << LogName.toWideString() << L" : " << DebugMessage.toWideString() << std::endl;
#endif
}
