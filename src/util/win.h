#pragma once

//This file is the alternative to
//#define WIN32_LEAN_AND_MEAN
//#define NOSERVICE
//#define NOMCX
//#include <windows.h>

//I made it to not pollute global namespace,
//and to reduce the number of intellisense suggestions

//wing.h is for gui.

#include "miniutil.h"

namespace win {
extern "C" {

typedef u4 HANDLE;

u4
__stdcall
CloseHandle(
	HANDLE hObject
	);

namespace file {
	const u4 MAX_PATH = 260;

	const u4 GENERIC_READ    = 0x80000000;
	const u4 GENERIC_WRITE   = 0x40000000;
	const u4 GENERIC_EXECUTE = 0x20000000;
	const u4 GENERIC_ALL     = 0x10000000;

	const u4 CREATE_NEW        = 1;
	const u4 CREATE_ALWAYS     = 2;
	const u4 OPEN_EXISTING     = 3;
	const u4 OPEN_ALWAYS       = 4;
	const u4 TRUNCATE_EXISTING = 5;

	const u4 INVALID_HANDLE_VALUE = -1;

	const u4 FILE_SHARE_READ   = 0x1;
	const u4 FILE_SHARE_WRITE  = 0x2;
	const u4 FILE_SHARE_DELETE = 0x4;

	typedef struct tagSECURITY_ATTRIBUTES {
		u4 nLength;
		void* pSecurityDescriptor;
		u4 bInheritHandle;
	} SECURITY_ATTRIBUTES;

	HANDLE
	__stdcall
	CreateFileA(
		const char* lpFileName,
		u4 dwDesiredAccess,
		u4 dwShareMode,
		SECURITY_ATTRIBUTES* pSecurityAttributes,
		u4 dwCreationDisposition,
		u4 dwFlagsAndAttributes,
		HANDLE hTemplateFile
		);

	HANDLE
	__stdcall
	CreateFileW(
		const wchar* lpFileName,
		u4 dwDesiredAccess,
		u4 dwShareMode,
		SECURITY_ATTRIBUTES* pSecurityAttributes,
		u4 dwCreationDisposition,
		u4 dwFlagsAndAttributes,
		HANDLE hTemplateFile
		);

	//#ifdef UNICODE
	//#define CreateFile  CreateFileW
	//#else
	//#define CreateFile  CreateFileA
	//#endif

	u4
	__stdcall
	WriteFile(
		HANDLE hFile,
		bytep pBuffer,
		u4 nNumberOfBytesToWrite,
		u4* pNumberOfBytesWritten,
		u4 pOverlapped
		);

	u4
	__stdcall
	ReadFile(
		HANDLE hFile,
		bytep pBuffer,
		u4 nNumberOfBytesToRead,
		u4* pNumberOfBytesRead,
		u4 pOverlapped
		);

	u4
	__stdcall
	SetEndOfFile(
		HANDLE hFile
		);

	const u4 FILE_BEGIN   = 0;
	const u4 FILE_CURRENT = 1;
	const u4 FILE_END     = 2;

	u4
	__stdcall
	SetFilePointer(
		HANDLE hFile,
		u4 lDistanceToMove,
		u4* lpDistanceToMoveHigh,
		u4 dwMoveMethod
		);

	typedef struct tagFILETIME {
		u4 dwLowDateTime;
		u4 dwHighDateTime;
	} FILETIME;

	typedef struct tagWIN32_FIND_DATAW {
		u4 dwFileAttributes;
		FILETIME ftCreationTime;
		FILETIME ftLastAccessTime;
		FILETIME ftLastWriteTime;
		u4 nFileSizeHigh;
		u4 nFileSizeLow;
		u4 dwReserved0;
		u4 dwReserved1;
		wchar cFileName[ MAX_PATH ];
		wchar cAlternateFileName[ 14 ];
	} WIN32_FIND_DATAW;

	const u4 FILE_ATTRIBUTE_DIRECTORY = 0x00000010;

	HANDLE
	__stdcall
	FindFirstFileW(
		const wchar* pFileName,
		WIN32_FIND_DATAW* pFindFileData
		);

	u4
	__stdcall
	FindNextFileW(
		HANDLE hFindFile,
		WIN32_FIND_DATAW* pFindFileData
		);

	u4
	__stdcall
	FindClose(
		HANDLE hFindFile
		);

	u4
	__stdcall
	MoveFileW(
		const wchar* pExistingFileName,
		const wchar* pNewFileName
		);

	u4
	__stdcall
	GetFileTime(
		HANDLE hFile,
		FILETIME* pCreationTime,
		FILETIME* pLastAccessTime,
		FILETIME* pLastWriteTime
		);

	u4
	__stdcall
	SetFileTime(
		HANDLE hFile,
		FILETIME* pCreationTime,
		FILETIME* pLastAccessTime,
		FILETIME* pLastWriteTime
		);

	//SetFileTime

	//BY_HANDLE_FILE_INFORMATION fileInformation;
	//GetFileInformationByHandle(file, &fileInformation);
	//u4 filesize = fileInformation.nFileSizeLow;
}

namespace clipboard {

	const u4 CF_TEXT = 1;
	const u4 CF_UNICODETEXT = 13;

	u4
	__stdcall
	IsClipboardFormatAvailable(
		u4 format);

	u4
	__stdcall
	OpenClipboard(
		HANDLE hWndNewOwner);


	HANDLE
	__stdcall
	GetClipboardData(
		u4 uFormat);

	//those are technically heap functions...
	void*
	__stdcall
	GlobalLock ( 
		HANDLE hMem
		);

	u4
	__stdcall
	GlobalUnlock(
		HANDLE hMem
		);

	u4
	__stdcall
	CloseClipboard();
}


namespace memory {
	const u4 PAGE_NOACCESS  = 0x01;
	const u4 PAGE_READONLY  = 0x02;
	const u4 PAGE_READWRITE = 0x04;
	const u4 MEM_COMMIT  = 0x1000;
	const u4 MEM_RESERVE = 0x2000;

	bytep
	__stdcall
	VirtualAlloc(
		bytep pAddress,
		u4 dwSize,
		u4 flAllocationType,
		u4 flProtect
		);
}

//MultiByte flags
const u4 MB_ERR_INVALID_CHARS = 0x08;

//WideChar flags
const u4 WC_ERR_INVALID_CHARS = 0x80;
//^ This one is only available since Vista.
//^ Use this only with UTF8
const u4 WC_NO_BEST_FIT_CHARS = 0x400;
//^ Slightly older value, WINVER >= 0x0500
//^ Use this with everything else.

//Code Page
const u4 CP_UTF7 = 65000;
const u4 CP_UTF8 = 65001;

int
__stdcall
MultiByteToWideChar(
    u4       CodePage,
    u4       dwFlags,
    const char*  pMultiByteStr,
    u4          cbMultiByte,
    const wchar*   pWideCharStr, //out
    int      cchWideChar); //out

int
__stdcall
WideCharToMultiByte(
    u4     CodePage,
    u4    dwFlags,
    const wchar*  pWideCharStr,
    u4      cchWideChar,
    const char*   pMultiByteStr,
    int      cbMultiByte,
    const char*   pDefaultChar,
    u4*  pUsedDefaultChar);

u4
__stdcall
GetTickCount();

} //extern "C" {
} //namespace win {