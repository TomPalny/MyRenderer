// MFCCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "MFCCameraDlg.h"
#include "afxdialogex.h"
#include "Camera.h"
#include "Resource.h"
#include <sstream>


// MFCCameraDlg dialog

IMPLEMENT_DYNAMIC(MFCCameraDlg, CDialogEx)

MFCCameraDlg::MFCCameraDlg(Camera* camera) 	: CDialogEx(IDD_CAMERA, NULL), _camera(camera)
{
}

MFCCameraDlg::~MFCCameraDlg()
{
}

void MFCCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_near);
	DDX_Control(pDX, IDC_EDIT2, m_far);
	DDX_Control(pDX, IDC_EDIT3, m_fovy);
	DDX_Control(pDX, IDC_CHECK1, m_skybox_checkbox);
}


BEGIN_MESSAGE_MAP(MFCCameraDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MFCCameraDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL MFCCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_near.SetWindowText(std::to_string(_camera->get_nearz()).c_str());
	m_far.SetWindowText(std::to_string(_camera->get_farz()).c_str());
	m_fovy.SetWindowText(std::to_string(_camera->get_fovy()).c_str());
	m_skybox_checkbox.SetCheck(_camera->should_show_skybox());
	return TRUE;  // return TRUE  unless you set the focus to a control
}
// MFCCameraDlg message handlers


void MFCCameraDlg::OnBnClickedOk()
{
	char buff[256] = { 0 };
	m_near.GetWindowText(buff, sizeof(buff));
	_camera->set_nearz(atof(buff));
	m_far.GetWindowText(buff, sizeof(buff));
	_camera->set_farz(atof(buff));
	m_fovy.GetWindowText(buff, sizeof(buff));
	_camera->set_fovy(atof(buff));
	_camera->set_show_skybox(m_skybox_checkbox.GetCheck());
	CDialogEx::OnOK();
}
