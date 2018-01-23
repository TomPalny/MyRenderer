#pragma once
#include "Model.h"
#include "afxwin.h"


// MFCModelDlg dialog

class MFCModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MFCModelDlg)

public:
	MFCModelDlg(Model* model);   // standard constructor
	virtual ~MFCModelDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadTextureClicked();
	BOOL OnInitDialog() override;
	Model* _model;
	CComboBox m_uv_combo;
	CComboBox m_normals_combo;
	CComboBox m_shading_combo;
	CComboBox m_fragment_shader_combo;
	CComboBox m_vertex_shader_combo;
	CComboBox m_animate_position_combo;
	CComboBox m_animate_color_combo;

	afx_msg void OnClearTextureClicked();
	afx_msg void OnBnClickedOk();
	CButton m_bounding_box;
	afx_msg void OnApplyClicked();


	template<typename T>
	void initialize_combo(CComboBox & combo, std::unordered_map<T, std::string> map, T current);
	CButton m_toon_shading;
};
