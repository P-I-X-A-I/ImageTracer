#include "stdafx.h"
#include "mainController.h"


void mainController::drawPathInWindow()
{
	// scale factor
	double scaleF[2];
	double scaleFactor;

	scaleF[0] = (double)(VIEW_L_WIDTH) / (double)BMP_imgWidth;
	scaleF[1] = (double)(VIEW_L_HEIGHT-72) / (double)BMP_imgHeight;

	if( scaleF[0] < scaleF[1] )
	{
		scaleFactor = scaleF[0];
	}
	else
	{
		scaleFactor = scaleF[1];
	}

	double scaledHeight = scaleFactor * BMP_imgHeight;
	double biasY = VIEW_L_HEIGHT - (int)scaledHeight;

	// right view
	int Origin_X = VIEW_L_WIDTH;
	int Origin_Y = 0;

	PAINTSTRUCT ps;
	HPEN h_RedPen = CreatePen( PS_SOLID, 1, 0x0000FF );
	HPEN h_BlackPen = CreatePen( PS_SOLID, 1, 0x000000 );
	HDC hdc = GetDC( h_mainWindow );

	double* pRed_Ptr = offset_Red_ptr;
	double* pBlue_Ptr = offset_Blue_ptr;
	double val_X, val_Y;
	bool isFirstPoint = true;




	// clear view
	HDC tempContext = CreateCompatibleDC(NULL);
	BitBlt( hdc,
		Origin_X, // origin x
		Origin_Y, // origin y
		VIEW_L_WIDTH, // width
		VIEW_L_HEIGHT, // height
			NULL, // src
			0,
			0,
			WHITENESS);
	DeleteDC( tempContext );


	// draw blue path
	if( mode == 0 || mode == 2 )
	{
		SelectObject( hdc, h_BlackPen );
	}
	else if( mode == 1 )
	{
		SelectObject( hdc, h_RedPen );
	}



	while( 1 )
	{
		val_X = (*pBlue_Ptr); pBlue_Ptr++;
		val_Y = (*pBlue_Ptr);


		if( (int)val_X == -200 || (int)val_Y == -200 )
		{
			break;
		}
		if( (int)val_X == -100 )// knot point
		{
			isFirstPoint = true;
		}
		else
		{
			if( isFirstPoint == true )
			{
				isFirstPoint = false;
				MoveToEx( hdc,
					(int)(val_X*scaleFactor) + VIEW_L_WIDTH,
					VIEW_L_HEIGHT - biasY - (int)(val_Y*scaleFactor),
					NULL );
			}
			else
			{
				LineTo( hdc,
					(int)(val_X*scaleFactor) + VIEW_L_WIDTH,
					VIEW_L_HEIGHT - biasY - (int)(val_Y*scaleFactor) );
			}
		}

		pBlue_Ptr++;
	}// while



	// cut path (red)
	SelectObject( hdc, h_RedPen );
	isFirstPoint = true;

	while( 1 )
	{
		val_X = (*pRed_Ptr); pRed_Ptr++;
		val_Y = (*pRed_Ptr);

		if( (int)val_X == -200 || (int)val_Y == -200 )
		{
			break;
		}
		else if( (int)val_X == -100 )
		{
			isFirstPoint = true;
		}
		else
		{
			if( isFirstPoint == true )
			{
				isFirstPoint = false;
				MoveToEx( hdc,
					(int)(val_X*scaleFactor) + VIEW_L_WIDTH,
					VIEW_L_HEIGHT - biasY - (int)(val_Y*scaleFactor),
					NULL );
			}
			else
			{
				LineTo( hdc,
					(int)(val_X*scaleFactor) + VIEW_L_WIDTH,
					VIEW_L_HEIGHT - biasY - (int)(val_Y*scaleFactor) );
			}
		}
		pRed_Ptr++;
	}// while


	EndPaint( h_mainWindow, &ps );
}