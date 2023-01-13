#include "stdafx.h"
#include "CharEncode.h"
#include <stringapiset.h>

bool GbkToUnicode_NoLimit(const char* pcMbSrc, std::wstring* pwUniStrDest)
{
	if (!pcMbSrc || !pwUniStrDest) return false;

	const unsigned int CP_GBK = 936;

	int dwBefore = MultiByteToWideChar(CP_GBK, 0, pcMbSrc, -1, NULL, 0);
	if (dwBefore <= 0)
	{
		return false;
	}

	wchar_t* pwBuffer = (wchar_t*)malloc((dwBefore + 1) * sizeof(wchar_t));
	if (NULL == pwBuffer)
	{
		return false;
	}

	int dwAfter = MultiByteToWideChar(CP_GBK, 0, pcMbSrc, -1, pwBuffer, dwBefore);

	if (dwBefore == dwAfter)
	{
		pwBuffer[dwBefore] = L'\0';
		*pwUniStrDest = (wchar_t*)pwBuffer;
	}
	free(pwBuffer);
	return dwBefore == dwAfter;
}

bool Utf8ToUnicode_NoLimit(const char* pcUtf8Src, std::wstring* pwUniStrDest)
{
	if (!pcUtf8Src || !pwUniStrDest) return false;

	int dwBefore = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, NULL, 0);
	if (dwBefore <= 0)
	{
		return false;
	}

	wchar_t* pwBuffer = (wchar_t*)malloc((dwBefore + 1) * sizeof(wchar_t));
	if (NULL == pwBuffer)
	{
		return false;
	}

	int dwAfter = MultiByteToWideChar(CP_UTF8, 0, pcUtf8Src, -1, pwBuffer, dwBefore);
	if (dwBefore == dwAfter)
	{
		pwBuffer[dwBefore] = L'\0';
		*pwUniStrDest = pwBuffer;
	}

	free(pwBuffer);

	return dwBefore == dwAfter;
}

bool UnicodeToUtf8_NoLimit(const wchar_t* pwUniSrc, std::string* pUtf8StrDest)
{
	if (!pwUniSrc || !pUtf8StrDest) return false;

	int dwBefore = WideCharToMultiByte(CP_UTF8, 0, pwUniSrc, -1, NULL, 0, NULL, NULL);
	if (dwBefore <= 0)
	{
		return false;
	}

	char* pcBuffer = (char*)malloc((dwBefore + 1) * sizeof(char));
	if (NULL == pcBuffer)
	{
		return false;
	}

	int dwAfter = WideCharToMultiByte(CP_UTF8, 0, pwUniSrc, -1, pcBuffer, dwBefore * sizeof(char), NULL, NULL);

	if (dwBefore == dwAfter)
	{
		pcBuffer[dwBefore] = '\0';
		*pUtf8StrDest = pcBuffer;
	}

	free(pcBuffer);

	return dwBefore == dwAfter;
}