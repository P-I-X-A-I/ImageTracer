#include "stdafx.h"
#include "mainController.h"

void mainController::getBitmapInformation()
{
	printf("Get bitmap information\n");


	// get bitmap info
	BITMAP bm_t;
	GetObject( h_loadBitmap, sizeof(BITMAP), &bm_t);

	printf("bitmap information\n width:%d\n height:%d\n widthBytes:%d\n bitsperpixel:%d\n plane:%d\n", 
		bm_t.bmWidth,
		bm_t.bmHeight,
		bm_t.bmWidthBytes,
		bm_t.bmBitsPixel,
		bm_t.bmPlanes);

	//bm_t.bmBits;
	//bm_t.bmBitsPixel;
	//bm_t.bmHeight;
	//bm_t.bmWidth;
	//bm_t.bmWidthBytes;
	//bm_t.bmType;
	//bm_t.bmPlanes;



	DWORD numOfReadBytes;
	int error;

	// get file type (from 0, 2byte) always "BM"
	SetFilePointer( fh_BMP, 0, NULL, FILE_BEGIN );
	char headSTR[3];
	headSTR[2] = '\0';
	error = ReadFile( fh_BMP, &headSTR, 2, &numOfReadBytes, NULL);
	printf("BMP:filetype:%s\n", headSTR);



	// file size( from 2, 4byte )
	SetFilePointer( fh_BMP, 2, NULL, FILE_BEGIN );
	UINT32 fileSize;
	error = ReadFile( fh_BMP, &fileSize, 4, &numOfReadBytes, NULL);
	printf("BMP:filesize %d\n", fileSize);



	// data offset( from 10, 4byte)
	SetFilePointer( fh_BMP, 10, NULL, FILE_BEGIN );
	UINT32 fileOffset;
	error = ReadFile( fh_BMP, &fileOffset, 4, &numOfReadBytes, NULL);
	printf("BMP:fileOffset %d\n", fileOffset);
	//**********************
	BMP_dataOffset = fileOffset;
	//**********************



	// info header size(from 14, 4byte)
	SetFilePointer( fh_BMP, 14, NULL, FILE_BEGIN);
	UINT32 headerSize;
	error = ReadFile( fh_BMP, &headerSize, 4, &numOfReadBytes, NULL);
	printf("BMP:headerSize %d\n", headerSize);

	if( headerSize == 40 )
	{
		// this bitmap is windows format, OK;
	}
	else
	{
		this->showAlert( TEXT("Invalid file format."), TEXT("This bitmap image may be OS/2 format. Please use WINDOWS bmp format.") );
		return;
	}




	// image width (from 18, 4byte
	SetFilePointer( fh_BMP, 18, NULL, FILE_BEGIN );
	UINT32 tempWidth;
	error = ReadFile( fh_BMP, &tempWidth, 4, &numOfReadBytes, NULL);
	//*******************************
	BMP_imgWidth = tempWidth;
	//*******************************
	printf("BMP:width %d\n", tempWidth);


	// image height ( from 22, 4byte )
	SetFilePointer( fh_BMP, 22, NULL, FILE_BEGIN );
	UINT32 tempHeight;
	error = ReadFile( fh_BMP, &tempHeight, 4, &numOfReadBytes, NULL);
	//********************************
	BMP_imgHeight = tempHeight;
	//*******************************
	printf("BMP:height %d\n", tempHeight);



	if( BMP_imgWidth > MAX_IMAGE_SIZE || BMP_imgHeight > MAX_IMAGE_SIZE )
	{
		this->showAlert( TEXT("This image is too huge."), TEXT("Please use an image which is smaller than 10000pixel width (or height)") );
		return;
	}


	// bpp (from 28, 2byte)
	SetFilePointer( fh_BMP, 28, NULL, FILE_BEGIN );
	UINT16 bpp;
	error = ReadFile( fh_BMP, &bpp, 2, &numOfReadBytes, NULL);
	//**********
	BMP_bpp = bpp;
	//**********
	printf("BMP:bpp %d\n", BMP_bpp);

	if(BMP_bpp == 24 || BMP_bpp == 8 )
	{// ok
	}
	else
	{
		this->showAlert( TEXT("Invarid file format."), TEXT("You have to use 8bit grayscale image or 24bit RGB image.") );
		return;
	}




	// image data size ( from 34, 4byte )
	SetFilePointer( fh_BMP, 34, NULL, FILE_BEGIN );
	UINT32 imageDataSize;
	error = ReadFile( fh_BMP, &imageDataSize, 4, &numOfReadBytes, NULL );
	printf("BMP:imageDataSize %d\n", imageDataSize);



	// color palette (from 46, 4byte )
	SetFilePointer( fh_BMP, 46, NULL, FILE_BEGIN );
	UINT32 colorPalette;
	error = ReadFile( fh_BMP, &colorPalette, 4, &numOfReadBytes, NULL );
	printf("BMP:colorPalette %d\n", colorPalette );



	// w dpi (from 38, 4byte)
	SetFilePointer(fh_BMP, 38, NULL, FILE_BEGIN);
	UINT32 dpiW;
	error = ReadFile( fh_BMP, &dpiW, 4, &numOfReadBytes, NULL );
	//***************
	BMP_dpi_w = (double)(dpiW) * 0.01 * 2.54;
	//***************
	printf("BMP:w dpi %1.4f\n", BMP_dpi_w);



	// h dpi (from 42, 4byte)
	SetFilePointer( fh_BMP, 42, NULL, FILE_BEGIN );
	UINT32 dpiH;
	error = ReadFile( fh_BMP, &dpiH, 4, &numOfReadBytes, NULL );
	//*************************
	BMP_dpi_h = (double)(dpiH) * 0.01 * 2.54;
	//************************
	printf("BMP:h dpi %1.4f\n", BMP_dpi_h );



	if( BMP_dpi_w < 300.0 || BMP_dpi_h < 300.0 )
	{
		this->showAlert(TEXT("Invalid file resolution."), TEXT("This image is too lough. You should use at least 300 dpi image. ( over 600dpi is highly recomended )"));
		return;
	}



	// set GUI
	if( mode == 0 )
	{
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, true );
		EnableWindow( h_Slider_milldepth, true );
		EnableWindow( h_Slider_cutdepth, false );
		EnableWindow( h_Slider_speed, true );

		EnableWindow( h_Text_drill, true );
		EnableWindow( h_Text_loop, true );
		EnableWindow( h_Text_milldepth, true );
		EnableWindow( h_Text_cutdepth, false );
		EnableWindow( h_Text_speed, true);
	}
	else if( mode == 1 )
	{
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, false );
		EnableWindow( h_Slider_milldepth, false );
		EnableWindow( h_Slider_cutdepth, true );
		EnableWindow( h_Slider_speed, true );
	
		EnableWindow( h_Text_drill, true );
		EnableWindow( h_Text_loop, false );
		EnableWindow( h_Text_milldepth, false );
		EnableWindow( h_Text_cutdepth, true );
		EnableWindow( h_Text_speed, true);
	}
	else if( mode == 2 )
	{
		EnableWindow( h_Slider_drill, true );
		EnableWindow( h_Slider_loop, true );
		EnableWindow( h_Slider_milldepth, true );
		EnableWindow( h_Slider_cutdepth, true );
		EnableWindow( h_Slider_speed, true );

		EnableWindow( h_Text_drill, true );
		EnableWindow( h_Text_loop, true );
		EnableWindow( h_Text_milldepth, true );
		EnableWindow( h_Text_cutdepth, true );
		EnableWindow( h_Text_speed, true);
	}

	EnableWindow( h_Text_imagesize, true );
	EnableWindow( h_Text_parameters, true );
	EnableWindow( h_Text_generatefor, true );

	EnableWindow( h_Radio_milling, true );
	EnableWindow( h_Radio_cutout, true );
	EnableWindow( h_Radio_both, true );

	EnableWindow( h_Button_generate, true );



	// set image size text
	double mmW, mmH;
	mmW = BMP_imgWidth / (BMP_dpi_w / 25.4 );
	mmH = BMP_imgHeight / (BMP_dpi_h / 25.4 );

	char* tempString = (char*)malloc(100);
	WCHAR tempTString[101]; 

	memset( tempString, '\0', 100);
	sprintf_s( tempString, 100, " W%1.1fmm x H%1.1fmm", mmW, mmH );
	printf("%s", tempString);
	
	setlocale( LC_ALL, "Japanese");
	mbstowcs_s( NULL, tempTString, 100, tempString, 100);
	SendMessageW( h_Text_imagesize, WM_SETTEXT, 0, (LPARAM)tempTString );




	// draw to Window

	// create context on memory and copy bmp to it
	h_memoryDC = CreateCompatibleDC(NULL);
	SelectObject( h_memoryDC, h_loadBitmap );

	HDC tempHDC = GetDC( h_mainWindow );


	// clear window
	BitBlt( tempHDC,
		VIEW_L_ORIGIN_X, // origin x
		VIEW_L_ORIGIN_Y, // origin y
		VIEW_L_WIDTH, // width
		VIEW_L_HEIGHT, // height
			h_memoryDC, // src
			0,
			0,
			WHITENESS);

	// calcurate streched image & draw
	double scaleW = (double)(VIEW_L_WIDTH) / (double)BMP_imgWidth;
	double scaleH = (double)(VIEW_L_HEIGHT - 72) / (double)BMP_imgHeight; // menu bar is 72pixel
	double scale;
	BOOL isWsmaller;

	if(scaleW < scaleH )
	{
		scale = scaleW;
		isWsmaller = false;
	}
	else
	{
		scale = scaleH;
		isWsmaller = true;
	}

	int draw_W = (int)(BMP_imgWidth * scale);
	int draw_H = (int)(BMP_imgHeight * scale);
	int posX, posY;

	if( isWsmaller )
	{
		posX = (VIEW_L_WIDTH-draw_W)/2;
		posY = 0;
	}
	else
	{
		posX = 0;
		posY = 0;
	}

	StretchBlt( tempHDC,
			 posX, posY, // window origin
			 draw_W, draw_H,// window area WH
			 h_memoryDC,
			 0, 0,
			 BMP_imgWidth, BMP_imgHeight,
			 SRCCOPY);

	DeleteObject( h_loadBitmap );
	DeleteDC( h_memoryDC  );
	

}



void mainController::checkBMP_andMakeRGBArray()
{

	int error;
	DWORD numOfReadBytes;

	// color palette for 8bit image
	unsigned char palette[256][3];


	// calcurate padding (dividable 4byte)
	int padding;

	if( BMP_bpp == 8 )
	{
		int temp = BMP_imgWidth % 4;
		switch( temp )
		{
		case 1:
			padding = 3;
			break;
		case 2:
			padding = 2;
			break;
		case 3:
			padding = 1;
			break;
		default:
			padding = 0;
			break;
		}
		printf("imgWidth %d, BMP data padding %d\n", BMP_imgWidth, BMP_imgWidth+padding);

		// get color palette
		SetFilePointer( fh_BMP, 54, NULL, FILE_BEGIN );

		for( int i = 0 ; i < 256 ; i++ )
		{
			unsigned char tempVal[4];
			error = ReadFile( fh_BMP, &tempVal[0], 1, &numOfReadBytes, NULL );
			error = ReadFile( fh_BMP, &tempVal[1], 1, &numOfReadBytes, NULL );
			error = ReadFile( fh_BMP, &tempVal[2], 1, &numOfReadBytes, NULL );
			error = ReadFile( fh_BMP, &tempVal[3], 1, &numOfReadBytes, NULL );
			palette[i][0] = tempVal[0];
			palette[i][1] = tempVal[1];
			palette[i][2] = tempVal[2];
		}
	}
	else if( BMP_bpp == 24 )
	{
		int temp = (BMP_imgWidth*3)%4;

		switch( temp )
		{
		case 1:
			padding = 3;
			break;
		case 2:
			padding = 2;
			break;
		case 3:
			padding = 1;
			break;
		default:
			padding = 0;
			break;
		}
		printf("imgWidth %d, padded %d\n", BMP_imgWidth, BMP_imgWidth+padding);
	}









	// alloc memory
	if(BMP_dataPtr != NULL)
	{
		free( BMP_dataPtr );
		BMP_dataPtr = NULL;
	}
	BMP_dataPtr = (unsigned char*)malloc( BMP_imgWidth * BMP_imgHeight * 3 + 1 );
	unsigned char* BMP_dataPtr_copy = BMP_dataPtr;


	BOOL isHasRed = false;
	BOOL isHasWhite = false;
	unsigned char check[3];
	double PERCENTAGE;


	// move file pointer
	SetFilePointer( fh_BMP, BMP_dataOffset, NULL, FILE_BEGIN );

	if( BMP_bpp == 8 )
	{
		// get index

		unsigned char index;

		for( int i = 0 ; i < BMP_imgHeight ; i++ )
		{
			// log
			
			PERCENTAGE = (double)i / (double)BMP_imgHeight;
			this->showLog( PERCENTAGE );


			for( int j = 0 ; j < BMP_imgWidth ; j++ )
			{
				error = ReadFile( fh_BMP, &index, 1, &numOfReadBytes, NULL );
				*BMP_dataPtr_copy = check[0] = palette[index][0]; BMP_dataPtr_copy++;
				*BMP_dataPtr_copy = check[1] = palette[index][1]; BMP_dataPtr_copy++;
				*BMP_dataPtr_copy = check[2] = palette[index][2]; BMP_dataPtr_copy++;

				if( !isHasWhite )
				{
				if( check[0] > 220 && check[1] > 220 && check[2] > 220 )
				{
					isHasWhite = true;
				}
				}
			}// j

			for( int k = 0 ; k < padding ; k++ )
			{
				// proceed pointer
				error = ReadFile( fh_BMP, &index, 1, &numOfReadBytes, NULL );
				//printf("pad");
			}
			//printf("\n");
		}// i
	}
	else if(BMP_bpp == 24 )
	{
		unsigned char VAL;

		for( int i = 0 ; i < BMP_imgHeight ; i++ )
		{
						// log
			
			PERCENTAGE = (double)i / (double)BMP_imgHeight;
			this->showLog( PERCENTAGE );


			for( int j = 0 ; j < BMP_imgWidth ; j++ )
			{
				// pointer += 3;
				error = ReadFile( fh_BMP, &VAL, 1, &numOfReadBytes, NULL );
				*BMP_dataPtr_copy = check[0] = VAL;
				BMP_dataPtr_copy++;

				error = ReadFile( fh_BMP, &VAL, 1, &numOfReadBytes, NULL );
				*BMP_dataPtr_copy = check[1] = VAL;
				BMP_dataPtr_copy++;
				
				error = ReadFile( fh_BMP, &VAL, 1, &numOfReadBytes, NULL );
				*BMP_dataPtr_copy = check[2] = VAL;
				BMP_dataPtr_copy++;

				if( !isHasRed )
				{
				if( check[0] < 20 && check[1] < 20 && check[2] > 220 )
				{
					isHasRed = true;
				}
				}

				if(!isHasWhite)
				{
				if( check[0] > 220 && check[1] > 220 && check[2] > 220 )
				{
					isHasWhite = true;
				}
				}
			}

			for( int k = 0 ; k < padding ; k++ )
			{
				// pointer += 1
				error = ReadFile( fh_BMP, &VAL, 1, &numOfReadBytes, NULL );
			}
		}// for i
	} // if 24




	// check image data
	if( mode == 2 )// both mode
	{
		if( isHasRed == false )
		{
			this->showAlert(TEXT("This image don't contain Red Color."), TEXT("When you select 'both' mode, The cutout path should be drawn by red color."));
			CloseHandle( fh_BMP );
			fh_BMP = INVALID_HANDLE_VALUE;
			return;
		}
	}

	if( mode == 1 )// cutout
	{
		if( isHasWhite == false )
		{
			this->showAlert(TEXT("This image don't contain White color."), TEXT("When you select 'Cutout' mode, The cutout path should be drawn by white color."));
			CloseHandle( fh_BMP );
			fh_BMP = INVALID_HANDLE_VALUE;
			return;
		}
	}


	if( isHasWhite == false )
	{
		this->showAlert(TEXT("This image don't contain White color."), TEXT("The circuit pattern should be drawn by white color."));
		CloseHandle( fh_BMP );
		fh_BMP = INVALID_HANDLE_VALUE;
		return;
	}


	// delete log
	this->deleteLog();



	// delete HBITMAP & HANDLE
	
	this->makeBinaryArray();
}