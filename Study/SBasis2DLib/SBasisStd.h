#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <atlconv.h> // A2W
#include <iostream>
#include <process.h> // _beginthreadex
#include "SMath.h"

#pragma comment (lib, "Winmm.lib")
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "fmod_vc.lib")

#pragma warning (disable:4244)		// 경고 무시 warning C4244: '=': 'LONG'에서 'float'(으)로 변환하면서 데이터가 손실될 수 있습니다

namespace SBASIS_CORE_LIB
{
	typedef std::basic_string<TCHAR>	T_STR;
	typedef std::basic_string<CHAR>		C_STR;
	typedef std::vector<T_STR>			T_STR_VECTOR;

	template<class S>
	class SSingleton
	{
	public:
		static S& GetInstance()
		{
			static S theSingle;
			return theSingle;
		}
	};
	static std::wstring to_mw(const std::string& _src)
	{
		USES_CONVERSION;
		return std::wstring(A2W(_src.c_str()));
	};
	static std::string to_wm(const std::wstring& _src)
	{
		USES_CONVERSION;
		return std::string(W2A(_src.c_str()));
	}
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte( CP_ACP, 0,
										   data, -1,
										   0, 0,
										   NULL, NULL );
		int iRet = WideCharToMultiByte( CP_ACP, 0,
										data, -1, // 소스
										retData, iLength, // 대상
										NULL, NULL );
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte( CP_ACP, 0,
										   src, -1,
										   0, 0,
										   NULL, NULL );
		int iRet = WideCharToMultiByte( CP_ACP, 0,
										src, -1, // 소스
										pDest, iLength, // 대상
										NULL, NULL );
		if (iRet == 0) return false;
		return true;
	}

	static void PRINT(char* fmt, ...) // 나열연산자
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}
	static void Error(const CHAR* msg = 0, const char* lpData = 0)
	{
		LPVOID* lpMsg = 0;
		FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
						NULL, WSAGetLastError(),
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(CHAR*)&lpMsg, 0, NULL );

		std::string szBuffer = (lpData != nullptr) ? lpData : "";
		szBuffer += "\n";
		szBuffer += (CHAR*)lpMsg;

		MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
		LocalFree(lpMsg);
	}
	static void Check(int iRet, int line)
	{
		if (iRet == SOCKET_ERROR)
		{
			CHAR szBuffer[256] = { 0, };
			sprintf_s(szBuffer, "%s[%d", __FILE__, line);
			Error("ERROR", szBuffer);
			exit(1);
		}
	}

	template <typename S>
	inline void SafeRelease(S *&p)
	{
		if (NULL != p)
		{
			p->Release();
			p = NULL;
		}
	}
}
