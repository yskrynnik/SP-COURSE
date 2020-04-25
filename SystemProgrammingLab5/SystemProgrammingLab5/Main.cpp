#include <stdio.h>
#include <Windows.h>
#include <locale.h>
HANDLE mutex = CreateMutexA(NULL, FALSE, "MyMutex");

int main()
{
	setlocale(LC_ALL, "russian");
	HANDLE mutex = CreateMutex(NULL, FALSE, "SpLAB5Prog");
	DWORD result = WaitForSingleObject(mutex, 0);
	printf("Лабораторная работа № 5\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\nЗапуск единственного экземпляра\n\n");
	if (result == WAIT_OBJECT_0)
	{
		printf("Программа запущена!\n");
		system("pause");
		ReleaseMutex(mutex);
	}
	else
	{
		printf("Не удалось запустить программу!\n");
		system("pause");
	}
	CloseHandle(mutex);
	return 0;
}