#include "stdafx.h"
#include "MFCMaterialDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <vector>
#include "Material.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MFCMaterialDlg::MFCMaterialDlg(MaterialPtr material)
	: CDialogEx(IDD_MATERIALS, NULL), m_material(material)
{
}

void MFCMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listbox);
	DDX_Control(pDX, IDC_AMBIENT, m_ambient);
	DDX_Control(pDX, IDC_DIFFUSE, m_diffuse);
	DDX_Control(pDX, IDC_SPECULAR, m_specular);
	DDX_Control(pDX, IDC_EMISSIVE, m_emissive);
}

BEGIN_MESSAGE_MAP(MFCMaterialDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MFCMaterialDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &MFCMaterialDlg::OnColorChosenFromList)
	ON_BN_CLICKED(IDC_AMBIENT, &MFCMaterialDlg::OnBnClickedAmbient)
	ON_BN_CLICKED(IDC_DIFFUSE, &MFCMaterialDlg::OnBnClickedDiffuse)
	ON_BN_CLICKED(IDC_SPECULAR, &MFCMaterialDlg::OnBnClickedSpecular)
END_MESSAGE_MAP()


// Message handlers
BOOL MFCMaterialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (auto str : Material::get_csv_material_names())
	{
		m_listbox.AddString(str.c_str());
	}

	update_colors();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void MFCMaterialDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}*/
	CDialogEx::OnSysCommand(nID, lParam);
}

void MFCMaterialDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void MFCMaterialDlg::update_colors()
{
	auto a = m_material->_ambient;
	auto d = m_material->_diffuse;
	auto s = m_material->_specular;
	m_ambient.SetColor(RGB(a.x * 255, a.y * 255, a.z * 255));
	m_diffuse.SetColor(RGB(d.x * 255, d.y * 255, d.z * 255));
	m_specular.SetColor(RGB(s.x * 255, s.y * 255, s.z * 255));
}

void MFCMaterialDlg::OnColorChosenFromList()
{
	char buffer[1024] = { 0 };
	int selected = m_listbox.GetCurSel();
	if (selected != -1)
	{
		m_listbox.GetText(selected, buffer);
		auto material_name = std::string(buffer);
		m_material = Material::get_from_csv(material_name);
	}
	update_colors();
}

void MFCMaterialDlg::UpdateMaterialAccordingToGui()
{
	COLORREF a = m_ambient.GetColor();
	m_material->_ambient.x = GetRValue(a) / 255.0f;
	m_material->_ambient.y = GetGValue(a) / 255.0f;
	m_material->_ambient.z = GetBValue(a) / 255.0f;

	COLORREF d = m_diffuse.GetColor();
	m_material->_diffuse.x = GetRValue(d) / 255.0f;
	m_material->_diffuse.y = GetGValue(d) / 255.0f;
	m_material->_diffuse.z = GetBValue(d) / 255.0f;

	COLORREF s = m_specular.GetColor();
	m_material->_specular.x = GetRValue(s) / 255.0f;
	m_material->_specular.y = GetGValue(s) / 255.0f;
	m_material->_specular.z = GetBValue(s) / 255.0f;

	// TODO: emissive
}

void MFCMaterialDlg::OnBnClickedAmbient()
{
	UpdateMaterialAccordingToGui();
}

void MFCMaterialDlg::OnBnClickedDiffuse()
{
	UpdateMaterialAccordingToGui();
}

void MFCMaterialDlg::OnBnClickedSpecular()
{
	UpdateMaterialAccordingToGui();
}
