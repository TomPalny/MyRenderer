#pragma once
#include <afxdialogex.h>
#include "afxvslistbox.h"
#include <vector>
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "Material.h"


class MFCMaterialDlg : public CDialogEx
{
public:
	MFCMaterialDlg(MaterialPtr material);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATERIALS };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	MaterialPtr m_material;
	afx_msg void OnBnClickedOk();
	CListBox m_listbox;
	afx_msg void update_colors();
	void OnColorChosenFromList();
	CMFCColorButton m_ambient;
	CMFCColorButton m_diffuse;
	CMFCColorButton m_specular;
	CMFCColorButton m_emissive;
	afx_msg void OnBnClickedAmbient();
	afx_msg void UpdateMaterialAccordingToGui();
	void OnBnClickedDiffuse();
	afx_msg void OnBnClickedSpecular();
	afx_msg void OnBnClickedEmissive();
};
