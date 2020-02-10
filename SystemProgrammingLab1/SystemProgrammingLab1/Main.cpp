#include <stdio.h>
#include <locale.h>
#include <Windows.h>
int main(void)
{
	setlocale(LC_CTYPE, "rus"); // смена кодовой таблицы символов - включение русского языка
	printf("Лабораторная работа № 1\nПо системному программированию\nВыполнил студент 535а группы Скрынник Егор\n\n");
	printf("TYPE\t\tSIZE\t\tMIN\t\tMAX\n");
	// Булевские
	printf("BOOL\t\t%i\t\t%i\t\t%i\n", sizeof(BOOL), FALSE, TRUE); 
	printf("BOOLEAN\t\t%i\t\t%i\t\t%i\n", sizeof(BOOLEAN), FALSE, TRUE);
	// Целочисленные
	printf("SHORT\t\t%i\t\t%i\t\t%i\n", sizeof(SHORT), -32768, 32767);
	printf("INT\t\t%i\t\t%i\t%i\n", sizeof(INT), -2147483647, 2147483647); 
	printf("LONG\t\t%i\t\t%i\t%i\n", sizeof(LONG), -2147483647, 2147483647); 
	// Целочисленные беззнаковые
	printf("BYTE\t\t%i\t\t%i\t\t%i\n", sizeof(BYTE), 0, 255);
	printf("WORD\t\t%i\t\t%i\t\t%i\n", sizeof(WORD), 0, 65535);
	printf("UINT\t\t%i\t\t%u\t\t%u\n", sizeof(UINT), 0, 4294967295);
	printf("ULONG\t\t%i\t\t%u\t\t%u\n", sizeof(ULONG), 0, 4294967295); 
	printf("DWORD\t\t%i\t\t%u\t\t%u\n", sizeof(DWORD), 0, 4294967295);
	// Вещественные
	printf("FLOAT\t\t%i\t\t%e\t%e\n", sizeof(FLOAT), 3.4E-38, 3.4E+38);
	printf("DOUBLE\t\t%i\t\t%e\t%e\n", sizeof(DOUBLE), 1.7E-308, 1.7E+308);
	// Символьные
	printf("CHAR\t\t%i\t\t%i\t\t%i\n", sizeof(CHAR), -128, 127);
	printf("WCHAR\t\t%i\t\t%i\t\t%i\n", sizeof(WCHAR), 0, 65535);
	printf("TCHAR\t\t%i\t\t%i\t\t%i\n", sizeof(TCHAR), 0, 65535);
	// Строковые
	printf("LPSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPSTR), 0, 4294967295);
	printf("LPCSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPCSTR), 0, 4294967295);
	printf("LPWSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPWSTR), 0, 4294967295);
	printf("LPCWSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPCWSTR), 0, 4294967295);
	printf("LPTSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPTSTR), 0, 4294967295);
	printf("LPCTSTR\t\t%i\t\t%u\t\t%u\n", sizeof(LPCTSTR), 0, 4294967295);
	// Handles
	printf("HANDLE\t\t%i\t\t%u\t\t%u\n", sizeof(HANDLE), 0, 4294967295);
	printf("HLOCAL\t\t%i\t\t%u\t\t%u\n", sizeof(HLOCAL), 0, 4294967295);
	printf("HGLOBAL\t\t%i\t\t%u\t\t%u\n", sizeof(HGLOBAL), 0, 4294967295);
	printf("HMODULE\t\t%i\t\t%u\t\t%u\n", sizeof(HMODULE), 0, 4294967295);
	printf("HMENU\t\t%i\t\t%u\t\t%u\n", sizeof(HMENU), 0, 4294967295);
	printf("HINSTANCE\t%i\t\t%u\t\t%u\n", sizeof(HINSTANCE), 0, 4294967295);
	system("pause");
	return 0;
}

