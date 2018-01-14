#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "GL\gl.h"
#include "font8x8_basic.h"
#include "Camera.h"
#include "Utils.h"
#include "Light.h"
#include "Material.h"

#define INDEX(width,x,y,c) (((x)+(y)*(width))*3+(c))
#define ZINDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int width, int height) : _width(width), _height(height), _material(Material::get_default())
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	set_window_size(width,height);
}

Renderer::~Renderer(void)
{
}

void Renderer::set_window_size(int width, int height)
{
	_width=width;
	_height = height;
	glViewport(0, 0, _width, _height);
}

void Renderer::draw(Model* model)
{
	auto modelm = model->get_transforms();
	auto view = _camera->get_view_matrix();
	auto projection = _camera->get_projection_matrix();

	for (auto kv : model->get_vaos())
	{
		auto vao = kv.second;
		auto shader = vao->get_shader_program();
		shader->set_uniform_attribute("modelView", view * modelm);
		shader->set_uniform_attribute("projection", projection);
		shader->set_uniform_attribute("light.position", view * _lights[0]->get_origin_in_world_coordinates());
		shader->set_uniform_attribute("light.ambient", vec4(_lights[0]->_material->_ambient));
		shader->set_uniform_attribute("light.diffuse", vec4(_lights[0]->_material->_diffuse));
		shader->set_uniform_attribute("light.specular", vec4(_lights[0]->_material->_specular));

		shader->set_uniform_attribute("material.ambient", model->_material->_ambient);
		shader->set_uniform_attribute("material.diffuse", model->_material->_diffuse);
		shader->set_uniform_attribute("material.specular", model->_material->_specular);
		shader->set_uniform_attribute("material.emissive", model->_material->_emissive);
		vao->draw();
	}
}

float Renderer::get_aspect_ratio()
{
	return (float) _width / _height;
}

void Renderer::clear_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_material(MaterialPtr material)
{
	_material = material;
}

void Renderer::set_parameters(Camera* camera, FillType fill_type, std::vector<Light*> lights)
{
	_camera = camera;
	_fill_type = fill_type;
	_lights = lights;
}

void Renderer::draw_string(const char* string, int left, int bottom)
{
	while (*string != '\0')
	{
		draw_letter(*string, left, bottom);
		++string;
		left += 10;
	}
}

void Renderer::draw_letter(char letter, int left, int bottom)
{
	char* letter_data = font8x8_basic[letter];
	for (int x = 0; x<8; x++)
	{
		for (int y = 0; y<8; y++)
		{
			if (letter_data[y] & 1 << (x))
			{
				//draw_point(left + x, bottom + 8 - y);
			}
		}
	}
}

void Renderer::draw_letter(char letter, vec4 point)
{
	// TODO
}

void Renderer::swap_buffers()
{
	glFlush();
	glutSwapBuffers();
}