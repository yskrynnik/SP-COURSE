#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

HANDLE semaphore;
// Генерация случайного integer
int random_integer(int min, int max);
// Функция очистки файла
bool clear_file(LPCSTR filename);
// Запуск потоков
HANDLE* launch_threads(int count);
// Функция потока
DWORD WINAPI thread_function(LPVOID param);

int main()
{
	setlocale(LC_ALL, "russian");
	int amount_of_handles; // Количество указателей
	int amount_of_threads; // Количество потоков
	srand(time(NULL));
	printf("Лабораторная работа № 5\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nКонтроль потоков\n\n");
	printf("Введите количество файловых указателей:");
	scanf("%i", &amount_of_handles);
	printf("Введите максимальное число потоков:");
	scanf("%i", &amount_of_threads);

	//Создание семафора
	semaphore = CreateSemaphoreA(NULL, amount_of_handles, amount_of_handles, "MySemaphore");
	if (semaphore == NULL) {
		printf("*** Произошла ошибка во время создания семайора ***");
		system("pause");
		return 1;
	}
	//Очистка файла
	if (clear_file("threadsresult.txt"))
	{
		printf("*** Произошла ошибка во время очистки файла ***");
		system("pause");
		return 1;
	}
	WaitForMultipleObjects(amount_of_threads, launch_threads(amount_of_threads), TRUE, INFINITE);
	system("pause");
	return 0;
}
HANDLE* launch_threads(int count)
{
	HANDLE* threads = new HANDLE[count];
	for (int i = 0; i < count; i++)
	{
		int* params = new int[2];
		params[0] = i; //Количество потоков
		params[1] = random_integer(1, 3); //Время сна.
		threads[i] = CreateThread(
			NULL, // дескриптор защиты
			0,  // начальный размер стека
			thread_function,  // функция потока
			(LPVOID)params, // параметры потока
			NULL, // опции создания
			NULL); // идентификатор потока
	}
	return threads;
}
 bool clear_file(LPCSTR filename)
{
	bool operation_failed = false;
	HANDLE file = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		operation_failed = true;
	}
	CloseHandle(file);
	return  operation_failed;
}
DWORD WINAPI thread_function(LPVOID param)
{
	int* params = (int*)param;
	clock_t start = clock();
	int thread_number = params[0];
	LPSTR str = new CHAR[128];

	DWORD result = WaitForSingleObject(semaphore, 500);
	while (result != WAIT_OBJECT_0)
	{
		result = WaitForSingleObject(semaphore, 1000);
		printf("Поток №%i ожидает указатель\n", thread_number);
	}
	printf("Поток №%i получил указатель. Уход в сон.\n", thread_number);
	Sleep(params[1] * 1000);
	HANDLE file = CreateFileA("threadsresult.txt", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		ReleaseSemaphore(semaphore, 1, NULL);
		return 0;
	}
	SetFilePointer(file, 0, NULL, FILE_END);
	clock_t finish = clock();
	float time_elapsed = (finish - start) / CLK_TCK;
	sprintf(str, "Поток №%i завершил свою работу спустя %f секунд\n\0", thread_number, time_elapsed);
	WriteFile(file, str, strlen(str), NULL, NULL);
	CloseHandle(file);

	printf("Поток №%i освободил указатель.\n", thread_number);
	ReleaseSemaphore(semaphore, 1, NULL); //Освобождение указателя
	return 0;
}

int random_integer(int min, int max)
{
	return min + rand() % (max + 1 - min);
}