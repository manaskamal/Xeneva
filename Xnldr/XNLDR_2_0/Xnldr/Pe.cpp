/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  Pe.cpp -- Portable Executable File Reader
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * -----------------------------------------------------
 */

#include <Include/Pe.h>
#include <Include/File.h>
#include <Include/Xeneva.h>
#include <Include/VirtualMemory.h>
#include <Include/Video.h>

static size_t StackSize;
KernelEntry EntryPoint;

/** Stack Address already defined, derived from Xnldr v1.0 **/
void* StackAddr = (void*)0xFFFFE00000000000;

/**
 * Copies bytes of data from one to another location
 *
 * @param dst -- Destination
 * @param src -- Source location
 * @param length -- Count of bytes
 */
static void CopyMem(void* dst, void* src, size_t length){
	uint8_t* dstp = (uint8_t*)dst;
	uint8_t* srcp = (uint8_t*)src;
	while (length--)
		*dstp++ = *srcp++;
}

/**
 * Makes destination address filled with 0
 *
 * @param dst -- target location
 * @param length -- Count of bytes
 *
 */
static void ZeroMem(void* dst, size_t length){
	
	uint8_t* dstp = (uint8_t*)dst;
	while (length--)
		*dstp++ = 0;
}

/**
 * Load an executable file to memory
 *
 * @param filename -- Filename with full path specified
 */

void LoadExec (CHAR16 *filename)
{
	uint8_t* buffer = OpenFile (filename);

	PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)buffer;
	PIMAGE_NT_HEADERS NtHeader = raw_offset<PIMAGE_NT_HEADERS>(DosHeader, DosHeader->e_lfanew);

	PSECTION_HEADER SectionHeader = raw_offset<PSECTION_HEADER>(&NtHeader->OptionalHeader, NtHeader->FileHeader.SizeOfOptionaHeader);
	size_t ImageBase = NtHeader->OptionalHeader.ImageBase;
	void*  ImBase = (void*)ImageBase;

	if (!CheckFree (ImBase, NtHeader->OptionalHeader.SizeOfImage))
	{
		DrawText("ERROR: Loading Image Base Address Failed\n");
		return;
	}

	if (!MapPage (ImBase, PADDR_T_MAX, NtHeader->OptionalHeader.SizeOfHeaders, PAGE_ATTRIBUTE_WRITABLE))
	{
		DrawText("ERROR: Binary Map Failed\n");
		return;
	}

	CopyMem (ImBase, buffer, NtHeader->OptionalHeader.SizeOfHeaders);

	for (size_t i=0; i < NtHeader->FileHeader.NumberOfSections; ++i)
	{
		CHAR16 Buf[9];
		CopyMem (Buf, SectionHeader[i].Name, 8);
		Buf[8] = 0;
		size_t LoadAddress = ImageBase + SectionHeader[i].VirtualAddress;
		void* SectAddr = (void*)LoadAddress;
		size_t SectSize = SectionHeader[i].VirtualSize;
		if (!MapPage ((void*)LoadAddress, PADDR_T_MAX, SectSize, PAGE_ATTRIBUTE_WRITABLE))
		{
			DrawText("ERROR:Binary Section Map Failed \n");
			return;
		}

		CopyMem(SectAddr, raw_offset<void*>(buffer, SectionHeader[i].PointerToRawData), SectionHeader[i].SizeOfRawData);
		if (SectionHeader[i].VirtualSize > SectionHeader[i].SizeOfRawData)
			ZeroMem(raw_offset<void*>(SectAddr, SectionHeader[i].SizeOfRawData), SectionHeader[i].VirtualSize - SectionHeader[i].SizeOfRawData);
		if ((SectionHeader[i].Characteristics  & IMAGE_SCN_MEM_WRITE) == 0)
			SetAttributes (SectAddr, SectSize, 0, PAGE_ATTRIBUTE_WRITABLE);
		if ((SectionHeader[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) == 0)
			SetAttributes (SectAddr, SectSize, PAGE_ATTRIBUTE_NO_EXEC, 0);
		if ((SectionHeader[i].Characteristics & IMAGE_SCN_MEM_NOT_PAGED) == 0)
			SetAttributes (SectAddr, SectSize, PAGE_ATTRIBUTE_NO_PAGING, 0);
		if ((SectionHeader [i].Characteristics & IMAGE_SCN_MEM_NOT_CACHED) == 0)
			SetAttributes (SectAddr, SectSize, PAGE_ATTRIBUTE_NO_CACHING, 0);

	}

	StackSize = NtHeader->OptionalHeader.SizeOfStackReserve;
	EntryPoint = (KernelEntry)(NtHeader->OptionalHeader.ImageBase + NtHeader->OptionalHeader.AddressOfEntryPoint);

	if (!MapPage (StackAddr, PADDR_T_MAX, StackSize, PAGE_ATTRIBUTE_WRITABLE))
	{
		DrawText ("ERROR: Binary Stack map failed\n");
	}

	DrawText("Binary Loaded\n");

}

/**
 * returns the stack size
 */
size_t GetStackSize ()
{
	return StackSize;
}

/**
 * returns the extracted entry point address
 */
KernelEntry GetEntryPoint ()
{
	return EntryPoint;
}

void* GetStackAddress ()
{
	return StackAddr;
}