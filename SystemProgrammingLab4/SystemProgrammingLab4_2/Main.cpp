#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <locale.h>

#define ARRAY_MAX_SIZE 7

CRITICAL_SECTION criticalSection;
HANDLE* threads;
int tls_index;
// Функция запуска потоков
void launch_threads(int count);
// Функция потоков
DWORD WINAPI thread_function(LPVOID arr);
// Функция подсчета массивов
void compute_array(int* main_array, int* result_array);
// Функция нахождения наибольшего делителя числа
int find_largest_divisor(int num);
// Функция нахождения суммы элементов массива
int array_sum(int* array);
// Функция вывода массивов
void print_array(int* arr);

int main()
{
	setlocale(LC_ALL, "Russian");
	int count;
	printf("Лабораторная работа № 4\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nПрограмма для работы с потоками\n\n");
	printf("Введите количество потоков:");
	scanf("%i", &count);

	threads = new HANDLE[count];
	srand(time(0));
	InitializeCriticalSection(&criticalSection);
	launch_threads(count);
	WaitForMultipleObjects(count, threads, TRUE, INFINITE);
	DeleteCriticalSection(&criticalSection);
	system("pause");
	return 0;
}

void launch_threads(int count)
{
	InitializeCriticalSection(&criticalSection);
	int** arrays = new int*[count];
	tls_index = TlsAlloc();

	for (int i = 0; i < count; i++)
	{
		arrays[i] = new int[ARRAY_MAX_SIZE];
		for (int j = 0; j < ARRAY_MAX_SIZE; j++)
			arrays[i][j] = rand() % 90 + 10;
		threads[i] = CreateThread(NULL, 0, thread_function, arrays[i], NULL, NULL);
	}
}

DWORD WINAPI thread_function(LPVOID param)
{
	EnterCriticalSection(&criticalSection);
	printf("Идентификатор потока: %u\n", GetCurrentThreadId());
	int* array = (int*)param;
	TlsSetValue(tls_index, (LPVOID)(new int[ARRAY_MAX_SIZE]));
	int sum = 0;

	printf("Массив потока: ");
	print_array(array);

	compute_array(array, (int*)TlsGetValue(tls_index));

	printf("\nПодсчитанный массив: ");
	print_array((int*)TlsGetValue(tls_index));

	sum = array_sum((int*)TlsGetValue(tls_index));
	printf("\nКонечная сумма: %d\n\n", sum);
	LeaveCriticalSection(&criticalSection);
	return 0;
}

void compute_array(int* main_array, int* result_array)
{
	for (int i = 0; i < ARRAY_MAX_SIZE; i++)
	{
		result_array[i] = find_largest_divisor(main_array[i]);
	}
}

int find_largest_divisor(int num)
{
	int j = num / 2;
	for (int i = j; i >= 2; i--)
	{
		if (num % i == 0)
			return i;
	}
}

int array_sum(int* array)
{
	int result = 0;
	for (int i = 0; i < ARRAY_MAX_SIZE; i++)
	{
		result += array[i];
	}
	return result;
}

void print_array(int* arr)
{
	for (int i = 0; i < ARRAY_MAX_SIZE; i++) {
		printf("%i ", arr[i]);
	}
}