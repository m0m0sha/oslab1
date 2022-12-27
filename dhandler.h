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
	DWORD sectorPerCluster, bytesPerSector, numOfFreeClusters, totalNumOfClusters, maxComponentLength, serialNumber, flags;

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
	GetVolumeInformationW(rootPath, volName, volNameMaxLength, &serialNumber, &maxComponentLength, &flags, fileSystemName, fileSystemNameLength);
	std::wcout << "Drive name: " << volName
		<< "\nSerial number: " << serialNumber
		<< "\nMax file name length: " << maxComponentLength
		<< "\nFile system: " << fileSystemName;
	// флаги
	if (flags & FILE_CASE_SENSITIVE_SEARCH)
		cout << "\nУказанный том поддерживает имена файлов с учетом регистра.";
	if (flags & FILE_CASE_PRESERVED_NAMES)
		cout << "\nУказанный том поддерживает сохраненный регистр имен файлов при размещении имени на диске.";
	if (flags & FILE_UNICODE_ON_DISK)
		cout << "\nУказанный том поддерживает Unicode в именах файлов, которые появляются на диске.";
	if (flags & FILE_PERSISTENT_ACLS)
		cout << "\nУказанный том сохраняет и обеспечивает соблюдение списков управления доступом (ACL).";
	if (flags & FILE_FILE_COMPRESSION)
		cout << "\nУказанный том поддерживает сжатие на основе файлов.";
	if (flags & FILE_VOLUME_QUOTAS)
		cout << "\nУказанный том поддерживает дисковые квоты.";
	if (flags & FILE_SUPPORTS_SPARSE_FILES)
		cout << "\nУказанный том поддерживает разреженные файлы.";
	if (flags & FILE_SUPPORTS_REPARSE_POINTS)
		cout << "\nУказанный том поддерживает точки повторной обработки.";
	if (flags & FILE_SUPPORTS_REMOTE_STORAGE)
		cout << "\nУказанный том поддерживает удаленное хранилище.";
	if (flags & FILE_RETURNS_CLEANUP_RESULT_INFO)
		cout << "\nУказанный том возвращает информацию о результате очистки.";
	if (flags & FILE_SUPPORTS_POSIX_UNLINK_RENAME)
		cout << "\nУказанный том поддерживает переименование posix unlink.";
	if (flags & FILE_VOLUME_IS_COMPRESSED)
		cout << "\nУказанный том является сжатым томом, например томом DoubleSpace.";
	if (flags & FILE_SUPPORTS_OBJECT_IDS)
		cout << "\nУказанный том поддерживает идентификаторы объектов.";
	if (flags & FILE_SUPPORTS_ENCRYPTION)
		cout << "\nУказанный том поддерживает зашифрованную файловую систему (EFS).";
	if (flags & FILE_NAMED_STREAMS)
		cout << "\nУказанный том поддерживает именованные потоки.";
	if (flags & FILE_READ_ONLY_VOLUME)
		cout << "\nУказанный том доступен только для чтения.";
	if (flags & FILE_SEQUENTIAL_WRITE_ONCE)
		cout << "\nУказанный том может быть записан только один раз.";
	if (flags & FILE_SUPPORTS_TRANSACTIONS)
		cout << "\nУказанный том поддерживает транзакции";
	if (flags & FILE_SUPPORTS_HARD_LINKS)
		cout << "\nФайловая система поддерживает прямые ссылки на другие устройства и разделы..";
	if (flags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
		cout << "\nУказанный том поддерживает расширенные атрибуты.";
	if (flags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
		cout << "\nФайловая система поддерживает открытие по FileID.";
	if (flags & FILE_SUPPORTS_USN_JOURNAL)
		cout << "\nУказанный том поддерживает журналы обновлений с порядковыми номерами (USN).";
	if (flags & FILE_SUPPORTS_INTEGRITY_STREAMS)
		cout << "\nУказанный том поддерживает потоки целостности.";
	if (flags & FILE_SUPPORTS_BLOCK_REFCOUNTING)
		cout << "\nУказанный том поддерживает совместное использование логических кластеров между файлами на одном томе..";
	if (flags & FILE_SUPPORTS_SPARSE_VDL)
		cout << "\nУказанный том поддерживает разреженный VDL..";
	if (flags & FILE_DAX_VOLUME)
		cout << "\nУказанный том является томом прямого доступа (DAX).";
	if (flags & FILE_SUPPORTS_GHOSTING)
		cout << "\nУказанный том поддерживает дублирование";

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
