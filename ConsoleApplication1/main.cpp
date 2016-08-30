#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

using namespace std;

void printProcessInfo(PROCESSENTRY32 pe32_)
{
	cout << "Process ID: " << pe32_.th32ProcessID << endl;
}

BOOL GetProcessList()
{
	HANDLE processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (processes == INVALID_HANDLE_VALUE)
	{
		cerr << "Invalid handle value!" << endl;
		return false;
	}
	cout << "Got processes handle!" << endl;


	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);


	if (!Process32First(processes, &pe32))
	{
		cerr << "First process failed!" << endl;
		CloseHandle(processes);		
		return false;
	}
	cout << "Got first process handle!" << endl;

	do
	{
		printProcessInfo(pe32);
	} while ( Process32Next(processes, &pe32) );


	cout << "Program done. Exiting..." << endl;
	CloseHandle(processes);
	return true;
}

int main()
{
	cout << "Hello world!" << endl;

	GetProcessList();

	return 0;
}