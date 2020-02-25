#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <locale.h>

#define AmountOfBytesToRead 2048
void FromUnicodeToAscii(char*);
void FromAsciiToUnicode(char*);
// Функция для создания файла результата, и открытия исходного текста.
bool CreateResultAndOpenSourceFiles(char*, HANDLE*, HANDLE*);

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "Russian");
	if (argv[1] != NULL && argv[2] != NULL) {
		if (strcmp(argv[1], "-u") == 0) {
			FromUnicodeToAscii(argv[2]);
		}
		else if (strcmp(argv[1], "-a") == 0) {
			FromAsciiToUnicode(argv[2]);
		}
		else {
			printf("Неверные параметры!");
		}
	}
	else {
		printf("Введите параметры, и попробуйте еще раз!\n");
	}
	system("pause");
	return 0;
}

void FromUnicodeToAscii(char* path) {
	HANDLE readFile;
	HANDLE writeFile;

	if (CreateResultAndOpenSourceFiles(path, &readFile, &writeFile)) {
		BYTE buff[AmountOfBytesToRead];
		DWORD dwBytes;
		while (ReadFile(readFile, buff, AmountOfBytesToRead, (LPDWORD)&dwBytes, NULL))
		{
			if (dwBytes == 0) break;
			//сначала вы вызываете MultiByteToWideChar, чтобы получить размер конечной строки (юникод)
			int uLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buff, dwBytes, NULL, 0);
			//указатель на юникод строку, которую необходимо конвертировать
			wchar_t* uStr = new wchar_t[uLength];
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buff, dwBytes, uStr, uLength);
			//Вызов WideCharToMultiByte для получения размера конечной строки (ANSI)
			int aLength = WideCharToMultiByte(CP_ACP, 0, uStr, uLength, NULL, 0, NULL, NULL);
			//Указатель на буфер, который получает преобразованную строку.
			char* aStr = new char[aLength];
			WideCharToMultiByte(CP_ACP, 0, uStr, uLength, aStr, aLength, NULL, NULL);

			if (!WriteFile(writeFile, aStr, aLength * sizeof(char), &dwBytes, NULL)) {
				printf("Ошибка записи!");
				break;
			}
		}
		CloseHandle(readFile);
		CloseHandle(writeFile);
	}
	return;
}

void FromAsciiToUnicode(char* path) {
	HANDLE readFile;
	HANDLE writeFile;

	if (CreateResultAndOpenSourceFiles(path, &readFile, &writeFile)) {


		BYTE buff[AmountOfBytesToRead];
		DWORD dwBytes;

		while (ReadFile(readFile, buff, AmountOfBytesToRead, (LPDWORD)&dwBytes, NULL))
		{
			if (dwBytes == 0) break;
			//Определение размера строки Unicode
			int uLength = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buff, dwBytes, NULL, 0);
			wchar_t* uStr = new wchar_t[uLength];
			//Конечная перекодировка
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)buff, dwBytes, uStr, uLength);
			if (!WriteFile(writeFile, uStr, uLength * sizeof(wchar_t), (LPDWORD)&dwBytes, NULL)) {
				printf("Ошибка записи!");
				break;
			}
		}
		CloseHandle(readFile);
		CloseHandle(writeFile);
	}
	return;
}

bool CreateResultAndOpenSourceFiles(char* path, HANDLE* readFile, HANDLE* writeFile) {
	bool Result = true;
	*readFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	*writeFile = CreateFileA("C:\\Users\\Snoop_dog40\\Desktop\\recording_result.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (*readFile == INVALID_HANDLE_VALUE || *writeFile == INVALID_HANDLE_VALUE) Result = false;
	return Result;
}