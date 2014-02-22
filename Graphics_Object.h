#pragma once
class Graphics_Object
{
public:
	Graphics_Object(int i, int x, int y, int h, int w, int m);
	int ID;
	void Loadimage(); 
	void Draw(HDC offScreenDC); 
	int height;
	int width;
	int x;
	int y;
	HDC image;
	int maskID;
	HDC mask;
	int cellCount;
	int Startx;
	int Starty;
	void Animate();
	int startCell;
	int currentCell;
	int endCell;
	bool loopCell;
	int hitheight;
	int hitwidth;
	bool HitDetection(Graphics_Object object);
	~Graphics_Object(void);
	
};

