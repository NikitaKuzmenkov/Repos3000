#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#pragma comment(lib,"WS2_32.lib")

using namespace std;

HANDLE hSlot;
LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\WitherStation");
int Buffer;


BOOL ReadSlot(SOCKET First)
{
	
	DWORD cbMessage, cMessage, cbRead;
	BOOL fResult;
	DWORD cAllMessages;

	cbMessage = cMessage = cbRead = 0;

	fResult = GetMailslotInfo(hSlot, // mailslot handle 
		(LPDWORD)NULL,               // no maximum message size 
		&cbMessage,                   // size of next message 
		&cMessage,                    // number of messages 
		(LPDWORD)NULL);              // no read time-out 

	if (!fResult)
	{
		printf("GetMailslotInfo failed with %d.\n", GetLastError());
		return FALSE;
	}

	if (cbMessage == MAILSLOT_NO_MESSAGE)
	{
		printf("Waiting for a message...\n");
		return TRUE;
	}

	cAllMessages = cMessage;

	while (cMessage != 0){
		
		fResult = ReadFile(hSlot,
			&Buffer,
			sizeof(Buffer),
			&cbRead,
			NULL);

		if (!fResult)
		{
			printf("ReadFile failed with %d.\n", GetLastError());
	
			return FALSE;
		}

			if (send(First, (char*)&Buffer, sizeof(Buffer), 0) == SOCKET_ERROR) {
				cout << "ERROR send..." << GetLastError() << endl;
				return 0;
			}


		fResult = GetMailslotInfo(hSlot,  
			(LPDWORD)NULL,                
			&cbMessage,                   
			&cMessage,                    
			(LPDWORD)NULL);              

		if (!fResult)
		{
			printf("GetMailslotInfo failed (%d)\n", GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

BOOL WINAPI MakeSlot(LPCTSTR lpszSlotName)
{
	hSlot = CreateMailslot(lpszSlotName,
		0,                             
		MAILSLOT_WAIT_FOREVER,        
		(LPSECURITY_ATTRIBUTES)NULL); 

	if (hSlot == INVALID_HANDLE_VALUE)
	{
		printf("CreateMailslot failed with %d\n", GetLastError());
		system("pause");
		return FALSE;
	}
	return TRUE;
}

int main() {

	WSADATA ds;
	WSAStartup(MAKEWORD(2, 2), &ds);
	SOCKET p = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (p == INVALID_SOCKET) {
		cout << WSAGetLastError();
		system("pause");
		return 0;
	}
	sockaddr_in dad;
	dad.sin_family = AF_INET;
	dad.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	dad.sin_port = htons(9999);
	if (bind(p, (SOCKADDR*)&dad, sizeof(dad)) == SOCKET_ERROR) {
		cout << "ERROR" << endl;
		system("pause");
		return 0;
	}
	if (listen(p, 1) == SOCKET_ERROR) {
		cout << "ERROR" << endl;
		system("pause");
		return 0;
	}

	int Add = sizeof(sockaddr_in);
	sockaddr_in Client;
	SOCKET First = accept(p, (SOCKADDR*)&Client, &Add);
	if (First == INVALID_SOCKET) {
		cout << WSAGetLastError();
		system("pause");
		return 0;
	}
	else {
		cout << "connected..." << endl;
	}

	
	MakeSlot(SlotName);

	while (TRUE)
	{
		ReadSlot(First);
		Sleep(1000);
	}

	
	system("pause");
	return 0;
}