/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  File.h -- UEFI File Handling Library
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 * ----------------------------------------------------
 */

#include <Include/File.h>
#include <Include/Stdlib/Stdint.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include <Include/Memory.h>
#include <Include/Console.h>

/**
 * EFI GUIDs
 */

#define EFI_FILE_INFO_ID \
{0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b }}


EFI_GUID GenericFileInfo = {
	0x9576E92,
	0x6D3F,
	0x11D2,
	{
		0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B
	}
};

/**
 * Open a file and extract its datas and return the data
 *
 * @param filename -- Path of the file with filename
 *
 */
uint8_t* OpenFile (CHAR16 *filename)
{
	EFI_GUID LoadedImageProtocol = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_LOADED_IMAGE *LoadedImage;
	EFI_STATUS Status;
	EFI_GUID  SimpleFSProtoclId = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;


	/* Locate our protocols */

	Status = gBootServices->HandleProtocol (gImageHandle, &LoadedImageProtocol, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* BootFS;

	Status = gBootServices->HandleProtocol (LoadedImage->DeviceHandle, &SimpleFSProtoclId, (void**)&BootFS);

	EFI_FILE_PROTOCOL* Root;

	/* Open the root volume */

	BootFS->OpenVolume (BootFS, &Root);

	if (Status != EFI_SUCCESS)
	{
		Printf(L"Failed to Open Root Volume\r\n");
		return 0;  //ErRor
	}

	/** 
	 * Open the file from here
	 */

    EFI_FILE_PROTOCOL *File;
	Status = Root->Open (Root, &File, filename, EFI_FILE_MODE_READ , 0);
	if (Status == EFI_NOT_FOUND)
	{
		Printf(L"Open File Protocol Failed \r\n");
		return 0;  //eRror
	}

	/**
	 * Extract the File information
	 * get the file size
	 */

	uint64_t FileInfoSize;
	EFI_FILE_INFO *FileInfo;
	void* Buffer[250];
	File->GetInfo (File, &GenericFileInfo, &FileInfoSize, (void**)&Buffer);
	Status = gBootServices->AllocatePool (EfiLoaderData, FileInfoSize, (void**)&FileInfo);
	if (Status != EFI_SUCCESS)
	{
		Printf(L"Failed to allocate pool for File Info\r\n");
		return 0;   // ERror oCcured 
	}


	FileInfo = (EFI_FILE_INFO*)Buffer;
	
	/**
	 * Allocate some pool memory for Read Size
	 */

	uint64_t ReadSize = FileInfo->FileSize;
	if (ReadSize > 16*1024*1024)
	{
		ReadSize = 16*1024*1024;
	}

	/* Free the pool */
	gBootServices->FreePool (&FileInfo);

	/**
	 * Now store the datas in FileBuffer
	 */
	uint8_t* FileBuffer = (uint8_t*)AllocatePool (ReadSize);
	uint64_t BufferSize = ((ReadSize + 4096 - 1) / 4096);

	Status = File->Read (File, &ReadSize, FileBuffer);
	
	if (Status != EFI_SUCCESS)
	{
		Printf(L"Read File error\r\n");
		return 0;
	}

	/* return the loaded data */
	return FileBuffer;
}