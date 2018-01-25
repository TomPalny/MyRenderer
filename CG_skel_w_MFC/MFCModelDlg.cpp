// MFCModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "MFCModelDlg.h"
#include "afxdialogex.h"
#include "Model.h"
#include "MeshModel.h"
#include <filesystem>
#include "resource.h"


// MFCModelDlg dialog

IMPLEMENT_DYNAMIC(MFCModelDlg, CDialogEx)

MFCModelDlg::MFCModelDlg(Model* model, Renderer* renderer) : CDialogEx(IDD_MODEL, NULL), _model(model), _renderer(renderer)
{
}

MFCModelDlg::~MFCModelDlg()
{
}

void MFCModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO6, m_uv_combo);
	DDX_Control(pDX, IDC_COMBO5, m_normals_combo);
	DDX_Control(pDX, IDC_COMBO4, m_shading_combo);
	DDX_Control(pDX, IDC_COMBO3, m_fragment_shader_combo);
	DDX_Control(pDX, IDC_COMBO2, m_vertex_shader_combo);
	DDX_Control(pDX, IDC_CHECK1, m_bounding_box);
	DDX_Control(pDX, IDC_COMBO7, m_animate_position_combo);
	DDX_Control(pDX, IDC_COMBO8, m_animate_color_combo);
	DDX_Control(pDX, IDC_CHECK2, m_toon_shading);
	DDX_Control(pDX, IDC_CHECK3, m_marble_texture);
	DDX_Control(pDX, IDC_CHECK4, m_environment_mapping);
}


BEGIN_MESSAGE_MAP(MFCModelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &MFCModelDlg::OnLoadTextureClicked)
	ON_BN_CLICKED(IDC_BUTTON2, &MFCModelDlg::OnClearTextureClicked)
	ON_BN_CLICKED(IDOK, &MFCModelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, &MFCModelDlg::OnApplyClicked)
	ON_BN_CLICKED(IDC_BUTTON4, &MFCModelDlg::OnLoadBumpTextureClicked)
	ON_BN_CLICKED(IDC_BUTTON5, &MFCModelDlg::OnClearBumpTextureClicked)
END_MESSAGE_MAP()

template <typename T>
void MFCModelDlg::initialize_combo(CComboBox& combo, std::unordered_map<T, std::string> map, T current)
{
	for (auto kv : map)
	{
		auto index = combo.AddString(kv.second.c_str());
		if (current == kv.first)
		{
			combo.SetCurSel(index);
		}
	}
}

// Message handlers
BOOL MFCModelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (auto& iter : experimental::filesystem::directory_iterator(Shader::SHADER_PATH))
	{
		auto filename = iter.path().filename().string();
		if (filename.find("vshader_", 0) == 0)
		{
			m_vertex_shader_combo.AddString(filename.c_str());
		}
		else if (filename.find("fshader_", 0) == 0)
		{
			m_fragment_shader_combo.AddString(filename.c_str());
		}
	}

	static const int SEARCH_ALL_OPTIONS = -1;
	auto mesh_vao = _model->get_vaos()[VAO_MESH];
	auto mesh_shader = mesh_vao->get_shader_program();
	auto vshader_filename = mesh_shader->get_vertex_shader_path();
	auto fshader_filename = mesh_shader->get_fragment_shader_path();
	m_vertex_shader_combo.SelectString(SEARCH_ALL_OPTIONS, vshader_filename.c_str());
	m_fragment_shader_combo.SelectString(SEARCH_ALL_OPTIONS, fshader_filename.c_str());
	
	m_bounding_box.SetCheck(_model->is_bounding_box_enabled());
	m_toon_shading.SetCheck(_model->is_toon_shading_enabled());
	m_marble_texture.SetCheck(_model->is_marble_texture_enabled());
	m_environment_mapping.SetCheck(_model->get_texture() != nullptr && _model->get_texture()->is_cubemap());
	initialize_combo(m_normals_combo, NORMAL_TYPE_MAP, _model->get_normal_type());
	initialize_combo(m_shading_combo, FILL_TYPE_MAP, _model->get_fill_type());
	initialize_combo(m_uv_combo, UV_TYPE_MAP, _model->get_uv_type());
	initialize_combo(m_animate_position_combo, POSITION_ANIMATION_MAP, _model->get_position_animation());
	initialize_combo(m_animate_color_combo, COLOR_ANIMATION_MAP, _model->get_color_animation());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

template <typename T>
T inverse_find(std::unordered_map<T,std::string> map, std::string value)
{
	for (auto kv : map)
	{
		if (kv.second == value)
		{
			return kv.first;
		}
	}
	throw std::exception("No matching elements in inverse_find");
}

template <typename T>
T get_enum_from_combobox(CComboBox& combo, std::unordered_map<T,std::string> map)
{
	char selection[256] = { 0 };
	combo.GetLBText(combo.GetCurSel(), selection);
	return inverse_find(map, selection);
}

void MFCModelDlg::OnBnClickedOk()
{
	OnApplyClicked();
	CDialogEx::OnOK();
}

void MFCModelDlg::OnLoadTextureClicked()
{
	CFileDialog dlg(TRUE, _T(".png"), NULL, NULL, _T("*.png|*.*"));
	dlg.m_ofn.lpstrInitialDir = "c:\\Projects\\technion\\graphics\\code\\textures\\";
	if (dlg.DoModal() == IDOK)
	{
		std::string path(static_cast<LPCTSTR>(dlg.GetPathName()));
		if (!experimental::filesystem::exists(path))
			return;

		TexturePtr texture = Texture::load_texture(path);
		_model->set_texture(texture);
	}
}


void MFCModelDlg::OnLoadBumpTextureClicked()
{
	CFileDialog dlg(TRUE, _T(".png"), NULL, NULL, _T("*.png|*.*"));
	dlg.m_ofn.lpstrInitialDir = "c:\\Projects\\technion\\graphics\\code\\textures\\";
	if (dlg.DoModal() == IDOK)
	{
		std::string path(static_cast<LPCTSTR>(dlg.GetPathName()));
		if (!experimental::filesystem::exists(path))
			return;

		_model->set_bump_texture(Texture::load_bumpmap(path));
	}
}


void MFCModelDlg::OnClearTextureClicked()
{
	TexturePtr no_texture(nullptr);
	_model->set_texture(no_texture);
}


void MFCModelDlg::OnClearBumpTextureClicked()
{
	TexturePtr no_texture(nullptr);
	_model->set_bump_texture(no_texture);
}


void MFCModelDlg::OnApplyClicked()
{
	char vertex_shader[256] = { 0 };
	char fragment_shader[256] = { 0 };
	m_vertex_shader_combo.GetLBText(m_vertex_shader_combo.GetCurSel(), vertex_shader);
	m_fragment_shader_combo.GetLBText(m_fragment_shader_combo.GetCurSel(), fragment_shader);
	
	ShaderProgramPtr shader = ShaderProgram::create(vertex_shader, "", fragment_shader);
	auto vaos = _model->get_vaos();
	if (vaos.find(VAO_MESH) != vaos.end())
	{
		vaos[VAO_MESH]->set_shader_program(shader);
	}

	_model->set_normal_type(get_enum_from_combobox(m_normals_combo, NORMAL_TYPE_MAP));
	_model->set_fill_type(get_enum_from_combobox(m_shading_combo, FILL_TYPE_MAP));
	_model->set_uv_type(get_enum_from_combobox(m_uv_combo, UV_TYPE_MAP));
	_model->set_position_animation(get_enum_from_combobox(m_animate_position_combo, POSITION_ANIMATION_MAP));
	_model->set_color_animation(get_enum_from_combobox(m_animate_color_combo, COLOR_ANIMATION_MAP));
	_model->set_bounding_box(m_bounding_box.GetCheck());
	_model->set_toon_shading(m_toon_shading.GetCheck());
	_model->set_marble_texture(m_marble_texture.GetCheck());
	_model->set_environment_mapping(m_environment_mapping.GetCheck());
}
