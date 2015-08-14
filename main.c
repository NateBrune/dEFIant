#include <efi.h>
#include <efilib.h>

VOID waitFor(UINTN seconds)
{
	UINT32 total = seconds * 100000000;
	UINTN counter = 0;
	while(counter<total){
		counter=counter+1;
	}
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
	EFI_STATUS Status;
    EFI_INPUT_KEY Key;

  CHAR16 banner[] =
L"                                                                     \n"
L"       /$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$                       /$$     \n"
L"      | $$| $$_____/| $$_____/|_  $$_/                      | $$     \n"
L"  /$$$$$$$| $$      | $$        | $$    /$$$$$$  /$$$$$$$  /$$$$$$   \n"
L" /$$__  $$| $$$$$   | $$$$$     | $$   |____  $$| $$__  $$|_  $$_/   \n"
L"| $$  | $$| $$__/   | $$__/     | $$    /$$$$$$$| $$  \\ $$  | $$     \n"
L"| $$  | $$| $$      | $$        | $$   /$$__  $$| $$  | $$  | $$ /$$ \n"
L"|  $$$$$$$| $$$$$$$$| $$       /$$$$$$|  $$$$$$$| $$  | $$  |  $$$$/ \n"
L" \\_______/|________/|__/      |______/ \\_______/|__/  |__/   \\___/   \n"
L"  																   \n"
L" Operating System Vendor:    Communism Systems                       \n"
L" Operating System Version:   4.1.4-1                				   \n";


    Print(L"\n");
    Status = Print(banner);
	UINT32 i = 0;
	Print(L"\n\n\n\nInitializing connection");
	while(i<300000000){
		if((i%100000000)==0){
			Print(L".");
		}
		i = i+1;
	}
	Print(L"\nIncoming transmission");
	i=0;
	while(i<300000000){
		if((i%100000000)==0){
			Print(L".");
		}
		i=i+1;
	}
	Print(L"\nLydia: ");
	waitFor(2);
	Print(L"Hello? Who are you?\n");
	Print(L">> ");
    WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
	uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &Key);

    return Status;
}
