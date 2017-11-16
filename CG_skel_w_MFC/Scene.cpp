#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;
void Scene::load_obj_model(string filename)
{
	MeshModel *model = new MeshModel(filename);
	model->translate(_renderer->get_width() / 2, _renderer->get_height() / 2, 0);
	model->scale(100, 100, 100);
	_models.push_back(model);
	model->set_renderer(_renderer);
	model->draw();
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
