#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//makeFolder() создает каталог с путем, полученным от пользователя, если такой папки не существует
	//путь к папке должен быть полным: С:\Users\User\Folder
	//в этом случае в С:\Users\User\ будет создана новая папка с именем Folder
	//в противном случае новая папка может быть создана только в папке с программой
	//эта функция не работает рекурсивно (т.е. группа вложенных папок не может быть создана одним вызовом)

void makeFolder() {
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\nВы действительно хотите создать новый каталог? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		cout << "Введите полное имя папки, не более 255 символов (например C:\\Users\\User\\Folder): ";
		wcin.getline(path, MAX_PATH, L'\n');

		if (CreateDirectory(path, NULL))
			cout << "Каталог успешно создан" << std::endl;
		else
			cout << "Произошла ошибка! Вероятно, вы ввели неверное имя" << std::endl;
	}
}

	//removeFolder() удаляет каталог с путем, полученным от пользователя
	//путь к папке должен быть полным: С:\Users\User\Folder
	//в этом случае в С:\Users\User\ папка с именем Folder будет удалена
	//в противном случае будет удален только каталог, находящийся в папке с программой;
	//эта функция может удалять только пустые папки

void removeFolder() {
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\nВы действительно хотите удалить каталог? Введите у если согласны ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		cout << "Введите полное имя папки, не более 255 символов (например C:\\Users\\User\\Folder): ";
		wcin.getline(path, MAX_PATH, L'\n');

		if (RemoveDirectory(path))
			cout << "Каталог успешно удален" << std::endl;
		else
			cout << "Произошла ошибка! Вероятно, вы ввели неверное имя или папка не была пустой" << std::endl;
	}
}
