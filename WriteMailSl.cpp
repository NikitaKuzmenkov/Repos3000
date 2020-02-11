#include <windows.h>
#include <stdio.h>
#include <string>
#include <ctime>


using namespace std;

LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\WitherStation");

int temper() {
	char buf[256];
	srand(time(0));
	int t = rand() % 40 - 20;

	return t;
}



int main()
{
	HANDLE hFile;
		hFile = CreateFile(SlotName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);



		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("CreateFile failed with %d.\n", GetLastError());
			return FALSE;
		}

		
	while (TRUE) {

		int a = temper();
		BOOL fResult;
		DWORD cbWritten;

		fResult = WriteFile(hFile,
			&a,
			sizeof(a),
			&cbWritten,
			(LPOVERLAPPED)NULL);

		if (!fResult) {
			printf("WriteFile failed with %d.\n", GetLastError());
			return FALSE;
		}

		printf("Slot written to successfully.\n");
		Sleep(2000);
	}
	return TRUE;
	CloseHandle(hFile);

	system("pause");
	return 0;
}