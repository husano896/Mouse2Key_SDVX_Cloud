#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <thread>
#include <chrono>  

using namespace std;

DWORD FindProcessId(const std::wstring& processName);
bool taskalive = false;
bool test_mode = false;
int delay = 5;
INPUT Button[4];

//Process & Active Window Checking
void check_task() {
	while (573) {
		DWORD pid = FindProcessId(L"sv3c.exe");
		HWND Active_Window = GetForegroundWindow();
		DWORD ThreadID;GetWindowThreadProcessId(Active_Window, &ThreadID);
		taskalive = (ThreadID == pid) || test_mode;
		this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

//Do the mouse stuff
void main_process() {
	//F_CK YOU 573 :^
	while (573) {
		if (!taskalive) continue;
			POINT CursorPos;
			GetCursorPos(&CursorPos);

			/*Virtual Key 
			MSDN : https://msdn.microsoft.com/zh-tw/library/windows/desktop/dd375731%28v=vs.85%29.aspx
			*/
			if ((CursorPos.x) < 100) {
				Button[0].ki.dwFlags = KEYEVENTF_SCANCODE;
				Button[0].ki.wScan = 0x10;  
			}
			else if ((CursorPos.x) > 100) {
				Button[1].ki.dwFlags = KEYEVENTF_SCANCODE;
				Button[1].ki.wScan = 0x11;  
			}

			if ((CursorPos.y) < 100) {
				Button[2].ki.dwFlags = KEYEVENTF_SCANCODE;
				Button[2].ki.wScan = 0x18;  
			}
			else if ((CursorPos.y) > 100) {
				Button[3].ki.dwFlags = KEYEVENTF_SCANCODE;
				Button[3].ki.wScan = 0x19;  
			}
			SendInput(4, Button, sizeof(INPUT));

			SetCursorPos(100, 100);
			//Prepare a keyup event
			Button[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Button[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Button[2].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			Button[3].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			
			this_thread::sleep_for(std::chrono::milliseconds(delay));
		}
}
int main(int argc, char** argv)
{
	//parameter 1 for delay
	if (argc > 1) {
		delay = atoi(argv[1]);
	}
	//parameter 2 for "any window" mode
	if (argc > 2) {
		test_mode = true;
	}
	
	//Note for English
	cout << "Mouse to key for SDVX III Cloud by xFly" << endl << "https://www.facebook.com/L.F.Arashi" << endl << endl;
	cout << "Current check delay : " << delay << " ms" << endl;
	cout << "Q/W For Left Analog Device & O/P For Right Analog Device" << endl;
	cout << "You can open the game now." << endl;
	cout << "Simply press Alt+Tab / Ctrl+C / Exit SDVX to release the mouse." << endl;

	//Not for TChinese
	cout << "----" << endl;
	cout << "左類比裝置對應為Q/W & 右類比裝置對應為O/P" << endl;
	cout << "你現在可以開啟遊戲。" << endl;
	cout << "使用Alt+Tab切換視窗 / Ctrl+C / 離開遊戲 來解除滑鼠鎖定。" << endl;

	//Keyboard Init
	Button[0].type = INPUT_KEYBOARD;
	Button[0].ki.time = 0;
	Button[0].ki.wVk = 'Q';
	Button[0].ki.dwExtraInfo = 0;

	Button[1].type = INPUT_KEYBOARD;
	Button[1].ki.time = 0;
	Button[1].ki.wVk = 'W';
	Button[1].ki.dwExtraInfo = 0;

	Button[2].type = INPUT_KEYBOARD;
	Button[2].ki.time = 0;
	Button[2].ki.wVk = 'O';
	Button[2].ki.dwExtraInfo = 0;

	Button[3].type = INPUT_KEYBOARD;
	Button[3].ki.time = 0;
	Button[3].ki.wVk = 'P';
	Button[3].ki.dwExtraInfo = 0;
	thread mThread(main_process);
	thread mThread2(check_task);
	// wait the thread stop
	mThread.join();
	mThread2.join();
	// Exit normally
	return 0;
}

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}