#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"WS2_32.lib")
#include <stdio.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <ctime> 
#include <math.h>
#include <windows.h>


using namespace std;

bool won = false;
void Dump(char board[]) {
	cout << board[0] << " | " << board[1] << " | " << board[2] << "\n";
	cout << "--------\n";
	cout << board[3] << " | " << board[4] << " | " << board[5] << "\n";
	cout << "--------\n";
	cout << board[6] << " | " << board[7] << " | " << board[8] << "\n";
}

bool CheckWin(char board[], char symbol) {
	// Is there a winner?
	// Check horizontal 
	if (board[0] == symbol && board[1] == symbol && board[2] == symbol) return true;
	if (board[3] == symbol && board[4] == symbol && board[5] == symbol) return true;
	if (board[6] == symbol && board[7] == symbol && board[8] == symbol) return true;
	// Check vertical
	if (board[0] == symbol && board[3] == symbol && board[6] == symbol) return true;
	if (board[1] == symbol && board[4] == symbol && board[7] == symbol) return true;
	if (board[2] == symbol && board[5] == symbol && board[8] == symbol) return true;
	// Check diagonals
	if (board[0] == symbol && board[4] == symbol && board[8] == symbol) return true;
	if (board[2] == symbol && board[4] == symbol && board[6] == symbol) return true;
	return false;
}
int main(int argc, char* argv[]) {
	char board[10] = { NULL };
	char ip[20] = "";
	cout << "Enter your ip: ";
	cin.getline(ip, sizeof(ip));
	char symbol = 'o';
	SOCKET clientSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0) {
		printf("The Winsock dll not found!\n");
		return 0;
	}


	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);



	if (clientSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(ip);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Client: connect() - Failed to connect.\n");
		WSACleanup();
		return 0;
	}
	else {
		printf("Client: Can start sending and receiving data...\n");
		printf("Your opponent goes first\n");
	}
	int buf = 0;

	int counter = 0;
	while (won == false) {
		recv(clientSocket, board, 10, 0);
		counter++;
		if (counter >= 9) { cout << "Two great minds are fighting in this miserable game."; send(clientSocket, board, 10, 0); system("pause"); break; }

		if (board[9] == '1') {
			cout << "You cant even play tic tac toe " << endl;
			break;
		}
		Dump(board);
		bool check = false;
		while (check == false) {
			cout << " Enter your coordinates " << endl;
			cin >> buf;
			if (board[buf] != NULL || buf == 9)
			{
				cout << "Are you blind? Guess so,this slot is already occupied";

			}
			else check = true;
		}
		board[buf] = symbol;
		counter++;
		Dump(board);

		if (CheckWin(board, symbol)) {
			board[9] = '1';
			cout << "I guess it's your greatest achievment in your life" << endl;
			send(clientSocket, board, 10, 0);
			break;
		}
		send(clientSocket, board, 10, 0);

	}
	WSACleanup();
	system("pause");
	return 0;
}

