// ImageTracer.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "ImageTracer.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGETRACER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMAGETRACER));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMAGETRACER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	//wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = CreateSolidBrush( RGB(240, 240, 240) );
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_IMAGETRACER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;
   
   // GUI position
   int guiLeft = 760;
   int block2_Y = 75;
   int block3_Y = 310;
   // init
   srand( (unsigned int)time(NULL) );

#ifdef _DEBUG
   createLogConsole();
#endif

   mainController_obj = new mainController();

   // for using slider, call InitCommonControls();
   InitCommonControls();
  




   HFONT h_miniFont = CreateFont(13, // height (size?)
								0, // width
								0, 
								0,
								FW_NORMAL, // thickness
								FALSE, // italic
								FALSE, // underline
								0, 
								SHIFTJIS_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								DEFAULT_PITCH,
								NULL // font name 
								);

   HFONT h_microFont = CreateFont( 12,
								0,
								0,
								0,
								FW_NORMAL,
								FALSE,
								FALSE,
								0,
								SHIFTJIS_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								DEFAULT_PITCH,
								NULL
								);
   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass,
						szTitle,
						WS_OVERLAPPED | WS_SYSMENU,
						CW_USEDEFAULT, // pos x
						0,	// pos Y
						944	, //width
						552, // height
						NULL,  // HWND. handler of parent window
						NULL,	// HMENU, ID of menu or child window
						hInstance,
						NULL);

   // set hWnd to mainController 
   mainController_obj->h_mainWindow = hWnd;
   mainController_obj->h_Instance = hInstance;

   // get hdc
 



   // Add GUI interface
   
   // Load image Button
   mainController_obj->h_Button_loadimage = CreateWindow( TEXT("BUTTON"),
					TEXT("Load Image"),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					guiLeft+10,
					10,
					140,
					25,
					hWnd,
					(HMENU)1000,
					hInstance,
					NULL );
   SendMessage( mainController_obj->h_Button_loadimage, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));




   // slider
   mainController_obj->h_Slider_drill = CreateWindowEx(0,
										TRACKBAR_CLASS,
										TEXT("tempTrackbar"),
										WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
										guiLeft,
										block2_Y + 35,
										140,
										20,
										hWnd,
										(HMENU)5000,
										hInstance,
										NULL);

   mainController_obj->h_Slider_loop = CreateWindowEx(0,
										TRACKBAR_CLASS,
										TEXT("tempTrackbar2"),
										WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
										guiLeft,
										block2_Y + 75,
										140,
										20,
										hWnd,
										(HMENU)5001,
										hInstance,
										NULL);


      mainController_obj->h_Slider_milldepth = CreateWindowEx(0,
										TRACKBAR_CLASS,
										TEXT("tempTrackbar3"),
										WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
										guiLeft,
										block2_Y + 115,
										140,
										20,
										hWnd,
										(HMENU)5002,
										hInstance,
										NULL);

	     mainController_obj->h_Slider_cutdepth = CreateWindowEx(0,
										TRACKBAR_CLASS,
										TEXT("tempTrackbar4"),
										WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
										guiLeft,
										block2_Y + 155,
										140,
										20,
										hWnd,
										(HMENU)5003,
										hInstance,
										NULL);
		 mainController_obj->h_Slider_speed = CreateWindowEx(0,
										TRACKBAR_CLASS,
										TEXT("tempTrackbar5"),
										WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
										guiLeft,
										block2_Y + 195,
										140,
										20,
										hWnd,
										(HMENU)5004,
										hInstance,
										NULL);

     // 3 radio button
   mainController_obj->h_Radio_milling = CreateWindow( TEXT("BUTTON"),
				 TEXT("Circuit milling"),
				 WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTORADIOBUTTON ,
				 guiLeft+5,
				 block3_Y+20,
				 140,
				 20,
				 hWnd,
				 (HMENU)2000,
				 hInstance,
				 NULL);
   SendMessage( mainController_obj->h_Radio_milling, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));

   
   mainController_obj->h_Radio_cutout = CreateWindow( TEXT("BUTTON"),
					TEXT("Cutout"),
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
					guiLeft+5,
					block3_Y+40,
					140,
					20,
					hWnd,
					(HMENU)3000,
					hInstance,
					NULL);
   SendMessage( mainController_obj->h_Radio_cutout, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));

      mainController_obj->h_Radio_both = CreateWindow( TEXT("BUTTON"),
					TEXT("Both"),
					WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,
					guiLeft+5,
					block3_Y+60,
					140,
					20,
					hWnd,
					(HMENU)4000,
					hInstance,
					NULL);
   SendMessage( mainController_obj->h_Radio_both, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));

   // generate & save button
   mainController_obj->h_Button_generate =  CreateWindow( TEXT("BUTTON"),
					TEXT("Generate"),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					guiLeft+10,
					410,
					140,
					25,
					hWnd,
					(HMENU)6000,
					hInstance,
					NULL );
   SendMessage( mainController_obj->h_Button_generate, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));

      mainController_obj->h_Button_save =  CreateWindow( TEXT("BUTTON"),
					TEXT("Save"),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					guiLeft+10,
					440,
					140,
					25,
					hWnd,
					(HMENU)7000,
					hInstance,
					NULL );
   SendMessage( mainController_obj->h_Button_save, WM_SETFONT, (WPARAM)h_miniFont, MAKELPARAM(FALSE, 0));
		 


   // static text
   mainController_obj->h_Text_drill = CreateWindow( TEXT("STATIC"),
											TEXT("Drill diameter : 0.4mm"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y + 20,
											150,
											15,
											hWnd,
											(HMENU)8001,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_drill, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_loop = CreateWindow( TEXT("STATIC"),
											TEXT("Loops for milling : 1"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y + 60,
											150,
											15,
											hWnd,
											(HMENU)8002,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_loop, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_milldepth = CreateWindow( TEXT("STATIC"),
											TEXT("Milling depth : 0.2mm"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y + 100,
											150,
											15,
											hWnd,
											(HMENU)8003,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_milldepth, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_cutdepth = CreateWindow( TEXT("STATIC"),
											TEXT("Cutout depth : 1.6mm"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y+140,
											150,
											15,
											hWnd,
											(HMENU)8004,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_cutdepth, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_speed = CreateWindow( TEXT("STATIC"),
											TEXT("Speed : 8mm/s"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y+180,
											150,
											15,
											hWnd,
											(HMENU)8009,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_speed, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM( FALSE, 0) );

   // comment text
   mainController_obj->h_Text_imagesize = CreateWindow( TEXT("STATIC"),
											TEXT("Image 0mm x 0mm"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											40,
											150,
											15,
											hWnd,
											(HMENU)8005,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_imagesize, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_parameters = CreateWindow( TEXT("STATIC"),
											TEXT("Parameters"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block2_Y,
											150,
											15,
											hWnd,
											(HMENU)8006,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_parameters, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_generatefor = CreateWindow( TEXT("STATIC"),
											TEXT("Generate for"),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											block3_Y,
											150,
											15,
											hWnd,
											(HMENU)8007,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_generatefor, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );

   mainController_obj->h_Text_log =  CreateWindow( TEXT("STATIC"),
											TEXT(""),
											WS_CHILD | WS_VISIBLE | SS_LEFT,
											guiLeft+10,
											395,
											150,
											15,
											hWnd,
											(HMENU)8008,
											hInstance,
											NULL);
   SendMessage( mainController_obj->h_Text_log, WM_SETFONT, (WPARAM)h_microFont, MAKELPARAM(FALSE, 0) );
   
   
   // Delete font
   //DeleteObject( h_miniFont );
   mainController_obj->awake();

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);


		mainController_obj->messageFromGUI( message, wmId, wmEvent, lParam);
	switch (message)
	{
	case WM_HSCROLL:
		break;
	case WM_VSCROLL:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:


		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;

		//*************************************** message from GUI
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
