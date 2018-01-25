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
#include <sstream>

#define INDEX(width,x,y,c) (((x)+(y)*(width))*3+(c))
#define ZINDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int width, int height) : _width(width), _height(height), _animation_param(1)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glClearColor(0.0, 0.0, 0.0, 0.0);

    GLint multisample = 0;
    GLint num_samples = 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &multisample);
    glGetIntegerv(GL_SAMPLES, &num_samples);
	std::cout << "GL_SAMPLE_BUFFERS=" << multisample << "; GL_SAMPLES=" << num_samples << std::endl;
	
	_turbulence_texture = Texture::load_noise();
	_skybox_texture = Texture::load_cubemap("c:\\Projects\\technion\\graphics\\code\\skyboxes\\ame_oasis\\oasisnight");
	//_skybox_texture = Texture::load_cubemap("c:\\Projects\\technion\\graphics\\code\\skyboxes\\arrakisday\\arrakisday");
	//_skybox_texture = Texture::load_cubemap("c:\\Projects\\technion\\graphics\\code\\skyboxes\\storforsen\\");
	set_window_size(width,height);
}

Renderer::~Renderer(void)
{
}

TexturePtr Renderer::get_skybox_texture() const
{
	return _skybox_texture;
}

void Renderer::set_skybox_texture(const TexturePtr& skybox_texture)
{
	_skybox_texture = skybox_texture;
}
void Renderer::set_window_size(int width, int height)
{
	_width=width;
	_height = height;
	glViewport(0, 0, _width, _height);
}

void Renderer::draw(Model* model)
{
	for (auto kv : model->get_vaos())
	{
		auto type = kv.first;
		auto vao = kv.second;
		if (type == VAO_BOUNDING_BOX && !model->is_bounding_box_enabled())
		{
			continue;
		}
		else if (type == VAO_VERTEX_NORMALS && model->get_normal_type() != VERTEX_NORMALS)
		{
			continue;
		}
		else if (type == VAO_FACE_NORMALS && model->get_normal_type() != FACE_NORMALS)
		{
			continue;
		}
		draw_vao(model, type, vao);
	}
}

float Renderer::get_aspect_ratio()
{
	return (float) _width / _height;
}

void Renderer::clear_screen()
{
	static const int DURATION_MS = 3000;
	_animation_param = (GetTickCount() % DURATION_MS) / (float) DURATION_MS;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::set_parameters(Camera* camera, std::vector<Light*> lights)
{
	_camera = camera;
	_lights = lights;
}

void Renderer::draw_vao(Model* model, VAOType type, const shared_ptr<VAO>& vao)
{
	auto texture = model->get_texture();
	auto bump_texture = model->get_bump_texture();
	auto modelm = model->get_transforms();
	auto view = _camera->get_view_matrix();
	auto projection = _camera->get_projection_matrix();
	auto shader = vao->get_shader_program();

	if (model->is_environment_mapping())
	{
		_skybox_texture->activate();
		shader->set_uniform_attributei("cubemapTexture", PRIMARY_TEXTURE_UNIT);
		shader->set_uniform_attributei("textureType", (int)TEXTURE_ENVIRONMENT);
	}
	else if (model->is_marble_texture_enabled())
	{
		_turbulence_texture->activate();
		shader->set_uniform_attributei("turbulenceTexture", PRIMARY_TEXTURE_UNIT);
		shader->set_uniform_attributei("textureType", (int) TEXTURE_MARBLE);

	}
	else if (texture == nullptr)
	{
		shader->set_uniform_attributei("textureType", (int)TEXTURE_NONE);
	}
	else if (texture->is_cubemap())
	{
		texture->activate();
		shader->set_uniform_attributei("cubemapTexture", PRIMARY_TEXTURE_UNIT);
		shader->set_uniform_attributei("textureType", (int)TEXTURE_CUBEMAP);
	}
	else
	{
		texture->activate();
		shader->set_uniform_attributei("myTexture", PRIMARY_TEXTURE_UNIT);
		shader->set_uniform_attributei("textureType", (int)TEXTURE_FILE);		
	}

	// bump
	if (bump_texture != nullptr)
	{
		bump_texture->activate();
		shader->set_uniform_attributei("bumpTexture", BUMP_MAPPING_TEXTURE_UNIT);
		shader->set_uniform_attributei("hasBumpTexture", 1);
	}
	else
	{
		shader->set_uniform_attributei("hasBumpTexture", 0);
	}

	shader->set_uniform_attributei("uvType", (int) model->get_uv_type());
	shader->set_uniform_attributei("fillType", (int) model->get_fill_type());
	shader->set_uniform_attributei("positionAnimationType", (int) model->get_position_animation());
	shader->set_uniform_attributei("colorAnimationType", (int) model->get_color_animation());
	shader->set_uniform_attribute("modelView", view * modelm);
	shader->set_uniform_attribute("view", view);
	shader->set_uniform_attribute("modelViewProjection", projection * view * modelm);
	shader->set_uniform_attribute("projection", projection);
	shader->set_uniform_attributei("numLights", _lights.size());
	shader->set_uniform_attributef("animationParam", _animation_param);
	for (int i=0; i<_lights.size(); i++)
	{
		std::stringstream ss;
		ss << "lights[" << i << "].";
		shader->set_uniform_attribute(ss.str() + "ambient", vec4(_lights[i]->_material->_ambient));
		shader->set_uniform_attribute(ss.str() + "diffuse", vec4(_lights[i]->_material->_diffuse));
		shader->set_uniform_attribute(ss.str() + "specular", vec4(_lights[i]->_material->_specular));
		bool is_directional = _lights[i]->light_type == DIRECTIONAL_LIGHT;
		if (is_directional)
		{
			shader->set_uniform_attribute(ss.str() + "position", view * _lights[i]->get_transforms() * vec4(1,1,1,0));
			shader->set_uniform_attributei(ss.str() + "directional", 1);
		}
		else
		{
			shader->set_uniform_attribute(ss.str() + "position", view * _lights[i]->get_transforms() * vec4(0, 0, 0, 1));
			shader->set_uniform_attributei(ss.str() + "directional", 0);
		}
	}

	shader->set_uniform_attribute("material.ambient", model->_material->_ambient);
	shader->set_uniform_attribute("material.diffuse", model->_material->_diffuse);
	shader->set_uniform_attribute("material.specular", model->_material->_specular);
	shader->set_uniform_attribute("material.emissive", model->_material->_emissive);

	// render and deal with toon shading
	shader->set_uniform_attributei("toonShading", model->is_toon_shading_enabled());
	shader->set_uniform_attributei("toonShadingStage2", 0);
	if (model->is_skybox()) // skyboxes are drawn inside out
	{
		glCullFace(GL_FRONT);
	}
	else
	{
		glCullFace(GL_BACK);
	}
	vao->draw();

	if (model->is_toon_shading_enabled())
	{
		shader->set_uniform_attributei("toonShadingStage2", 1);
		glCullFace(GL_FRONT);
		vao->draw();
	}
}

void Renderer::swap_buffers()
{
	glFlush();
	glutSwapBuffers();
}
