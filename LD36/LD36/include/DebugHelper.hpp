#pragma once

#include "stdafx.hpp"
#include <fstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif 
#include <Windows.h>


static const std::wstring DebugLog = L"Debug: ";
static const std::wstring AssetLog = L"Asset: ";
static const std::wstring ErrorLog = L"ERROR!: ";

static void VDebugPrintF(const std::wstring& LogType, wchar_t* Format, va_list ArgList)
{
	const Uint32 MAX_CHARS = 1024;
	static wchar_t StrBuffer[MAX_CHARS];

	int CharsWritten = _vsnwprintf_s(StrBuffer, MAX_CHARS, Format, ArgList);

	OutputDebugString(LogType.c_str());
	OutputDebugString(StrBuffer);
	OutputDebugString(L"\n");
	std::wcout << std::wstring(StrBuffer) << L"\n";
}

static void DebugPrintF(const std::wstring& LogType, wchar_t* Format, ...)
{
#ifndef PLAYABLE_BUILD
	va_list ArgList;
	va_start(ArgList, Format);

	VDebugPrintF(LogType, Format, ArgList);
	va_end(ArgList);
#endif
}

#define NO_INLINE __declspec(noinline)
