
// SoC ProjectView.cpp : CSoCProjectView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
		// ǥ�� �μ� ����Դϴ�.
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
	// CSoCProjectView ����/�Ҹ�
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
		m_BTMInfo[0] = "Ž����...";
		m_BTMInfo[1] = "��¡���(98g)";
		m_BTMInfo[2] = "���ڱ�(75g)";
		m_BTMInfo[3] = "���̼Ҵ� ����(250ml)";
		m_BTMInfo[4] = "�����(90g)";
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
		// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
		//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

		return CScrollView::PreCreateWindow(cs);
	}

	// CSoCProjectView �׸���

	void CSoCProjectView::OnDraw(CDC* pDC)
	{
		CSoCProjectDoc*pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		ClearRect(pDC, RGB(255,255,255));

		// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
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
		// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
		sizeTotal.cx = 640;
		sizeTotal.cy = 480;
		SetScrollSizes(MM_TEXT, sizeTotal);

		CSoCProjectDoc*pDoc = GetDocument();
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
		RECT r;
		GetClientRect(&r);
		pDoc->m_hCamWnd = capCreateCaptureWindow(
			_T("Capture Window"), WS_CHILD | WS_VISIBLE, 5, 5, r.right-5, r.bottom-5, this->m_hWnd, NULL);

		// ��ġ�� ����̽��� ������ 0 ~ 9������ ī�޶� ���� �� �� �ִ�.
		if(!capDriverConnect(pDoc->m_hCamWnd, 0)) AfxMessageBox(_T("��ķ �ν� ���� �Ф�"));

		// ���� ����̹� ������ ���� ���� ������
		capDriverGetCaps(pDoc->m_hCamWnd, &pDoc->m_psCapsInfo, sizeof(pDoc->m_psCapsInfo));

		//���� ���� ��ȯ�� �����ϴ��� Ȯ���Ѵ�.
		if(pDoc->m_psCapsInfo.fHasDlgVideoFormat) {
			// ���� ���� ��ȯ�� �����ϸ� �Ʒ� �Լ��� ȣ���Ѵ�.
			// ȣ��Ǵ� �Լ��� ���ο� ���̾�α׸� ȣ���ϰ� �ػ󵵿� ��������, ������ ����ũ����� ������ �� �ִ�.
			// �̶�, �������� �ʴ� ���������� �����ϸ� ���� ȭ���� ���Ե� ���̾�...
			capDlgVideoFormat(pDoc->m_hCamWnd);
		}

		// m_psCapsInfo.fHasOverlay���� overlay�� ������ ���� ������(=0) ��� �� �� ����.
		if(pDoc->m_psCapsInfo.fHasOverlay) {
			// �ϵ���� �������̴� �ý��� ���ϸ� �ٿ��ش�.(optioinal)
			capOverlay(pDoc->m_hCamWnd, FALSE);
		}

		// BITMAPINFO ����
		capGetVideoFormat(pDoc->m_hCamWnd, &pDoc->m_BTMInfo, sizeof(pDoc->m_BTMInfo));

		// 1/1000 ������ ������ ��µȴ�.
		capPreviewRate(pDoc->m_hCamWnd, 1);

		// ������ ������ ����Ѵ�.
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
	

	// CSoCProjectView �μ�

	BOOL CSoCProjectView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// �⺻���� �غ�
		return DoPreparePrinting(pInfo);
	}

	void CSoCProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	}

	void CSoCProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
	{
		// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
	}


	// CSoCProjectView ����

#ifdef _DEBUG
	void CSoCProjectView::AssertValid() const
	{
		CScrollView::AssertValid();
	}

	void CSoCProjectView::Dump(CDumpContext& dc) const
	{
		CScrollView::Dump(dc);
	}

	CSoCProjectDoc* CSoCProjectView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSoCProjectDoc)));
		return (CSoCProjectDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CSoCProjectView �޽��� ó����


	void CSoCProjectView::OnInverse()
	{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		/*CSoCProjectDoc* pDoc = GetDocument();	//document�� �����ϱ� ����
		ASSERT_VALID(pDoc);	 //instant address�� ������

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

		// RGB ó�� ��
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
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		m_IMGState = ORIGINAL;
	}


	void CSoCProjectView::OnButton_GRAY()
	{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		m_IMGState = GRAY;
	}


	void CSoCProjectView::OnButton_MINUS()
	{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		m_IMGState = MINUS;
	}


	void CSoCProjectView::OnButton_BINARY()
	{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		m_IMGState = BINARY;
	}


	BOOL CSoCProjectView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

		//return CScrollView::OnEraseBkgnd(pDC);
		return FALSE;
	}


	void CSoCProjectView::OnButton_Label()
	{
		// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
		m_IMGState = LABEL;
	}
