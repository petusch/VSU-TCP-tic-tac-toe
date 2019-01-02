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
	char symbol = 'x';
	SOCKADDR_STORAGE from;
	int fromlen, socket_type, bytesRecv, bytesSent;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST], startBuffer[50];
	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	// Load the DLL
	if (wsaerr != 0) {
		cout << "The winsock dll not found!" << endl;
		return 0;
	}

	// Create a socket
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	// Bind the socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		cout << "bind() failed: " << WSAGetLastError() << endl;
		system("pause");
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}

	// Listen for a new connection
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		cout << "Listen(): Error listening on socket " << WSAGetLastError() << endl;
	}
	else {
		cout << "Listen() is ok, I'm waiting for connections..." << endl;
	}
	// Accept the connection
	fromlen = sizeof(socket_type);
	fromlen = sizeof(from);
	acceptSocket = accept(serverSocket, (SOCKADDR*)&from, &fromlen);
	if (acceptSocket == INVALID_SOCKET) {
		cout << "accept failed: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}

	cout << "Accepted connection from host and port " << endl;
	cout << "Accepted connection" << endl;
	// Receive the start game message
	if (acceptSocket == INVALID_SOCKET) {
		cout << "Accept Failed!" << endl;
	}
	int buf = 0;

	int counter = 0;
	//NoughtsAndCrosses *tictactoe = new NoughtsAndCrosses();
	while (won == false) {
		bool check = false;
		while (check == false) {
			cout << "Enter your coordinates " << endl;
			cin >> buf;
			if (board[buf] != NULL || buf == 9)
			{
				cout << "Are you blind? Guess so,this slot is already occupied";

			}
			else check = true;
		}
		counter++;
		board[buf] = symbol;
		Dump(board);
		if (counter == 9) { cout << "Two great minds are fighting in this miserable game."; send(acceptSocket, board, 10, 0); break; }
		if (CheckWin(board, symbol)) {
			board[9] = '1';
			cout << "I guess it's your greatest achievment in your life" << endl;
			send(acceptSocket, board, 10, 0);
			break;
		}
		send(acceptSocket, board, 10, 0);
		recv(acceptSocket, board, 10, 0);
		counter++;
		if (board[9] == '1') {
			cout << "You cant even play tic tac toe " << endl;
			break;
		}

		Dump(board);


	}
	WSACleanup();
	system("PAUSE");
	return 0;

}

/*class NoughtsAndCrosses {
protected:
// An array of integers holding the bits
// 3 by 3 board is represented as a one dimensional array
char board[9];
int counter = 0;
// Players symbols are either X or O
char playerSymbol;
char aiSymbol;
bool CheckWin(char symbol) {
bool won = false;
// Is there a winner?
// Check horizontal
if (board[0] == symbol && board[1] == symbol && board[2] == symbol) won = true;
if (board[3] == symbol && board[4] == symbol && board[5] == symbol) won = true;
if (board[6] == symbol && board[7] == symbol && board[8] == symbol) won = true;
// Check vertical
if (board[0] == symbol && board[3] == symbol && board[6] == symbol) won = true;
if (board[1] == symbol && board[4] == symbol && board[7] == symbol) won = true;
if (board[2] == symbol && board[5] == symbol && board[8] == symbol) won = true;
// Check diagonals
if (board[0] == symbol && board[4] == symbol && board[8] == symbol) won = true;
if (board[2] == symbol && board[4] == symbol && board[6] == symbol) won = true;
return true;
}
public:
NoughtsAndCrosses() {
for (int n = 0; n < 9; n++) {
board[n] = ' ';
}
}

bool Play(char symbol, int pos) {
// Ensure the position is free
if (board[pos] == ' ') {
board[pos] = symbol;
CheckWin(symbol);
return true;
}
else return false;
}
int IsFinished(int counter) {
counter++;
return counter;
}
void Dump() {
cout << board[0] << " | " << board[1] << " | " << board[2] << "\n";
cout << "--------\n";
cout << board[3] << " | " << board[4] << " | " << board[5] << "\n";
cout << "--------\n";
cout << board[6] << " | " << board[7] << " | " << board[8] << "\n";
}
void Send(SOCKET s) {
// Create an array that can hold the object
char sendObject[sizeof(NoughtsAndCrosses)];
// Copy the object into an array
memcpy(sendObject, this, sizeof(NoughtsAndCrosses));
// Invoke send passing it the socket etc
send(s, sendObject, sizeof(NoughtsAndCrosses), 0);
}
};*/