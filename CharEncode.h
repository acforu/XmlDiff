#pragma once
#include <string>

bool GbkToUnicode_NoLimit(const char* pcMbSrc, std::wstring* pwUniStrDest);
bool Utf8ToUnicode_NoLimit(const char* pcUtf8Src, std::wstring* pwUniStrDest);
bool UnicodeToUtf8_NoLimit(const wchar_t* pwUniSrc, std::string* pUtf8StrDest);