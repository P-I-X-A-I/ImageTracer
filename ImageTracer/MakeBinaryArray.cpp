#include "stdafx.h"
#include "mainController.h"

void mainController::makeBinaryArray()
{

	int PO_WORD_SIZE = sizeof(potrace_word) * 8;
	int PADDING_SIZE = PO_WORD_SIZE - (BMP_imgWidth % PO_WORD_SIZE );


	if( pixBin_Red != NULL )
	{
		free( pixBin_Red );
		pixBin_Red = NULL;
	}

	if( pixBin_Blue != NULL )
	{
		free( pixBin_Blue );
		pixBin_Blue = NULL;
	}



	// alloc memory
	pixBin_Red = (unsigned char*)malloc( BMP_imgHeight * (BMP_imgWidth + PADDING_SIZE));
	pixBin_Blue = (unsigned char*)malloc( BMP_imgHeight * (BMP_imgWidth + PADDING_SIZE));
	memset( pixBin_Red, 0, BMP_imgHeight * (BMP_imgWidth + PADDING_SIZE));
	memset( pixBin_Blue, 0, BMP_imgHeight * (BMP_imgWidth + PADDING_SIZE));
	unsigned char* pixBin_Red_copy = pixBin_Red;
	unsigned char* pixBin_Blue_copy = pixBin_Blue;


	// get data from
	unsigned char* bmpPtr = BMP_dataPtr;
	unsigned char RGB[3];

	for( int i = 0 ; i < BMP_imgHeight ; i++ )
	{
		for( int j = 0 ; j < BMP_imgWidth ; j++ )
		{
			RGB[0] = *bmpPtr; bmpPtr++; //B
			RGB[1] = *bmpPtr; bmpPtr++; //G
			RGB[2] = *bmpPtr; bmpPtr++; //R

			if( RGB[2] > 128 )
			{
				*pixBin_Red_copy = 1;
				pixBin_Red_copy++;
			}
			else
			{
				*pixBin_Red_copy = 0;
				pixBin_Red_copy++;
			}

			if( RGB[0] > 128 )
			{
				*pixBin_Blue_copy = 1;
				pixBin_Blue_copy++;
			}
			else
			{
				*pixBin_Blue_copy = 0;
				pixBin_Blue_copy++;
			}
		}

		for( int k = 0 ; k < PADDING_SIZE ; k++ )
		{
			*pixBin_Red_copy = 0;
			pixBin_Red_copy++;
			*pixBin_Blue_copy = 0;
			pixBin_Blue_copy++;
		}
	}// i


	this->makePotraceBitmap(PADDING_SIZE);
}