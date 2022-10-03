#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//printDriveInfo(LPTSTR) �������� ��������� ���� � ����� ����� � ������� ���������� � �����: ���, ���, �������� �����, ������������ ����� ����� �����, ��� �������� �������, �����
void printDriveInfo(LPTSTR rootPath) {
	setlocale(LC_ALL, "Russian");
	const DWORD volNameMaxLength = 50, fileSystemNameLength = 10;

	UINT driveType;
	wchar_t volName[volNameMaxLength], fileSystemName[fileSystemNameLength];
	DWORD sectorPerCluster, bytesPerSector, numOfFreeClusters, totalNumOfClusters, maxComponentLength, serialNumber;

	//��� �����
	driveType = GetDriveType(rootPath);
	switch (driveType) {
	case 0:
		cout << "\n�������������\n";
		break;
	case DRIVE_REMOVABLE:
		cout << "\n������� ����\n";
		break;
	case DRIVE_FIXED:
		cout << "\n������� ����\n";
		break;
	case DRIVE_REMOTE:
		cout << "\n������� ����\n";
		break;
	case DRIVE_CDROM:
		cout << "\nCD-ROM\n";
		break;
	case DRIVE_RAMDISK:
		cout << "\nRAM-����\n";
		break;
	default:
		cout << "\n���-�� �����������...\n";
	}

	//���
	GetVolumeInformationW(rootPath, volName, volNameMaxLength, &serialNumber, &maxComponentLength, NULL, fileSystemName, fileSystemNameLength);
	std::wcout << "Drive name: " << volName
		<< "\nSerial number: " << serialNumber
		<< "\nMax file name length: " << maxComponentLength
		<< "\nFile system: " << fileSystemName;

	//������������
	GetDiskFreeSpace(rootPath, &sectorPerCluster, &bytesPerSector, &numOfFreeClusters, &totalNumOfClusters);
	cout << "\n����� ���������� ���������: " << totalNumOfClusters
		<< "\n���������� ��������� ���������: " << numOfFreeClusters
		<< "\n�������� �� �������: " << sectorPerCluster
		<< "\n������ �� ������: " << bytesPerSector << '\n' << std::endl;
}

	//printDrivesList() ������� ����� ���� ������, ������� ����� ����� � ���� ����������, � ���������� �������� ����� ��������� ���������� � ���

void printDrivesList() {
	setlocale(LC_ALL, "Russian");
	int position = 0;
	DWORD drives = GetLogicalDrives();

	wchar_t choice, desiredDisk;
	wchar_t path[] = L"A:\\";

	//�������� ��� ����� ������ � ������ DWORD ����� ������ �����
	cout << "�������������� �����: \n\n";
	for (DWORD i = 1; position < sizeof(DWORD) * 8; i <<= 1, ++position) {
		if (drives & i)
			cout << "���� " << char('A' + position) << ":\n";
	}
	cout << "\n��� ��������� �����\n";


	cout << "\n������ �������� ����� ��������� ���������� �� ����� �� ������?\n"
		<< "������� y ���� ��������, ����� ������ �����, ���� ��� ";

	//�������� ���������� � �����, ���� ���������
	wcin >> choice;
	wcin.ignore(INT_MAX, '\n');
	while (towupper(choice) == 'Y') {
		cout << "������� ����, � ������� �� ������ ������ (�������� C): ";
		wcin >> desiredDisk;
		desiredDisk = towupper(desiredDisk);
		std::cin.ignore(INT_MAX, '\n');

		if (desiredDisk >= 'A' && desiredDisk <= 'Z' && drives & (1 << (desiredDisk - 'A'))) {
			path[0] = desiredDisk;
			printDriveInfo(path);
		}
		else cout << "������������ ����\n";

		cout << "\n������ �������� ����� ��������� ���������� ��� �� ����� �� ������?\n"
			<< "������� y ���� ��������, ����� ������ �����, ���� ���:\ ";
		wcin >> choice;
		wcin.ignore(INT_MAX, '\n');
	}
}
