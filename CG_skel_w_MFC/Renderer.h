#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"

class Material;
using namespace std;

class Renderer
{
	MaterialPtr _material;
	Camera* _camera;
	FillType _fill_type;
	vector<Light*> _lights;
	int _width, _height;
	GLuint _shader_program;

public:
	Renderer(int width, int height);
	~Renderer(void);

	void swap_buffers();
	void clear_screen();
	void set_material(MaterialPtr material);
	void set_parameters(Camera* camera, FillType fill_type, std::vector<Light*> lights);
	void set_window_size(int width, int height);
	void draw_string(const char* string, int left, int bottom);
	void draw_letter(char letter, int left, int bottom);
	void draw_letter(char letter, vec4 point);
	void draw(Model* model);
	float get_aspect_ratio();
};
