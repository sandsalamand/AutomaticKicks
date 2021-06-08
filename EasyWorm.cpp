#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>
#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX, INT_MIN
#include <stdlib.h>  // for strtol

int convertArg(char* argv[], int index, int* result)
{
	char* p;

	errno = 0;
	long conv = strtol(argv[index], &p, 10);

	// Check for errors: e.g., the string does not represent an integer
	// or the integer is larger than int
	if (errno != 0 || *p != '\0' || conv > INT_MAX || conv < INT_MIN) {
		// error code
		return -1;
	}
	else {
		// No error
		*result = (int)conv;
		return 0;
	}
}

int main(int argc, char* argv[])
{
	printf("Program Started Successfully.\nHold Control to continuously execute worm attacks.\nReminder: Holding Ctrl immediately after rolling will override the kick action and perform a regular attack, so wait a sec after rolling to attack.\nIn case you need to adjust the timings on attacks, two arguments can be supplied to this script. Example usage: \"yourpath\\easyworm.exe -5 200\" will reduce the startup delay by 5 milliseconds, and increase the delay between worm attacks by ~200ms\nPress F7 to toggle the script on/off; Press F8 close this program");
	
	//timings
	int startupDelay = 10;
	int betweenAttacksDelay = 1750;

	//argument parsing
	int argInt = 0;
	if (argc == 2 && convertArg(argv, 1, &argInt) == 0) {
		startupDelay += argInt;
	} //add/subtracts from startup delay using supplied command line argument
	if (argc == 3 && convertArg(argv, 2, &argInt) == 0) {
		betweenAttacksDelay += argInt;
	} //adds/subtracts from delay between attacks

	//control variables
	bool fToggle = 1;
	bool recentWASDPress = 0;
	int timer = startupDelay;

	//structs
	INPUT kbInput;
	INPUT mouseClick;

	// Set Up W Key Press Event
	kbInput.type = INPUT_KEYBOARD;
	kbInput.ki.wScan = 0; // hardware scan code for key
	kbInput.ki.time = 0;
	kbInput.ki.dwExtraInfo = 0;
	kbInput.ki.wVk = 0x57; // virtual-key code for the "w" key
	kbInput.ki.dwFlags = 0; // 0 for key press

	//Set Up Mouse Click Event
	mouseClick.type = INPUT_MOUSE;
	mouseClick.mi.dx = 0;
	mouseClick.mi.dy = 0;
	mouseClick.mi.mouseData = 0;
	mouseClick.mi.time = 0;
	mouseClick.mi.dwExtraInfo = 0;

	while ((GetKeyState(VK_F8) & 0x8000) == 0)
	{
		if ((GetKeyState(VK_F7) & 0x8000) != 0) {
			fToggle = !fToggle;
			fToggle == 0? printf("\nProgram toggled off ") : printf("\nProgram toggled on ");
			Sleep(300);}
		if (fToggle == 1)
		{
			if ((GetKeyState(0x57) & 0x8000) || (GetKeyState(0x41) & 0x8000) || (GetKeyState(0x53) & 0x8000) || (GetKeyState(0x44) & 0x8000))
			{
				recentWASDPress = true;
				timer = startupDelay;
			}
			if((GetKeyState(VK_CONTROL) & 0x8000) && recentWASDPress == false)
			{
				//Hold W Key
				kbInput.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &kbInput, sizeof(INPUT));

				//Click the mouse
				mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
				SendInput(1, &mouseClick, sizeof(INPUT));
				Sleep(200);

				//Release W Key
				kbInput.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
				SendInput(1, &kbInput, sizeof(INPUT));

				//Release Mouse Click
				mouseClick.mi.dwFlags = (MOUSEEVENTF_LEFTUP);
				SendInput(1, &mouseClick, sizeof(INPUT));

				Sleep(betweenAttacksDelay);
			}
			if (recentWASDPress == true) {
				timer--;
				Sleep(1);
			}
			if (timer <= 0) {
				recentWASDPress = false;
				timer = startupDelay;
			}
		}
	}
	// Exit normally
	return 0;
}