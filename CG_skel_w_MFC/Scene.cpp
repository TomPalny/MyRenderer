#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"
#include "PrimitiveModel.h"

using namespace std;

// defined in CG_skel_w_MFC.cpp
void init_menu();

Scene::Scene(Renderer* renderer) : _active_model(nullptr), _renderer(renderer),
									_normal_type(NO_NORMALS), _operation_mode(TRANSLATE_MODE),
									_transform_mode(WORLD_TRANSFORM)
{
}

void Scene::set_normals_type(NormalType normal_type)
{
	_normal_type = normal_type;
	redraw_necessary();
}

void Scene::add_objects_to_menu()
{
	int i = 0;
	for (auto model : _models)
	{
		glutAddMenuEntry(model->get_name(), i++);
	}
}

void Scene::set_operation_mode(OperationMode mode)
{
	_operation_mode = mode;
}

void Scene::add_pyramid_model()
{
	const auto model = PrimitiveModel::create_pyramid(0.0f, 0.0f, 0.0f);
	load_model_at_center(model, "Pyramid");
}

void Scene::open_file()
{
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	if (dlg.DoModal() == IDOK)
	{
		std::string s(static_cast<LPCTSTR>(dlg.GetPathName()));
		load_model_at_center(new MeshModel(static_cast<LPCTSTR>(dlg.GetPathName())), static_cast<LPCTSTR>(dlg.GetFileTitle()));
	}
}

void Scene::load_model_at_center(Model* model, const string name)
{
	model->set_name(name);
	model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0, WORLD_TRANSFORM);
	model->scale(100, 100, 100, WORLD_TRANSFORM);

	_active_model = model;
	_models.push_back(_active_model);
	_active_model->set_renderer(_renderer);
	init_menu();
	redraw_necessary();
}

void Scene::draw_status_string()
{
	string status;
	if (_active_model != NULL)
	{
		status += _active_model->get_name();
	}

	if (_transform_mode == WORLD_TRANSFORM)
	{
		status += " // WORLD  // ";
	}
	else
	{
		status += " // OBJECT  // ";
	}

	if (_operation_mode == ROTATE_MODE)
	{
		status += "ROTATE";
	}
	else if (_operation_mode == SCALE_MODE)
	{
		status += "SCALE";
	}
	else
	{
		status += "TRANSLATE";
	}

	_renderer->draw_string(status.c_str(), 15, 0);
}

void Scene::redraw_necessary()
{
	glutPostRedisplay();
}

void Scene::draw_one_model(Model* model)
{
	model->update_matrix();
	model->draw();
	if (_normal_type == VERTEX_NORMALS)
	{
		model->draw_vertex_normals();
	}
	else if (_normal_type == FACE_NORMALS)
	{
		model->draw_face_normals();
	}
}
void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all _models to draw themselves
	_renderer->clear_screen();
	for(auto model : _models )
	{
		draw_one_model(model);
	}

	draw_status_string();
	_renderer->SwapBuffers();
}

void Scene::draw_demo() const
{
	_renderer->clear_screen();
	_renderer->SetDemoBuffer();
	_renderer->SwapBuffers();
}

void Scene::switch_active_model(int id)
{
	_active_model = _models[id];
}

void Scene::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: // escape
		exit(EXIT_SUCCESS);
		break;
	case 32: // spacebar
		_transform_mode = (TransformMode)((_transform_mode + 1) % NUMBER_OF_TRANSFORM_MODES);
		break;
	case 'o':
		open_file();
		break;
	case 'p':
		add_pyramid_model();
		break;
	case 'v':
		_normal_type = (NormalType) ((_normal_type + 1) % NUMBER_OF_NORMAL_TYPES);
		break;
	case 's':
		set_operation_mode(SCALE_MODE);
		break;
	case 't':
		set_operation_mode(TRANSLATE_MODE);
		break;
	case 'r':
		set_operation_mode(ROTATE_MODE);
		break;
	case '\t':
		// switch between models
		const auto it = std::find(_models.begin(), _models.end(), _active_model);
		const auto index = std::distance(_models.begin(), it);
		const auto new_index = (index + 1) % _models.size();
		_active_model = _models[new_index];
		break;
	}
	redraw_necessary();
}

void Scene::keyboard_special(int key, int x, int y)
{
	if (_active_model == NULL)
	{
		return;
	}

	float move_distance = 0.04f;
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
	{
		move_distance *= 4;
	}
	static const float LARGER_SCALE_FACTOR = 1.4;
	static const float SMALLER_SCALE_FACTOR = 0.6;
	static const float NO_SCALE = 1;
	static const float THETA = 0.1;

	switch (key) {
	case GLUT_KEY_RIGHT:
		if (_operation_mode == SCALE_MODE)
			_active_model->scale(LARGER_SCALE_FACTOR, NO_SCALE, NO_SCALE, _transform_mode);
		else if (_operation_mode == ROTATE_MODE)
			_active_model->rotate(THETA, 'y', _transform_mode);
		else
			_active_model->translate(move_distance, 0, 0, _transform_mode);
		break;
	case GLUT_KEY_LEFT:
		if (_operation_mode == SCALE_MODE)
			_active_model->scale(SMALLER_SCALE_FACTOR, NO_SCALE, NO_SCALE, _transform_mode);
		else if (_operation_mode == ROTATE_MODE)
			_active_model->rotate(-THETA, 'y', _transform_mode);
		else
			_active_model->translate(-move_distance, 0, 0, _transform_mode);
		break;
	case GLUT_KEY_UP:
		if (_operation_mode == SCALE_MODE)
			_active_model->scale(NO_SCALE, LARGER_SCALE_FACTOR, NO_SCALE, _transform_mode);
		else if (_operation_mode == ROTATE_MODE)
			_active_model->rotate(THETA, 'x', _transform_mode);
		else
			_active_model->translate(0, move_distance, 0, _transform_mode);
		break;
	case GLUT_KEY_DOWN:
		if (_operation_mode == SCALE_MODE)
			_active_model->scale(NO_SCALE, SMALLER_SCALE_FACTOR, NO_SCALE, _transform_mode);
		else if (_operation_mode == ROTATE_MODE)
			_active_model->rotate(-THETA, 'x', _transform_mode);
		else
			_active_model->translate(0, -move_distance, 0, _transform_mode);
		break;
	case GLUT_KEY_PAGE_UP:
		_active_model->rotate(THETA, 'z', _transform_mode);
		break;
	case GLUT_KEY_PAGE_DOWN:
		_active_model->rotate(-THETA, 'z', _transform_mode);
		break;
	}

	redraw_necessary();
}
