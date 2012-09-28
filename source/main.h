//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <tchar.h>
#include <windows.h>
#include <winioctl.h>
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *DeviceInformationTable;
	TLabel *Label1;
	TStringGrid *PTTable;
	TLabel *Label2;
	TButton *ButtonOpenFile;
	TOpenDialog *OpenDialog1;
	TLabel *Label3;
	TEdit *EditInputFilename;
	TEdit *EditWriteSector;
	TMemo *MemoSectorData;
	TButton *ButtonReadSector;
	TLabel *Label4;
	TLabel *Label5;
	TButton *ButtonWriteFile;
	TLabel *Label6;
	TEdit *EditWriteResult;
	TEdit *EditReadSectorId;
	TButton *ButtonChooseFile;
	TLabel *Label7;
	TEdit *EditOutputFilename;
	TEdit *EditGetSector;
	TLabel *Label8;
	TLabel *Label9;
	TButton *ButtonGetFile;
	TLabel *Label10;
	TEdit *EditGetLastSector;
	TSaveDialog *SaveDialog1;
	TStatusBar *StatusBar1;
	TLabel *Label11;
	TLabel *Label12;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TLabel *Label13;
	TStringGrid *UnpartitionedSpaceTable;
	void __fastcall ButtonOpenFileClick(TObject *Sender);
	void __fastcall ButtonReadSectorClick(TObject *Sender);
	void __fastcall ButtonWriteFileClick(TObject *Sender);
	void __fastcall ButtonChooseFileClick(TObject *Sender);
	void __fastcall ButtonGetFileClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
};

HANDLE InitDriveHandle(char id)
{
	HANDLE hDevice;               // handle to the drive to be examined
	char cPhysicalDrive[] = "\\\\.\\PhysicalDrive0";
	cPhysicalDrive[17]=id;
	hDevice = CreateFileA(
		cPhysicalDrive,
		//TEXT("\\\\.\\PhysicalDrive0"),  // drive to open
		//TEXT("\\\\.\\E:"),  // drive to open
		//TEXT("\\\\.\\PhysicalDrive1"),  // drive to open
		//GENERIC_READ | GENERIC_WRITE,					// access
		GENERIC_ALL,					// access
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,				// share mode
		NULL,							// default security attributes
		OPEN_EXISTING,					// disposition
		FILE_FLAG_WRITE_THROUGH,//0,//FILE_FLAG_NO_BUFFERING,			// file attributes
		NULL);							// do not copy file attributes

	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the drive
	{
		//GetLastError ();
		ShowMessage("InitDriveHandle failed.");
		return (FALSE);
	}
	else
		return hDevice;
}

void GetDriveGeometry(HANDLE hDevice, DISK_GEOMETRY_EX *pdg)
{
	BOOL bResult;
	DWORD junk;							// discard results

	bResult = DeviceIoControl(
		hDevice,						// device to be queried
		IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,  // operation to perform
		NULL, 0,						// no input buffer
		pdg, sizeof(*pdg),				// output buffer
		&junk,							// # bytes returned
		(LPOVERLAPPED) NULL);			// synchronous I/O

	if(!bResult)
		ShowMessage("GetDriveGeometry failed.");

}

void GetDriveLayout(HANDLE hDevice, DRIVE_LAYOUT_INFORMATION_EX *DriveLayout,DWORD dwLayoutSize)
{
	DWORD nBytesReturned;

	DeviceIoControl (hDevice,
								IOCTL_DISK_GET_DRIVE_LAYOUT_EX,
								NULL,
								0,
								DriveLayout,
								dwLayoutSize,
								&nBytesReturned,
								NULL);
}

void ReadDriveSector(HANDLE hDevice, DISK_GEOMETRY_EX pdg, LPVOID cBuffer, long offset)
{
	BOOL bDriveReadResult;
	DWORD dwBytesRead;

	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = offset;
	liDistanceToMove.QuadPart *= (long)512;

	SetFilePointerEx(hDevice, liDistanceToMove , NULL, FILE_BEGIN);
	bDriveReadResult = ReadFile(
		hDevice,
		cBuffer,
		pdg.Geometry.BytesPerSector,		//sectors to read
		&dwBytesRead,
		NULL);

	if(!bDriveReadResult)
		ShowMessage("ReadDriveSector failed.");
}

void WriteDriveSector(HANDLE hDevice, DISK_GEOMETRY_EX pdg, LPVOID cBuffer, long offset)
{
	BOOL bDriveWriteResult;
	DWORD dwBytesWritten;

	LARGE_INTEGER liDistanceToMove;
	liDistanceToMove.QuadPart = offset;
	liDistanceToMove.QuadPart *= (long)512;

	SetFilePointerEx(hDevice, liDistanceToMove , NULL, FILE_BEGIN);
	bDriveWriteResult = WriteFile(
		hDevice,
		cBuffer,
		pdg.Geometry.BytesPerSector,
		&dwBytesWritten,
		NULL);

	if(!bDriveWriteResult)
		ShowMessage("WriteDriveSector failed");
}

void Encrypt(char * cBuffer)
{
	//encryption function should be here
	for(int z = 0; z < 512; z++)
	{
		cBuffer[z]=cBuffer[z] ^ 1;
	}
}

void Decrypt(char * cBuffer)
{
	//decryption function should be here
	for(int z = 0; z < 512; z++)
	{
		cBuffer[z]=cBuffer[z] ^ 1;
	}
}
BOOL IsUserAdmin(VOID)
/*++ MSDN TAKEN FUNCTION
Routine Description: This routine returns TRUE if the caller's
process is a member of the Administrators local group. Caller is NOT
expected to be impersonating anyone and is expected to be able to
open its own process and process token.
Arguments: None.
Return Value:
   TRUE - Caller has Administrators local group.
   FALSE - Caller does not have Administrators local group. --
*/
{
BOOL b;
SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
PSID AdministratorsGroup;
b = AllocateAndInitializeSid(
    &NtAuthority,
    2,
    SECURITY_BUILTIN_DOMAIN_RID,
    DOMAIN_ALIAS_RID_ADMINS,
    0, 0, 0, 0, 0, 0,
    &AdministratorsGroup);
if(b)
{
	if (!CheckTokenMembership( NULL, AdministratorsGroup, &b))
	{
		 b = FALSE;
    }
    FreeSid(AdministratorsGroup);
}

return(b);
}
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
