#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"
#include "PrimitiveModel.h"

using namespace std;

Scene::Scene(Renderer* renderer) : _active_model(nullptr), _renderer(renderer), _normal_type(NO_NORMALS)
{
}

void Scene::load_obj_model(const string filename)
{
	const auto model = new MeshModel(filename);
	load_model_at_center(model);
}

void Scene::add_pyramid_model()
{
	const auto model = PrimitiveModel::create_pyramid(0.0f, 0.0f, 0.0f);
	load_model_at_center(model);
}

void Scene::load_model_at_center(Model* model)
{
	model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0);
	model->scale(100, 100, 100);

	_active_model = model;
	_models.push_back(_active_model);
	_active_model->set_renderer(_renderer);
	/* we don't use redraw_necessary() because that will force a redraw of ALL models */
	_active_model->draw();
	_renderer->SwapBuffers();
}

void Scene::redraw_necessary()
{
	glutPostRedisplay();
}

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all _models to draw themselves
	_renderer->clear_screen();
	for(auto model : _models )
	{
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
	_renderer->SwapBuffers();
}

void Scene::draw_demo()
{
	_renderer->clear_screen();
	_renderer->SetDemoBuffer();
	_renderer->SwapBuffers();
}

void Scene::keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27: // escape
		exit(EXIT_SUCCESS);
		break;
	case 'p':
		add_pyramid_model();
		break;
	case 'v':
		_normal_type = (NormalType) ((_normal_type + 1) % NUMBER_OF_NORMAL_TYPES);
		redraw_necessary();
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
	//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogBoxCallback);
	switch (key) {
	case GLUT_KEY_RIGHT:
		_active_model->translate(move_distance, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		_active_model->translate(-move_distance, 0, 0);
		break;
	case GLUT_KEY_UP:
		_active_model->translate(0, move_distance, 0);
		break;
	case GLUT_KEY_DOWN:
		_active_model->translate(0, -move_distance, 0);
		break;
	}

	redraw_necessary();
}
