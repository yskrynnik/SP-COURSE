#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

#define ARRAY_MAX_SIZE 60000000

CRITICAL_SECTION section;
int* array;
// Запуск потоков на выполнение
HANDLE* launch_threads(LPVOID use_critical_section);
// Функция потока
DWORD WINAPI thread_function(LPVOID param);
// Функция заполнения массива
void fill_array(int* array);
// Функция генерация случайного целого
int random_integer(int min, int max);

int main()
{
	setlocale(LC_ALL, "russian");	
	HANDLE* threads;
	clock_t start;
	float elapsed_time;
	srand(time(NULL));
	printf("Лабораторная работа № 5\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\n3х поточная работа\n\n");
	
	InitializeCriticalSection(&section);
	array = new int[ARRAY_MAX_SIZE];
	fill_array(array);
	start = clock();
	threads = launch_threads((LPVOID)TRUE);
	WaitForMultipleObjects(3, threads , TRUE, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("Время выполнения с использованием критической секции составляет %f секунд\n\n", elapsed_time);

	start = clock();
	threads = launch_threads((LPVOID)FALSE);
	WaitForMultipleObjects(3, threads , TRUE, INFINITE);
	for (int i = 0; i < 3; i++)
		CloseHandle(threads[i]);

	elapsed_time = ((float)(clock() - start)) / CLK_TCK;
	printf("Время выполнения без использования критической секции составляет %f секунд\n\n", elapsed_time);

	DeleteCriticalSection(&section);
	system("pause");
}
HANDLE* launch_threads(LPVOID use_critical_section)
{
	HANDLE* threads = new HANDLE[3];
	for (int i = 0; i < 3; i++)
	{
		int* params = new int[2];
		params[0] = i; //Тип операции
		params[1] = (int)use_critical_section;
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
DWORD WINAPI thread_function(LPVOID param)
{
	int* params = (int*)param;
	if ((bool)params[1])
	{
		while (!TryEnterCriticalSection(&section))
		{
			//Простой, ожиданиея пока освободится критическая секция
		}
	}
	int min = array[0];
	int max = array[0];
	float avg = 0;
	switch (params[0])
	{
		case 0:
		for (int i = 0; i < ARRAY_MAX_SIZE; i++)
		{
			if (min > array[i])
				min = array[i];
		}
		printf("Минимум: %i\n", min);
		break;
		case 1:
			
			for (int i = 0; i < ARRAY_MAX_SIZE; i++)
			{
				if (max < array[i])
					max = array[i];
			}
			printf("Максимум: %i\n", max);
		break;
		case 2:
			
			for (int i = 0; i < ARRAY_MAX_SIZE; i++)
			{
				avg += array[i];
			}
			printf("Среднее: %f\n", avg / ARRAY_MAX_SIZE);
		break;
	}
	if ((bool)params[1])
	{
		LeaveCriticalSection(&section);
	}
	return 0;
}

void fill_array(int* array)
{
	for (int i = 0; i < ARRAY_MAX_SIZE; i++)
	{
		array[i] = random_integer(0, 1000);
	}
}

int random_integer(int min, int max)
{
	return min + rand() % (max + 1 - min);
}

