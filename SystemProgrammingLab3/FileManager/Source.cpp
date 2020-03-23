#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>;
#include <iostream>
#include <locale.h>
using namespace std;
// Выбор директории
void SetDirectory(TCHAR *buff);
// Вывод файлов выбранной директории
void PrintDirectory(TCHAR *buff);
// Копирование файла
void FileCopy(TCHAR *buff);
// Создание директории
void MakeDirectory(TCHAR *buff);
// Подробная информация о файле
void DetailedInfoAboutFile(TCHAR *buff);
// Удаление файла или пустой директории
void DeleteFileOrDirectory(TCHAR *buff);
int main() {
	setlocale(LC_ALL, "rus");
	int choice;
	for (;;) {
		TCHAR buff[MAX_PATH];
		GetCurrentDirectory(sizeof(buff), buff);
		system("cls");
		printf("Лабораторная работа № 3\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nФайловый менеджер\n\n");
		printf("Текущая директория:%s\nМеню:\n",buff);
		printf("1 - Выбор директории\n");
		printf("2 - Печать файлов выбранной директории\n");
		printf("3 - Копировать файл\n");
		printf("4 - Создание директории\n");
		printf("5 - Получить подробную информацию о файле\n");
		printf("6 - Удаление файла или пустой директории\n");
		printf("7 - Выход\n");
		printf("Введите пункт меню:");
		scanf("%i", &choice);
		system("cls");
		switch (choice) {
		case 1: SetDirectory(buff); break;
		case 2: PrintDirectory(buff); break;
		case 3: FileCopy(buff); break;
		case 4:	MakeDirectory(buff); break;
		case 5: DetailedInfoAboutFile(buff); break;
		case 6: DeleteFileOrDirectory(buff); break;
		case 7: return 0; break;
		default:printf("Выбран неверный пункт меню попробуйте еще раз!\n");
		}
		system("pause");
		system("cls");
	}
}

void SetDirectory(TCHAR *buff) {
	for (;;) {
		printf("Введите полный путь к директории\n");
		printf("Например: C://folder\n>>");
		TCHAR path[MAX_PATH];
		getchar();
		fgets(path, MAX_PATH - 1,stdin);
		path[strlen(path) - 1] = '\0';
		if (!SetCurrentDirectory(path))
			printf("\n***Ошибка! Неверный путь к директории. ***\n");
		else {
			GetCurrentDirectory(sizeof(path), path);
			printf("Ваша текущая директория: %s\n",path);
			strcpy(buff, path);
			return;
		}
		system("pause");
	}
}
void PrintDirectory(TCHAR *buff) {
	printf("Файлы в текущей директории: \n");
	WIN32_FIND_DATA FindFileData;
	TCHAR find[MAX_PATH];
	strcpy(find, buff);
	strcat(find, "\\*");
	HANDLE hf = FindFirstFile(find, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do{
			SYSTEMTIME systime;
			FILETIME localfiletime;
			FileTimeToLocalFileTime(&FindFileData.ftCreationTime, &localfiletime);
			FileTimeToSystemTime(&localfiletime, &systime);
			LONGLONG size = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
			printf("%-40s %20lu bytes ",FindFileData.cFileName,size);
			printf("%2u.%u.%u %u:%u\n", systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute);
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}
void FileCopy(TCHAR *buff) {
	for (;;) {
		printf("Введите имя файла в текущей директории например lab1.txt\n>>");
		TCHAR find[MAX_PATH], s[MAX_PATH], newDirectory[MAX_PATH];
		strcpy(find, buff);
		getchar();
		fgets(s, MAX_PATH - 1, stdin);
		s[strlen(s) - 1] = '\0';
		strcat(find, "\\");
		strcat(find, s);
		printf("\nВведите путь куда следует копировать файл например(D://...)\n>>");
		fgets(newDirectory, MAX_PATH - 1, stdin);
		newDirectory[strlen(newDirectory) - 1] = '\0';
		strcat(newDirectory, "\\");
		strcat(newDirectory, s);
		if (CopyFile(find, newDirectory, false)) return;
		else cout << "***Ошибка! Неккоректный путь или имя файла ***\n";
	}
}
void MakeDirectory(TCHAR *buff) {
	TCHAR newDir[MAX_PATH], way[MAX_PATH];
	printf("\nВведите название новой директории\n>>");
	getchar();
	fgets(newDir, MAX_PATH - 1, stdin);
	newDir[strlen(newDir) - 1] = '\0';
	strcpy(way, buff);
	strcat(way, "\\");
	strcat(way, newDir);
	if (CreateDirectory(way, NULL))
		printf("Директория успешно создана\n");
	else
		printf("***Ошибка! Ну удалось создать директорию ***\n");
}
void DetailedInfoAboutFile(TCHAR *buff) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	printf("Введите имя файла:\n>>");
	getchar();
	fgets(file, MAX_PATH - 1, stdin);
	file[strlen(file) - 1] = '\0';
	strcpy(way, buff);
	strcat(way, "\\");
	strcat(way, file);
	HANDLE hFile1 = CreateFile(way, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BY_HANDLE_FILE_INFORMATION information;
	BOOL f = GetFileInformationByHandle(hFile1, &information);
	CloseHandle(hFile1);
	if (!f) {
		printf("*** Ошибка неккоректное имя файла ***\n");
	}
	else {
		printf("Файл ");
		if ((information.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0)
			printf("скрытый ");
		if ((information.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)!=0)
			printf("системный ");
		if ((information.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)!=0)
			printf("архив ");
		if ((information.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)!=0)
			printf("сжатый ");
		if ((information.dwFileAttributes & FILE_ATTRIBUTE_NORMAL )!=0)
			printf("нормальный ");
		cout << ".\n";
		SYSTEMTIME systime;
		FILETIME localfiletime;
		FileTimeToLocalFileTime(&information.ftCreationTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		printf("Время создания - %2u.%u.%u %u:%u\n", systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute);
		FileTimeToLocalFileTime(&information.ftLastWriteTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		printf("Время изменения - %2u.%u.%u %u:%u\n", systime.wDay, systime.wMonth, systime.wYear, systime.wHour, systime.wMinute);
	}
}
void DeleteFileOrDirectory(TCHAR *buff) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	printf("Введите имя файла или директории которую вы хотите удалить!\n>>");
	getchar();
	fgets(file, MAX_PATH - 1, stdin);
	file[strlen(file) - 1] = '\0';
	strcpy(way, buff);
	strcat(way, "\\");
	strcat(way, file);
	BOOL g = DeleteFile(way), k;
	if (!g) {
		k = RemoveDirectory(way);
	}
	if (!g && !k) {
		DWORD lastErrorCode = GetLastError();
		void* cstr; //В эту переменную будет записано сообщение
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR)&cstr,
			0,
			NULL
		);
		// Преобразование сообщения в читабельный вид
		PTCHAR res((char*)cstr);
		printf("%s", res);
	}
}

