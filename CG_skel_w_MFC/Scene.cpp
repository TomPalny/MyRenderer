#include "stdafx.h"
#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;
void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
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

void Scene::drawDemo()
{
	_renderer->SetDemoBuffer();
	_renderer->SwapBuffers();
}
