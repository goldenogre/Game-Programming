#pragma once
#define GAME_NAME		"Game_A"
#define WINDOW_NAME		"Drake's Quest"
#define W_WIDTH			640
#define W_HEIGHT		480 // HARD CODED said 460? typo?
// options for 16:9 shovelnight: 400:240 - 384:216 - 512:288 
// 16 * 16 tiles wont work with 384/216
#define GAME_RES_WIDTH	384 // 24 tiles
#define GAME_RES_HEIGHT 240 // 15 tiles

#define GAME_BPP		32 // bits per pixel no compression, canvases pure RGB, 1-2 mbs for a bitmap.. so faster, but bigger 
#define GAME_DRAWING_AREA_MEMORY_SIZE (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))

typedef struct GAMEBITMAP {
	BITMAPINFO bitmapInfo;
	/*int height;
	int width;
	int bpp;*/
	void* memory;
} GAMEBITMAP;

typedef struct PIXEL32 {
	// 8 bits, 32 bits total,
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alptha;

} PIXEL32;

// prototype
LRESULT CALLBACK mainWindowProc(_In_ HWND windowHandle, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam); // integer value returns to windows. repoinse related

DWORD  createMainGameWindow(void); // sal _In_ DWORD is 32-bit unsigned integer

BOOL gameIsAlreadyRunning(void);

void processPlayerInput(void);
void renderFrameGraphics(void);


