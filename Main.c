// CONTINUE EP0007


/**
* 10/1/2022
* Creating a video game in C from scratch
* Debug/Release (Release for optimisation but hard to debug) x64
* 
* enhance main with params (int argc, char *argv[])
* Setting up from main to WinMain for win app!
* 
* Must switch linker in properties;CHANGE SUBSAYSTEM to windows.
* */
/* 1. Create window message loop. (press a key, move etc)
 * 2. call back Window Procedure
 * 
 * Removing WIN32..RS# is not defined. 
 */
#include <stdio.h>
// surpress warnings of windows.h
#pragma warning(push, 3)
#include <windows.h> // OutputDebugText
#pragma warning(pop)
// type defs unsigned char now uint8_t: Better because it's explicit in data type.
#include <stdint.h>
// Include
#include "Main.h"

// Global variable that's accessible in all the program.
BOOL gGameIsRunning; // 0 or not. lower g is global. or g_main... init to 0 on compile FALSE
//HANDLE gGameWindow;
HWND gGameWindow;
GAMEBITMAP gBackBuffer;// gDrawingSurface;
MONITORINFO gMonitorInfo = { sizeof(MONITORINFO) };


// F5 to run (C++ you can remove arg names and it will omit)
int WinMain(HINSTANCE instance, HINSTANCE previousInstance, PSTR commandLine, INT cmdShow) {
	
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(cmdShow);
	if (gameIsAlreadyRunning == TRUE) {
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!", MB_ICONEXCLAMATION | MB_OK);
	}

	if (createMainGameWindow() != ERROR_SUCCESS) {
		goto Exit;
	}
	// Init drawing surface   400 by 240 !!! wide screen
	gBackBuffer.bitmapInfo.bmiHeader.biSize = sizeof(gBackBuffer.bitmapInfo.bmiHeader);
	gBackBuffer.bitmapInfo.bmiHeader.biWidth = GAME_RES_WIDTH; // actual resolution 256*240 is nes 4:3 to 16:9 1920 * 1080  -- 256 by 224 = snes
	gBackBuffer.bitmapInfo.bmiHeader.biHeight = GAME_RES_HEIGHT;
	gBackBuffer.bitmapInfo.bmiHeader.biBitCount = GAME_BPP; // compatable for winAPI
	gBackBuffer.bitmapInfo.bmiHeader.biCompression = BI_RGB;
	gBackBuffer.bitmapInfo.bmiHeader.biPlanes = 1;
	if((gBackBuffer.memory = VirtualAlloc(NULL, GAME_DRAWING_AREA_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)) == NULL){
		// virtual when large 64kb if heapalloc for small NULL windows mem address will be created by windows 
		MessageBoxA(NULL, "Failed to allocate memory for drawing surface!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	memset(gBackBuffer.memory, 0x7F,GAME_DRAWING_AREA_MEMORY_SIZE); // 0xFF 255
	
	MSG message = { 0 };

	gGameIsRunning = TRUE; 
	// message loop blocks thread... no do.
	// one loop == 1 frame
	while (gGameIsRunning) {
		while (PeekMessageA(&message, gGameWindow, 0, 0, PM_REMOVE )) { // looks at message queue PM_REMOVE takes message out.
			// TranslateMessage(&message);
			DispatchMessageA(&message); // message to window class Added to windowClass. 
		}
		processPlayerInput();

		renderFrameGraphics();

		// sleep function.. strong opinions.. 0 milliseconds only? 
		Sleep(1); // helps but temporary for now. 
	}
	

	Exit:
	return (0);
}



// call back
LRESULT CALLBACK mainWindowProc(_In_ HWND windowHandle, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	
	LRESULT result = 0;

	switch (message) {
		// closes program when X is clicked
		case WM_CLOSE: // doc sais return 0, handles by result message.
		{
			// user hits x 
			gGameIsRunning = FALSE;
			PostQuitMessage(0);
			break;
		}
		
		default: {
			result =  DefWindowProc(windowHandle, message, wParam, lParam);
		}
			
	}
	return (result);

}

DWORD  createMainGameWindow(void) {
	DWORD result = ERROR_SUCCESS;

	// register window class and make window
	WNDCLASSEXA windowClass = { 0 }; // add 0 to prevent garbage data and use with data structures.



	windowClass.cbSize = sizeof(WNDCLASSEXA); // can swap with 0 top init
	windowClass.style = 0;
	windowClass.lpfnWndProc = mainWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(NULL); // Now we don't have to pass the instance == instance from WinMain
	windowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION); // NULL = 0
	windowClass.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	windowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));//(HBRUSH)(COLOR_WINDOW + 1); // color default grey || pink now with a macro! 255 bits, blue/red = purple
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = GAME_NAME "_WINDOWCLASS"; // concats define with game name
	//27:29
	// window class

	if (RegisterClassExA(&windowClass) == 0) // ! is not readable google documentation for return values
	{
		result = GetLastError();
		MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	// need to show window
	gGameWindow = CreateWindowExA(WS_EX_CLIENTEDGE, windowClass.lpszClassName, WINDOW_NAME, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, W_WIDTH, W_HEIGHT, NULL, NULL, GetModuleHandle(NULL), NULL);
	//ShowWindow(windowHandle, TRUE); added WS_VISIBLE
	if (gGameWindow == NULL) // ! is not readable google documentation for return values
	{
		result = GetLastError();
		MessageBoxA(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}
	// label
	if (GetMonitorInfoA(MonitorFromWindow(gGameWindow, MONITOR_DEFAULTTOPRIMARY), &gMonitorInfo) == 0) { // takes the data and stores it into variable.
		result = ERROR_MONITOR_NO_DESCRIPTOR;
		goto Exit;
	}



	// CONTINUE
	int monitorWidth = gMonitorInfo.rcMonitor.right - gMonitorInfo.rcMonitor.left; // accounts for multiple displays
	int monitorHeight = gMonitorInfo.rcMonitor.bottom - gMonitorInfo.rcMonitor.top;

Exit:
	return(result);
} 

// 
BOOL gameIsAlreadyRunning(void) {
	HANDLE mutex = NULL;
	mutex = CreateMutexA(NULL,FALSE, GAME_NAME "_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return (TRUE);
	}
	else {
		return(FALSE);
	}
}

// all input goes here, gamepad, keyboard etx
void processPlayerInput(void) {
	int16_t escapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE); //(int16_t) not working ?? was a short: vurtual keycode BLUE means it knows. GREEN from a include. BYTE is 8 bits INT 4 bytes, aka 8*4 [32]bits1116
	if (escapeKeyIsDown) {
		// send own window message WM_QUIT message
		SendMessageA(gGameWindow, WM_CLOSE, 0, 0);
	}
}
// Render frame 
void renderFrameGraphics(void) {
	// take backbuffer (drawing surface)
	HDC deviceContext = GetDC(gGameWindow); // get device context
	// stretches images automatically with window. 
	StretchDIBits(deviceContext, 0, 0, 100, 100, 0, 0, 100, 100, gBackBuffer.memory, &gBackBuffer.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
											
	// release it always!
	ReleaseDC(gGameWindow, deviceContext);
}

/**
* Special notes:
* UNICODE vs ANSI
* UTF-16 (16 bits for more characters)
* UNICODE (Windows native)
* OutputDebugString(L"Hello\n"); L for unicode, not asci
* Out.....StrinW(wide for 16bit)A for (ASCII)
* Prefer UNICODE version of APIs.
* */