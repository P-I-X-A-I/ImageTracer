#include "stdafx.h"
#include "mainController.h"


bool mainController::doOffset()
{
	printf("do offset \n");

	// clipper variables
	ClipperLib::Path subj;
	ClipperLib::Paths subj_Paths;
	ClipperLib::Paths solution;
	std::vector<ClipperLib::Paths> Paths_Vector;
	ClipperLib::ClipperOffset offset_obj;


	// offset value
	double actDiameter = S_DRILL * 0.1;
	double offsetDelta = (BMP_dpi_w / 25.4 ) * actDiameter;

	double* redPtr_copy = pointStrip_Red_ptr;
	double* bluePtr_copy = pointStrip_Blue_ptr;
	long tempVal[2];

	bool isFirst_Kugiri = true;


	// red point
	while (1)
	{
		tempVal[0] = (long)(*redPtr_copy); redPtr_copy++;
		tempVal[1] = (long)(*redPtr_copy); redPtr_copy++;


		if( tempVal[0] == -200 || tempVal[1] == -200 )// end code
		{
			if( isFirst_Kugiri == false )
			{
				// add subj to subj_path
				subj_Paths.push_back( subj );

				// add Paths to offset object
				offset_obj.AddPaths( subj_Paths, ClipperLib::jtRound, ClipperLib::etClosedPolygon );

				// do offset
				offset_obj.Execute( solution, offsetDelta*0.5);

				// push result to vector
				Paths_Vector.push_back(solution);

				// clear
				offset_obj.Clear();
				subj_Paths.clear();
				subj.clear();
				solution.clear();
			}
			break;
		}// if end code
		else if( tempVal[0] == -100 || tempVal[0] == -150 )// kugiri code
		{
			if( isFirst_Kugiri == true )
			{
				isFirst_Kugiri = false;
			}
			else
			{
				subj_Paths.push_back(subj);
				subj.clear();
			}
		}// if kugiri
		else // add point
		{
			subj << ClipperLib::IntPoint( (long)tempVal[0], (long)tempVal[1] );
		}// add point

	}// while



	// access to point
	long numSolution = Paths_Vector.size();

	// clear point and add kugiri
	offset_Red_Vector.clear();
	offset_Red_Vector.push_back( ClipperLib::IntPoint(-100, -100));

	for( int i = 0 ; i < numSolution ; i++ )
	{
		
		ClipperLib::Paths tempPaths = Paths_Vector[i];
		long numPath = tempPaths.size(); // this value is usually 1,

		for( int j = 0 ; j < numPath ; j++ )
		{
			ClipperLib::Path eachPath = tempPaths[j];
			long numOf_PathPoint = eachPath.size();


			for( int k = 0 ; k < numOf_PathPoint ; k++ )
			{
				//add point
				offset_Red_Vector.push_back( eachPath[k] );
			}// for k

			// add first point of closed path
			offset_Red_Vector.push_back( eachPath[0] );

			// add kugiri point
			offset_Red_Vector.push_back( ClipperLib::IntPoint(-100, -100) );


		}// for j
	}// for i


	// add End code
	offset_Red_Vector.push_back( ClipperLib::IntPoint(-200, -200) );



	// blue**************************************************

	subj_Paths.clear();
	subj.clear();
	solution.clear();
	Paths_Vector.clear();
	offset_obj.Clear();


	// reset pointer for loop
	bluePtr_copy = pointStrip_Blue_ptr;
	isFirst_Kugiri = true;


	while(1)
	{
		tempVal[0] = (long)(*bluePtr_copy); bluePtr_copy++;
		tempVal[1] = (long)(*bluePtr_copy); bluePtr_copy++;

		if( tempVal[0] == -200 || tempVal[1] == -200 )
		{
			if( isFirst_Kugiri == false )
			{
				// add subj to subj_path
				subj_Paths.push_back( subj );

				//add path to offset object
				offset_obj.AddPaths( subj_Paths, ClipperLib::jtRound, ClipperLib::etClosedPolygon );

				// do offset
				offset_obj.Execute( solution, offsetDelta*0.5);

				// add path to vector
				Paths_Vector.push_back( solution );

				// clear path
				offset_obj.Clear();
				subj.clear();
				solution.clear();
			}
			break;
		}// if -200
		else if( tempVal[0] == -100 || tempVal[0] == -150 ) // kugiri
		{
			if( isFirst_Kugiri == true )
			{
				isFirst_Kugiri = false;
			}
			else
			{
				subj_Paths.push_back( subj );
				subj.clear();
			}
		}
		else
		{
			subj << ClipperLib::IntPoint( (long)tempVal[0], (long)tempVal[1] );
		}
	}//while



	// loop
	ClipperLib::Paths finalPaths;
	ClipperLib::Paths tempPaths;
	ClipperLib::Paths tempSolution;

	// copy paths_vector to temp paths
	for( int i = 0 ; i < Paths_Vector.size() ; i++ )
	{
		ClipperLib::Paths eachPaths = Paths_Vector[i];

		for( int j = 0 ; j < eachPaths.size() ; j++ )
		{
			ClipperLib::Path temp = eachPaths[j];

			tempPaths.push_back( temp );
			finalPaths.push_back( temp );
		}
	}//



	int LOOPCOUNT;

	if( mode == 0 || mode == 2 )
	{
		LOOPCOUNT = S_LOOPS;
	}
	else
	{
		LOOPCOUNT = 0;
	}


	for( int i = 1 ; i < LOOPCOUNT ; i++ )
	{
		offset_obj.Clear();
		offset_obj.AddPaths( tempPaths, ClipperLib::jtRound, ClipperLib::etClosedPolygon );
		offset_obj.Execute( tempSolution, offsetDelta*0.8 );

		// add path to final path
		tempPaths.clear();
		for( int j = 0 ; j < tempSolution.size() ; j++ )
		{
			finalPaths.push_back( tempSolution[j] );
			tempPaths.push_back( tempSolution[j] );
		}
	}





	// access to point
	// clear point adn add kugiri
	offset_Blue_Vector.clear();
	offset_Blue_Vector.push_back( ClipperLib::IntPoint(-100, -100 ) );

	long numOfBluePath = finalPaths.size();

	for( int i = 0 ; i < numOfBluePath ; i++ )
	{
		ClipperLib::Path eachPath = finalPaths[i];

		for( int j = 0 ; j < eachPath.size() ; j++ )
		{
			offset_Blue_Vector.push_back(eachPath[j] );
		}

		// add first point of closed path
		offset_Blue_Vector.push_back( eachPath[0] );

		// add kugiri
		offset_Blue_Vector.push_back( ClipperLib::IntPoint(-100,-100));
	}

	// add End code
	offset_Blue_Vector.push_back( ClipperLib::IntPoint(-200, -200));



	//make offset pointer strip to memory
	if( offset_Red_ptr != NULL )
	{
		free( offset_Red_ptr );
		offset_Red_ptr = NULL;
	}

	if( offset_Blue_ptr != NULL )
	{
		free( offset_Blue_ptr );
		offset_Blue_ptr = NULL;
	}





	long numByte_RedVector = offset_Red_Vector.size() * 2 * sizeof(double) +1;
	long numByte_BlueVector = offset_Blue_Vector.size() * 2 * sizeof(double) +1;
	offset_Red_ptr = (double*)malloc( numByte_RedVector );
	offset_Blue_ptr = (double*)malloc( numByte_BlueVector );
	memset( offset_Red_ptr, 0, numByte_RedVector );
	memset( offset_Blue_ptr, 0, numByte_BlueVector );

	double* offRed_Copy = offset_Red_ptr;
	double* offBlue_Copy = offset_Blue_ptr;


	for( int i = 0 ; i < offset_Red_Vector.size() ; i++ )
	{
		*offRed_Copy = (double)offset_Red_Vector[i].X; offRed_Copy++;
		*offRed_Copy = (double)offset_Red_Vector[i].Y; offRed_Copy++;
	}


	for( int i = 0 ; i < offset_Blue_Vector.size() ; i++ )
	{
		*offBlue_Copy = (double)offset_Blue_Vector[i].X; offBlue_Copy++;
		*offBlue_Copy = (double)offset_Blue_Vector[i].Y; offBlue_Copy++;
	}



	return true;
}