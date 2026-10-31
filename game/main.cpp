#include "raylib.h"
#include "raymath.h"



// CALCULATING PEN TIP POSITION---------------------------------------------------------------------------------------------------------------------------------
// hypotrochoid formula.
Vector2 PenPosition(float t, float blueCircleRadius, float blackCircleRadius, float penDist, Vector2 center)
{
	float distance = blueCircleRadius - blackCircleRadius;  // distance from the middle to the black circle's center
	float spin = distance / blackCircleRadius * t; // how far the black circle has rotated on itself

	Vector2 pen;
	pen.x = center.x +       distance * cos(t)     +     penDist * cos(spin);
	pen.y = center.y +       distance * sin(t)     -     penDist * sin(spin);
	return pen;
}




//SLIDEER-----------------------------------------------------------------------------------------------------------------

float Slider(float x, float y, float w, float value, float minValue, float maxValue, const char* label)
{
	Vector2 mouse = GetMousePosition();

	// BQ: is the mouse over the bar?
	bool mouseOnBar = mouse.x >= x && mouse.x <= x + w && mouse.y >= y - 8 && mouse.y <= y + 18;


	// 10 UNIT INCREMENTS/DECREMENTS
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && mouseOnBar)
	{
		float percent = (mouse.x - x) / w;// 0=left, 1=right
		value = minValue + percent * (maxValue - minValue); // percent into anormal value

		value = round(value / 10) * 10;// round up to the nearest 10
	}

	// draw the slider (note: the handle(ciurcle) position is the opposite calculation)
	float percent = (value - minValue) / (maxValue - minValue);
	DrawRectangle(x, y, w, 10, LIGHTGRAY);
	DrawCircle(x + percent * w, y + 5, 10, DARKGRAY);
	DrawText(TextFormat("%s: %d", label, (int)value), x, y - 22, 18, BLACK);

	return value;
}













// DRAWING THE THING----------------------------------------------------------------------------------------------------------------------------
void Hypotrochoid()
{
	//setup stuff
	int wh = 1200;
	InitWindow(wh, wh, "HYPOTROCHOID");
	SetTargetFPS(60);
	//-----------------------------------------------------
	


	//init values -----------------------------------------------------
	
	Vector2 center = Vector2{ wh / 2.0f, wh / 2.0f }; //center of the whole shenenigans

	float blueCircleRadius = 140;
	float blackCircleRadius = 60;
	float penDist = 260;

	float t = 0;// ever growing angle
	float speed = 0.02f;
	int trailLength = 100;



	// running it-------------------------------------------------------------
	while (!WindowShouldClose())
	{
		t += speed; //update stuff while it runs

		BeginDrawing();
		ClearBackground(RAYWHITE);

		//=================================================================================================
		// SLIDERS. Giving values friom the returning slider fucntion
		blueCircleRadius = (int)Slider(20, 40, 200, blueCircleRadius, 100, 380, "Blue radius");
		blackCircleRadius = (int)Slider(20, 100, 200, blackCircleRadius, 10, 90, "Black radius");
		penDist = (int)Slider(20, 160, 200, penDist, 10, 250, "Pen distance");
	   //-----------------------------------------------------------------------------------------------------------



		// BLUE circle 
		DrawCircleLinesV(center, blueCircleRadius, BLUE);
		//--------------------------------------------------------------------


		//Faint red line - making  a bunch of smaller lines
		for (float i = 0; i < 90 * 2 * PI; i += 0.05f)
		{
			       //PenPosition(float t, float blueCircleRadius, float blackCircleRadius, float penDist, Vector2 center)
			Vector2 p1 = PenPosition(i, blueCircleRadius, blackCircleRadius, penDist, center);
			Vector2 p2 = PenPosition(i + 0.05f, blueCircleRadius, blackCircleRadius, penDist, center);
			DrawLineEx(p1, p2, 1, Color{ 255, 170, 170, 255 });
		}
		//---------------------------------------------------------------------------------------------------------------------------------------------


		// BLACK CIRCLE STUFF=====================================================================================================
		// BLACK small circle = its center travels on a smaller circle of radius (blue - black), so it always stays inside the blue oe
		Vector2 smallCenter;
		smallCenter.x = center.x + (blueCircleRadius - blackCircleRadius) * cos(t);
		smallCenter.y = center.y + (blueCircleRadius - blackCircleRadius) * sin(t);

		DrawCircleLinesV(smallCenter, blackCircleRadius, BLACK); //black circle outline
		DrawCircleV(smallCenter, 3, BLACK);//black circle center

		//DRawing pen and arm connected to black circle
		Vector2 pen = PenPosition(t, blueCircleRadius, blackCircleRadius, penDist, center);
		DrawLineEx(smallCenter, pen, 2, BLACK);// the actual functio to draw the line

		//----------------------------------------------------------------------------------------------------------------------------------------------------




		// THICK red smaller line-----------------------------------------------------------------------------------------------------------------------------------
		for (int i = 0; i < trailLength; i++)
		{
			float tNewer = t - i * speed;
			float tOlder = t - (i + 1) * speed;

			Vector2 p1 = PenPosition(tNewer, blueCircleRadius, blackCircleRadius, penDist, center);
			Vector2 p2 = PenPosition(tOlder, blueCircleRadius, blackCircleRadius, penDist, center);
			DrawLineEx(p1, p2, 4, RED);
		}





		EndDrawing();
	}

	CloseWindow();
}

int main(void)
{
	Hypotrochoid();
	return 0;
}