#pragma once

#include "stdafx.h"
#include <fstream>

#ifndef PLAYABLE_BUILD
static void PrintToDebug(const sf::String& LogName, const sf::String& DebugMessage)
{
	std::wcout << LogName.toWideString() << L" : " << DebugMessage.toWideString() << std::endl;
}
#endif