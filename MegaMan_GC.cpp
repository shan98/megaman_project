// MegaMan_GC.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MegaMan_GC.h"
#include "Graphics_Object.h"
#include "resource.h"
#include "MegamanObj.h"
#include "BackgroundObj.h"
#include "WeaponsOBJ.h"
#include "GroundEnemy.h"
#include <stdio.h>
#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
///////////////Mine below///////////////////////////////
BackgroundObj background(IDB_Background,0,0,2560, 800, 0.2);
BackgroundObj GroundObj(IDB_GROUND,0,450,774,120, 1);
MegamanObj MEGAMAN;
GroundEnemy *enemies[100];
int numEnemies = 0;
int mapPosition= 0;
int x =0;
Graphics_Object BigBlock(IDB_BigBlock,x,GROUND - 64, 64, 64, IDB_BigBlockMask);
Graphics_Object *blocks[100];

int numBlocks = 0;

HDC offScreenDC;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void setScreen();
void drawScreen(HWND hWnd);
void loadMap();
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MEGAMAN_GC, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEGAMAN_GC));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEGAMAN_GC));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MEGAMAN_GC);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 700, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	
	SetTimer(hWnd, 1, 50, NULL);
	loadMap();

	setScreen();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	///////////Timer code////////////
	
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		//RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
		hdc = BeginPaint(hWnd, &ps);
		background.Draw(offScreenDC);
		
		GroundObj.Draw(offScreenDC);
		MEGAMAN.Draw(offScreenDC);
		for(int i = 0; i< numBlocks; i++)
		{
			blocks[i]->Draw(offScreenDC);
		}
		
		MEGAMAN.drawBullets(offScreenDC);
		for(int i=0 ;i <numEnemies ;i++)
		{
			enemies[i]->Draw(offScreenDC);
			enemies[i]->drawBullets(offScreenDC);
		}
		
		drawScreen(hWnd);

		// TODO: Add any drawing code here...
		ReleaseDC(hWnd, hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		for(int i = 0; i<numBlocks; i++)
		{
		delete blocks[i];
		}
		for(int i = 0; i<numEnemies; i++)
		{
			delete enemies[i];
		}
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:

		switch (wParam) 
		{
		case VK_RIGHT:	
			if (MEGAMAN.IsJumping() == true)
			{
				MEGAMAN.speedx = 10;
				MEGAMAN.state = JUMPRIGHT;
				MEGAMAN.Starty = 82;
			} else
			{
			MEGAMAN.AnimateMOVERIGHT();
			}

			break;
		case VK_LEFT:	
			if (MEGAMAN.IsJumping() == true)
			{
				MEGAMAN.speedx = -10;
				MEGAMAN.state = JUMPLEFT;
				MEGAMAN.Starty = 135;
			} else 
			{
				MEGAMAN.AnimateMOVELEFT();
			}
			

			break;

		case VK_UP:		
			if ((MEGAMAN.state == MOVELEFT) || (MEGAMAN.state == STANDLEFT))
			{

				MEGAMAN.AnimateJUMPLEFT();


			}else if ((MEGAMAN.state == MOVERIGHT) || (MEGAMAN.state == STANDRIGHT)) 
			{
				MEGAMAN.AnimateJUMPRIGHT();
			}
			MEGAMAN.speedy= -20;
			break;

		case VK_SPACE:
			
			if ((MEGAMAN.state == MOVELEFT) || (MEGAMAN.state == STANDLEFT))
			{
				MEGAMAN.AnimateSHOOTINGLEFT();
				

			}else if ((MEGAMAN.state == MOVERIGHT) || (MEGAMAN.state == STANDRIGHT)) 
			{
				MEGAMAN.AnimateSHOOTINGRIGHT();
			}

			if ((MEGAMAN.state == SHOOTINGLEFT) || (MEGAMAN.state == MOVELEFT) || (MEGAMAN.state == STANDLEFT) || (MEGAMAN.state == JUMPLEFT))
			{
				MEGAMAN.fireBullets(MEGAMAN.x,MEGAMAN.y + 10,-10,0);
			}else
			{
				MEGAMAN.fireBullets(MEGAMAN.x + 40,MEGAMAN.y + 10,10,0);
			}

			break;
	}
	break;
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_RIGHT:
			if (MEGAMAN.IsJumping() == false)
			{
			MEGAMAN.AnimateSTANDRIGHT();
			}
			break;
		case VK_LEFT:	
			if (MEGAMAN.IsJumping() == false)
			{
			MEGAMAN.AnimateSTANDLEFT();
			}
			break;
		case VK_SPACE:
			if (MEGAMAN.state == SHOOTINGLEFT)
			{
				MEGAMAN.AnimateSTANDLEFT();
			} else if (MEGAMAN.state == SHOOTINGRIGHT)
			{
				MEGAMAN.AnimateSTANDRIGHT();
			}
			//break;
			}
		break;
	case WM_TIMER:
		//megaman's stuff//
		MEGAMAN.move();
		MEGAMAN.moveBullets();\
			for (int i =0;i<numEnemies;i++)
			{
				if (enemies[i]->isDead==false){
					for (int j=0;j<numBlocks;j++)
					{
						enemies[i]->ColideWithBox(blocks[j]);
						enemies[i]->BulletCollideWithBox(blocks[j]);
					}
				}
				
				if(!enemies[i]->isDead){
					MEGAMAN.BulletCollideWithOBJ(enemies[i]);

				}
				enemies[i]->BulletCollideWithOBJ(&MEGAMAN);
				

			}
			for(int i = 0;i<numEnemies;i++){
				enemies[i]->move();
				enemies[i]->moveBullets();
			}
			for (int i=0;i<numBlocks;i++)
			{

				MEGAMAN.ColideWithBox(blocks[i]);
				MEGAMAN.BulletCollideWithBox(blocks[i]);
			}
			//enemy stuff//

			PostMessage(hWnd,WM_PAINT, 0, 0);

			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);

	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;

}
void setScreen(){
	HDC htempDC;
	HBITMAP offScreenBMP;
	htempDC = GetDC(0);
	offScreenDC= CreateCompatibleDC(htempDC);
	offScreenBMP = CreateCompatibleBitmap(htempDC, 800, 600);
	SelectObject(offScreenDC, offScreenBMP);
	ReleaseDC(0, htempDC);

}
void drawScreen(HWND hWnd){
	HDC screenDC;
	screenDC = GetDC(hWnd);
	BitBlt(screenDC,0,0,700,560, offScreenDC, 0, 0,SRCCOPY);
	DeleteDC(screenDC);
}
void loadMap(){
	char section [80];
	int x=0;
	int y=0;
	int index=0;
	FILE	*file;
	errno_t error;
	error = fopen_s(&file,"../levels/level1.ini","r");
	
	while (true){

		fscanf(file, "%s",section);

		if (strcmp(section, "[Red-Block]"  )==0){
			while (true){

				fscanf(file,"%d %d %d",&index,&x,&y);
				if(index==-1){
					break;
				}
				blocks[numBlocks] = new Graphics_Object(IDB_RedBlock,x,y, 32, 32, IDB_RedBlockMask);
				numBlocks++;

			}
		}

		if (strcmp(section, "[Big-Block]"  )==0){
			while (true)
			{
				fscanf(file,"%d %d %d",&index,&x,&y);
				if(index==-1){
					break;
				}
				
				blocks[numBlocks] = new Graphics_Object(IDB_BigBlock,x,y, 64, 64, IDB_BigBlockMask);
				numBlocks++;
			}
		}

		if (strcmp(section, "[Ground-Enemy]"  )==0){
		   while (true){
			fscanf(file,"%d %d %d",&index,&x,&y);
			if(index==-1){
				break;
			}
			enemies[numEnemies] = new GroundEnemy();
			enemies[numEnemies]->x=x;
			enemies[numEnemies]->y=y;
			numEnemies++;
		   }}
		if (strcmp(section, "[End]"  )==0){
			break;
		}


	}

	fclose(file);
}