#include "stdafx.h"
#include "mainController.h"


mainController::mainController(void)
{
	printf("mainController init\n");


}


mainController::~mainController(void)
{
}


void mainController::awake()
{
	printf("like a awakeFromNib\n");
	
	fh_BMP = INVALID_HANDLE_VALUE;

	// init variables
	BMP_dataOffset = 0;
	BMP_imgWidth = 0;
	BMP_imgHeight = 0;
	BMP_bpp = 0;
	BMP_dpi_w = 0.0;
	BMP_dpi_h = 0.0;
	BMP_dataPtr = NULL;
	pixBin_Red = NULL;
	pixBin_Blue = NULL;

	mode = 0;

	S_DRILL = 4;
	S_LOOPS = 1;
	S_MILL_DEPTH = 1;
	S_CUT_DEPTH = 4;
	S_SPEED = 2;

	poBitmap_Red_Ptr = NULL;
	poBitmap_Blue_Ptr = NULL;
	poParam_Red_t = NULL;
	poParam_Blue_t = NULL;
	poState_Red_t = NULL;
	poState_Blue_t = NULL;

	pointStrip_Red_ptr = NULL;
	pointStrip_Blue_ptr = NULL;
	offset_Red_ptr = NULL;
	offset_Blue_ptr = NULL;

	isProcessed = false;

	//select radio
	SendMessage( h_Radio_milling, BM_SETCHECK, BST_CHECKED, 0 );


	// set Slider Position


	SendMessage( h_Slider_drill, TBM_SETPOS, 0, S_DRILL );
	SendMessage( h_Slider_loop, TBM_SETPOS, 0, S_LOOPS );
	SendMessage( h_Slider_milldepth, TBM_SETPOS, 0, S_MILL_DEPTH );
	SendMessage( h_Slider_cutdepth, TBM_SETPOS, 0, S_CUT_DEPTH );
	SendMessage( h_Slider_speed, TBM_SETPOS, 0, S_SPEED );

	SendMessage( h_Slider_drill, TBM_SETRANGE, true, MAKELPARAM( 1, 20 ) );
	SendMessage( h_Slider_loop, TBM_SETRANGE, true, MAKELPARAM(1, 8) );
	SendMessage( h_Slider_milldepth, TBM_SETRANGE, true, MAKELPARAM(1, 3) );
	SendMessage( h_Slider_cutdepth, TBM_SETRANGE, true, MAKELPARAM(1, 6) );
	SendMessage( h_Slider_speed, TBM_SETRANGE, true, MAKELPARAM( 0, 4 ) ); 


	// set GUI default status
	EnableWindow( h_Radio_milling, false );
	EnableWindow( h_Radio_cutout, false );
	EnableWindow( h_Radio_both, false );

	EnableWindow( h_Slider_drill, false );
	EnableWindow( h_Slider_loop, false );
	EnableWindow( h_Slider_milldepth, false );
	EnableWindow( h_Slider_cutdepth, false );
	EnableWindow( h_Slider_speed, false );

	EnableWindow( h_Button_generate, false );
	EnableWindow( h_Button_save, false );

	EnableWindow( h_Text_drill, false );
	EnableWindow( h_Text_loop, false );
	EnableWindow( h_Text_milldepth, false );
	EnableWindow( h_Text_cutdepth, false );
	EnableWindow( h_Text_speed, false );
	EnableWindow( h_Text_imagesize, false );
	EnableWindow( h_Text_parameters, false );
	EnableWindow( h_Text_generatefor, false );

	
}


void mainController::showAlert(LPCTSTR title, LPCTSTR sub )
{
	MessageBox( h_mainWindow, sub, title, MB_OK );

	EnableWindow( h_Radio_milling, false );
	EnableWindow( h_Radio_cutout, false );
	EnableWindow( h_Radio_both, false );

	EnableWindow( h_Slider_drill, false );
	EnableWindow( h_Slider_loop, false );
	EnableWindow( h_Slider_milldepth, false );
	EnableWindow( h_Slider_cutdepth, false );
	EnableWindow( h_Slider_speed, false );

	EnableWindow( h_Button_generate, false );
	EnableWindow( h_Button_save, false );

	EnableWindow( h_Text_drill, false );
	EnableWindow( h_Text_loop, false );
	EnableWindow( h_Text_milldepth, false );
	EnableWindow( h_Text_cutdepth, false );
	EnableWindow( h_Text_speed, false );
	EnableWindow( h_Text_imagesize, false );
	EnableWindow( h_Text_parameters, false );
	EnableWindow( h_Text_generatefor, false );


	// create context on memory and copy bmp to it
	HDC tempContext = CreateCompatibleDC(NULL);
	//SelectObject( h_memoryDC, h_loadBitmap );

	HDC tempHDC = GetDC( h_mainWindow );


	// clear window
	BitBlt( tempHDC,
		VIEW_L_ORIGIN_X, // origin x
		VIEW_L_ORIGIN_Y, // origin y
		VIEW_L_WIDTH, // width
		VIEW_L_HEIGHT, // height
			NULL, // src
			0,
			0,
			WHITENESS);

	DeleteDC( tempContext );
}



void mainController::showLog( double percentage )
{
	char* tempString = (char*)malloc(100);
	WCHAR setString[100];

	memset( tempString, '\0', 100 );

	sprintf_s( tempString, 100, "processing...%1.3f", percentage*100.0);
	mbstowcs_s( NULL, setString, 100, tempString, 100 );
	SendMessageW( h_Text_log, WM_SETTEXT, 0, (LPARAM)setString);

	free( tempString );
}

void mainController::deleteLog()
{
	WCHAR setString[1];
	setString[0] = '\0';
	SendMessageW( h_Text_log, WM_SETTEXT, 0, (LPARAM)setString);
}