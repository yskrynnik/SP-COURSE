#include <stdio.h>
#include <Windows.h>
#include <locale.h>

// Function that generates system error.
void generate_error();
// Function that receive information about the current system.
void get_information();

int main(int argc, char *argv[])
{
	//D:\Xai\СП\SP-COURSE\SystemProgrammingLab2\Debug\SystemProgrammingLab2.exe
	//D:\Xai\СП\SP-COURSE\SystemProgrammingLab2\Debug\SystemProgrammingLab2.exe -s 
	//D:\Xai\СП\SP-COURSE\SystemProgrammingLab2\Debug\SystemProgrammingLab2.exe -e
	setlocale(LC_ALL, "Russian");
	if (argv[1] != NULL) {
		if (strcmp(argv[1],"-s") == 0)
		{
			get_information();
		}
		if (strcmp(argv[1], "-e") == 0)
		{
			generate_error();
		}
	}
	else
	{
		printf("Введите параметры, и попробуйте еще раз!\n");
	}
	system("pause");
	return 0;
}

void generate_error() {
	MEMORYSTATUS memory_info;
	// Receiving info about memory.
	GlobalMemoryStatus(&memory_info);
	// ========================================
	// Generate an error
	// Trying to allocate all virtual memory memory.
	if (LocalAlloc(LPTR, memory_info.dwTotalVirtual) == NULL) {
		LPVOID error_message;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&error_message,
			0, NULL);
		printf("Системное описания произошедшей ошибки:\n%s", error_message);
	}
	return;
}
void get_information() {
	//Contains information about system.
	SYSTEM_INFO system_info;
	//Copy the system information to the structure system_info. 
	GetSystemInfo(&system_info);
	// Printing information about system. 
	printf("System information: \n");
	printf("Oem Id: %u\n", system_info.dwOemId);
	printf("Processor architecture: %u\n", system_info.wProcessorArchitecture);
	printf("Page size: %u\n", system_info.dwPageSize);
	printf("Minimum application address: %lx\n", system_info.lpMinimumApplicationAddress);
	printf("Maximum application address: %lx\n", system_info.lpMaximumApplicationAddress);
	printf("Active processor mask: %u\n", system_info.dwActiveProcessorMask);
	printf("Number of processors: %u\n", system_info.dwNumberOfProcessors);
	printf("Processor type: %u\n", system_info.dwProcessorType);
}