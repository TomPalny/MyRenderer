#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"

using namespace std;

Scene::Scene() : _active_model(NULL)
{
}

void Scene::add_objects_to_menu()
{
	int i = 0;
	for (auto model : _models)
	{
		glutAddMenuEntry(model->get_name(),i++);
	}
}

void Scene::load_obj_model(const string filename, const string objName)
{
	_active_model = new MeshModel(filename);
	_active_model->set_name(objName);
	_active_model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0);
	_active_model->scale(100,100,100);
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
	}
}

void Scene::keyboard_special(int key, int x, int y)
{
	if (_active_model == NULL)
	{
		return;
	}

	static const float MOVE_DISTANCE = 0.02f;
	static const float LARGER_SCALE_FACTOR = 1.4;
	static const float SMALLER_SCALE_FACTOR = 0.6;
	static const float NO_SCALE = 1;
	static const float THETA = 0.1;
	//DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogBoxCallback);
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (scale_flag) _active_model->scale(LARGER_SCALE_FACTOR, NO_SCALE, NO_SCALE);
		else if (rotate_flag) _active_model->rotate(THETA, 'x');
		else _active_model->translate(MOVE_DISTANCE, 0, 0);
		break;
	case GLUT_KEY_LEFT:
		if (scale_flag)  _active_model->scale(SMALLER_SCALE_FACTOR, NO_SCALE, NO_SCALE);
		else if (rotate_flag) _active_model->rotate(-THETA, 'x');
		else _active_model->translate(-MOVE_DISTANCE, 0, 0);
		break;
	case GLUT_KEY_UP:
		if (scale_flag) _active_model->scale(NO_SCALE, LARGER_SCALE_FACTOR, NO_SCALE);
		else if(rotate_flag) _active_model->rotate(THETA, 'y');
		else _active_model->translate(0, MOVE_DISTANCE, 0);
		break;
	case GLUT_KEY_DOWN:
		if (scale_flag) _active_model->scale(NO_SCALE, SMALLER_SCALE_FACTOR, NO_SCALE);
		else if (rotate_flag) _active_model->rotate(-THETA, 'y');
		else _active_model->translate(0, -MOVE_DISTANCE, 0);
		break;
	case GLUT_KEY_PAGE_UP:
		_active_model->rotate(THETA, 'z');
		break;
	case GLUT_KEY_PAGE_DOWN:
		_active_model->rotate(-THETA, 'z');
		break;
	}

	redraw_necessary();
}
