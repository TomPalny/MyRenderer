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
	float *_buffer; // 3*width*height
	float *_zbuffer; // width*height
	float _r, _g, _b; // TODO: remove this?
	MaterialPtr _material;
	int _rotating_color; // used to assigned static colors to faces
	Camera* _camera;
	FillType _fill_type;
	vector<Light*> _lights;

	void draw_line_implementation(vec2 point1, vec2 point2, bool inverted);
	vec2 viewport_to_screen_coordinates(vec2 point);

	bool point_in_range(int x, int y);
	bool point_in_range(vec4 point);

	void draw_point(int x, int y);
	void draw_line(vec2 point1, vec2 point2);

	//////////////////////////////
	// openGL stuff. Don't touch.
	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////

public:
	Renderer(int width, int height);
	~Renderer(void);

	/*void Init();
	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* normals=NULL);
	void SetCameraTransform(const mat4& cTransform);
	void SetProjection(const mat4& projection);
	void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
	
	void ClearColorBuffer();
	void ClearDepthBuffer();*/

	void swap_buffers();
	void clear_screen();
	void set_color(float r, float g, float b);
	void set_material(MaterialPtr material);
	void set_parameters(Camera* camera, FillType fill_type, std::vector<Light*> lights);
	void set_window_size(int width, int height);

	void draw_line(vec4 point1, vec4 point2);
	void draw_string(const char* string, int left, int bottom);
	void draw_letter(char letter, int left, int bottom);
	void draw_letter(char letter, vec4 point);
	void assign_rotating_color();
	float calculate_color(int channel, MaterialPtr light, float diffuse_modifier, float specular_modifier);
	vec3 get_gouraud_lighting_for_point(vec4 point, vec4 normal, const mat4& model, const mat4& view);
	vec3 get_phong_normal(vec4 point, vec4 normal, const mat4& model, const mat4& view);
	vec3 get_lighting_for_point(vec4 point, const vec3& N, const mat4& model, const mat4& view);
	void setup_flat_lighting(const Face& face, const mat4& model, const mat4& view);
	void setup_gouraud_lighting(const Face& face, const mat4& model, const mat4& view);
	void draw(Model* model);
	void draw_model_filled(Model* model);
	void draw_model_wireframe(Model* model);
	void draw_face_normals(Model* model);
	float get_aspect_ratio();

	bool _fog_enabled;
	bool _supersampling;
	int _width, _height;
};
