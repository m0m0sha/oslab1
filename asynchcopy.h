#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

//���������� ��������������� �������� �����-������
const int OverlappingOperationsAmount = 16;
//���������, ������������ ��� ��������������� ������� �����, ������������� �� ���� �������� �����-������
const int BlockSizeClustersMultiplier = 16;

	//��� ��������� ������������� ��� �������� �������� ������ � �������� � ����� ���������� � ������������ ����� �� ������� � ������� �����

typedef union {
	unsigned long long total;
	struct {
		DWORD lesser;
		DWORD higher;
	};
} size;

	//��� ��������� ������������� ��� �������� ������ � �������� � ��������� ��������� ������ (��������� �� ��������� ����������� ��������������� ���� hEvent � OVERLAPPED)

typedef struct {
	void* buffer;
	size currentOffset;
} threadProps;

HANDLE src, dst;
size filesize, clustersize;

void _stdcall afterWriteRoutine(DWORD errorCode, DWORD bytesTransfered, LPOVERLAPPED props);
void _stdcall afterReadRoutine(DWORD errorCode, DWORD bytesTransfered, LPOVERLAPPED props);

	//������������ ���������� ����� �������� ������; 
	//����� �� ����������, ����� �� ��� ���������� ����������� � ������� ������, � ���������� ���, ���� ��� ����������

void _stdcall afterWriteRoutine(DWORD errorCode, DWORD bytesTransfered, LPOVERLAPPED props) {
	//��������� threadProps ���������� ����� hEvent, ������� �� ��������� ���
	threadProps* tprop = static_cast<threadProps*>(props->hEvent);

	//�������� �����
	tprop->currentOffset.total += clustersize.total * OverlappingOperationsAmount;
	props->Offset = tprop->currentOffset.lesser;
	props->OffsetHigh = tprop->currentOffset.higher;

	//��� ����� ���������� �����������, ������ ���� �������� ����� ��������� � �������� ������ �����
	if (filesize.total > tprop->currentOffset.total)
		ReadFileEx(src,
			tprop->buffer,
			clustersize.total,
			props,
			afterReadRoutine);
}

	//������������ ���������� ����� �������� ������;
	//����� �� ���������� ����������� ������ � ���� �����������

void _stdcall afterReadRoutine(DWORD errorCode, DWORD bytesTransfered, LPOVERLAPPED props) {
	//��������� threadProps ���������� ����� hEvent, ������� �� ��������� ���
	threadProps* tprop = static_cast<threadProps*>(props->hEvent);

	WriteFileEx(dst,
		tprop->buffer,
		bytesTransfered,
		props,
		afterWriteRoutine);
}

	//�������� ���� � ������ OverlappingOperationsAmount, ������� ��������� ��������������� �������� �����-������

void copyFileAsynch() {
	wchar_t srcPath[MAX_PATH], dstPath[MAX_PATH], srcRoot[] = L"A:\\";
	wchar_t answer;

	threadProps tprops[OverlappingOperationsAmount];
	OVERLAPPED props[OverlappingOperationsAmount];

	DWORD sectorPerCluster, bytesPerSector, mock, startTime, endTime;

	cout << "\n�� ������������� ������ ���������� ���� ����������? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) != 'Y') return;

	//����������� ���������
	cout << "������� ������ ��� �����, �� ����� 255 ��������(�������� C : \\Users\\User\\file.mp4): ";
	wcin.getline(srcPath, MAX_PATH, L'\n');

	//����������� ������ ����������
	cout << "������� ������ ��� ����� �����, �� ����� 255 ��������(�������� C : \\Users\\User\\newfile.mp4): ";
	wcin.getline(dstPath, MAX_PATH, L'\n');

	//�������� ������
	src = CreateFile(srcPath,
		FILE_GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED,
		NULL);
	dst = CreateFile(dstPath,
		FILE_GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_NEW,
		FILE_FLAG_OVERLAPPED,
		NULL);

	//�������� ������������ ��������
	if (src == INVALID_HANDLE_VALUE || dst == INVALID_HANDLE_VALUE) {
		cout << "�������� ���� �� ��� ������ ��� ����� �� ���� �������\n"
			<< "��������� ������������ �����, �������� ����������, � ����� � ���." << std::endl;
		return;
	}

	//����������� �������
	filesize.lesser = GetFileSize(src, &filesize.higher);

	//����������� ������� ��������
	srcRoot[0] = srcPath[0];
	//�������������� �������� �������� ������������ ����� ���������
	GetDiskFreeSpace(srcRoot, &sectorPerCluster, &bytesPerSector, &mock, &mock);
	clustersize.total = sectorPerCluster * bytesPerSector * BlockSizeClustersMultiplier;

	startTime = GetTickCount();

	for (int i = 0; i < OverlappingOperationsAmount; ++i) {
		tprops[i].buffer = malloc(clustersize.total + 1);
		tprops[i].currentOffset.total = i * clustersize.total;

		props[i].Internal = 0;
		props[i].InternalHigh = 0;
		props[i].Pointer = 0;
		props[i].Offset = tprops[i].currentOffset.lesser;
		props[i].OffsetHigh = tprops[i].currentOffset.higher;

		props[i].hEvent = tprops + i;

		ReadFileEx(src,
			tprops[i].buffer,
			clustersize.total,
			props + i,
			afterReadRoutine);
	}

	//��������� ��������� ��������
	while (SleepEx(100, true));

	endTime = GetTickCount();
	cout << "�������! �������� ����������. " << endTime - startTime << " ��." << std::endl;

	//�������� ������, ������������ ������
	CloseHandle(src);
	CloseHandle(dst);
	for (int i = 0; i < OverlappingOperationsAmount; ++i) free(tprops[i].buffer);
}