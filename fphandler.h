#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;


	//desypherFlags() получает DWORD и печатает все флаги атрибутов файла, которые содержит DWORD

void desypherFlags(DWORD flags) {

	if (flags & FILE_ATTRIBUTE_READONLY) cout << "file is readonly;\n";
	if (flags & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) cout << "file is not fully present locally;\n";
	if (flags & FILE_ATTRIBUTE_RECALL_ON_OPEN) cout << "file is virtual;\n";
	if (flags & FILE_ATTRIBUTE_REPARSE_POINT)cout << "file is a symbolic link;\n";
	if (flags & FILE_ATTRIBUTE_SPARSE_FILE) cout << "file is sparse;\n";
	if (flags & FILE_ATTRIBUTE_SYSTEM) cout << "system uses only a part of this file;\n";
	if (flags & FILE_ATTRIBUTE_TEMPORARY) cout << "file is being used for temporary storage;\n";
	if (flags & FILE_ATTRIBUTE_VIRTUAL) cout << "file is reserved for system use;\n";
	if (flags & FILE_ATTRIBUTE_PINNED) cout << "file should be kept fully present locally even when not being actively accessed;\n";
	if (flags & FILE_ATTRIBUTE_UNPINNED) cout << "file should not be kept fully present locally except when being actively accessed;\n";
	if (flags & FILE_ATTRIBUTE_ARCHIVE) cout << "file is an archive;\n";
	if (flags & FILE_ATTRIBUTE_COMPRESSED) cout << "file is compressed;\n";
	if (flags & FILE_ATTRIBUTE_DEVICE) cout << "file is reserved for system use;\n";
	if (flags & FILE_ATTRIBUTE_DIRECTORY) cout << "file is a directory;\n";
	if (flags & FILE_ATTRIBUTE_ENCRYPTED) cout << "file is encrypted;\n";
	if (flags & FILE_ATTRIBUTE_HIDDEN) cout << "file is hidden;\n";
	if (flags & FILE_ATTRIBUTE_INTEGRITY_STREAM) cout << "file is configured with integrity;\n";
	if (flags & FILE_ATTRIBUTE_NORMAL) cout << "file has no special attributes;\n";
	if (flags & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) cout << "file not to be indexed by the content indexing service;\n";
	if (flags & FILE_ATTRIBUTE_NO_SCRUB_DATA) cout << "file is not to be read by the background data integrity scanner;\n";
	if (flags & FILE_ATTRIBUTE_OFFLINE) cout << "file is not available immediately;\n";
}
	//fillTime() получает указатель на структуру SYSTEMTIME и заполняет ее содержимым, полученным от пользователя

void fillTime(SYSTEMTIME* time) {
	WORD value;

	cout << "Вы должны ввести время. Обратите внимание, что это должно быть UTC 0.\n";
	cout << "Введите год (например, 2022): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wYear = value;
	cout << "Введите месяц (например, 10): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMonth = value;
	cout << "Введите день (например, 1): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wDay = value;
	cout << "Введите час (например, 7): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wHour = value;
	cout << "Введите минуты (например, 9): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMinute = value;
	cout << "Введите секунды (например, 2): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wSecond = value;
}

	//processFileAttributes() выводит информацию о файле, путь к которому пользователь указывает с клавиатуры
	//после этого функция предоставляет возможность изменить флаги файла и время

void processFileAttributes() {
	_BY_HANDLE_FILE_INFORMATION fileInfo;
	SYSTEMTIME time;
	FILETIME fileTime;
	HANDLE file;
	wchar_t answer;
	wchar_t path[MAX_PATH];

	DWORD newFlags;

	cout << "\nВы действительно хотите работать с атрибутами файлов? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) != 'Y') return;

	// попытка открыть файл
	cout << "Введите полное имя файла, не более 255 символов (например C:\\Users\\User\\file.mp4): ";
	wcin.getline(path, MAX_PATH, L'\n');
	file = CreateFile(path,
		GENERIC_READ | FILE_WRITE_ATTRIBUTES,
		FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "Файл не существует" << std::endl;
		return;
	}

	// попытка получить информацию
	if (!GetFileInformationByHandle(file, &fileInfo)) {
		cout << "Невозможно получить информацию о файле" << std::endl;
		return;
	}

	// расшифровка флагов
	desypherFlags(fileInfo.dwFileAttributes);

	// вывод времени
	FileTimeToSystemTime(&(fileInfo.ftCreationTime), &time);
	cout << "Файл был создан в: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "ч : " << time.wMinute << "м : " << time.wSecond << "с UTC 0,\n";
	FileTimeToSystemTime(&(fileInfo.ftLastAccessTime), &time);
	cout << "Последний раз к файлу обращались в: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "ч : " << time.wMinute << "м : " << time.wSecond << "с UTC 0,\n";
	FileTimeToSystemTime(&(fileInfo.ftLastWriteTime), &time);
	cout << "Последний раз файл был изменен в: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "ч : " << time.wMinute << "м : " << time.wSecond << "с UTC 0,\n";

	// другая информация
	cout << "Файл хранится на томе с серийным номером: " << fileInfo.dwVolumeSerialNumber;
	cout << ",\nРазмер файла: " << fileInfo.nFileSizeHigh << fileInfo.nFileSizeLow;
	cout << " байтов,\nКоличество ссылок на этот файл: " << fileInfo.nNumberOfLinks;
	cout << ",\nИдентификатор файла: " << fileInfo.nFileIndexHigh << fileInfo.nFileIndexLow << std::endl;

	// changing flags
	cout << ".\nВы хотите изменить флаги файлов? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		cout << "Введите число флагов. Если у вас есть вопросы,\nознакомьтесь https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants: ";
		wcin >> newFlags;
		if (SetFileAttributes(path, newFlags))
			cout << "Флаги установлены\n";
		else
			cout << "Не удалось установить флаги\n";
	}

	// changing time
	cout << "Вы хотите изменить время создания файла? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, &fileTime, NULL, NULL))
			cout << "Время установлено\n";
		else
			cout << "Не удалось установить время\n";
	}

	cout << "Вы хотите изменить время последнего доступа к файлу? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, NULL, &fileTime, NULL))
			cout << "Время установлено\n";
		else
			cout << "Не удалось установить время\n";
	}

	cout << "Вы хотите изменить время последнего изменения файла? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, NULL, NULL, &fileTime))
			cout << "Время установлено\n";
		else
			cout << "Не удалось установить время\n";
	}

	CloseHandle(file);
}
