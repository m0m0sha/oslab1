#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;

	//createNewFile() ��������� ������������ ��������� ���� � ��������� ����� � ���������� ��������: ����� �������, ����� ����������� �������������
	//��� ������� �� �������� ���������� (�.�. ������ ��������� ����� �� ����� ���� ������� ��� �������� ������ �����)

void createNewFile() {
	DWORD rights = 0, accessMode = 0;
	HANDLE result;
	wchar_t answer;
	wchar_t path[MAX_PATH];

	cout << "\n�� ������������� ������ ������� ����� ����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//����������� ����
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\file.mp4): ";
		wcin.getline(path, MAX_PATH, L'\n');

		//����������� ���� �������
		cout << "������� ����� ������� � ����� (r_ ������ ��� ������, _w ������ ��� ������, rw ��� ������ � ������)\n"
			<< "���� �� ��������� ������� ����� �������� �����-���� �������� �����, �� ��� ����� ���������� ��� _\n"
			<< "��� �����: ";
		wcin >> answer;
		if (answer == 'r') rights |= GENERIC_READ;
		wcin >> answer;
		if (answer == 'w') rights |= GENERIC_WRITE;
		wcin.ignore(INT_MAX, '\n');

		//����������� ������ ������
		cout << "������� � ����� ������ �������, ����� ����������, ����� �� ������ �������� �������� � ����� ������:\n"
			<< "'___' - ��� ������, ������ ��� ���������� ����� ����������������� � ������\n"
			<< "'d__' - �������� ���� � ��������� �����\n"
			<< "'_r_' - ���������� �������\n"
			<< "'__w' - ���������� �������\n"
			<< "�� ����� ������ ������ ����� ���������� ������� (��������, dr_ ��� drw), �� ���������� �������.\n"
			<< "���� �� ��������� ������� ����� �������� �����-���� �������� �����, �� ��� ����� ���������� ��� _"
			<< "��� �����: ";
		wcin >> answer;
		if (answer == 'd') rights |= FILE_SHARE_DELETE;
		wcin >> answer;
		if (answer == 'r') rights |= FILE_SHARE_READ;
		wcin >> answer;
		if (answer == 'w') rights |= FILE_SHARE_WRITE;
		wcin.ignore(INT_MAX, '\n');

		//�������� ����� � �������������� �� �������, ���� ��� ����
		result = CreateFile(path, rights, accessMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (result != INVALID_HANDLE_VALUE) {
			CloseHandle(result);
			cout << "���� ������� ������" << std::endl;
			return;
		}

		if (GetLastError() == ERROR_FILE_EXISTS) 
			cout << "���� � ����� ������ ��� ����������" << std::endl;
		else
			cout << "��������� ������! �� ��������� �������� ��� �����?" << std::endl;
	};
}

	//moveFile() ��������� ������������ ���������� ���� �� ������ ����� � ������ � ������������ ���� ����� ����� ���
	//������� ����������� ������� ������������ ������������ ���� � ������������� ��

void moveFile() {
	wchar_t answer;
	wchar_t src[MAX_PATH], dst[MAX_PATH];


	cout << "\n�� ������������� ������ ����������� ����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//����������� ���������
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\file.mp4): ";
		wcin.getline(src, MAX_PATH, L'\n');

		//����������� ������ ����������
		cout << "������� ������ ��� ������ �����, �� ����� 255 �������� (�������� C:\\Users\\User\\newfile.mp4): ";
		wcin.getline(dst, MAX_PATH, L'\n');

		//����������� �����
		if (MoveFile(src, dst)) 
			cout << "�������!" << std::endl;
		else if (GetLastError() == 183L)
			cout << "������� ���� ��� ����������" << std::endl;
		else
			cout << "������! �� ����� ���������� ����?" << std::endl;
	}
}

	//copyFile() ��������� ������������ ���������� ���� � ������������ ���� ����� ��� ��� �����������
	//������� ����������� ������� ������������ ������������ ���� � ������������� ��

void copyFile() {
	wchar_t answer;
	wchar_t src[MAX_PATH], dst[MAX_PATH];


	cout << "\n�� ������������� ������ ����������� ����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) == 'Y') {
		//����������� ���������
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\file.mp4): ";
		wcin.getline(src, MAX_PATH, L'\n');

		//����������� ������ ����������
		cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\newfile.mp4): ";
		wcin.getline(dst, MAX_PATH, L'\n');

		//����������� �����
		if (CopyFile(src, dst, true))
			cout << "�������!" << std::endl;
		else if (GetLastError() == 80L)
			cout << "������� ���� ��� ����������" << std::endl;
		else
			cout << "������! �� ����� ���������� ����?" << std::endl;
	}
}
