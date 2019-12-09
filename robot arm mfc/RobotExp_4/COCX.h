/*

#pragma once

// 머신에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 콘텐츠를 수정하지 마세요. Microsoft Visual C++를 통해 이 클래스가 다시 생성될 경우 
// 수정 내용을 덮어씁니다.

/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"

class COCX : public CWnd
{
protected:
	DECLARE_DYNCREATE(COCX)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= {0xc9fe01c2,0x2746,0x479b,{0x96,0xab,0xe0,0xbe,0x99,0x31,0xb0,0x18}};
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 특성
public:
enum
{
	Solid = 0,
	Dash = 1,
	Dot = 2,
	DashDot = 3,
	DashDotDot = 4,
	Null = 5,
	XYStep = 6,
	YXStep = 7,
	Bars = 8,
	Stick = 9
}LineType;

enum
{
	Nosym = 0,
	Dots = 1,
	Rectangles = 2,
	Diamonds = 3,
	Asterisk = 4,
	DownTriangles = 5,
	UpTriangles = 6,
	LeftTriangles = 7,
	RightTriangles = 8
}SymbolType;

enum
{
	Flat = 0,
	Scope = 1,
	Bitmap = 2
}FrameStyle;

enum
{
	None = 0,
	Track = 1,
	Zoom = 2,
	PanXY = 3,
	PanX = 4,
	PanY = 5
}TrackModeState;



// 작업
public:


};

*/