#include <Windows.h>
#include <iostream>
#include <string.h>

using namespace std;

int main() 
{
	HANDLE hNamedPipe;
	DWORD dwBytesRead; //for of bytes that was read
	DWORD dwBytesWrite; //for amount of bytes that was written
	char pchMessage[80];
	int nMessageLength;
	//making named pipe for reading/writing
	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", // NAME
		PIPE_ACCESS_DUPLEX, // NOT EVEN A QUESTION
		PIPE_TYPE_MESSAGE | PIPE_WAIT, //SYNCHRONOUS TRANSMITION OF MSGS
		1,//MAX AMOUNT OF PIPE COPIES????
		0,//SIZE OF OUTPUT BUFFER
		0,//INPUT
		INFINITE,//CLIENT WAITING TIME
		NULL//SAFETY ATTRIBUTES
	);
		if (hNamedPipe == INVALID_HANDLE_VALUE) {
			cerr << "Create named pipe failed." << endl
				<< "The last error code is " << GetLastError() << endl;
			cin.get();
			system("pause");

			return 0;
		}

		cout << "The server is waiting for connection with a client." << endl;
		if (!ConnectNamedPipe(
			hNamedPipe,//PIPE HANDLE
			NULL//SYNCH CONNECTION 
		))
		{
			cerr << "Connectnamed pipe failed." << endl
				<< "The last errror code is " << GetLastError() << endl;
			CloseHandle(hNamedPipe);
			cin.get();
			system("pause");

			return 0;
		}
		while (pchMessage != "exit") {

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
			cin.getline(pchMessage, 80);
			nMessageLength = strlen(pchMessage) + 1;

			if (!WriteFile(
				hNamedPipe,
				pchMessage,
				nMessageLength,
				&dwBytesWrite,
				NULL
			)) {
				cerr << "Writing failed." << endl
					<< "The last error code is " << GetLastError() << endl;
				CloseHandle(hNamedPipe);
				cin.get();
				system("pause");

				return 0;
			}
		}
		CloseHandle(hNamedPipe);
		return 0;
}