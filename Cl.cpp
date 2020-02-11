#include <Windows.h>
#include <iostream>

#pragma comment(lib,"WS2_32.lib")

using namespace std;

int main() {
	WSADATA fd;
	WSAStartup(MAKEWORD(2, 2), &fd);
	SOCKET loc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (loc == INVALID_SOCKET)
	{
		cout << "Error..." << GetLastError() << endl;
		return 0;
	}
	sockaddr_in dad;
	char ip[20];
	cout << "Enter ip: ";
	cin.getline(ip, sizeof(ip));
	dad.sin_family = AF_INET;
	dad.sin_addr.S_un.S_addr = inet_addr(ip);
	dad.sin_port = htons(9999);
	int con = connect(loc, (SOCKADDR*)&dad, sizeof(dad));
	
	int b;
	while (TRUE) {
		
		if (recv(loc, (char*)&b, sizeof(b), 0) == SOCKET_ERROR) {
			cout << "ERROR recv... " << GetLastError() << endl;
			system("pause");
			return 0;
		}
		cout << b << endl;
		
	}
}