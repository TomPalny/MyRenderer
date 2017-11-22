#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"
#include "PrimitiveModel.h"
#include "Camera.h"
#include <sstream>
#include <filesystem>

using namespace std;

// defined in CG_skel_w_MFC.cpp
void init_menu();

Scene::Scene(Renderer* renderer) : _active_model(nullptr), _renderer(renderer),
									_normal_type(NO_NORMALS), _operation_mode(TRANSLATE_MODE),
									_transform_mode(WORLD_TRANSFORM)
{
	// default view is from the front
	auto front = new Camera(vec3(0, 0, 1));
	front->perform_operation(Translate(0, 0, 1), WORLD_TRANSFORM);
	front->set_renderer(renderer);
	front->set_name("Camera1 (Front)");
	_cameras.push_back(front);
	_models.push_back(front);

	auto left = new Camera(vec3(30, 30, 0));
	left->perform_operation(Translate(30, 30, 0), WORLD_TRANSFORM);
	left->set_renderer(renderer);
	left->set_name("Camera2 (Left)");
	_cameras.push_back(left);
	_models.push_back(left);

	_active_camera = front;
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

void Scene::switch_camera(unsigned int number)
{
	_active_camera = _cameras[number];
	redraw_necessary();
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
	model->perform_operation(Scale(100, 100, 100), MODEL_TRANSFORM);
	//model->translate(200, 200, 0, WORLD_TRANSFORM);
	//model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0, WORLD_TRANSFORM);
	
	_active_model = model;
	_models.push_back(_active_model);
	_active_model->set_renderer(_renderer);
	init_menu();
	redraw_necessary();
}

void Scene::draw_status_string()
{
	stringstream ss;
	if (_active_model != NULL)
	{
		auto p = _active_model->get_origin_in_world_coordinates();
		ss << _active_model->get_name() << " [" << p.x << "," << p.y << "," << p.z << "] // ";
	}

	if (_transform_mode == WORLD_TRANSFORM)
	{
		ss << "WORLD  // ";
	}
	else
	{
		ss << "OBJECT  // ";
	}

	if (_operation_mode == ROTATE_MODE)
	{
		ss << "ROTATE";
	}
	else if (_operation_mode == SCALE_MODE)
	{
		ss << "SCALE";
	}
	else
	{
		ss << "TRANSLATE";
	}

	_renderer->set_color(0, 1, 0);
	_renderer->draw_string(ss.str().c_str(), 15, 15);
}

void Scene::redraw_necessary()
{
	glutPostRedisplay();
}

void Scene::draw_one_model(Model* model)
{
	_active_camera->look_at(vec4(0, 0, 0, 1));
	model->update_matrix(_active_camera->get_view_matrix());
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
	_renderer->swap_buffers();
}

void Scene::draw_demo() const
{
	_renderer->clear_screen();
	_renderer->draw_demo();
	_renderer->swap_buffers();
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
	case '1':
	case '2':
		switch_camera(key - '1');
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

	static const float LARGER_SCALE_FACTOR = 1.4;
	static const float SMALLER_SCALE_FACTOR = 0.6;
	static const float NO_SCALE = 1;
	float move_distance = 1.0f;
	float theta = 0.1;

	// use "larger" transforms when shift is held
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
	{
		move_distance *= 4;
		theta *= 2;
	}

	mat4 operation;
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(LARGER_SCALE_FACTOR, NO_SCALE, NO_SCALE);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(theta, 'y');
		else
			operation = Translate(move_distance, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(SMALLER_SCALE_FACTOR, NO_SCALE, NO_SCALE);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(-theta, 'y');
		else
			operation = Translate(-move_distance, 0, 0);
		break;
	case GLUT_KEY_UP:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(NO_SCALE, LARGER_SCALE_FACTOR, NO_SCALE);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(theta, 'x');
		else
			operation = Translate(0, move_distance, 0);
		break;
	case GLUT_KEY_DOWN:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(NO_SCALE, SMALLER_SCALE_FACTOR, NO_SCALE);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(-theta, 'x');
		else
			operation = Translate(0, -move_distance, 0);
		break;
	case GLUT_KEY_PAGE_UP:
		if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(theta, 'z');
		else
			operation = Translate(0, 0, move_distance);
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(-theta, 'z');
		else
			operation = Translate(0, 0, -move_distance);
		break;
	}

	_active_model->perform_operation(operation, _transform_mode);
	redraw_necessary();
}
