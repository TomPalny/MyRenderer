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
                                   _transform_mode(WORLD_TRANSFORM), _camera_mode(PERSPECTIVE_CAMERA),
                                   _wireframe_mode(true)
{
	// default view is from the front
	auto front = new Camera(1);
	front->perform_operation(Translate(0, 0, 5), WORLD_TRANSFORM);
	front->set_name("Camera1 (Front)");
	front->look_at2(vec3(0, 0, 5), vec3(0, 0, 0));
	_cameras.push_back(front);
	_models.push_back(front);
	_bounding_boxes.push_back(false);

	auto left = new Camera(2);
	left->perform_operation(Translate(8, 1, 0), WORLD_TRANSFORM);
	left->set_name("Camera2 (Left)");
	left->look_at(vec3(0, 0, 0));
	_cameras.push_back(left);
	_models.push_back(left);
	_bounding_boxes.push_back(false);

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
		glutAddMenuEntry(model->get_name().c_str(), i++);
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
	if (model->_bounding_box != nullptr)
	{
		model->_bounding_box->set_name(name);
	}
	_active_model = model;
	_models.push_back(_active_model);
	_bounding_boxes.push_back(false);
	
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

	if (_camera_mode == PERSPECTIVE_CAMERA)
	{
		ss << "PERSPECTIVE";
	}
	else
	{
		ss << "ORTHOGRAPHIC";
	}

	ss << " // FOVY=" << (int) _renderer->_fovy;

	_renderer->set_color(0, 1, 0);
	_renderer->draw_string(ss.str().c_str(), 15, 15);
}

void Scene::redraw_necessary()
{
	glutPostRedisplay();
}

void Scene::draw_one_model(Model* model, bool draw_bounding_box)
{
	if (_wireframe_mode)
		_renderer->draw_model_wireframe(model, _active_camera, _camera_mode);
	else
		_renderer->draw_model(model, _active_camera, _camera_mode);

	if(!model->is_camera() && model->_bounding_box != nullptr) 
	{
		if (draw_bounding_box)
			_renderer->draw_model_wireframe(model->_bounding_box, _active_camera, _camera_mode);
	}
	/*
	if (_normal_type == VERTEX_NORMALS)
	{
		model->draw_vertex_normals(_camera_mode);
	}
	else if (_normal_type == FACE_NORMALS)
	{
		model->draw_face_normals(_camera_mode);
	}*/
}

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all _models to draw themselves
	_renderer->clear_screen();
	int i = 0;
	for(auto model : _models )
	{
		draw_one_model(model, _bounding_boxes[i++]);
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
	const auto index = std::distance(_models.begin(), it);
	const auto new_index = (index + 1) % _models.size();
	

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
	case 'w':
		_wireframe_mode = !_wireframe_mode;
		break;
	case 's':
		set_operation_mode(SCALE_MODE);
		break;
	case 't':
		set_operation_mode(TRANSLATE_MODE);
		break;
	case '+':
		_renderer->_fovy += 3.0f;
		break;
	case '-':
		_renderer->_fovy -= 3.0f;
		break;
	case 'r':
		set_operation_mode(ROTATE_MODE);
		break;
	case '\t':
		// switch between models
		_active_model = _models[new_index];
		break;
	case 'b':
		_bounding_box_index = index;
		_bounding_boxes[_bounding_box_index] = !_bounding_boxes[_bounding_box_index];
		break;
	case '`':
		_camera_mode = (CameraMode)((_camera_mode + 1) % NUMBER_OF_CAMERA_MODES);
		break;
	case 'l':
		_active_camera->look_at(_active_model->get_origin_in_world_coordinates().to_vec3_divide_by_w());
		break;
	}
	redraw_necessary();
}

mat4 Scene::get_operation_for_keyboard(int key, int x, int y)
{
	static const float LARGER_SCALE_FACTOR = 1.4;
	static const float SMALLER_SCALE_FACTOR = 0.6;
	static const float NO_SCALE = 1;
	float move_distance = 0.3f;
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
	if (_active_model->is_camera())
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
	// for models that aren't the camera, apply the transform to their bounding box
	else if (_active_model->_bounding_box != nullptr)
	{
		_active_model->_bounding_box->perform_operation(operation, _transform_mode);
	}
	redraw_necessary();
}
