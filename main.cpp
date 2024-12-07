#include <bits/stdc++.h>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <cwchar>
using namespace std;

/*
部分参考代码：
void TopWindow(HWND &hWnd) {
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
int main() {
	HWND hWnd=GetForegroundWindow();
	while(1) {
		TopWindow(hWnd);
		Sleep(10);
	}
}

#include <windows.h>
#include <iostream>

int main() {
    // 注册Ctrl + B (Ctrl = 0x11, B = 0x42)
    if (RegisterHotKey(NULL, 1, MOD_CONTROL, 0x42)) {
        std::cout << "热键Ctrl+B已注册，按Ctrl+B触发。\n";
    } else {
        std::cout << "注册热键失败！\n";
        return 1;
    }

    // 消息循环，等待热键触发
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            std::cout << "Ctrl+B 被按下！\n";
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


int main()
{
    // 注册两个热键 Ctrl+F1 , Ctrl+F2
    if (0 == RegisterHotKey(NULL, 1, MOD_CONTROL, VK_F1)) {
        cout << "RegisterHotKey error : " << GetLastError() << endl;
    }
    if (0 == RegisterHotKey(NULL, 2, MOD_CONTROL, VK_F2)) {
        cout << "RegisterHotKey error : " << GetLastError() << endl;
    }

    // 消息循环
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)){

        switch (msg.message) {
        case WM_HOTKEY:
        {
            if (1 == msg.wParam) {
                cout << "1" << endl;
            }
                
            else if (2 == msg.wParam) {
                cout << "2" << endl;
            }

            break;
        }
            
        default:
            break;
        }

    }

    cout << "finished." << endl;
    return 0;
}
*/

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

	if(!(access("ntsd.exe", F_OK) == 0)){
        printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
		printf("ERR");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
		printf("] 未发现ntsd，无法运行程序\n");
		printf("请将ntsd重命名为ntsd.exe并放于编译后的可执行文件所在的文件夹内\n");
		printf("如果没有ntsd，请在此链接下载：https://pan.huang1111.cn/s/A6ooESB\n");
        system("pause");
        exit(1);
    }

	HWND hWnd = GetForegroundWindow();
	
	time_t nowtime;
	while(1 == 1){
		TopWindow(hWnd);
		time(&nowtime);
		tm *p = localtime(&nowtime);
		wstring processName = L"StudentMain.exe";
		if (isProcessRunning(processName)) {
			printf("[%04d:%02d:%02d %02d:%02d:%02d ", p->tm_year + 1900, p -> tm_mon + 1, p -> tm_mday, p -> tm_hour, p -> tm_min, p -> tm_sec);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
			printf("INFO");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
			printf("] 检测到极域进程，已尝试使用ntsd关闭\n");
			system("ntsd.exe -c q -pn StudentMain.exe");
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