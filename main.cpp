#include <bits/stdc++.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <cwchar>
using namespace std;

void TopWindow(HWND &hWnd) {
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

bool isProcessRunning(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            int size = MultiByteToWideChar(CP_ACP, 0, pe32.szExeFile, -1, NULL, 0);
            if (size > 0) {
                std::wstring exeFile(size - 1, L'\0');
                MultiByteToWideChar(CP_ACP, 0, pe32.szExeFile, -1, &exeFile[0], size);

                if (processName == exeFile) {
                    CloseHandle(hSnapshot);
                    return true;
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return false;
}

int main() {
	HWND hWnd = GetForegroundWindow();
	
	time_t nowtime;
	while(1 == 1){
		TopWindow(hWnd);
		time(&nowtime);
		tm *p = localtime(&nowtime);
		wstring processName = L"StudentMain.exe";
		if (isProcessRunning(processName)) {
			printf("[%04d:%02d:%02d %02d:%02d:%02d INFO] 检测到极域进程，已尝试使用ntsd关闭\n", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			system("ntsd.exe -c q -pn StudentMain.exe");
			Sleep(500);
	    }else{
			printf("[%04d:%02d:%02d %02d:%02d:%02d WARN] 未检测到极域进程\n", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			Sleep(500);
		} 
	}
    return 0;
}