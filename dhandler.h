#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//printDriveInfo(LPTSTR) получает юникодный путь к корню диска и выводит информацию о диске: тип, имя, серийный номер, максимальная длина имени файла, имя файловой системы, объем
void printDriveInfo(LPTSTR rootPath) {
	setlocale(LC_ALL, "Russian");
	const DWORD volNameMaxLength = 50, fileSystemNameLength = 10;

	UINT driveType;
	wchar_t volName[volNameMaxLength], fileSystemName[fileSystemNameLength];
	DWORD sectorPerCluster, bytesPerSector, numOfFreeClusters, totalNumOfClusters, maxComponentLength, serialNumber;

	//Тип диска
	driveType = GetDriveType(rootPath);
	switch (driveType) {
	case 0:
		cout << "\nНеопределимый\n";
		break;
	case DRIVE_REMOVABLE:
		cout << "\nСъемный диск\n";
		break;
	case DRIVE_FIXED:
		cout << "\nЖесткий диск\n";
		break;
	case DRIVE_REMOTE:
		cout << "\nСетевой диск\n";
		break;
	case DRIVE_CDROM:
		cout << "\nCD-ROM\n";
		break;
	case DRIVE_RAMDISK:
		cout << "\nRAM-диск\n";
		break;
	default:
		cout << "\nЧто-то неожиданное...\n";
	}

	//Имя
	GetVolumeInformationW(rootPath, volName, volNameMaxLength, &serialNumber, &maxComponentLength, NULL, fileSystemName, fileSystemNameLength);
	std::wcout << "Drive name: " << volName
		<< "\nSerial number: " << serialNumber
		<< "\nMax file name length: " << maxComponentLength
		<< "\nFile system: " << fileSystemName;

	//Пространство
	GetDiskFreeSpace(rootPath, &sectorPerCluster, &bytesPerSector, &numOfFreeClusters, &totalNumOfClusters);
	cout << "\nОбщее количество кластеров: " << totalNumOfClusters
		<< "\nКоличество свободных кластеров: " << numOfFreeClusters
		<< "\nСекторов на кластер: " << sectorPerCluster
		<< "\nБайтов на сектор: " << bytesPerSector << '\n' << std::endl;
}

	//printDrivesList() выводит имена всех дисков, которые можно найти в этом устройстве, и предлагает получить более подробную информацию о них

void printDrivesList() {
	setlocale(LC_ALL, "Russian");
	int position = 0;
	DWORD drives = GetLogicalDrives();

	wchar_t choice, desiredDisk;
	wchar_t path[] = L"A:\\";

	//получить все имена дисков с дисков DWORD путем сдвига битов
	cout << "Присутствующие диски: \n\n";
	for (DWORD i = 1; position < sizeof(DWORD) * 8; i <<= 1, ++position) {
		if (drives & i)
			cout << "Диск " << char('A' + position) << ":\n";
	}
	cout << "\nВсе найденные диски\n";


	cout << "\nХотите получить более подробную информацию об одном из дисков?\n"
		<< "Введите y если согласны, любую другую букву, если нет ";

	//получить информацию о диске, если требуется
	wcin >> choice;
	wcin.ignore(INT_MAX, '\n');
	while (towupper(choice) == 'Y') {
		cout << "Введите диск, о котором вы хотите узнать (например C): ";
		wcin >> desiredDisk;
		desiredDisk = towupper(desiredDisk);
		std::cin.ignore(INT_MAX, '\n');

		if (desiredDisk >= 'A' && desiredDisk <= 'Z' && drives & (1 << (desiredDisk - 'A'))) {
			path[0] = desiredDisk;
			printDriveInfo(path);
		}
		else cout << "Неправильный диск\n";

		cout << "\nХотите получить более подробную информацию еще об одном из дисков?\n"
			<< "Введите y если согласны, любую другую букву, если нет:\ ";
		wcin >> choice;
		wcin.ignore(INT_MAX, '\n');
	}
}
