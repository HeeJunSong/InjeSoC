
// SoC ProjectView.h : CSoCProjectView 클래스의 인터페이스
//

#pragma once
#include "SoC ProjectDoc.h"
//#include "EdgeMaster.h"

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

//using namespace EdgeMaster;

class CSoCProjectView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CSoCProjectView();
	DECLARE_DYNCREATE(CSoCProjectView)

// 특성입니다.
public:
	CSoCProjectDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	static RECT GetIMGSize();
	static CString statSTR;
	static int grayRate;
	static int m_FPS;
	static int m_FrameCNT;
	enum IMG_STATE{ORIGINAL = 0, GRAY, MINUS, BINARY, LABEL};
	static IMG_STATE m_IMGState;
	static HBITMAP m_IMGExample[9];
	static int m_SelectedBTM;
	static CString m_BTMInfo[9];
	static int m_MatchRate;
	static void DrawBitmap(CDC *pDC,int x,int y,HBITMAP hBit);
	static void DrawPixels(CDC* pDC,int x,int y,HBITMAP hBit, BYTE*pixels);
	static void DrawTextDBF(CDC* pDC,int x,int y, CString str);
	static void ClearRect(CDC *pDC, COLORREF color);
	
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CSoCProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInverse();
	static LRESULT CALLBACK VideoCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr);
	afx_msg void OnButton_ORG();
	afx_msg void OnButton_GRAY();
	afx_msg void OnButton_MINUS();
	afx_msg void OnButton_BINARY();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButton_Label();
};

#ifndef _DEBUG  // SoC ProjectView.cpp의 디버그 버전
inline CSoCProjectDoc* CSoCProjectView::GetDocument() const
   { return reinterpret_cast<CSoCProjectDoc*>(m_pDocument); }
#endif

