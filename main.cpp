#include <bits/stdc++.h>
#include <io.h>
#include <fstream>
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

void kill_as_ntsd(tm *p){
	printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
	printf("INFO");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	printf("] 检测到极域进程，已尝试使用ntsd关闭\n");
	system("ntsd.exe -c q -pn StudentMain.exe");
}

void kill_as_taskkill(tm *p){
	printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
	printf("INFO");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
	printf("] 检测到极域进程，已尝试使用taskkill关闭\n");
	system("ntsd.exe -c q -pn StudentMain.exe");
}

int main() {
	HWND hWnd = GetForegroundWindow();
	
	time_t nowtime;

	if(!(_access(".config", 00) == 0)){
		time(&nowtime);
		tm *p = localtime(&nowtime);
        printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
		printf("WARN");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		printf("] 未发现配置文件，已尝试创建配置文件\n");
		ofstream config(".config");
		if(!config.is_open()) {
			time(&nowtime);
			tm *p = localtime(&nowtime);
			printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
			printf("ERR");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			printf("] 未发现配置文件且创建配置文件失败，无法运行程序\n");
		}else{
			config << 0;
			config.close();
			time(&nowtime);
			tm *p = localtime(&nowtime);
			printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
			printf("INFO");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			printf("] 配置文件创建成功\n");
		}
        exit(1);
	}

	if(!(_access("ntsd.exe", 00) == 0)){
		time(&nowtime);
		tm *p = localtime(&nowtime);
        printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
		printf("ERR");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		printf("] 未发现ntsd，无法运行程序\n");
		printf("请将ntsd重命名为ntsd.exe并放于编译后的可执行文件所在的文件夹内\n");
        system("pause");
        exit(1);
    }

	bool type;
	ifstream configInupt(".config");
	configInupt >> type;

	while(1 == 1){
		TopWindow(hWnd);
		time(&nowtime);
		tm *p = localtime(&nowtime);
		wstring processName = L"StudentMain.exe";
		if (isProcessRunning(processName)) {
			if(!type) kill_as_ntsd(p);
			else kill_as_taskkill(p);
			Sleep(500);
	    }else{
			printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
			printf("WARN");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			printf("] 未检测到极域进程\n");
			Sleep(500);
		} 
	}
    return 0;
}
