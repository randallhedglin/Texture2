// INCLUDES /////////////////////////////////////////////////////////////////

#include<afxwin.h>
#include<io.h>
#include<mmsystem.h>
#include<math.h>
#include"texture2.h"

// GLOBALS //////////////////////////////////////////////////////////////////

HWND hWnd;

// PROTOTYPES ///////////////////////////////////////////////////////////////

class CxFrameWnd:public CFrameWnd
	{
	public:
		CxFrameWnd();
		~CxFrameWnd();
	protected:
		CWindowDC* VideoDisplay;
		CDC* DoubleBuffer;
		CBitmap* DoubleBufferBmp;
		BOOL RedrawActive;
		UINT HorzRes,VertRes;
									    	    	    				
		void Initialize(void);
		void RenderScreen(void);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnRButtonDown(UINT nID,CPoint point);
		afx_msg void OnClose();
		DECLARE_MESSAGE_MAP();
	};

/////////////////////////////////////////////////////////////////////////////

class CxWinApp:public CWinApp
	{
	public:
		virtual BOOL InitInstance();
	};

// FUNCTIONS ////////////////////////////////////////////////////////////////

CxFrameWnd::CxFrameWnd()
	{
	const char* GameScreen;
	
	GameScreen=AfxRegisterWndClass(CS_BYTEALIGNWINDOW|
								   CS_BYTEALIGNCLIENT|
								   CS_CLASSDC,
	   							   LoadCursor(AfxGetInstanceHandle(),
	   							   			  IDC_ARROW),
								   (HBRUSH)GetStockObject(BLACK_BRUSH),
								   LoadIcon(AfxGetInstanceHandle(),
								   			  "ProgramIcon"));
	Create(GameScreen,
		   "Windows Video Game",
		   WS_POPUP,
		   rectDefault);
	
	hWnd=m_hWnd;
	
	if(!(SetTimer(TMR_INITIALIZE,100,NULL)))
		{
		MessageBox("Timer resource not available.\nClose some open applications or reboot.",
				   "Fatal Error - Aborting",
				   MB_OK);
		SendMessage(WM_CLOSE);
		}		
	}
        	
CxFrameWnd::~CxFrameWnd()
	{
	KillTimer(TMR_MAINLOOP);
	KillTimer(TMR_FPSCOUNT);
	
	VideoDisplay->SelectStockObject(NULL_PEN);
	delete VideoDisplay;
	
	DoubleBuffer->SelectStockObject(NULL_PEN);
	delete DoubleBuffer;

	DoubleBufferBmp->DeleteObject();
	delete DoubleBufferBmp;
	}

void CxFrameWnd::Initialize(void)		
	{
	//srand(UINT(timeGetTime()));
	
	VideoDisplay=new CWindowDC(this);
	HorzRes=VideoDisplay->GetDeviceCaps(HORZRES);
	VertRes=VideoDisplay->GetDeviceCaps(VERTRES);
	
	DoubleBuffer=new CDC;             
	DoubleBuffer->CreateCompatibleDC(VideoDisplay);
	
	DoubleBufferBmp=new CBitmap;
	DoubleBufferBmp->CreateCompatibleBitmap(VideoDisplay,HorzRes,VertRes);
	DoubleBuffer->SelectObject(DoubleBufferBmp);
    DoubleBuffer->BitBlt(0,0,HorzRes,VertRes,NULL,0,0,BLACKNESS);
    }

void CxFrameWnd::RenderScreen(void)
	{

	}

void CxFrameWnd::OnTimer(UINT nIDEvent)
	{
	static UINT frame=0;
	static UINT fps=0;
	char s[255];
					
	switch(nIDEvent)
		{
		case(TMR_INITIALIZE):
			KillTimer(TMR_INITIALIZE);
			Initialize();
			RedrawActive=FALSE;
			if(!(SetTimer(TMR_MAINLOOP,1000/DESFPS,NULL)))
				{
				MessageBox("Timer resource not available.\nClose some open applications or reboot.",
						   "Fatal Error - Aborting",
						   MB_OK);
				SendMessage(WM_CLOSE);
				}		
			if(!(SetTimer(TMR_FPSCOUNT,1000,NULL)))
				{
				MessageBox("Timer resource not available.\nClose some open applications or reboot.",
						   "Fatal Error - Aborting",
						   MB_OK);
				SendMessage(WM_CLOSE);
				}		
			break;
		case(TMR_MAINLOOP):
			if(RedrawActive)
				return;
			RedrawActive=TRUE;
			RenderScreen();
		    VideoDisplay->BitBlt(0,0,HorzRes,VertRes,DoubleBuffer,0,0,SRCCOPY);
		    sprintf(s,"%d",fps);
			VideoDisplay->TextOut(0,0,s,2);
	        frame++;
		    RedrawActive=FALSE;
		    break;
		case(TMR_FPSCOUNT):
			fps=frame;
			frame=1;
			break;
		}
	}

void CxFrameWnd::OnRButtonDown(UINT nID,CPoint point)
	{
	SendMessage(WM_CLOSE);
	}

void CxFrameWnd::OnClose()
	{
	if(MessageBox("Are you sure you want to quit?",
				  "Query",
				  MB_YESNO|MB_ICONQUESTION)==IDYES)
		DestroyWindow();
	}

BEGIN_MESSAGE_MAP(CxFrameWnd,CFrameWnd)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CxWinApp::InitInstance()
	{
	m_pMainWnd=new CxFrameWnd();
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED|m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
	}

// MAIN /////////////////////////////////////////////////////////////////////

CxWinApp Texture2;
