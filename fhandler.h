#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//makeFolder() ������� ������� � �����, ���������� �� ������������, ���� ����� ����� �� ����������
	//���� � ����� ������ ���� ������: �:\Users\User\Folder
	//� ���� ������ � �:\Users\User\ ����� ������� ����� ����� � ������ Folder
	//� ��������� ������ ����� ����� ����� ���� ������� ������ � ����� � ����������
	//��� ������� �� �������� ���������� (�.�. ������ ��������� ����� �� ����� ���� ������� ����� �������)

void makeFolder() {
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\n�� ������������� ������ ������� ����� �������? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\Folder): ";
		wcin.getline(path, MAX_PATH, L'\n');

		if (CreateDirectory(path, NULL))
			cout << "������� ������� ������" << std::endl;
		else
			cout << "��������� ������! ��������, �� ����� �������� ���" << std::endl;
	}
}

	//removeFolder() ������� ������� � �����, ���������� �� ������������
	//���� � ����� ������ ���� ������: �:\Users\User\Folder
	//� ���� ������ � �:\Users\User\ ����� � ������ Folder ����� �������
	//� ��������� ������ ����� ������ ������ �������, ����������� � ����� � ����������;
	//��� ������� ����� ������� ������ ������ �����

void removeFolder() {
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\n�� ������������� ������ ������� �������? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\Folder): ";
		wcin.getline(path, MAX_PATH, L'\n');

		if (RemoveDirectory(path))
			cout << "������� ������� ������" << std::endl;
		else
			cout << "��������� ������! ��������, �� ����� �������� ��� ��� ����� �� ���� ������" << std::endl;
	}
}
