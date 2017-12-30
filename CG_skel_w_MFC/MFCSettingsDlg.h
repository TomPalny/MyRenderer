#pragma once
#include "afxwin.h"


// MFCSettingsDlg dialog

class MFCSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MFCSettingsDlg)

public:
	MFCSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MFCSettingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
														// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBlur();
	afx_msg void OnBnClickedAntialiasing();
	afx_msg void OnBnClickedFog();

	bool blur;
	bool antialiasing;
	bool fog;
	bool bloom;
	CButton check_blur;
	CButton check_antialiasing;
	CButton check_fog;
	CButton check_bloom;
	afx_msg void OnBnClickedBloom();
};
