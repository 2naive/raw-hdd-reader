//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormMain *FormMain;

/* disk geometry and layout */
DISK_GEOMETRY_EX pdg;
DWORD dwLayoutSize = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + 64 /*enough*/ * sizeof(PARTITION_INFORMATION_EX);
DRIVE_LAYOUT_INFORMATION_EX * DriveLayout = (DRIVE_LAYOUT_INFORMATION_EX*) new char[dwLayoutSize];

/* read/write buffers */
CHAR * cBuffer = (char*)VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE);
CHAR * cWriteBuffer = (char*)VirtualAlloc(NULL, 512, MEM_COMMIT, PAGE_READWRITE);

/* other */
HANDLE hDevice;
FILE * fInputFile;
FILE * fOutputFile;
long lSize;
long iSectorId = 800000000;
long iStartingSectorId;

char cDriveId = '0';

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
	/* check if user has adm privs using msdn example funct */
	if(!IsUserAdmin())
	{
		ShowMessage("You need Administrator privileges to use this program");
		Application->Terminate();
	}

	/* initializing device / getting device information / printing information */
	hDevice = InitDriveHandle(cDriveId);
	GetDriveGeometry(hDevice, &pdg);
	GetDriveLayout(hDevice, DriveLayout, dwLayoutSize);


	/* printing device information */
	DeviceInformationTable->Cells[0][0]="Cylinders";
	DeviceInformationTable->Cells[0][1]="Tracks/cylinde";
	DeviceInformationTable->Cells[0][2]="Sectors/track";
	DeviceInformationTable->Cells[0][3]="Bytes/sector";
	DeviceInformationTable->Cells[0][4]="Disk size";
	DeviceInformationTable->Cells[0][5]="Sectors";

	DeviceInformationTable->Cells[1][0]=pdg.Geometry.Cylinders.QuadPart;
	DeviceInformationTable->Cells[1][1]=pdg.Geometry.TracksPerCylinder;
	DeviceInformationTable->Cells[1][2]=pdg.Geometry.SectorsPerTrack;
	DeviceInformationTable->Cells[1][3]=pdg.Geometry.BytesPerSector;
	DeviceInformationTable->Cells[1][4]=pdg.DiskSize.QuadPart / (1024 * 1024 * 1024);
	DeviceInformationTable->Cells[1][5]=pdg.DiskSize.QuadPart/512;

	/* printing partitions information*/
	PTTable->Cells[0][0]="ID";
	PTTable->Cells[1][0]="Starting sector";
	PTTable->Cells[2][0]="Offset";
	PTTable->Cells[3][0]="Last sector";

	for (DWORD i = 0; i < DriveLayout->PartitionCount; i++)
	{
		if(i>0)
			PTTable->RowCount++;
		if(DriveLayout->PartitionEntry[i].PartitionNumber<1)
			continue;
		PTTable->Cells[0][i+1]=DriveLayout->PartitionEntry[i].PartitionNumber;
		PTTable->Cells[1][i+1]=DriveLayout->PartitionEntry[i].StartingOffset.QuadPart/512;
		PTTable->Cells[2][i+1]=DriveLayout->PartitionEntry[i].PartitionLength.QuadPart/512;
		if(DriveLayout->PartitionEntry[i].StartingOffset.QuadPart/512 + DriveLayout->PartitionEntry[i].PartitionLength.QuadPart/512 > 1)
			PTTable->Cells[3][i+1]=(DriveLayout->PartitionEntry[i].StartingOffset.QuadPart/512+DriveLayout->PartitionEntry[i].PartitionLength.QuadPart/512-1);
		else
			PTTable->Cells[3][i+1]=0;
	}

	/* trying to define free unallocated space */

	UnpartitionedSpaceTable->Cells[0][0]="ID";
	UnpartitionedSpaceTable->Cells[1][0]="Starting sector";
	UnpartitionedSpaceTable->Cells[2][0]="Last sector";
	UnpartitionedSpaceTable->Cells[3][0]="Free space (MB)";

	for (int i = 1; i <= PTTable->RowCount; i++)
	{

		if(strlen(PTTable->Cells[0][i+1].t_str())==0 && DeviceInformationTable->Cells[1][5].ToInt()-PTTable->Cells[3][i].ToInt()>0)
		{
			UnpartitionedSpaceTable->Cells[0][i]=i;
			UnpartitionedSpaceTable->Cells[1][i]=PTTable->Cells[3][i].ToInt()+1;
			UnpartitionedSpaceTable->Cells[2][i]=DeviceInformationTable->Cells[1][5];
			UnpartitionedSpaceTable->Cells[3][i]=(DeviceInformationTable->Cells[1][5].ToInt()-PTTable->Cells[3][i].ToInt()+1)/2048;
			break;
		}
		else
		{
			UnpartitionedSpaceTable->Cells[0][i]=i;
			UnpartitionedSpaceTable->Cells[1][i]=PTTable->Cells[3][i].ToInt()+1;
			UnpartitionedSpaceTable->Cells[2][i]=PTTable->Cells[1][i+1];
			UnpartitionedSpaceTable->Cells[3][i]=(PTTable->Cells[1][i+1].ToInt()-PTTable->Cells[3][i].ToInt()+1)/2048;
		}
	}
}
//---------------------------------------------------------------------------





void __fastcall TFormMain::ButtonOpenFileClick(TObject *Sender)
{
	if(OpenDialog1->Execute())
	{
		EditInputFilename->Text=OpenDialog1->FileName.t_str();
		StatusBar1->SimpleText="Ready for file writing";
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::ButtonReadSectorClick(TObject *Sender)
{
	ReadDriveSector(hDevice, pdg, cBuffer, EditReadSectorId->Text.ToInt() );
	MemoSectorData->Text=cBuffer;
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::ButtonWriteFileClick(TObject *Sender)
{

	if(EditWriteSector->Text.ToInt()>0 && strlen(OpenDialog1->FileName.t_str())>0)
	{
		fInputFile = fopen(OpenDialog1->FileName.t_str(), "rb+");
		if(fInputFile==NULL)
		{
			ShowMessage("The file could not be opened");
			return;
		}
		iSectorId = EditWriteSector->Text.ToInt();
		if(iSectorId>0)
		{
			while(!feof(fInputFile))
			{
				fread(cWriteBuffer, 1, 512, fInputFile);
				Encrypt(cWriteBuffer);
				WriteDriveSector(hDevice, pdg, cWriteBuffer, iSectorId);
				memset(cWriteBuffer, 0, 512);
				iSectorId++;
			}
		}
		fclose(fInputFile);

		EditWriteResult->Text=iSectorId-1;
		StatusBar1->SimpleText="File was written";
	}
	else
	{
		ShowMessage("Choose file to write or target device starting sector");
		return;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonChooseFileClick(TObject *Sender)
{
	if(SaveDialog1->Execute())
	{
		EditOutputFilename->Text=SaveDialog1->FileName.t_str();
		StatusBar1->SimpleText="Ready for file reading";
	}
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::ButtonGetFileClick(TObject *Sender)
{
	if(EditGetLastSector->Text.ToInt()-EditGetSector->Text.ToInt()>0 && strlen(SaveDialog1->FileName.t_str())>0)
	{

		fOutputFile = fopen(SaveDialog1->FileName.t_str(),"wb+");
		if(fOutputFile==NULL)
		{
			ShowMessage("The file could not be opened");
			return;
		}
		for(int z = 0; z <= EditGetLastSector->Text.ToInt()-EditGetSector->Text.ToInt(); z++)
		{
			memset(cBuffer, 0, 512);
			ReadDriveSector(hDevice, pdg, cBuffer, EditGetSector->Text.ToInt() + z);
			Decrypt(cBuffer);
			fwrite(cBuffer, 1, 512, fOutputFile);
		}
		fclose(fOutputFile);
		StatusBar1->SimpleText="File was saved";
	}
	else
	{
		ShowMessage("Choose file to write or set up correct sectors values");
		return;
	}
}
//---------------------------------------------------------------------------




