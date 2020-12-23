#Xnldr v2.0

Xnldr also called as 'Xeneva Loader' is an EFI-Based bootloader which boots the entire kernel
As like every bootloader, it collects necessary information from UEFI firmware and creates an
information structure that needed to be passed to the kernel. 

#To build Xnldr v2.0 

I use Visual Studio 2012 Express Edition
and anyone can use upgraded version of Visual Studio

NOTE : In Visual Studio Linker Option, output path is "E://EFI/BOOT/BOOTx64.EFI" for me as
I use an USB Flash memory to test the project. I Patitioned it as GPT and use FAT32 file system
anyone can specify their own output location in the project

#Toolchain

For now, the project is only compatible with Microsoft toolchain, in future the project will be
compatible with GCC and other compilers.

 
Manas Kamal Choudhury
<<<<<<< HEAD
Assam
=======
Assam
>>>>>>> dfdf2a119c80003a5ebb81cc073bbc5992206fd1
