#include <iostream>

#include "dhandler.h"
#include "fhandler.h"
#include "fphandler.h"
#include "asynchcopy.h"
#include "flhandler.h"

using std::cout;
using std::cin;
using std::endl;

/*
    help() показывает, что это приложение может сделать
*/
void help() {
    cout << "Введите a, чтобы увидеть все диски\n"
        << "введите b, чтобы начать создание новой папки\n"
        << "введите c, чтобы начать удаление папки\n"
        << "введите d, чтобы начать создание нового файла\n"
        << "введите e, чтобы начать перемещение файла по другому пути\n"
        << "введите f, чтобы начать копирование файла по другому пути\n"
        << "введите g, чтобы начать асинхронное копирование файла (1.2)\n"
        << "введите h, чтобы получить информацию об указанных атрибутах файла и изменить их при необходимости\n"
        << "введите i, чтобы закрыть это приложение\n\n"<< endl;
}


int main()
{
    setlocale(LC_ALL, "Russian");
    char option = 'y';

    // массив всех функций, которые пользователь может вызывать с этого интерфейса
    void (*funcs[])() = {
        printDrivesList,
        makeFolder,
        removeFolder,
        createNewFile,
        moveFile,
        copyFile,
        copyFileAsynch,
        processFileAttributes
    };

    while (option != 'I') {
        system("cls");
        help();

        cout << "Сделайте свой выбор и нажмите Enter: ";
        cin >> option;
        option = toupper(option);
        std::cin.ignore(INT_MAX, '\n');

        if (option >= 'A' && option <= 'I') {
            if (option != 'I')
                funcs[option - 'A']();
        }
        else cout << "\nНеправильный выбор\n";

        system("pause");
    }

    cout << "\nПрограмма закончила работу.\n";
    return 0;
}