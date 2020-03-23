#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <vector>
#include <locale.h>
using namespace std;
const char recordsFileName[] = "records.recordbase";
struct Record {
	UINT number;
	FILETIME creationTime;
	TCHAR data[81];
	UINT changesCounter;
};
struct Header {
	UINT notNullRecordsCount;
	UINT FileSize;
};
//	Cчитывает headerInfo из файла
Header ReadHeader();
//	Cчитывает все записи, идущие после структуры headerInfo
vector<Record> ReadRecs();
//	Выводите все записи
void PrintRecords();
//	Добавляет запись в конец
void AddRecord();
//	Изменяет запись по номеру
void ChangeRecord();
//	Удаляет запись по номеру
void DeleteRecord();
// Сохранение записей
void SaveRecords(Header headerInfo, vector <Record> records);
int main()
{
	setlocale(LC_ALL, "Russian");
	int choice;
	for (;;) {
		printf("Лабораторная работа № 3\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nПрограмма для работы с записями\n\n");
		printf("Меню:\n");
		printf("1 - Просмотр всех записей\n");
		printf("2 - Добавления записи\n");
		printf("3 - Изменение записи\n");
		printf("4 - Удаления записи\n");
		printf("5 - Выход\n");
		printf("Введите пункт меню:");
		scanf("%i", &choice);
		system("cls");
		switch (choice) {
		case 1: PrintRecords(); break;
		case 2: AddRecord(); break;
		case 3: ChangeRecord(); break;
		case 4:	DeleteRecord(); break;
		case 5: return 0; break;
		default:printf("Выбран неверный пункт меню попробуйте еще раз!\n");
		}
		system("pause");
		system("cls");
	}
}

Header ReadHeader() {
	Header headerInfo;
	DWORD readedBytes;
	HANDLE recordsFile = CreateFile(recordsFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(recordsFile, NULL, NULL, FILE_BEGIN);
	ReadFile(recordsFile, &headerInfo, sizeof(Header), &readedBytes, NULL);
	if (readedBytes == 0) {
		headerInfo.notNullRecordsCount = 0;
		headerInfo.FileSize = 0;
	}
	CloseHandle(recordsFile);
	return headerInfo;
}

vector<Record> ReadRecs() {
	HANDLE recordsFile = CreateFile(recordsFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(recordsFile, sizeof(Header), NULL, FILE_BEGIN);
	vector<Record> records(0);
	Record i;
	DWORD readedBytes = 0;
	for (;;) {
		bool k = ReadFile(recordsFile, &i, sizeof(Record), &readedBytes, NULL);
		if (readedBytes == 0) break;
		records.push_back(i);
	}
	CloseHandle(recordsFile);
	return records;
}
void PrintRecords() {
	SYSTEMTIME SysTime;
	vector <Record> records = ReadRecs();
	Header count = ReadHeader();
	printf("Размер файла: %u байтов\nКоличество непустых записей: %u\n", count.FileSize,count.notNullRecordsCount);
	for (int i = 0; i < records.size(); i++) {
		FileTimeToSystemTime(&records[i].creationTime, &SysTime);
		printf("#%i ",records[i].number);
		printf("%2u.%2u.%u %u:%u ",SysTime.wDay,SysTime.wMonth,SysTime.wYear,SysTime.wHour,SysTime.wMinute);
		printf("%5u ", records[i].changesCounter);
		printf("%s;\n", records[i].data);
	}
}
void AddRecord() {
	Header headerInfo = ReadHeader();
	vector <Record> records = ReadRecs();
	SYSTEMTIME k, s;
	GetLocalTime(&k);
	FILETIME fileTime;
	Record record;
	//Перевод времени из системного в файловое
	SystemTimeToFileTime(&k, &fileTime);
	FileTimeToSystemTime(&fileTime, &s);
	printf("Введите данные размером до 80 символов, не используя пробел или введите * чтобы создать пустую запись:\n");
	scanf("%s", record.data);
	if (strcmp(record.data, "*") == 0) {
		record.number = records.size();
		record.creationTime = fileTime;
		strcpy(record.data, "");
		record.changesCounter = 0;
	}
	else {
		record.number = records.size();
		record.creationTime = fileTime;
		record.changesCounter = 0;
		headerInfo.notNullRecordsCount++;
	}
	records.push_back(record);
	headerInfo.FileSize = sizeof(headerInfo) + sizeof(Record)*records.size();
	SaveRecords(headerInfo, records);
}
void ChangeRecord() {
	SYSTEMTIME sysTime;
	Header headerInfo = ReadHeader();
	vector <Record> records = ReadRecs();
	UINT recordNumber;
	printf("Введите номер записи которую вы хотите изменить:"); 
	scanf("%u",&recordNumber);
	try {
		FileTimeToSystemTime(&records.at(recordNumber).creationTime, &sysTime);
		printf("\nВыбранная запись:\n");
		printf("Номер записи: %u\n", records[recordNumber].number);
		printf("Время создания записи: %u.%u.%u %u:%u\n", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute);
		printf("Данные записи: %s\n", records[recordNumber].data);
		printf("Количество изменений: %u\n", records[recordNumber].changesCounter);
		printf("\nВведите новые данные(Без пробелов и меньше 80 символов)\nДля обнуления записи введите *\nВвод:");
		TCHAR str[81];
		strcpy(str, records[recordNumber].data);
		scanf("%s", records[recordNumber].data);
		if (strcmp(records[recordNumber].data, "*") == 0) strcpy(records[recordNumber].data, "");
		if (strcmp(str, "") == 0 && strcmp(records[recordNumber].data, "") != 0) headerInfo.notNullRecordsCount++;
		if (strcmp(str, "") != 0 && strcmp(records[recordNumber].data, "") == 0) headerInfo.notNullRecordsCount--;
		records[recordNumber].changesCounter++;
	}
	catch(const std::out_of_range& e)
	{
		printf("*** Произошла ошибка изменении записи ***\n");
	}
	SaveRecords(headerInfo, records);
}
void DeleteRecord() {
	Header headerInfo = ReadHeader();
	vector <Record> records = ReadRecs();
	try {
		int i;
		printf("Введите номер записи которую хотите удалить: ");
		scanf("%i", &i);
		if (strcmp(records.at(i).data, "") != 0) headerInfo.notNullRecordsCount--;
		records.erase(records.begin() + i);
		vector<Record>(records).swap(records);
		for (int k = 0; k < records.size(); k++) {
			records[k].number = k;
		}
	}
	catch (const std::out_of_range& e)
	{
	printf("\n*** Произошла ошибка при удалении записи ***");
	}
	SaveRecords(headerInfo, records);
}
void SaveRecords(Header headerInfo, vector <Record> records)
{
	HANDLE recordsFile = CreateFile(recordsFileName, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(recordsFile, NULL, NULL, FILE_BEGIN);
	WriteFile(recordsFile, &headerInfo, sizeof(Header), NULL, NULL);
	SetFilePointer(recordsFile, NULL, NULL, FILE_END);
	for (int i = 0; i < records.size(); i++) WriteFile(recordsFile, &records[i], sizeof(Record), NULL, NULL);
	CloseHandle(recordsFile);
}