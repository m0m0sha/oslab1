#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//createNewFile() позволяет пользователю создавать файл в указанном месте и определять атрибуты: права доступа, режим совместного использования
	//эта функция не работает рекурсивно (т.е. группа вложенных папок не может быть создана для хранения нового файла)

void createNewFile() {
	DWORD rights = 0, accessMode = 0;
	HANDLE result;
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\nВы действительно хотите создать новый файл? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//определение пути
		cout << "Введите полное имя файла, не более 255 символов (например C:\\Users\\User\\file.mp4): ";
		wcin.getline(path, MAX_PATH, L'\n');

		//определение прав доступа
		cout << "Введите права доступа к файлу (r_ только для чтения, _w только для записи, rw для чтения и записи)\n"
			<< "если из указанной позиции будет получена какая-либо неверная буква, то она будет обработана как _\n"
			<< "Ваш выбор: ";
		wcin >> answer;
		if (answer == 'r') rights |= GENERIC_READ;
		wcin >> answer;
		if (answer == 'w') rights |= GENERIC_WRITE;
		wcin.ignore(INT_MAX, '\n');

		//определение режима обмена
		cout << "Войдите в режим общего доступа, чтобы определить, могут ли другие процессы работать с новым файлом:\n"
			<< "'___' - нет обмена, только это приложение будет взаимодействовать с файлом\n"
			<< "'d__' - удаление доли и изменение имени\n"
			<< "'_r_' - поделиться чтением\n"
			<< "'__w' - поделиться записью\n"
			<< "вы также можете ввести любую комбинацию режимов (например, dr_ или drw), но соблюдайте порядок.\n"
			<< "если из указанной позиции будет получена какая-либо неверная буква, то она будет обработана как _"
			<< "Ваш выбор: ";
		wcin >> answer;
		if (answer == 'd') rights |= FILE_SHARE_DELETE;
		wcin >> answer;
		if (answer == 'r') rights |= FILE_SHARE_READ;
		wcin >> answer;
		if (answer == 'w') rights |= FILE_SHARE_WRITE;
		wcin.ignore(INT_MAX, '\n');

		//создание файла и предупреждение об ошибках, если они есть
		result = CreateFile(path, rights, accessMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (result != INVALID_HANDLE_VALUE) {
			CloseHandle(result);
			cout << "Файл успешно создан" << std::endl;
			return;
		}

		if (GetLastError() == ERROR_FILE_EXISTS) 
			cout << "Файл с таким именем уже существует" << std::endl;
		else
			cout << "Произошла ошибка! Вы правильно написали имя файла?" << std::endl;
	};
}

	//moveFile() позволяет пользователю перемещать файл из одного места в другое с возможностью дать файлу новое имя
	//функция отслеживает попытки перезаписать существующий файл и предотвращает их

void moveFile() {
	wchar_t answer;
	wchar_t src[MAX_PATH], dst[MAX_PATH];


	cout << "\nВы действительно хотите переместить файл? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//определение источника
		cout << "Введите полное имя файла, не более 255 символов (например C:\\Users\\User\\file.mp4): ";
		wcin.getline(src, MAX_PATH, L'\n');

		//определение пункта назначения
		cout << "Введите полное имя нового файла, не более 255 символов (например C:\\Users\\User\\newfile.mp4): ";
		wcin.getline(dst, MAX_PATH, L'\n');

		//перемещение файла
		if (MoveFile(src, dst)) 
			cout << "Успешно!" << std::endl;
		else if (GetLastError() == 183L)
			cout << "Целевой файл уже существует" << std::endl;
		else
			cout << "Ошибка! Вы ввели правильные пути?" << std::endl;
	}
}

	//copyFile() позволяет пользователю копировать файл с возможностью дать новое имя для копирования
	//Функция отслеживает попытки перезаписать существующий файл и предотвращает их

void copyFile() {
	wchar_t answer;
	wchar_t src[MAX_PATH], dst[MAX_PATH];


	cout << "\nВы действительно хотите скопировать файл? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//определение источника
		cout << "Введите полное имя файла, не более 255 символов (например C:\\Users\\User\\file.mp4): ";
		wcin.getline(src, MAX_PATH, L'\n');

		//определение пункта назначения
		cout << "Введите полное имя копии, не более 255 символов (например C:\\Users\\User\\newfile.mp4): ";
		wcin.getline(dst, MAX_PATH, L'\n');

		//копирование файла
		if (CopyFile(src, dst, true))
			cout << "Успешно!" << std::endl;
		else if (GetLastError() == 80L)
			cout << "Целевой файл уже существует" << std::endl;
		else
			cout << "Ошибка! Вы ввели правильные пути?" << std::endl;
	}
}
