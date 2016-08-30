#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

using namespace std;

void EnableDebugPriv()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}

void printProcessInfo(PROCESSENTRY32 pe32_)
{
	wcout << "Process ID: " << pe32_.th32ProcessID << " Name: " << pe32_.szExeFile << endl;
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

		DWORD processId = pe32.th32ProcessID;
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, processId);
		if (process == nullptr) {
			cerr << "Unable to OpenProcess() " << processId << endl;
			continue;
		}	
	} while ( Process32Next(processes, &pe32) );


	cout << "Program done. Exiting..." << endl;
	CloseHandle(processes);
	return true;
}

int main()
{
	EnableDebugPriv();

	cout << "Hello world!" << endl;

	GetProcessList();

	Sleep(20000);

	return 0;
}