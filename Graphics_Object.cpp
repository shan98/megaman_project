#include "StdAfx.h"
#include "Graphics_Object.h"
#include "Resource.h"

Graphics_Object::Graphics_Object(int i, int posx, int posy, int h, int w, int m)
{ID = i;
x= posx;
y= posy;
width = w;
height = h;
maskID = m;
Startx = 0;
Starty = 0;
currentCell = 0;

endCell = 0;
loopCell = true;
Loadimage();
hitheight = height;
hitwidth = width;
startCell = 0;

}
void Graphics_Object::Loadimage()
{
	HDC hTempDC;
HMODULE Module;
HBITMAP imageBMP;
HBITMAP imageMask;
hTempDC = GetDC (0);
Module = GetModuleHandle(NULL);
//Initaialize graphics Piture

imageBMP = (HBITMAP)LoadImage(Module, (LPCWSTR)ID,IMAGE_BITMAP, 0,0,LR_DEFAULTCOLOR);
image = CreateCompatibleDC(hTempDC);
SelectObject (image, imageBMP) ;
////////////////////////////***********************************///////////////////////////
imageMask = (HBITMAP)LoadImage(Module, (LPCWSTR)maskID,IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR);
mask = CreateCompatibleDC(hTempDC);
SelectObject (mask, imageMask) ;


ReleaseDC(0, hTempDC); //freeing up memory




}
void Graphics_Object::Draw(HDC offScreenDC)
{
	BitBlt(offScreenDC, x- mapPosition, y, width, height,  mask, Startx, Starty, SRCAND);
BitBlt(offScreenDC, x- mapPosition, y, width, height, image, Startx, Starty, SRCPAINT); 

}
void Graphics_Object::Animate()
{
	
	currentCell++;
	if (currentCell >= endCell)
	{
	
		if (loopCell == true)
		{
			currentCell = startCell;
		}else
		{
			currentCell = endCell;
		}
	}
	Startx = width*currentCell;
		
	
}

bool Graphics_Object::HitDetection(Graphics_Object object)
{	
	if (object.x < x + hitwidth)
	{//object moving left and colliding//

	
	if (object.x + object.hitwidth > x)
	{//object moving right and colliding//
	
	if (object.y + object.hitheight > y)
	{//object moving up and colliding//
	
	if (object.y< y + hitheight)
	{//object moving down and colliding//
		return true;
	}
	}
	}
	}
	return false;
}


Graphics_Object::~Graphics_Object(void)
{
}
