#include "stdafx.h"
#define wszDrive L"\\\\.\\PhysicalDrive0"

BOOL DeviceIOControlOp(LPCTSTR wszPath, void* inData, SIZE_T inDataSize,void* outData, SIZE_T outDataSize, DWORD op)
{
	HANDLE hDevice = INVALID_HANDLE_VALUE;  
	DWORD junk = 0;                    

	hDevice = CreateFileW(wszPath,          // drive to open
		GENERIC_READ | GENERIC_WRITE,                // no access to the drive
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,             //sec
		OPEN_EXISTING,    
		0,                
		NULL);            

	if (hDevice == INVALID_HANDLE_VALUE)    // cannot open the drive
	{
		return FALSE;
	}

	BOOL res = DeviceIoControl(hDevice, op, inData, inDataSize, outData, outDataSize, &junk, (LPOVERLAPPED)NULL);
	if (res == FALSE) {
		wprintf(L"DEVICEIOOP failed. Error %ld.\n", GetLastError());
	}
	CloseHandle(hDevice);
	return res;
}

BOOL GetDiskFreeSpaceInfo(LPCTSTR path) {
	DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;
	if (GetDiskFreeSpaceW(path, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters)) {
		wprintf(L"Path      = %ws\n", path);
		wprintf(L"Sectors per cluster = %d\n", sectorsPerCluster);
		wprintf(L"Bytes per sector = %d\n", bytesPerSector);
		wprintf(L"Free clusters = %d\n", numberOfFreeClusters);
		wprintf(L"Total clusters = %d\n", totalNumberOfClusters);
		return TRUE;
	}
	else {
		wprintf(L"Failed to check info for path:     %ws\n", path);
		return FALSE;
	}
}

BOOL CheckDiskGeometry(LPCTSTR path) {
	DISK_GEOMETRY pdg = { 0 }; // disk drive geometry structure
	ULONGLONG DiskSize = 0;    // size of the drive, in bytes
	if (DeviceIOControlOp(path, NULL,0, &pdg, sizeof(pdg), IOCTL_DISK_GET_DRIVE_GEOMETRY))
	{
		wprintf(L"Drive path      = %ws\n", wszDrive);
		wprintf(L"Cylinders       = %ld\n", pdg.Cylinders);
		wprintf(L"Tracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
		wprintf(L"Sectors/track   = %ld\n", (ULONG)pdg.SectorsPerTrack);
		wprintf(L"Bytes/sector    = %ld\n", (ULONG)pdg.BytesPerSector);

		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
			(ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
		wprintf(L"Disk size       = %I64d (Bytes)\n"
			L"                = %.2f (Gb)\n",
			DiskSize, (double)DiskSize / (1024 * 1024 * 1024));
		return TRUE;
	}else{
		wprintf(L"GetDriveGeometry failed. Error %ld.\n", GetLastError());
		return FALSE;
	}
}

#define BITMAP_CHUNK 64*1024*1024

BOOL GetVolumeBitmap(LPCTSTR path) {
	VOLUME_BITMAP_BUFFER* bitmap;
	STARTING_LCN_INPUT_BUFFER lcnInputBuffer;
	UINT32 bitmapSize = BITMAP_CHUNK + sizeof(LARGE_INTEGER)*2;
	bitmap = (VOLUME_BITMAP_BUFFER*)malloc(bitmapSize);
	lcnInputBuffer.StartingLcn.QuadPart = 0;
	UINT32 largestClusterSize = 0,workingClusterSize = 0;	
	BOOL res;
	DWORD errCode;
	while (TRUE) {
		res = DeviceIOControlOp(path, &lcnInputBuffer, sizeof(lcnInputBuffer), bitmap, bitmapSize, FSCTL_GET_VOLUME_BITMAP);
		if (res == FALSE) {
			errCode = GetLastError();
			if (errCode != ERROR_MORE_DATA) {
				wprintf(L"GetVolumeBitmap failed (Gain admin rights to access volume info). Error %ld.  %ws \n ", errCode, path);
				return FALSE;
			}
		}

		for (int i = 0; i < bitmapSize; i++) {
			if (bitmap->Buffer[i] == 0) {
				workingClusterSize++;
			}
			else {
				if (workingClusterSize >= largestClusterSize) {
					largestClusterSize = workingClusterSize;
					workingClusterSize = 0;
				}
			}
		}

		if(res == TRUE) {
			wprintf(L"Largest chained cluster size is (%ld)", largestClusterSize);
			return TRUE;
		}
	}
}

BOOL GetFileInfo(LPCTSTR path) {
	RETRIEVAL_POINTERS_BUFFER* buffer;
	STARTING_VCN_INPUT_BUFFER vcnInputBuffer;
	UINT32 bitmapSize = BITMAP_CHUNK + sizeof(LARGE_INTEGER) * 2;
	buffer = (RETRIEVAL_POINTERS_BUFFER*)malloc(bitmapSize);
	vcnInputBuffer.StartingVcn.QuadPart = 0;
	BOOL res;
	DWORD errCode;
	while (TRUE) {
		res = DeviceIOControlOp(path, &vcnInputBuffer, sizeof(vcnInputBuffer), buffer, bitmapSize, FSCTL_GET_RETRIEVAL_POINTERS);
		if (res == FALSE) {
			errCode = GetLastError();
			if (errCode != ERROR_MORE_DATA) {
				wprintf(L"GetFileInfo failed. Error %ld.  %ws \n ", errCode, path);
				return FALSE;
			}
		}

		wprintf(L"\nExtents count %ld", buffer->ExtentCount);	
		for (int i = 0; i < buffer->ExtentCount; i++) {
			wprintf(L"\n LCN: %ld NEXTVCN: %ld", buffer->Extents[i].Lcn, buffer->Extents[i].NextVcn);
		}

		if (res == TRUE) {
			return TRUE;
		}
	}
}

int wmain(int argc, wchar_t *argv[])
{
	GetDiskFreeSpaceInfo(NULL);
	CheckDiskGeometry(wszDrive);
	wchar_t VolumeName[MAX_PATH] = L"";
	GetVolumeBitmap(L"\\\\.\\C:");
	if (argc > 0) {
		wprintf(L"\n%ws", argv[1]);
		GetFileInfo(argv[1]);
	}
}


