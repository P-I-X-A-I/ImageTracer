#pragma once

#include "potracelib.h"
#include "clipper.hpp"
#include <fstream>

#define MAX_IMAGE_SIZE 10000
#define MEM_SIZE 200

#define VIEW_L_ORIGIN_X 0
#define VIEW_L_ORIGIN_Y 0
#define VIEW_L_WIDTH 360
#define VIEW_L_HEIGHT 550

class mainController
{
public:
	//hinstance
	HINSTANCE h_Instance;

	//
	HWND h_mainWindow;


	// GUI handler
	HWND h_Button_loadimage;
	HWND h_Radio_milling;
	HWND h_Radio_cutout;
	HWND h_Radio_both;
	HWND h_Slider_drill;
	HWND h_Slider_loop;
	HWND h_Slider_milldepth;
	HWND h_Slider_cutdepth;
	HWND h_Slider_speed;
	HWND h_Button_generate;
	HWND h_Button_save;

	// static text
	HWND h_Text_drill;
	HWND h_Text_loop;
	HWND h_Text_milldepth;
	HWND h_Text_cutdepth;
	HWND h_Text_speed;
	HWND h_Text_imagesize;
	HWND h_Text_parameters;
	HWND h_Text_generatefor;
	HWND h_Text_log;

	// bitmap
	HBITMAP h_loadBitmap;
	HANDLE fh_BMP;
	int BMP_dataOffset;
	int BMP_imgWidth;
	int BMP_imgHeight;
	int BMP_bpp;
	double BMP_dpi_w;
	double BMP_dpi_h;

	unsigned char* BMP_dataPtr;
	unsigned char* pixBin_Red;
	unsigned char* pixBin_Blue;

	HDC h_memoryDC;


	// variables
	int mode;
	int S_DRILL;
	int S_LOOPS;
	int S_MILL_DEPTH;
	int S_CUT_DEPTH;
	int S_SPEED;
	bool isProcessed;

	// potrace
	potrace_word* poBitmap_Red_Ptr;
	potrace_word* poBitmap_Blue_Ptr;
	potrace_bitmap_t poBitmap_Red_t;
	potrace_bitmap_t poBitmap_Blue_t;
	potrace_param_t* poParam_Red_t;
	potrace_param_t* poParam_Blue_t;
	potrace_state_t* poState_Red_t;
	potrace_state_t* poState_Blue_t;


	// point
	double* pointStrip_Red_ptr;
	double* pointStrip_Blue_ptr;
	double* offset_Red_ptr;
	double* offset_Blue_ptr;


	// clipper
	std::vector<ClipperLib::IntPoint> offset_Red_Vector;
	std::vector<ClipperLib::IntPoint> offset_Blue_Vector;


	// G code
	std::string G_CODE_string;




	// method
	mainController(void);
	~mainController(void);

	void awake();
	void showAlert( LPCTSTR title, LPCTSTR sub);
	void showLog( double percentage );
	void deleteLog();

	//GUI
	void messageFromGUI(UINT mes, int id, int eventCode, LPARAM lParam );
	void loadImage();

	void radioButton(int num);

	// BMP
	void getBitmapInformation();

	// Generate
	void checkBMP_andMakeRGBArray();
	void makeBinaryArray();

	// Potrace
	void makePotraceBitmap( int padW );
	void doTracing();

	// clipper
	bool doOffset();


	// draw path
	void drawPathInWindow();


	// export G-CODE
	void exportGCODE();
	int calcurateChecksum(char* cPtr);
	void liftUp_Z( long* LineNum, std::string* cppString );
	void liftDowns_Z( double depth, long* LineNum, std::string* cppString );
	void moveToPoint( double* point_ptr, double* scalePtr, long* LineNum, std::string* cppString );
	void setFeedRate( double feedRate, long* LineNum, std::string* cppString );


	void setCurvePointFrom(potrace_dpoint_t headPoint, potrace_curve_t* curvePtr, double** dpPtr, int idx );
	void setCornerPointFrom(potrace_dpoint_t headPoint, potrace_curve_t* curvePtr, double** dpPtr, int idx );

};

