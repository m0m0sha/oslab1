#pragma once

#include <iostream>

#include <windows.h>
#include <fileapi.h>

using std::cout;
using std::wcin;


	//desypherFlags() �������� DWORD � �������� ��� ����� ��������� �����, ������� �������� DWORD

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
	//fillTime() �������� ��������� �� ��������� SYSTEMTIME � ��������� �� ����������, ���������� �� ������������

void fillTime(SYSTEMTIME* time) {
	WORD value;

	cout << "�� ������ ������ �����. �������� ��������, ��� ��� ������ ���� UTC 0.\n";
	cout << "������� ��� (��������, 2022): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wYear = value;
	cout << "������� ����� (��������, 10): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMonth = value;
	cout << "������� ���� (��������, 1): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wDay = value;
	cout << "������� ��� (��������, 7): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wHour = value;
	cout << "������� ������ (��������, 9): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wMinute = value;
	cout << "������� ������� (��������, 2): ";
	wcin >> value;
	wcin.ignore(INT_MAX, '\n');
	time->wSecond = value;
}

	//processFileAttributes() ������� ���������� � �����, ���� � �������� ������������ ��������� � ����������
	//����� ����� ������� ������������� ����������� �������� ����� ����� � �����

void processFileAttributes() {
	_BY_HANDLE_FILE_INFORMATION fileInfo;
	SYSTEMTIME time;
	FILETIME fileTime;
	HANDLE file;
	wchar_t answer;
	wchar_t path[MAX_PATH];

	DWORD newFlags;

	cout << "\n�� ������������� ������ �������� � ���������� ������? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');

	if (towupper(answer) != 'Y') return;

	// ������� ������� ����
	cout << "������� ������ ��� �����, �� ����� 255 �������� (�������� C:\\Users\\User\\file.mp4): ";
	wcin.getline(path, MAX_PATH, L'\n');
	file = CreateFile(path,
		GENERIC_READ | FILE_WRITE_ATTRIBUTES,
		FILE_SHARE_DELETE | FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file == INVALID_HANDLE_VALUE) {
		cout << "���� �� ����������" << std::endl;
		return;
	}

	// ������� �������� ����������
	if (!GetFileInformationByHandle(file, &fileInfo)) {
		cout << "���������� �������� ���������� � �����" << std::endl;
		return;
	}

	// ����������� ������
	desypherFlags(fileInfo.dwFileAttributes);

	// ����� �������
	FileTimeToSystemTime(&(fileInfo.ftCreationTime), &time);
	cout << "���� ��� ������ �: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "� : " << time.wMinute << "� : " << time.wSecond << "� UTC 0,\n";
	FileTimeToSystemTime(&(fileInfo.ftLastAccessTime), &time);
	cout << "��������� ��� � ����� ���������� �: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "� : " << time.wMinute << "� : " << time.wSecond << "� UTC 0,\n";
	FileTimeToSystemTime(&(fileInfo.ftLastWriteTime), &time);
	cout << "��������� ��� ���� ��� ������� �: " << time.wDay << "." << time.wMonth << "." << time.wYear
		<< ", " << time.wHour << "� : " << time.wMinute << "� : " << time.wSecond << "� UTC 0,\n";

	// ������ ����������
	cout << "���� �������� �� ���� � �������� �������: " << fileInfo.dwVolumeSerialNumber;
	cout << ",\n������ �����: " << fileInfo.nFileSizeHigh << fileInfo.nFileSizeLow;
	cout << " ������,\n���������� ������ �� ���� ����: " << fileInfo.nNumberOfLinks;
	cout << ",\n������������� �����: " << fileInfo.nFileIndexHigh << fileInfo.nFileIndexLow << std::endl;

	// changing flags
	cout << ".\n�� ������ �������� ����� ������? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		cout << "������� ����� ������. ���� � ��� ���� �������,\n������������ https://learn.microsoft.com/en-us/windows/win32/fileio/file-attribute-constants: ";
		wcin >> newFlags;
		if (SetFileAttributes(path, newFlags))
			cout << "����� �����������\n";
		else
			cout << "�� ������� ���������� �����\n";
	}

	// changing time
	cout << "�� ������ �������� ����� �������� �����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, &fileTime, NULL, NULL))
			cout << "����� �����������\n";
		else
			cout << "�� ������� ���������� �����\n";
	}

	cout << "�� ������ �������� ����� ���������� ������� � �����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, NULL, &fileTime, NULL))
			cout << "����� �����������\n";
		else
			cout << "�� ������� ���������� �����\n";
	}

	cout << "�� ������ �������� ����� ���������� ��������� �����? ������� � ���� �������� ";
	wcin >> answer;
	wcin.ignore(INT_MAX, '\n');
	if (towupper(answer) == 'Y') {
		fillTime(&time);
		SystemTimeToFileTime(&time, &fileTime);
		if (SetFileTime(file, NULL, NULL, &fileTime))
			cout << "����� �����������\n";
		else
			cout << "�� ������� ���������� �����\n";
	}

	CloseHandle(file);
}
