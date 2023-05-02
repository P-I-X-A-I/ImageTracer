#include "stdafx.h"
#include "mainController.h"

void mainController::messageFromGUI(UINT mes, int id, int eventCode, LPARAM lParam )
{
	if( mes == WM_COMMAND )
	{
	if( eventCode == BN_CLICKED )
	{
		//printf("id:%d event:%d\n", id, eventCode);
		switch( id )
		{
		case 1000: // load image
			this->loadImage();
			break;
		case 2000: // radio 1
			this->radioButton(0);
			break;
		case 3000: // radio 2
			this->radioButton(1);
			break;
		case 4000: // radio 3
			this->radioButton(2);
			break;
		case 6000: // generate

			isProcessed = false;
			this->checkBMP_andMakeRGBArray();

			if( isProcessed )
			{
				// do offset
				isProcessed = this->doOffset();
			}

			// draw path to window
			this->drawPathInWindow();

			EnableWindow( h_Button_save, true);
			break;
		case 7000: // save
			this->exportGCODE();
			break;
		default:
			break;
		}//switch
	}// if clicked
	}// if
	else if( mes == WM_HSCROLL || mes == WM_VSCROLL )
	{
		int sValue;
		WCHAR setTString[100];

		// alloc memory
		char* tempString = (char*)malloc(100);
		memset( tempString, '\0', 100 );
		
		//setlocale( LC_ALL, "Japanese");



		if( (HWND)lParam == h_Slider_drill )
		{
			sValue = SendMessage( h_Slider_drill, TBM_GETPOS, 0, 0 );
			S_DRILL = sValue;
			//printf("drill %d %d pos%d\n", id, eventCode, sValue);
			sprintf_s( tempString, 100, " Drill diameter : %1.1fmm", S_DRILL*0.1 );
			mbstowcs_s( NULL, setTString, 100, tempString, 100);
			SendMessageW( h_Text_drill, WM_SETTEXT, 0, (LPARAM)setTString );
		}
		else if( (HWND)lParam == h_Slider_loop )
		{
			sValue = SendMessage( h_Slider_loop, TBM_GETPOS, 0, 0 );
			S_LOOPS = sValue;
			//printf("loop %d %d pos%d\n", id, eventCode, sValue);
			sprintf_s( tempString, 100, "Loops for milling : %d", S_LOOPS );
			mbstowcs_s( NULL, setTString, 100, tempString, 100 );
			SendMessageW( h_Text_loop, WM_SETTEXT, 0, (LPARAM)setTString );
		}
		else if( (HWND)lParam == h_Slider_milldepth )
		{
			sValue = SendMessage( h_Slider_milldepth, TBM_GETPOS, 0, 0 );
			S_MILL_DEPTH = sValue;
			//printf("milldepth %d %d pos%d\n", id, eventCode, sValue);
			sprintf_s( tempString, 100, "Milling depth : %1.1fmm", S_MILL_DEPTH * 0.2 );
			mbstowcs_s( NULL, setTString, 100, tempString, 100 );
			SendMessageW( h_Text_milldepth, WM_SETTEXT, 0, (LPARAM)setTString );		}
		else if( (HWND)lParam == h_Slider_cutdepth )
		{
			sValue = SendMessage( h_Slider_cutdepth, TBM_GETPOS, 0, 0 );
			S_CUT_DEPTH = sValue;
			//printf("cutdepth %d %d pos%d\n", id, eventCode, sValue);
			sprintf_s( tempString, 100, "Cutout depth : %1.1fmm", S_CUT_DEPTH*0.4 );
			mbstowcs_s( NULL, setTString, 100, tempString, 100 );
			SendMessageW( h_Text_cutdepth, WM_SETTEXT, 0, (LPARAM)setTString );
		}
		else if( (HWND)lParam == h_Slider_speed )
		{
			sValue = SendMessage( h_Slider_speed, TBM_GETPOS, 0, 0);
			S_SPEED = sValue;
			sprintf_s( tempString, 100, "Speed : %dmm/s", S_SPEED*2 + 4 );
			mbstowcs_s( NULL, setTString, 100, tempString, 100 );
			SendMessageW ( h_Text_speed, WM_SETTEXT, 0, (LPARAM)setTString );
		}

		free( tempString );
	}
}


void mainController::loadImage()
{
	printf("loadImage\n");

	OPENFILENAME ofn;
	TCHAR filePath[MAX_PATH];
	TCHAR fileTitle[129];
	TCHAR defaultDirectory[MAX_PATH];

	// set filePath[0] to null (necessary)
	filePath[0] = '\0';
	GetCurrentDirectory( MAX_PATH, defaultDirectory );
	

	// init ofn and load bitmap
	memset( &ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = h_mainWindow;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = fileTitle;
	ofn.nMaxFileTitle = 128;
	ofn.lpstrFilter = TEXT("bitmap file( *.bmp)\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = defaultDirectory;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;



	if( !GetOpenFileName(&ofn))
	{
		printf("file open canceled.\n");
	}
	else
	{
		// load bitmap file as HBITMAP
		h_loadBitmap = (HBITMAP)LoadImage( NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// load bitmap as HANDLE
		if(fh_BMP != INVALID_HANDLE_VALUE )
		{
			CloseHandle(fh_BMP);
		}
		fh_BMP = CreateFile( filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);




		if(h_loadBitmap == NULL || fh_BMP == INVALID_HANDLE_VALUE )
		{
			this->showAlert( TEXT("file open error"), TEXT("no bitmap file found.") );
		}
		else
		{
			this->getBitmapInformation();
		}
	}
}



void mainController::radioButton(int num)
{
	mode = num;

	switch( num )
	{
	case 0:
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, true );
		EnableWindow( h_Slider_milldepth, true );
		EnableWindow( h_Slider_cutdepth, false);

		EnableWindow( h_Text_drill, true);
		EnableWindow( h_Text_loop, true);
		EnableWindow( h_Text_milldepth, true);
		EnableWindow( h_Text_cutdepth, false);
		break;
	case 1:
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, false );
		EnableWindow( h_Slider_milldepth, false );
		EnableWindow( h_Slider_cutdepth, true);

		EnableWindow( h_Text_drill, true);
		EnableWindow( h_Text_loop, false);
		EnableWindow( h_Text_milldepth, false);
		EnableWindow( h_Text_cutdepth, true);
		break;
	case 2:
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, true );
		EnableWindow( h_Slider_milldepth, true );
		EnableWindow( h_Slider_cutdepth, true);

		EnableWindow( h_Text_drill, true);
		EnableWindow( h_Text_loop, true);
		EnableWindow( h_Text_milldepth, true);
		EnableWindow( h_Text_cutdepth, true);
		break;
	default:
		break;
	}
}