#pragma once

#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include "V2.h"
 
const double PI = 3.141592653589793238463;

struct Color
{
	float R, G, B, A;

	Color(float r, float g, float b, float a = 1)  {  R = r;   G = g;   B = b;   A = a; }
	Color() {}

	const static Color Black, White, Red, Green, Blue, Magenta, Cyan, Yellow, Gray;  // use Color::Black
};
  


namespace G2D
{
	// Main Graphic System Function

	void InitWindow(int & argc, char** & argv, V2 WindowSize, V2 WindowStartPos, std::string name);
	void Run(void animCallBack(), void render());
	void ClearScreen(Color c);
	void Show();  // send all draw commands 
	 

	// draw string
	 
	void DrawStringFontMono (V2 pos, std::string text, float fontSize = 20, float thickness = 3, Color c = Color::Black);
	void DrawRectangle(V2 P1, V2 Size, Color c, bool fill = false);
 
	// Draw Geometry
	void DrawLine(V2 P1, V2 P2, Color c );
	void DrawPolygon(std::vector<V2> & PointList, Color c, bool fill = false);
	void DrawCircle(V2 C, float r, Color c,  bool fill=false);

	// Timing function
	double ElapsedTimeFromLastCallbackSeconds();
	double ElapsedTimeFromStartSeconds();
}