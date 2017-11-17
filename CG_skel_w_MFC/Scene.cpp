#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "GL/freeglut.h"

using namespace std;
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

void Scene::draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all _models to draw themselves
	for(auto model : _models )
	{
		model->draw();
	}
	_renderer->SwapBuffers();
}

void Scene::draw_demo()
{
	_renderer->SetDemoBuffer();
	_renderer->SwapBuffers();
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
	switch (key) {
	case GLUT_KEY_LEFT:
		break;
	}
}
