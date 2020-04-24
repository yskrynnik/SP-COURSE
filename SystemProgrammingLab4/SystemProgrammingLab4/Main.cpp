#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <locale.h>
using namespace std;
// Функция разархивации архива
bool unpack_files(LPSTR packed_file, LPSTR result_file);
// Функция архивации архива
bool pack_files(LPSTR unpacked_file, LPSTR result_file);
// Функция запуска дочернего процесса
bool launch_process(LPSTR command_line);
// Функция Вывода ошибки
void print_error();

int main()
{
	setlocale(LC_ALL, "Russian");
	int choice;
	for(;;)
	{
		system("cls");
		printf("Лабораторная работа № 4\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nПрограмма для работы с архивами\n\n");
		printf("Меню:\n");
		printf("1 - Упаковка архива\n");
		printf("2 - Распаковка архива\n");
		printf("3 - Выход\n");
		printf("Введите пункт меню:");
		scanf("%i", &choice);
		
		switch (choice)
		{
		case 1:
		{
			LPSTR unpacked = new CHAR[MAX_PATH];
			LPSTR result_file = new CHAR[MAX_PATH];

			printf("Путь к файлу или директории:");
			scanf("%s", unpacked);
			printf("Путь к результату архивации:");
			scanf("%s", result_file);
			if(pack_files(unpacked, result_file))
			{
				printf("Упаковка архива завершена успешно!\n");
			}
			break;
		}
		case 2:
		{
			LPSTR packed_file = new CHAR[MAX_PATH];
			LPSTR result_file = new CHAR[MAX_PATH];

			printf("Введите путь к архиву:");
			scanf("%s", packed_file);
			printf("Введите директорию куда распаковывать архив:");
			scanf("%s", result_file);
			if(unpack_files(packed_file, result_file))
			{
				printf("Распаковка архива завершена успешно!\n");
			}
			break;
		}
		case 3: return 0; break;
		default:printf("Выбран неверный пункт меню попробуйте еще раз!\n");
		}
		system("pause");
		system("cls");
	}
}
	bool unpack_files(LPSTR packed_file, LPSTR result_file)
	{
		LPSTR command_line = new CHAR[MAX_PATH];
		ZeroMemory(command_line, MAX_PATH);
		// Установка пути к 7z and и объкта архивирования .zip
		strncpy(command_line, "C:\\Program Files\\7-Zip\\7z.exe e", MAX_PATH - strlen(command_line));
		strncat(command_line, packed_file, MAX_PATH - strlen(command_line));
		// -o set установка директории для распаковки
		strncat(command_line, " -o", MAX_PATH - strlen(command_line));
		strncat(command_line, result_file, MAX_PATH - strlen(command_line));
		// -y Авто-ответ на все вопросы которые может спросить система да
		strncat(command_line, " -y", MAX_PATH - strlen(command_line));
		return launch_process(command_line);
	}
	bool pack_files(LPSTR unpacked_file, LPSTR result_file)
	{
		LPSTR command_line = new CHAR[MAX_PATH];
		ZeroMemory(command_line, MAX_PATH);
		// Указание пути к 7z и результирующему архиву
		strncpy(command_line, "C:\\Program Files\\7-Zip\\7z.exe a -tzip ", MAX_PATH - strlen(command_line));
		strncat(command_line, result_file, MAX_PATH - strlen(command_line));
		strncat(command_line, " ", MAX_PATH - strlen(command_line));
		strncat(command_line, unpacked_file, MAX_PATH - strlen(command_line));
		return launch_process(command_line);
	}
	bool launch_process(LPSTR command_line){
			// Создание пайпов
			HANDLE read_pipe;
			HANDLE write_pipe;
			// Установка атрибутов безопасности
			SECURITY_ATTRIBUTES security_attributes;
			security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
			security_attributes.bInheritHandle = TRUE;
			security_attributes.lpSecurityDescriptor = NULL;

			CreatePipe(&read_pipe, &write_pipe, &security_attributes, 0);
			if (read_pipe == INVALID_HANDLE_VALUE || write_pipe == INVALID_HANDLE_VALUE)
			{
				print_error();
				return false;
			}

			if (!SetHandleInformation(read_pipe, HANDLE_FLAG_INHERIT, 0))
			{
				print_error();
				return false;
			}

			STARTUPINFOA startup_info;
			ZeroMemory(&startup_info, sizeof(STARTUPINFOA));
			startup_info.cb = sizeof(startup_info);
			startup_info.hStdError = write_pipe;
			startup_info.dwFlags |= STARTF_USESTDHANDLES;

			PROCESS_INFORMATION process_information;
			ZeroMemory(&process_information, sizeof(process_information));

			if (!CreateProcessA(NULL, command_line, NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startup_info, &process_information)) {
				print_error(); // Выводим ошибку если не удалось создать процесс.
				return false;
			}
			WaitForSingleObject(process_information.hProcess, INFINITE);
			DWORD readed = 0;
			LPSTR result = new CHAR[1024];
			ZeroMemory(result, 1024);
			OVERLAPPED overlapped;
			while (ReadFile(read_pipe, result, 1024, &readed, &overlapped))
			{
				printf("%s", result);
			}
			CloseHandle(read_pipe);
			CloseHandle(write_pipe);
			CloseHandle(process_information.hProcess);
			CloseHandle(process_information.hThread);
			return true;
	}

void print_error()
{
	printf("\n***Ошибка!***\nТекст ошибки:");
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
	PTCHAR res((char*)cstr);
	printf("%s", res);
}