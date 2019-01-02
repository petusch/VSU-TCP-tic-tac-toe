#include <windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	char computerName[80];
	char pipeName[80];
	HANDLE hNamedPipe;
	DWORD dwBytesWritten;
	DWORD dwBytesRead;
	char pchMessage[80];
	int nMessageLength;

	cout << "Enter aname of server computer ";
	cin >> computerName;
	wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe", computerName);
	if (!WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER)) {
		cout << "The canal doesn't exist" << endl;
		cout << "Error: " << GetLastError() << endl;
		cin.get();
		return 0;
	}
	else cout << "This canal was free" << endl;
	hNamedPipe = CreateFile(
		pipeName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with pipe failed." << endl
			<< "The last error code is " << GetLastError() << endl;
		cin.get();
		system("pause");
		return 0;
	}
	cin.get();
	while (pchMessage != "exit")
	{
		cin.getline(pchMessage, 80);
		nMessageLength = strlen(pchMessage) + 1;
		if (!WriteFile(
			hNamedPipe,
			pchMessage,
			nMessageLength,
			&dwBytesWritten,
			NULL
		)) {
			cerr << "Writing failed." << endl
				<< "The last error code is " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		if (!ReadFile(
			hNamedPipe,
			pchMessage,
			sizeof(pchMessage),
			&dwBytesRead,
			NULL
		))
		{
			cerr << "Data reading from pipe failed." << endl
				<< "The last error code is " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		cout << pchMessage << endl;
	}
	CloseHandle(hNamedPipe);
	return 0;
	
}