#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"

class Material;
using namespace std;

class Renderer
{
	Camera* _camera;
	vector<Light*> _lights;
	int _width, _height;
	GLuint _shader_program;
	float _animation_param;

public:
	Renderer(int width, int height);
	~Renderer(void);

	void swap_buffers();
	void clear_screen();
	void set_parameters(Camera* camera, std::vector<Light*> lights);
	void set_window_size(int width, int height);
	void draw(Model* model);
	void draw_vao(Model* model, VAOType type, const shared_ptr<VAO>& vao);
	float get_aspect_ratio();
};
