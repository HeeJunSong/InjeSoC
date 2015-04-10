
// SoC ProjectDoc.h : CSoCProjectDoc Ŭ������ �������̽�
//


#pragma once


class CSoCProjectDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CSoCProjectDoc();
	DECLARE_DYNCREATE(CSoCProjectDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CSoCProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	BITMAPINFO m_BTMInfo;
	HWND m_hCamWnd;
	static CAPSTATUS m_psCapStatus;
	CAPDRIVERCAPS m_psCapsInfo;
};
