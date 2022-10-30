#pragma once
#define GAME_NAME		"Game_A"
#define WINDOW_NAME		"Drake's Quest"
#define W_WIDTH			384
#define W_HEIGHT		216 
// options for 16:9 shovelnight: 400:240 - 384:216 - 512:288 
#define GAME_RES_WIDTH	384
#define GAME_RES_HEIGHT 216

#define GAME_BPP		32 // bits per pixel no compression, canvases pure RGB, 1-2 mbs for a bitmap.. so faster, but bigger 
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))

typedef struct GAMEBITMAP {
	BITMAPINFO bitmapInfo;
	/*int height;
	int width;
	int bpp;*/
	void* memory;
} GAMEBITMAP;


// prototype
LRESULT CALLBACK mainWindowProc(_In_ HWND windowHandle, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam); // integer value returns to windows. repoinse related

DWORD  createMainGameWindow(void); // sal _In_ DWORD is 32-bit unsigned integer

BOOL gameIsAlreadyRunning(void);

void processPlayerInput(void);
void renderFrameGraphics(void);


