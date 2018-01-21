#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"
#include "Camera.h"
#include <sstream>
#include <filesystem>
#include "MFCMaterialDlg.h"
#include "MFCSettingsDlg.h"
#include "MFCModelDlg.h"
#include "MFCCameraDlg.h"

using namespace std;

// defined in CG_skel_w_MFC.cpp
void init_menu();

void timer_callback(int param)
{
	glutPostRedisplay();
	glutTimerFunc(10, timer_callback, 0);
}

Scene::Scene(Renderer* renderer) : _active_model(nullptr), _renderer(renderer), _operation_mode(TRANSLATE_MODE),
                                   _transform_mode(WORLD_TRANSFORM), _projection_type(PERSPECTIVE_PROJECTION)
{
	// default view is from the front
	auto front = new Camera(1);
	front->perform_operation(Translate(0, 0, 5), WORLD_TRANSFORM);
	front->set_name("Camera1 (Front)");
	front->look_at(vec3(0, 0, 0));
	_cameras.push_back(front);
	_models.push_back(front);

	auto left = new Camera(2);
	left->perform_operation(Translate(8, 1, 0), WORLD_TRANSFORM);
	left->set_name("Camera2 (Left)");
	left->look_at(vec3(0, 0, 0));
	_cameras.push_back(left);
	_models.push_back(left);

	auto top = new Camera(3);
	top->perform_operation(Translate(1.5, 5, 0), WORLD_TRANSFORM);
	top->set_name("Camera3 (Top)");
	top->look_at(vec3(0, 0, 0));
	_cameras.push_back(top);
	_models.push_back(top);

	_active_camera = front;

	auto light1 = new Light();
	light1->perform_operation(Translate(2, 3, 0), WORLD_TRANSFORM);
	_models.push_back(light1);

	glutTimerFunc(3000, timer_callback, 0);
}

void Scene::add_objects_to_menu()
{
	int i = 0;
	for (auto model : _models)
	{
		glutAddMenuEntry(model->get_name().c_str(), i++);
	}
}

void Scene::show_materials_dialog()
{
	if (_active_model == nullptr)
	{
		return;
	}

	MFCMaterialDlg dlg(_active_model->_material, dynamic_cast<Light*>(_active_model) != nullptr);
	dlg.DoModal();
	_active_model->_material = dlg.m_material;
}

void Scene::show_settings_dialog()
{
}

void Scene::show_model_dialog()
{
	MFCModelDlg dlg(_active_model);
	dlg.DoModal();
}

void Scene::show_camera_dialog()
{
	MFCCameraDlg dlg(_active_camera);
	dlg.DoModal();
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
	const auto model = MeshModel::create_pyramid_model();
	load_model_at_center(model, "Pyramid");
}

void Scene::add_light()
{
	const auto light = new Light();
	load_model_at_center(light, "Doesn't Matter");
	// TODO: don't we need to add this to _lights?
}

void Scene::open_file()
{
	CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
	dlg.m_ofn.lpstrInitialDir = "c:\\Projects\\technion\\graphics\\code\\examples\\";
	if (dlg.DoModal() == IDOK)
	{
		std::string path(static_cast<LPCTSTR>(dlg.GetPathName()));
		if (!experimental::filesystem::exists(path))
			return;

		Model* model = new MeshModel(path); 
		load_model_at_center(model, static_cast<LPCTSTR>(dlg.GetFileTitle()));
		
	}
}

void Scene::load_model_at_center(Model* model, const string name)
{
	model->set_name(name);
	_active_model = model;
	_models.push_back(_active_model);
	
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
		ss << "ROTATE // ";
	}
	else if (_operation_mode == SCALE_MODE)
	{
		ss << "SCALE // ";
	}
	else
	{
		ss << "TRANSLATE // ";
	}

	if (_projection_type == PERSPECTIVE_PROJECTION)
	{
		ss << "PERSPECTIVE // ";
	}
	else
	{
		ss << "ORTHOGRAPHIC // ";
	}

	glutSetWindowTitle(ss.str().c_str());
}

void Scene::redraw_necessary()
{
	glutPostRedisplay();
}

void Scene::draw()
{
	// TODO: shininess
	std::vector<Light*> lights;
	for(Model* model : _models)
	{
		if (model->get_type() == LIGHT_MODEL)
		{
			lights.push_back(dynamic_cast<Light*>(model));
		}
	}
	
	_active_camera->set_projection_type(_projection_type);
	_active_camera->set_aspect_ratio(_renderer->get_aspect_ratio());
	_renderer->clear_screen();
	_renderer->set_parameters(_active_camera, lights); // TODO: should we pass references here?

	int i = 0;
	for(auto model : _models )
	{
		_renderer->draw(model);
	}

	draw_status_string();
	_renderer->swap_buffers();
}

void Scene::switch_active_model(int id)
{
	_active_model = _models[id];
}

void Scene::keyboard(unsigned char key, int x, int y)
{
	const auto it = std::find(_models.begin(), _models.end(), _active_model);
	auto index = std::distance(_models.begin(), it);
	int direction = 1;
	if (glutGetModifiers() & GLUT_ACTIVE_SHIFT)
		direction = -1;
	index += direction;
	index = index % _models.size();
	
	switch (key) {
	case 27: // escape
		exit(EXIT_SUCCESS);
		break;
	case 32: // spacebar
		_transform_mode = (TransformMode)((_transform_mode + 1) % NUMBER_OF_TRANSFORM_MODES);
		break;
	case '1':
	case '2':
	case '3':
		switch_camera(key - '1');
		break;
	case 'd':
		if (dynamic_cast<Light*>(_active_model) != nullptr)
		{
			dynamic_cast<Light*>(_active_model)->light_type = (LightType)((dynamic_cast<Light*>(_active_model)->light_type + 1) % NUM_LIGHT_TYPES);
		}
		break;
	case 'o':
		open_file();
		break;
	case 'p':
		add_pyramid_model();
		break;
	case '+':
		add_light();
		break;
	case 'r':
		set_operation_mode(ROTATE_MODE);
		break;
	case 's':
		set_operation_mode(SCALE_MODE);
		break;
	case 't':
		set_operation_mode(TRANSLATE_MODE);
		break;

	case '\t':
		// switch between models
		_active_model = _models[index];
		break;
	case '`':
		_projection_type = (ProjectionType)((_projection_type + 1) % NUMBER_OF_PROJECTION_TYPES);
		break;
	case 'l':
		_active_camera->look_at(_active_model->get_origin_in_world_coordinates().to_vec3_divide_by_w());
		break;
	case 'q':
		for(auto kv : _active_model->get_vaos())
		{
			auto vao = kv.second;
			vao->reload_shader();
		}
		break;
	case 'c':
		show_camera_dialog();
		break;
	case 'm':
		show_materials_dialog();
		break;
	case 'z':
		show_settings_dialog();
		break;
	case 'x':
		show_model_dialog();
		break;
	}
	redraw_necessary();
}

mat4 Scene::get_operation_for_keyboard(int key, int x, int y)
{
	static const float LARGER_SCALE_FACTOR = 1.4f;
	static const float SMALLER_SCALE_FACTOR = 0.6f;
	static const float NO_SCALE = 1.0f;
	float move_distance = 0.3f;
	float theta = 0.1f;

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
			operation = RotateMat(-theta, 'x');
		else
			operation = Translate(0, move_distance, 0);
		break;
	case GLUT_KEY_DOWN:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(NO_SCALE, SMALLER_SCALE_FACTOR, NO_SCALE);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(theta, 'x');
		else
			operation = Translate(0, -move_distance, 0);
		break;
	case GLUT_KEY_PAGE_UP:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(NO_SCALE, NO_SCALE, LARGER_SCALE_FACTOR);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(theta, 'z');
		else
			operation = Translate(0, 0, move_distance);
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (_operation_mode == SCALE_MODE)
			operation = Scale(NO_SCALE, NO_SCALE, SMALLER_SCALE_FACTOR);
		else if (_operation_mode == ROTATE_MODE)
			operation = RotateMat(-theta, 'z');
		else
			operation = Translate(0, 0, -move_distance);
		break;
	}
	return operation;
}

int Scene::get_reverse_key(int key)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		return GLUT_KEY_LEFT;
	case GLUT_KEY_LEFT:
		return GLUT_KEY_RIGHT;
	case GLUT_KEY_UP:
		return GLUT_KEY_DOWN;
	case GLUT_KEY_DOWN:
		return GLUT_KEY_UP;
	case GLUT_KEY_PAGE_UP:
		return GLUT_KEY_PAGE_DOWN;
	case GLUT_KEY_PAGE_DOWN:
		return GLUT_KEY_PAGE_UP;
	}
	return 0;
}

void Scene::keyboard_special(int key, int x, int y)
{
	if (_active_model == NULL)
	{
		return;
	}

	mat4 operation = get_operation_for_keyboard(key, x, y);
	_active_model->perform_operation(operation, _transform_mode);

	// for the camera apply a reverse transformation to the view matrix
	if (_active_model->get_type() == CAMERA_MODEL)
	{
		Camera* camera = dynamic_cast<Camera*>(_active_model);
		key = get_reverse_key(key);
		mat4 inverse_operation = get_operation_for_keyboard(key, x, y);
		if (_transform_mode == MODEL_TRANSFORM && _operation_mode == ROTATE_MODE)
		{
			vec3 camera_origin = camera->get_origin_in_world_coordinates().to_vec3_divide_by_w();
			mat4 move = Translate(camera_origin.x, camera_origin.y, camera_origin.z);
			mat4 move_back = Translate(-camera_origin.x, -camera_origin.y, -camera_origin.z);
			inverse_operation = move * inverse_operation * move_back;
		}
		camera->apply_view_transformation(inverse_operation);
	}
	redraw_necessary();
}
