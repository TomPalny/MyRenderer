#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "Camera.h"

using namespace std;
class Renderer
{
	float *_buffer; // 3*width*height
	float *_zbuffer; // width*height
	int _width, _height;
	float _r, _g, _b;

	void draw_line_implementation(vec2 point1, vec2 point2, bool inverted);
	vec2 viewport_to_screen_coordinates(vec2 point);

	bool point_in_range(int x, int y);
	bool point_in_range(vec4 point);

	void draw_point(int x, int y);
	void draw_line(vec2 point1, vec2 point2);
	void draw_line(vec4 point1, vec4 point2);

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
	void set_window_size(int width, int height);
	int get_width();
	int get_height();

	void draw_string(const char* string, int left, int bottom);
	void draw_letter(char letter, int left, int bottom);
	void draw_letter(char letter, vec4 point);
	void draw_model(Model* model, Camera* camera, CameraMode camera_mode);
};
