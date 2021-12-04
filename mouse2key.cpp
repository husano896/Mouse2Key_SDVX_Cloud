#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <thread>
#include <chrono>

using namespace std;

DWORD FindProcessId(const std::wstring &processName);
bool taskalive = false;
bool test_mode = false;
short doIT = 0;
int delay = 5;
INPUT Button[4];

#define MOUSE_LOCKPOS 100
#define KEYUPFLAG (KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP)
#define KEYDOWNFLAG KEYEVENTF_SCANCODE

//Process & Active Window Checking
void check_task()
{
	DWORD pid = FindProcessId(L"sv6c.exe");
	HWND Active_Window = GetForegroundWindow();
	DWORD ThreadID;
	GetWindowThreadProcessId(Active_Window, &ThreadID);
	taskalive = (ThreadID == pid) || test_mode;
	if (pid && taskalive)
		SetActiveWindow(Active_Window);
}

//Do the mouse stuff
void main_process()
{

	if (!taskalive)
		return;
	POINT CursorPos;
	GetCursorPos(&CursorPos);

	/*
	Virtual Key 
	MSDN : https://msdn.microsoft.com/zh-tw/library/windows/desktop/dd375731%28v=vs.85%29.aspx
	*/

	//Prepare a keyup event
	Button[0].ki.dwFlags = KEYUPFLAG;
	Button[1].ki.dwFlags = KEYUPFLAG;
	Button[2].ki.dwFlags = KEYUPFLAG;
	Button[3].ki.dwFlags = KEYUPFLAG;

	if (CursorPos.x < MOUSE_LOCKPOS)
	{
		Button[0].ki.dwFlags = KEYDOWNFLAG;
		Button[0].ki.wScan = 0x10;
		doIT = 2;
	}
	else if (CursorPos.x > MOUSE_LOCKPOS)
	{
		Button[1].ki.dwFlags = KEYDOWNFLAG;
		Button[1].ki.wScan = 0x11;
		doIT = 2;
	}

	if (CursorPos.y < MOUSE_LOCKPOS)
	{
		Button[2].ki.dwFlags = KEYDOWNFLAG;
		Button[2].ki.wScan = 0x18;
		doIT = 2;
	}
	else if (CursorPos.y > MOUSE_LOCKPOS)
	{
		Button[3].ki.dwFlags = KEYDOWNFLAG;
		Button[3].ki.wScan = 0x19;
		doIT = 2;
	}

	if (doIT > 0)
	{
		SendInput(4, Button, sizeof(INPUT));
		SetCursorPos(MOUSE_LOCKPOS, MOUSE_LOCKPOS);
		doIT--;
	}
}
int main(int argc, char **argv)
{
	//parameter 1 for delay
	if (argc > 1)
	{
		delay = atoi(argv[1]);
	}
	//parameter 2 for "any window" mode
	if (argc > 2)
	{
		test_mode = true;
	}

	//Note for English
	cout << "Mouse to key for SDVX VI Cloud by xFly" << endl
		 << "xFly.Dragon" << endl
		 << endl;
	cout << "Current check delay : " << delay << " ms" << endl;
	cout << "Q/W For Left Analog Device & O/P For Right Analog Device" << endl;
	cout << "You can open the game now." << endl;
	cout << "Press Alt+Tab / Ctrl+C / Exit SDVX to release the mouse." << endl;

	//Note for TChinese
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

	while (573)
	{
		main_process();
		check_task();
		Sleep(delay);
	}
	// Exit normally
	return 0;
}

DWORD FindProcessId(const std::wstring &processName)
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
