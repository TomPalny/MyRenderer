#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"
#include "PrimitiveModel.h"

using namespace std;

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

void Scene::load_obj_model(const string filename, const string name)
{
	load_model_at_center(new MeshModel(filename), name);
}

void Scene::load_model_at_center(Model* model, const string name)
{
	model->set_name(name);
	model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0, WORLD_TRANSFORM);
	model->scale(100, 100, 100, WORLD_TRANSFORM);

	_active_model = model;
	_models.push_back(_active_model);
	_active_model->set_renderer(_renderer);
	/* we don't use redraw_necessary() because that will force a redraw of ALL models */
	draw_one_model(model);
	_renderer->SwapBuffers();
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

	if (model == _active_model)
	{
		// something is weird with our window height
		_renderer->draw_string(model->get_name(), 10, _renderer->get_height()-110);
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
	if (_transform_mode == WORLD_TRANSFORM)
	{
		_renderer->draw_string("WORLD", 15, 0);
	}
	else
	{
		_renderer->draw_string("OBJECT", 15, 0);
	}
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
		if (_transform_mode == WORLD_TRANSFORM)
		{
			_transform_mode = MODEL_TRANSFORM;
		}
		else
		{
			_transform_mode = WORLD_TRANSFORM;
		}
		redraw_necessary();
		break;
	case 'p':
		add_pyramid_model();
		break;
	case 'v':
		_normal_type = (NormalType) ((_normal_type + 1) % NUMBER_OF_NORMAL_TYPES);
		redraw_necessary();
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
