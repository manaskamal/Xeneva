/**
 *
 *   Copyright (C) Manas Kamal Choudhury 2020
 *
 *   Memory.cpp -- Xnldr Memory arranger
 *
 *   /PROJECT - Xnldr v2.0
 *   /AUTHOR  - Manas Kamal Choudhury
 *
 * --------------------------------------------------
 */

#include <Include/Memory.h>
#include <Include/EfiLib.h>
#include <Include/Console.h>

/**
 * Pool Allocate 
 *
 * @param MemSize -- Buffer Size for allocation
 */
void *AllocatePool (const size_t MemSize)
{
	void *buffer;
	return (gBootServices->AllocatePool (EfiLoaderData, MemSize, &buffer) < 0) ? nullptr : buffer;
}

/**
 * Free Pool memory that has been allocated
 *
 * @param Buffer -- Address of the allocated memory to be freed
 *
 */
void FreePool (void* Buffer)
{
	gBootServices->FreePool (Buffer);
}


/**
 * Memory manager simply checks the available physical memory
 * for use and then setup virtual memory and provides the access
 * to memory functions
 */

EFI_STATUS GetMemory ()
{
	XENEVA_INFO *info = GetXenevaInfo ();

	EFI_STATUS status = EFI_SUCCESS;

	uint32_t Mem_Map_Size = sizeof (EFI_MEMORY_DESCRIPTOR) * 16;
	uint32_t Mem_Map_Size_Out = Mem_Map_Size;
	uint32_t Mem_Map_Key = 0;
	uint32_t Mem_Map_Descriptor_Size = 0;
	uint32_t Mem_Map_Descriptor_Version = 0;
	uint32_t Descriptor_count = 0;
	uint32_t i = 0;
	int32_t  j = 48;
	uint8_t *buffer = nullptr;
	EFI_MEMORY_DESCRIPTOR *Mem_Descriptor_Ptr = nullptr;
	EFI_INPUT_KEY key;
	uint16_t mem_blocks = 0;

	/*
	 * Let's start for Memory Searching process
	 */
	do
	{
		buffer = (uint8_t*)AllocatePool (Mem_Map_Size);
		
		if (buffer == nullptr)
			break;

		/* Call UEFI Memory Map service */
		status = gBootServices->GetMemoryMap ((size_t*)&Mem_Map_Size_Out, (EFI_MEMORY_DESCRIPTOR*)buffer, (size_t*)&Mem_Map_Key,
			(size_t*)&Mem_Map_Descriptor_Size, (UINT32*)&Mem_Map_Descriptor_Version);

		if (EFI_ERROR (status))
		{
			FreePool (buffer);
			Mem_Map_Size += sizeof (EFI_MEMORY_DESCRIPTOR) * 16;
		}

	} while (EFI_ERROR(status));

	/* Now check if buffer is available or not */
	if (buffer != nullptr)
	{
		/**
		 * We are here, means no problem
		 * Let's search for free physical memory
		 */

		Descriptor_count = Mem_Map_Size_Out / Mem_Map_Descriptor_Size;
		Mem_Descriptor_Ptr = (EFI_MEMORY_DESCRIPTOR*) buffer;
		for (i = 0; (i<Descriptor_count) && (j > 0); i++, j--)
		{
			/**
			 * Now iterate through available descriptors for free
			 * physical memory
			 */
			Mem_Descriptor_Ptr = (EFI_MEMORY_DESCRIPTOR*)(buffer+ (i * Mem_Map_Descriptor_Size));
         
			if (Mem_Descriptor_Ptr->Type == EfiConventionalMemory || 
				Mem_Descriptor_Ptr->Type == EfiPersistentMemory)
			{
				/**
				 * Now store available memory in Xeneva Boot Information 
				 * structure in an array order
				 */
			     info->FreeMemory[mem_blocks].Type = Mem_Descriptor_Ptr->Type;
			     info->FreeMemory[mem_blocks].PhysicalStart = Mem_Descriptor_Ptr->PhysicalStart;
			     info->FreeMemory[mem_blocks].Size = Mem_Descriptor_Ptr->NumberOfPages;
			     info->FreeMemory[mem_blocks].Type = Mem_Descriptor_Ptr->Type;
			     info->FreeMemory[mem_blocks].Attribute = Mem_Descriptor_Ptr->Attribute;
			     info->FreeMemory[mem_blocks].VirtualStart = Mem_Descriptor_Ptr->VirtualStart;	
			     mem_blocks++;
			}
		}
       
		/**
		 * We are done now Free the pool
		 */
		FreePool (buffer);
	}

	/**
	 * Slot 0 is always 0
	 */
	if (info->FreeMemory[0].Size < 0x06000000)
	{
		Printf (L"\r\n ***Not Enough Physical Memory Found in Block 1, Resuming *** \r\n");
	}
		
	/* return EFI_SUCCESS code */
	return EFI_SUCCESS;
}
