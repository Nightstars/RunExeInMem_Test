// RunExeInMem_Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MmLoadExe.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szFileName[] = "F:/myprogram/kuwo/kuwomusic/9.0.5.0_BDS2/KwMusic.exe"; 

	// 打开EXE文件并获取EXE文件大小
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)

	{
		ShowError("CreateFile err");
		return 1;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 申请动态内存并读取DLL到内存中
	BYTE *pData = new BYTE[dwFileSize];
	if (NULL == pData)
	{
		ShowError("new");
		return 2;
	}
	DWORD dwRet = 0;
	ReadFile(hFile, pData, dwFileSize, &dwRet, NULL);
	CloseHandle(hFile);

	// 判断有无重定位表
	if (FALSE == IsExistRelocationTable(pData))
	{
		printf("[FALSE] IsExistRelocationTable\n");
		return 0;
	}
	// 将内存DLL加载到程序中
	LPVOID lpBaseAddress = MmRunExe(pData, dwFileSize);
	if (NULL == lpBaseAddress)
	{
		ShowError("MmRunExe");
		return 3;
	}

	system("pause");
	return 0;
}

