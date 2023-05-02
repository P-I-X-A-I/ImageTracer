#include "stdafx.h"
#include "mainController.h"

void progressCall( double val, void* ptr )
{
	printf("progress %f\n", val);
}

void mainController::makePotraceBitmap( int padW )
{
	printf("Create Potrace Bitmap\n");

	int PO_SIZE = sizeof( potrace_word )*8;
	potrace_word maskBit[64];


	// create maskBit
	maskBit[PO_SIZE-1] = 1;
	for( int i = PO_SIZE-2 ; i >= 0 ; i-- )
	{
		maskBit[i] = maskBit[i+1] << 1;
	}



	// create po bitmap
	int numByte = (BMP_imgWidth+padW) * BMP_imgHeight / 8;

	if( poBitmap_Red_Ptr != NULL )
	{
		free( poBitmap_Red_Ptr );
		poBitmap_Red_Ptr = NULL;
	}
	if( poBitmap_Blue_Ptr != NULL )
	{
		free( poBitmap_Blue_Ptr );
		poBitmap_Blue_Ptr = NULL;
	}


	poBitmap_Red_Ptr = (potrace_word*)malloc( numByte );
	poBitmap_Blue_Ptr = (potrace_word*)malloc( numByte );
	memset( poBitmap_Red_Ptr, 0, numByte );
	memset( poBitmap_Blue_Ptr, 0, numByte );


	// init potrace_bitmap_t
	poBitmap_Red_t.w = BMP_imgWidth;
	poBitmap_Red_t.h = BMP_imgHeight;
	poBitmap_Red_t.dy = (BMP_imgWidth+padW) / PO_SIZE;
	poBitmap_Red_t.map = poBitmap_Red_Ptr;

	poBitmap_Blue_t.w = BMP_imgWidth;
	poBitmap_Blue_t.h = BMP_imgHeight;
	poBitmap_Blue_t.dy = (BMP_imgWidth+padW) / PO_SIZE;
	poBitmap_Blue_t.map = poBitmap_Blue_Ptr;


	// copy pointer
	potrace_word* tempR = poBitmap_Red_Ptr;
	potrace_word* tempB = poBitmap_Blue_Ptr;
	unsigned char* pixBin_R_copy = pixBin_Red;
	unsigned char* pixBin_B_copy = pixBin_Blue;


	// create data
	for( int i = 0 ; i < BMP_imgHeight ; i++ )
	{
		for( int j = 0 ; j < (BMP_imgWidth + padW) ; j++ )
		{
		
			int INDEX = j % PO_SIZE;

			// red
			if( *pixBin_R_copy == 1 )
			{
				*tempR = *tempR | maskBit[INDEX];
			}
			else
			{
				// do nothing
			}
			pixBin_R_copy++;



			if( *pixBin_B_copy == 1 )
			{
				*tempB = *tempB | maskBit[INDEX];
			}
			else
			{
				// do nothing
			}
			pixBin_B_copy++;


			if( INDEX == (PO_SIZE-1))
			{
				tempR++;
				tempB++;
			}
		}//j
	}// i


	this->doTracing();
}



void mainController::doTracing()
{
	potrace_progress_t progress_t;
	progress_t.min = 0.0;
	progress_t.max = 100.0;
	progress_t.epsilon = 1.0;
	progress_t.callback = progressCall;



	if( poParam_Red_t != NULL )
	{
		potrace_param_free( poParam_Red_t );
		poParam_Red_t = NULL;
	}
	if( poParam_Blue_t != NULL )
	{
		potrace_param_free( poParam_Blue_t );
			poParam_Blue_t = NULL;
	}


	poParam_Red_t = potrace_param_default();
	poParam_Blue_t = potrace_param_default();

	poParam_Red_t->turdsize = 10; // default 3 // remove small area
	poParam_Red_t->turnpolicy = POTRACE_TURNPOLICY_MINORITY; // default
	poParam_Red_t->alphamax = 1.0; // smoothness, default
	poParam_Red_t->opticurve = 1; // default
	poParam_Red_t->opttolerance = 0.2; // default;
	poParam_Red_t->progress = progress_t;

	poParam_Blue_t->turdsize = 10;
	poParam_Blue_t->turnpolicy = POTRACE_TURNPOLICY_MINORITY;
	poParam_Blue_t->alphamax = 1.0;
	poParam_Blue_t->opticurve = 1;
	poParam_Blue_t->opttolerance = 0.2;
	poParam_Blue_t->progress = progress_t;





	if( poState_Red_t != NULL )
	{
		potrace_state_free( poState_Red_t );
		poState_Red_t = NULL;
	}
	if( poState_Blue_t != NULL )
	{
		potrace_state_free( poState_Blue_t );
		poState_Blue_t = NULL;
	}




	// tracing
	if( mode == 2 )
	{
		poState_Red_t = potrace_trace( poParam_Red_t, &(poBitmap_Red_t) );
	}
	
	poState_Blue_t = potrace_trace( poParam_Blue_t, &(poBitmap_Blue_t) );



	// check status
	if( mode == 2 )
	{
		if( poState_Red_t->status == POTRACE_STATUS_OK )
		{
			printf("tracing red success!\n");
		}
		else
		{
			printf("tracing red error");
			this->showAlert(TEXT("Error."), TEXT("This image can't be traced. try another image."));
		}
	}// if mode 2

	if( poState_Blue_t->status == POTRACE_STATUS_OK )
	{
		printf("tracing blue success!\n");
	}
	else
	{
		printf("tracing blue error");
		this->showAlert(TEXT("Error."), TEXT("This image can't be traced. try another image."));
	}






	// count numof path for allocing memory
	potrace_path_t* pathIn_Red = NULL;
	potrace_path_t* pathIn_Blue = NULL;

	if( mode == 2 )
	{
		pathIn_Red = poState_Red_t->plist;
	}
	pathIn_Blue = poState_Blue_t->plist;

	int numOfPath_Red = 0;
	int numOfPath_Blue = 0;
	int numOfPoint_Red = 0;
	int numOfPonit_Blue = 0;


	if( mode == 2 )
	{
		while(1)
		{
			if( pathIn_Red != NULL )
			{
				potrace_curve_t tempCurve = pathIn_Red->curve;

				for( int k = 0  ; k < tempCurve.n ; k++ )
				{
					int tag = tempCurve.tag[k];

					if( tag == POTRACE_CURVETO )
					{
						numOfPoint_Red += 10;
					}
					else if( tag == POTRACE_CORNER )
					{
						numOfPoint_Red += 2;
					}
				}//for k


				if( pathIn_Red->next != NULL)
				{
				pathIn_Red = pathIn_Red->next;
				numOfPath_Red++;
				}
				else
				{
					numOfPath_Red++;
					pathIn_Red = NULL;
				}
			}
			else
			{ break; }
		}//while
	}// if mode 2


	while(1)
	{
		if( pathIn_Blue != NULL)
		{
			potrace_curve_t tempCurve = pathIn_Blue->curve;

			for( int k = 0 ; k < tempCurve.n ; k++ )
			{
				int tag = tempCurve.tag[k];

				if( tag == POTRACE_CURVETO )
				{
					numOfPonit_Blue += 10;
				}
				else if( tag == POTRACE_CORNER )
				{
					numOfPonit_Blue += 2;
				}
			}// for

				if( pathIn_Blue->next != NULL)
				{
					pathIn_Blue = pathIn_Blue->next;
					numOfPath_Blue++;
				}
				else
				{
					numOfPath_Blue++;
					pathIn_Blue = NULL;
				}

		}
		else
		{break;}
	}// while

	printf("path Red %d, point Red %d\n", numOfPath_Red, numOfPoint_Red );
	printf("path Blue %d, point Blue %d\n", numOfPath_Blue, numOfPonit_Blue );





	// reerve memory for point
	// create point strip
	// 
	if( pointStrip_Red_ptr != NULL)
	{
		free( pointStrip_Red_ptr );
		pointStrip_Red_ptr = NULL;
	}

	if( pointStrip_Blue_ptr != NULL)
	{
		free( pointStrip_Blue_ptr );
		pointStrip_Blue_ptr = NULL;
	}


	long memSize_Red = ((numOfPoint_Red * 2) + (numOfPath_Red * 2) + 2) * sizeof(double);
	long memSize_Blue = ((numOfPonit_Blue * 2) + (numOfPath_Blue * 2) + 2) * sizeof(double);


	pointStrip_Red_ptr = (double*)malloc( memSize_Red );
	pointStrip_Blue_ptr = (double*)malloc( memSize_Blue );
	memset( pointStrip_Red_ptr, 0, memSize_Red);
	memset( pointStrip_Blue_ptr, 0, memSize_Blue);



	// pointer copy
	double* pStrip_Red_copy = pointStrip_Red_ptr;
	double* pStrip_Blue_copy = pointStrip_Blue_ptr;


	// write endcode once, for when no paths are found
	double* tempRP = pStrip_Red_copy;
	*tempRP = -200.5;
	tempRP++;
	*tempRP = -200.5;

	double* tempBP = pStrip_Blue_copy;
	*tempBP = -200.5;
	tempBP++;
	*tempBP = -200.5;



	// make point strip
	if( mode == 2)
	{
		pathIn_Red = poState_Red_t->plist;

		while(1)
		{
			if( pathIn_Red != NULL)
			{
				potrace_curve_t* tempCurvePtr = &pathIn_Red->curve;
				int numPointInPath = 0;
				char sign = (char)pathIn_Red->sign;

				double kugiri_code;

				if( sign == 43 )//+
				{
					kugiri_code = -100.1;
				}
				else if( sign == 45 ) // -
				{
					kugiri_code = -150.1;
				}

				// set kugiri
				*pStrip_Red_copy = kugiri_code;
				pStrip_Red_copy++;
				double* numWrite_ptr = pStrip_Red_copy;
				pStrip_Red_copy++;


				for( int k = 0 ; k < tempCurvePtr->n ; k++ )
				{
					// get tag
					int tag = tempCurvePtr->tag[k];

					// get point
					int pIndex;
					if( k == 0 ){ pIndex = tempCurvePtr->n -1;}
					else{ pIndex = k-1;}

					potrace_dpoint_t headPoint = tempCurvePtr->c[pIndex][2];

					if( tag == POTRACE_CURVETO )
					{
						// add curvepoint
						this->setCurvePointFrom( headPoint, tempCurvePtr, &pStrip_Red_copy, k);
						numPointInPath += 10;
					}
					else if( tag == POTRACE_CORNER)
					{
						// add corner point
						this->setCornerPointFrom( headPoint, tempCurvePtr, &pStrip_Red_copy, k);
						numPointInPath += 2;
					}
				}// for k

				// go to next path
				pathIn_Red = pathIn_Red->next;

				// write num of point in path
				*numWrite_ptr = (double)numPointInPath + 0.5;

			}// if path in red != null
			else
			{
				// write end code
				*pStrip_Red_copy = -200.5;
				pStrip_Red_copy++;
				*pStrip_Red_copy = -200.5;

				break;
			}
		}// while
	}// if mode 2




	///// set pointstrip of blue
	pathIn_Blue = poState_Blue_t->plist;
	while(1)
	{
		if( pathIn_Blue != NULL )
		{
			potrace_curve_t* tempCurvePtr = &pathIn_Blue->curve;
			int numPointInPath = 0;
			int sign = pathIn_Blue->sign;

			double kugiri_code;

			if(sign == 43)//+
			{
				kugiri_code = -100.1;
			}
			else if( sign == 45 )//-
			{
				kugiri_code = -150.1;
			}

			// set kugiri
			*pStrip_Blue_copy = kugiri_code;
			pStrip_Blue_copy++;
			double* numWrite_Ptr = pStrip_Blue_copy;
			pStrip_Blue_copy++;


			for( int k = 0 ; k < tempCurvePtr->n ; k++ )
			{
				// get tag
				int tag = tempCurvePtr->tag[k];

				// get point
				int pIndex;
				if( k == 0 ){ pIndex = tempCurvePtr->n -1; }
				else{ pIndex = k-1; }

				potrace_dpoint_t headPoint = tempCurvePtr->c[pIndex][2];

				if( tag == POTRACE_CURVETO )
				{
					this->setCurvePointFrom( headPoint, tempCurvePtr, &pStrip_Blue_copy, k );
					numPointInPath += 10;
				}
				else if( tag == POTRACE_CORNER )
				{
					this->setCornerPointFrom( headPoint, tempCurvePtr, &pStrip_Blue_copy, k);
					numPointInPath += 2;
				}

			}// for k


			// set next path
			pathIn_Blue = pathIn_Blue->next;

			// set numof point in path
			*numWrite_Ptr = (double)numPointInPath + 0.5;

		}// is path != null
		else
		{
			// write end code
			*pStrip_Blue_copy = -200.5;
			pStrip_Blue_copy++;
			*pStrip_Blue_copy = -200.5;
			break;
		}


	}// while

	isProcessed = true;
}







/// Utility
void mainController::setCurvePointFrom(potrace_dpoint_t headPoint, potrace_curve_t* curvePtr, double** dpPtr, int idx )
{
	double hP[2];
	double uP[2];
	double wP[2];
	double eP[2];
	potrace_dpoint_t U = curvePtr->c[idx][0];
	potrace_dpoint_t W = curvePtr->c[idx][1];
	potrace_dpoint_t E = curvePtr->c[idx][2];

	hP[0] = headPoint.x;	hP[1] = headPoint.y;
	uP[0] = U.x;			uP[1] = U.y;
	wP[0] = W.x;			wP[1] = W.y;
	eP[0] = E.x;			eP[1] = E.y;

	for( int i = 0 ; i < 10 ; i++ )
	{
		double t = 0.1 * i;
		double A = (1.0-t)*(1.0-t)*(1.0-t);
		double B = 3.0*t*(1.0-t)*(1.0-t);
		double C = 3.0*t*t*(1.0-t);
		double D = t*t*t;

		double tP[2];
		tP[0] = A*hP[0] + B*uP[0] + C*wP[0] + D*eP[0];
		tP[1] = A*hP[1] + B*uP[1] + C*wP[1] + D*eP[1];

		*(*dpPtr) = tP[0];
		(*dpPtr)++;
		*(*dpPtr) = tP[1];
		(*dpPtr)++;
	}
}




void mainController::setCornerPointFrom(potrace_dpoint_t headPoint, potrace_curve_t* curvePtr, double** dpPtr, int idx )
{
	double hX = headPoint.x;
	double hY = headPoint.y;
	double nextX = curvePtr->c[idx][1].x;
	double nextY = curvePtr->c[idx][1].y;

	*(*dpPtr) = hX;
	(*dpPtr)++;

	*(*dpPtr) = hY;
	(*dpPtr)++;

	*(*dpPtr) = nextX;
	(*dpPtr)++;

	*(*dpPtr) = nextY;
	(*dpPtr)++;
}
