
// SoC ProjectView.cpp : CSoCProjectView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SoC Project.h"
#endif

#include "SoC ProjectView.h"
#include "MainFrm.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CLIP(x) (((x) <0)?0:(((x)>255)?255:(x)))
#define ISINSIDE(x,y) (x < IMG_WIDTH && y < IMG_HEIGHT && x >=0 && y >= 0) ? true : false
#define RED		0
#define GREEN	1
#define BLUE	2

// CSoCProjectView

IMPLEMENT_DYNCREATE(CSoCProjectView, CScrollView)

	BEGIN_MESSAGE_MAP(CSoCProjectView, CScrollView)
		// 표준 인쇄 명령입니다.
		ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
		ON_COMMAND(ID_INVERSE, &CSoCProjectView::OnInverse)
		ON_WM_PAINT()
		ON_COMMAND(ID_ORG, &CSoCProjectView::OnButton_ORG)
		ON_COMMAND(ID_GRAY, &CSoCProjectView::OnButton_GRAY)
		ON_COMMAND(ID_MIN, &CSoCProjectView::OnButton_MINUS)
		ON_COMMAND(ID_BIN, &CSoCProjectView::OnButton_BINARY)
		ON_WM_ERASEBKGND()
		ON_COMMAND(ID_LABEL, &CSoCProjectView::OnButton_Label)
	END_MESSAGE_MAP()
	CString CSoCProjectView::statSTR;
	int CSoCProjectView::grayRate;
	int CSoCProjectView::m_FPS;
	int CSoCProjectView::m_FrameCNT;
	CSoCProjectView::IMG_STATE CSoCProjectView::m_IMGState;
	HBITMAP CSoCProjectView::m_IMGExample[9];
	int CSoCProjectView::m_SelectedBTM;
	CString CSoCProjectView::m_BTMInfo[9];
	int CSoCProjectView::m_MatchRate;
	// CSoCProjectView 생성/소멸
	CSoCProjectView::CSoCProjectView()
	{
		//this->m_bProceed = false;
		//m_bProceed = false;
		grayRate = 10;
		m_FPS = 0;
		m_FrameCNT = 0;
		m_IMGState = ORIGINAL;
		m_SelectedBTM = 0;
		m_MatchRate = 0;
		m_IMGExample[0] = (HBITMAP)::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BLANK));
		m_IMGExample[1] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_OZ));
		m_IMGExample[2] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_GJKK));
		m_IMGExample[3] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_DMSODA));
		m_IMGExample[4] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_SWKK));
		m_IMGExample[5] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_R));
		m_IMGExample[6] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_G));
		m_IMGExample[7] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_B));
		m_IMGExample[8] = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_RG));
		m_BTMInfo[0] = "탐색중...";
		m_BTMInfo[1] = "오징어땅콩(98g)";
		m_BTMInfo[2] = "감자깡(75g)";
		m_BTMInfo[3] = "데미소다 포도(250ml)";
		m_BTMInfo[4] = "새우깡(90g)";
		m_BTMInfo[5] = "RGB(255,0,0)";
		m_BTMInfo[6] = "RGB(0,255,0)";
		m_BTMInfo[7] = "RGB(0,0,255)";
		m_BTMInfo[8] = "RGB(255,255,0)";
	}

	CSoCProjectView::~CSoCProjectView()
	{
	}

	BOOL CSoCProjectView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: CREATESTRUCT cs를 수정하여 여기에서
		//  Window 클래스 또는 스타일을 수정합니다.

		return CScrollView::PreCreateWindow(cs);
	}

	// CSoCProjectView 그리기

	void CSoCProjectView::OnDraw(CDC* pDC)
	{
		CSoCProjectDoc*pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		ClearRect(pDC, RGB(255,255,255));

		// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
		CAPTUREPARMS cp;
 
		capCaptureGetSetup(pDoc->m_hCamWnd, &cp, sizeof(cp) ); // get the current defaults      

		cp.dwRequestMicroSecPerFrame = 1;          // Set desired frame rate     
		cp.fMakeUserHitOKToCapture   = FALSE;
		cp.fYield                    = TRUE;       // we want capture on a background thread.
		cp.wNumVideoRequested        = (WORD) 1;   // we may get less than this - no problem
		cp.fCaptureAudio             = FALSE;     
		cp.vKeyAbort                 = 0;          // If no key is provided, it won't stop...
		cp.fAbortLeftMouse           = FALSE;
		cp.fAbortRightMouse          = FALSE;
		cp.fLimitEnabled             = FALSE;      // we want to stop     
		cp.fMCIControl               = FALSE;

		capCaptureSetSetup(pDoc->m_hCamWnd, &cp, sizeof(cp) );
	}

	void CSoCProjectView::DrawBitmap(CDC* pDC,int x,int y,HBITMAP hBit)
	{
		HDC hdc = pDC->GetSafeHdc();
		HDC MemDC;
		HBITMAP OldBitmap;
		int bx,by;
		BITMAP bit;

		MemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

		GetObject(hBit,sizeof(BITMAP),&bit);
		bx=bit.bmWidth;
		by=bit.bmHeight;

		BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
	}

	void CSoCProjectView::DrawPixels(CDC* pDC,int x,int y,HBITMAP hBit, BYTE* pixels)
	{
		HDC hdc = pDC->GetSafeHdc();
		HDC MemDC;
		HBITMAP OldBitmap;
		int bx,by;
		BITMAP bit;

		MemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

		GetObject(hBit,sizeof(BITMAP),&bit);
		bx=bit.bmWidth;
		by=bit.bmHeight;
		
		for(int i = 0, jump = 0; i < by; i++)
		{
			for(int j = 0; j < bx; j++, jump += 2)
			{
				int r = pixels[(i * bx) + j + jump + RED],
					g = pixels[(i * bx) + j + jump + GREEN],
					b = pixels[(i * bx) + j + jump + BLUE];
				SetPixel(MemDC,j,by-i-1,RGB(r,g,b));
			}
		}

		BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
	}

	void CSoCProjectView::DrawTextDBF(CDC* pDC,int x,int y, CString str)
	{
		HDC hDC = pDC->GetSafeHdc();
		HDC MemDC;
		HBITMAP OldBitmap;
		RECT rect;
		
		CWnd *pWnd = AfxGetMainWnd();
		HWND hWnd = pWnd->m_hWnd;
		::GetClientRect(hWnd, &rect);

		MemDC = CreateCompatibleDC(hDC);
		OldBitmap = CreateCompatibleBitmap(MemDC, rect.right, rect.bottom);
		OldBitmap = (HBITMAP)SelectObject(MemDC, OldBitmap);

		BitBlt(MemDC, 0,0, rect.right, rect.bottom, hDC, 0,0, SRCCOPY);
		TextOut(MemDC, x, y, str, lstrlen(str));

		BitBlt(hDC,0,0,rect.right,rect.bottom,MemDC,0,0,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
	}

	void CSoCProjectView::ClearRect(CDC *pDC, COLORREF color)
	{
		HDC hdc = pDC->GetSafeHdc();
		HDC MemDC;
		HBITMAP OldBitmap;
		RECT rect;

		CWnd *pWnd = AfxGetMainWnd();
		HWND hWnd = pWnd->m_hWnd;
		::GetClientRect(hWnd, &rect);

		MemDC = CreateCompatibleDC(hdc);
		OldBitmap = CreateCompatibleBitmap(MemDC, rect.right, rect.bottom);
		OldBitmap = (HBITMAP)SelectObject(MemDC, OldBitmap);

		HBRUSH brush = CreateSolidBrush(color);
		FillRect(MemDC, &rect, brush);
		BitBlt(hdc,0,0,rect.right,rect.bottom,MemDC,0,0,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
	}


	void CSoCProjectView::OnInitialUpdate()
	{
		CScrollView::OnInitialUpdate();

		CSize sizeTotal;
		// TODO: 이 뷰의 전체 크기를 계산합니다.
		sizeTotal.cx = 640;
		sizeTotal.cy = 480;
		SetScrollSizes(MM_TEXT, sizeTotal);

		CSoCProjectDoc*pDoc = GetDocument();
		// TODO: 여기에 생성 코드를 추가합니다.
		RECT r;
		GetClientRect(&r);
		pDoc->m_hCamWnd = capCreateCaptureWindow(
			_T("Capture Window"), WS_CHILD | WS_VISIBLE, 5, 5, r.right-5, r.bottom-5, this->m_hWnd, NULL);

		// 설치된 디바이스를 순서로 0 ~ 9까지의 카메라를 지정 할 수 있다.
		if(!capDriverConnect(pDoc->m_hCamWnd, 0)) AfxMessageBox(_T("웹캠 인식 실패 ㅠㅠ"));

		// 현재 드라이버 정보에 관한 내용 얻어오기
		capDriverGetCaps(pDoc->m_hCamWnd, &pDoc->m_psCapsInfo, sizeof(pDoc->m_psCapsInfo));

		//비디오 포맷 변환을 지원하는지 확인한다.
		if(pDoc->m_psCapsInfo.fHasDlgVideoFormat) {
			// 비디오 포맷 변환을 지원하면 아래 함수를 호출한다.
			// 호출되는 함수는 새로운 다이얼로그를 호출하고 해상도와 포맷형식, 프레임 버퍼크기등을 지정할 수 있다.
			// 이때, 지원되지 않는 비디오포멧을 설정하면 검정 화면을 보게될 것이야...
			capDlgVideoFormat(pDoc->m_hCamWnd);
		}

		// m_psCapsInfo.fHasOverlay에서 overlay가 지원이 되지 않으면(=0) 사용 할 수 없다.
		if(pDoc->m_psCapsInfo.fHasOverlay) {
			// 하드웨어 오버레이는 시스템 부하를 줄여준다.(optioinal)
			capOverlay(pDoc->m_hCamWnd, FALSE);
		}

		// BITMAPINFO 설정
		capGetVideoFormat(pDoc->m_hCamWnd, &pDoc->m_BTMInfo, sizeof(pDoc->m_BTMInfo));

		// 1/1000 단위로 영상이 출력된다.
		capPreviewRate(pDoc->m_hCamWnd, 1);

		// 프리뷰 영상을 재생한다.
		capPreview(pDoc->m_hCamWnd, TRUE);

		CAPTUREPARMS cp;     
	
		capCaptureGetSetup(pDoc->m_hCamWnd, &cp, sizeof(cp) );	// get the current defaults      
	
		cp.dwRequestMicroSecPerFrame = 1;					// Set desired frame rate     
		cp.fMakeUserHitOKToCapture   = FALSE;
		cp.fYield                    = TRUE;                // we want capture on a background thread.
		cp.wNumVideoRequested        = (WORD) 1;			// we may get less than this - no problem
		cp.fCaptureAudio             = FALSE;     
		cp.vKeyAbort                 = 0;                   // If no key is provided, it won't stop...
		cp.fAbortLeftMouse           = FALSE;
		cp.fAbortRightMouse          = FALSE;
		cp.fLimitEnabled             = FALSE;				// we want to stop     
		cp.fMCIControl               = FALSE;
	
		capCaptureSetSetup(pDoc->m_hCamWnd, &cp, sizeof(cp) ); 
		
		capSetCallbackOnVideoStream(pDoc->m_hCamWnd, VideoCallbackProc);
		capSetCallbackOnFrame(pDoc->m_hCamWnd, VideoCallbackProc);
	}
	

	// CSoCProjectView 인쇄

	BOOL CSoCProjectView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// 기본적인 준비
		return DoPreparePrinting(pInfo);
	}

	void CSoCProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
	}

	void CSoCProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: 인쇄 후 정리 작업을 추가합니다.
	}


	// CSoCProjectView 진단

#ifdef _DEBUG
	void CSoCProjectView::AssertValid() const
	{
		CScrollView::AssertValid();
	}

	void CSoCProjectView::Dump(CDumpContext& dc) const
	{
		CScrollView::Dump(dc);
	}

	CSoCProjectDoc* CSoCProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSoCProjectDoc)));
		return (CSoCProjectDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CSoCProjectView 메시지 처리기


	void CSoCProjectView::OnInverse()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		/*CSoCProjectDoc* pDoc = GetDocument();	//document를 참조하기 위해
		ASSERT_VALID(pDoc);	 //instant address를 가져옴

		for(int i=0; i<256; i++){
			for(int j=0; j<256; j++){
				pDoc->m_OutImg[i][j] = 255 - pDoc->m_InImg[i][j];
			}
		}
		Invalidate(FALSE);*/
	}

	RECT CSoCProjectView::GetIMGSize(){
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = IMG_WIDTH;
		rect.bottom = IMG_HEIGHT;
		return rect;
	}

	LRESULT CALLBACK CSoCProjectView::VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
	{
		static int IMGRGB[IMG_WIDTH][IMG_HEIGHT][3];

		int jump, i, j;

		for(i = 0, jump = 0; i < IMG_HEIGHT; i++)
		{
			for(j = 0; j < IMG_WIDTH; j++, jump += 2)
			{
				IMGRGB[j][i][RED] = lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 2];
				IMGRGB[j][i][GREEN] = lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 1];
				IMGRGB[j][i][BLUE] = lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 0];
			}
		}

		// RGB 처리 끝
		for(i = 0, jump = 0; i < IMG_HEIGHT; i++)
		{
			for(j = 0; j < IMG_WIDTH; j++, jump += 2)
			{
				lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 2] = IMGRGB[j][i][RED];
				lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 1] = IMGRGB[j][i][GREEN];
				lpVHdr->lpData[(IMG_WIDTH * i + j) + jump + 0] = IMGRGB[j][i][BLUE];
			}
		}
		return (LRESULT) TRUE;
	}

	void CSoCProjectView::OnButton_ORG()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		m_IMGState = ORIGINAL;
	}


	void CSoCProjectView::OnButton_GRAY()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		m_IMGState = GRAY;
	}


	void CSoCProjectView::OnButton_MINUS()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		m_IMGState = MINUS;
	}


	void CSoCProjectView::OnButton_BINARY()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		m_IMGState = BINARY;
	}


	BOOL CSoCProjectView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

		//return CScrollView::OnEraseBkgnd(pDC);
		return FALSE;
	}


	void CSoCProjectView::OnButton_Label()
	{
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		m_IMGState = LABEL;
	}
