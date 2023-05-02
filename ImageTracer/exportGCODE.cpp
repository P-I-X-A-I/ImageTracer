#include "stdafx.h"
#include "mainController.h"


void mainController::exportGCODE()
{
	double FEEDRATE = (S_SPEED * 2 + 4)*10.0;

	//************************************
	printf("export G-CODE\n");
	//************************************

	double* redPoint_ptr = offset_Red_ptr;
	double* bluePoint_ptr = offset_Blue_ptr;

	double val[2];
	bool isFirstPoint = true;
	long LINE_NUMBER = 0;

	double p_to_mm = (25.4 / BMP_dpi_w );

	//************************************
	printf("save G-CODE, point to mm %f", p_to_mm );
	//************************************



	// erase all string
	//************************************
	printf("erase string\n");
	//************************************

	G_CODE_string.erase( G_CODE_string.begin(), G_CODE_string.end() );
	G_CODE_string.reserve(20000000);//20MB


	// add Head of G-code
	//************************************
	printf("append head g-code\n");
	//************************************
	char head[] = "G92 X5.0 Y5.0 Z5.0\nG21\n";
	G_CODE_string.append( head );


	double* target_ptr = bluePoint_ptr;

	// mill depth & cut depth
	int iter;
	double tempDepth = 0.2;

	double box_w = (double)BMP_imgWidth;
	double box_h = (double)BMP_imgHeight;

	if( mode == 0 || mode == 2 )
	{
		iter = S_MILL_DEPTH;
		tempDepth = 0.2;
	}
	else if( mode == 1 )
	{
		iter = S_CUT_DEPTH;
	}


	//************************************
	printf("create blue path g-code\n");
	//************************************
	for( int N = 0 ; N < iter ; N++ )
	{
		target_ptr = bluePoint_ptr;

		while (1)
		{
			val[0] = *target_ptr; target_ptr++;
			val[1] = *target_ptr; target_ptr++;

			if( val[0] < -199.0 )// end code
			{
				break;
			}
			else if( val[0] < -99.0 && val[0] > -101.0 )// kugiri code
			{
				isFirstPoint = true;

				this->setFeedRate( 500.0, &LINE_NUMBER, &G_CODE_string );
				this->liftUp_Z( &LINE_NUMBER, &G_CODE_string );
			}
			else
			{
				if ((val[0] >= -10.0 && val[0] <= 20.0) ||
					(val[0] >= box_w - 20.0 && val[0] <= box_w + 20.0) ||
					(val[1] >= -10.0 && val[1] < 20.0) ||
					(val[1] >= box_h - 20.0 && val[1] <= box_h + 20.0)
					)
				{
					printf("outer\n");
				}
				else
				{
					if (isFirstPoint == true)
					{
						// mode to first point
						this->setFeedRate(500.0, &LINE_NUMBER, &G_CODE_string);
						this->moveToPoint(val, &p_to_mm, &LINE_NUMBER, &G_CODE_string);

						//lift down z
						this->setFeedRate(FEEDRATE, &LINE_NUMBER, &G_CODE_string);
						this->liftDowns_Z(5.0 + tempDepth * (N + 1), &LINE_NUMBER, &G_CODE_string);

						isFirstPoint = false;
					}
					else
					{
						this->moveToPoint(val, &p_to_mm, &LINE_NUMBER, &G_CODE_string);
					}
				}
			}
		}//while
	}//for N


	
	//************************************
	printf("create red path g-code\n");
	//************************************
	// both mode
	if( mode == 2)
	{
		iter = S_CUT_DEPTH;
		tempDepth = 0.4;

		for( int N = 0 ; N < iter ; N++ )
		{
			target_ptr = redPoint_ptr;

			while(1)
			{
				val[0] = *target_ptr; target_ptr++;
				val[1] = *target_ptr; target_ptr++;

				if( val[0] < -199.0 )// end code
				{
					break;
				}
				else if( val[0] < -99.0 && val[0] > -101.0 )// kugiri
				{
					isFirstPoint = true;

					// lift up z
					this->setFeedRate( 500.0, &LINE_NUMBER, &G_CODE_string );
					this->liftUp_Z( &LINE_NUMBER, &G_CODE_string );
				}
				else
				{
					if ((val[0] >= -10.0 && val[0] <= 20.0) ||
						(val[0] >= box_w - 20.0 && val[0] <= box_w + 20.0) ||
						(val[1] >= -10.0 && val[1] < 20.0) ||
						(val[1] >= box_h - 20.0 && val[1] <= box_h + 20.0)
						)
					{
						printf("outer\n");
					}
					else
					{
						if (isFirstPoint == true)
						{
							// mode to first point
							this->setFeedRate(500.0, &LINE_NUMBER, &G_CODE_string);
							this->moveToPoint(val, &p_to_mm, &LINE_NUMBER, &G_CODE_string);

							// lift down z
							this->setFeedRate(FEEDRATE, &LINE_NUMBER, &G_CODE_string);
							this->liftDowns_Z(5.0 + tempDepth * (N + 1), &LINE_NUMBER, &G_CODE_string);

							isFirstPoint = false;
						}
						else
						{
							this->moveToPoint(val, &p_to_mm, &LINE_NUMBER, &G_CODE_string);
						}
					}
				}
			}// while
		}// for N

	}// if mode 2



	// lift up z
	this->setFeedRate( 500.0, &LINE_NUMBER, &G_CODE_string );
	this->liftUp_Z( &LINE_NUMBER, &G_CODE_string );


	// return to home
	G_CODE_string.append( "G28 Z0\n" );
	G_CODE_string.append( "G28 X0 Y0\n" );
	//printf("%s", G_CODE_string.c_str());



	// save Gcode file
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];
	TCHAR defaultDirectory[MAX_PATH];

	defaultDirectory[0] = '\0';
	GetCurrentDirectory( MAX_PATH, defaultDirectory );

	// init ofn and load bitmap
	memset( &ofn, 0, sizeof(OPENFILENAME));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = h_mainWindow;
	ofn.lpstrInitialDir = defaultDirectory;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = TEXT("gcode");
	ofn.lpstrFilter = TEXT("G-CODE file( *.gcode)\0");
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if( !GetSaveFileName(&ofn) )
	{
		printf("canceled\n");
	}
	else
	{
		std::ofstream ofs;
		ofs.open( szFile );
		ofs << G_CODE_string.c_str() << std::endl;
		ofs.close();
	}
}	





int mainController::calcurateChecksum(char* cPtr)
{
	int cs = 0;
	char* cPtr_copy = cPtr;

	while(1)
	{
		char tempChar = *cPtr_copy;
		cPtr_copy++;

		if( tempChar == '\0' || tempChar == '*' )
		{
			break;
		}

		cs = cs ^ tempChar;
	}

	return cs;
}

void mainController::liftUp_Z( long* LineNum, std::string* cppString )
{

	char* tempChar_ptr = (char*)malloc( MEM_SIZE );
	memset( tempChar_ptr, '\n', MEM_SIZE );
	sprintf_s( tempChar_ptr, MEM_SIZE, "G1 Z1.0\n" );
	cppString->append( tempChar_ptr );

	free( tempChar_ptr );

	*LineNum += 1;
}


void mainController::liftDowns_Z( double depth, long* LineNum, std::string* cppString )
{

	char* tempChar_ptr = (char*)malloc( MEM_SIZE );
	memset( tempChar_ptr, '\n', MEM_SIZE );
	sprintf_s( tempChar_ptr, MEM_SIZE, "G1 Z%1.4f\n", depth );

	cppString->append( tempChar_ptr );

	free( tempChar_ptr );

	*LineNum += 1;
}

void mainController::moveToPoint( double* point_ptr, double* scalePtr, long* LineNum, std::string* cppString )
{

	double* ptr_copy = point_ptr;
	double pX = *ptr_copy; ptr_copy++;
	double pY = *ptr_copy;

	pX *= (*scalePtr);
	pY *= (*scalePtr);

	char* tempChar_ptr = (char*)malloc( MEM_SIZE );
	memset( tempChar_ptr, '\n', MEM_SIZE );
	sprintf_s( tempChar_ptr, MEM_SIZE, "G1 X%1.5f Y%1.5f\n", pX, pY );

	cppString->append( tempChar_ptr );

	free( tempChar_ptr );

	*LineNum += 1;
}

void mainController::setFeedRate( double feedRate, long* LineNum, std::string* cppString )
{

	char* tempChar_ptr = (char*)malloc( MEM_SIZE );
	memset( tempChar_ptr, '\n', MEM_SIZE );
	sprintf_s( tempChar_ptr, MEM_SIZE, "G1 F%1.4f\n", feedRate );
	cppString->append( tempChar_ptr );

	free( tempChar_ptr );

	*LineNum += 1;
}