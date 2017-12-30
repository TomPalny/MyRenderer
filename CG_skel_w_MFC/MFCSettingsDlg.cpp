// MFCSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "MFCSettingsDlg.h"
#include "afxdialogex.h"


// MFCSettingsDlg dialog

IMPLEMENT_DYNAMIC(MFCSettingsDlg, CDialogEx)

MFCSettingsDlg::MFCSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTINGS, pParent)
{

}

MFCSettingsDlg::~MFCSettingsDlg()
{
}

void MFCSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLUR, check_blur);
	DDX_Control(pDX, IDC_ANTIALIASING, check_antialiasing);
	DDX_Control(pDX, IDC_FOG, check_fog);
}

BOOL MFCSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	check_fog.SetCheck(fog);
	check_antialiasing.SetCheck(antialiasing);
	check_blur.SetCheck(blur);
	return TRUE;
}


BEGIN_MESSAGE_MAP(MFCSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BLUR, &MFCSettingsDlg::OnBnClickedBlur)
	ON_BN_CLICKED(IDC_ANTIALIASING, &MFCSettingsDlg::OnBnClickedAntialiasing)
	ON_BN_CLICKED(IDC_FOG, &MFCSettingsDlg::OnBnClickedFog)
END_MESSAGE_MAP()


// MFCSettingsDlg message handlers


void MFCSettingsDlg::OnBnClickedBlur()
{
	blur = check_blur.GetCheck();
}


void MFCSettingsDlg::OnBnClickedAntialiasing()
{
	antialiasing = check_antialiasing.GetCheck();
}


void MFCSettingsDlg::OnBnClickedFog()
{
	fog = check_fog.GetCheck();
}
