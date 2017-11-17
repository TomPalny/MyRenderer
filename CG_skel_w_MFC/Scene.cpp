#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"

using namespace std;

Scene::Scene() : _active_model(NULL)
{
}

void Scene::load_obj_model(string filename)
{
	_active_model = new MeshModel(filename);
	_active_model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0);
	_active_model->scale(100, 100, 100);
	_models.push_back(_active_model);
	_active_model->set_renderer(_renderer);
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
	case '\t':
		// switch between models
		auto it = std::find(_models.begin(), _models.end(), _active_model);
		auto index = std::distance(_models.begin(), it);
		auto new_index = (index + 1) % _models.size();
		_active_model = _models[new_index];
	}
}

void Scene::keyboard_special(int key, int x, int y)
{
	if (_active_model == NULL)
	{
		return;
	}

	static const float MOVE_DISTANCE = 0.02f;
	switch (key) {
	case GLUT_KEY_RIGHT:
		_active_model->translate(MOVE_DISTANCE, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		_active_model->translate(-MOVE_DISTANCE, 0, 0);
		break;
	case GLUT_KEY_UP:
		_active_model->translate(0, MOVE_DISTANCE, 0);
		break;
	case GLUT_KEY_DOWN:
		_active_model->translate(0, -MOVE_DISTANCE, 0);
		break;
	}

	redraw_necessary();
}
