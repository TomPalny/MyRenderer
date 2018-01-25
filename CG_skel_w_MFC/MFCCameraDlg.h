#pragma once
#include "Camera.h"
#include "afxwin.h"
#include "Resource.h"

// MFCCameraDlg dialog

class MFCCameraDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MFCCameraDlg)

public:
	MFCCameraDlg(Camera* camera);   // standard constructor
	virtual ~MFCCameraDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;
	Camera* _camera;
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_near;
	CEdit m_far;
	CEdit m_fovy;
	afx_msg void OnBnClickedOk();
	CButton m_skybox_checkbox;
};
