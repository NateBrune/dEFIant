#include <efi.h>
#include <efilib.h>

EFI_STATUS readFile(           // show one bmp
	IN EFI_FILE *dir,
	IN INT32 index,
	IN EFI_SYSTEM_TABLE *systab,
	IN VOID *buf
);

EFI_STATUS tostring(           // convert digital to string with ".bmp", for example : 9 -> 9.bmp
	IN	INT32 index,
	OUT CHAR16 *filename
);



EFI_STATUS tostring(INT32 index, CHAR16 *filename)
{
	if (index==0)
	{
		StrCpy(filename, L"0.story");
		return EFI_SUCCESS;
	}
	INT32 x=100000;
	INT32 c=0;
	while(index/x==0){x=x/10;}
	while(TRUE)
	{
		filename[c]=index/x+48;
		index=index%x;
		c++;
		x=x/10;
		if(x==0)
			break;
	}
	StrCpy(filename+c, L".story");
	return EFI_SUCCESS;
}

EFI_STATUS readFile(
	IN EFI_FILE *dir,
	IN INT32 index,
	IN EFI_SYSTEM_TABLE *systab,
	IN VOID *buf
)
{
	EFI_STATUS status;
	EFI_FILE *file;
	CHAR16 filename[10]; // Allow up to 9999 stories (Probably going to have to increase buffer size when this engine gets super popular :)
	INT32 index
	tostring(index, filename);
	Status=dir->Open(dir, &file, filename, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY); // open the file

	EFI_FILE_INFO *fileinfo;
	UINTN infosize = SIZE_OF_EFI_FILE_INFO;
	EFI_GUID info_type = EFI_FILE_INFO_ID;
	status = file->GetInfo(file, &info_type, &infosize, NULL);  // get the info size of file

	systab->BootServices->AllocatePool(AllocateAnyPages, infosize, (VOID **)&fileinfo);
	status=file->GetInfo(file, &info_type, &infosize, fileinfo);  // get info of file

	UINTN filesize = fileinfo->FileSize; // get filesize from info

	systab->BootServices->AllocatePool(AllocateAnyPages, filesize, (VOID **)&buf); // Allocate some room

	status=file->Read(file, &filesize, buf);

	systab->BootServices->FreePool(buf);
	systab->BootServices->FreePool(fileinfo);

	return EFI_SUCCESS;
}


EFI_STATUS EFIAPI efi_main(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab) {

	InitializeLib(image_handle, systab);
	EFI_STATUS Status;
	systab->ConOut->ClearScreen(systab->ConOut); // CLEAR!

    EFI_GUID simplefs_guid = SIMPLE_FILE_SYSTEM_PROTOCOL; // a simple file system protocol in efi
	                      // (different from UEFI specification. In specification, this is called EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID )

	EFI_FILE_IO_INTERFACE *simplefs;                      // the simple file system's interface
	                      // (different from UEFI specification. In specification, this is called EFI_SIMPLE_FILE_SYSTEM_PROTOCOL)
	EFI_FILE *root;               //  root directory
	EFI_FILE *dir;
	CHAR16 *dirname=L"\\stories"; // name of the directory where bmp pictures are put

	status=systab->BootServices->LocateProtocol(&simplefs_guid, NULL, (VOID **)&simplefs);
	if(EFI_ERROR(status)) Print(L"locate protocol failed \n");

	status=simplefs->OpenVolume(simplefs, &root);
	if(EFI_ERROR(status)) Print(L"open volume failed\n");

	status=root->Open(root, &dir, dirname, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if(EFI_ERROR(status)) Print(L"open directory failed\n");

	VOID *buf;
	INT32 index=1;
	status = readFile(dir, index, systab, buf); // Put the contents of 1.story into buf
	Print(L"%s", buf);

    WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
	uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &Key);

    return Status;
}
