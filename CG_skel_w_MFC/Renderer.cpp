#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "font8x8_basic.h"
#include "Camera.h"
#include "Utils.h"
#include "Light.h"

#define INDEX(width,x,y,c) ((x+y*width)*3+c)
#define ZINDEX(width,x,y) (x+y*width)

Renderer::Renderer(int width, int height) :
	_buffer(NULL), _width(width), _height(height), _r(1.0), _g(1.0), _b(1.0), _rotating_color(1)
{
	InitOpenGLRendering();
	set_window_size(width,height);
}

Renderer::~Renderer(void)
{
}

void Renderer::set_window_size(int width, int height)
{
	if (_buffer != NULL)
	{
		delete _buffer;
		delete _zbuffer;
	}

	_width=width;
	_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	_buffer = new float[3 * _width * _height];
	_zbuffer = new float[_width * _height];
}

bool Renderer::point_in_range(int x, int y)
{
	if (x < 0 || x >= _width || y < 0 || y >= _height)
	{
		return false;
	}
	return true;
}

void Renderer::draw_point(int x, int y)
{
	if (!point_in_range(x, y))
	{
		return;
	}
	_buffer[INDEX(_width, x, y, 0)] = _r;
	_buffer[INDEX(_width, x, y, 1)] = _g;
	_buffer[INDEX(_width, x, y, 2)] = _b;
}

void Renderer::draw_line_implementation(vec2 point1, vec2 point2, const bool inverted)
{
	if(point1.x > point2.x)
	{
		swap(point1.x, point2.x);
		swap(point1.y, point2.y);
	}
	
	int y = point1.y;
	float dx = point2.x - point1.x;
	float dy = fabs(point2.y - point1.y);
	float error = dx / 2.0f;
	int ystep = (point1.y < point2.y) ? 1 : -1;
	
	for (int x = point1.x; x <= point2.x ; x++)
	{
		if(inverted)
		{
			draw_point(y, x);
		}
		else
		{
			draw_point(x, y);
		}
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

// transforms points from the canonical view volumn to screen coordinates
vec2 inline Renderer::viewport_to_screen_coordinates(vec2 point)
{
	// TODO: we should probably round these
	float x = _width / 2.0f * (point.x + 1);
	float y = _height / 2.0f * (point.y + 1);
	return vec2(x, y);
}

bool Renderer::point_in_range(vec4 point)
{
	if (fabs(point.x) > fabs(point.w) || 
		fabs(point.y) > fabs(point.w) || 
		fabs(point.z) > fabs(point.w))
	{
		return false;
	}
	return true;
}

// see https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
// there seems to be a mistake in that post with the edge function for counter clockwise points so that is 
// fixed here
float double_area(vec2 v0, vec2 v1, vec2 p)
{
	return (v1.x - v0.x) * (p.y - v0.y) - (v1.y - v0.y) * (p.x - v0.x);
}

void Renderer::draw(Model* model)
{
	if (_fill_type == FILL_WIREFRAME)
		draw_model_wireframe(model);
	else
		draw_model_filled(model);
}

void Renderer::draw_model_filled(Model* model)
{
	_rotating_color = 1;
	set_color(1, 1, 1);
	mat4 projection = _camera->get_projection_matrix();
	mat4 view = _camera->get_view_matrix();
	mat4 modelm = model->get_transforms();
	mat4 modelview = view * modelm;
	mat4 total_transform = projection * modelview;

	for (int i = 0; i < model->get_faces()->size(); i++)
	{
		const auto& face = model->get_faces()->at(i);
		const auto point1 = total_transform * face.point1;
		const auto point2 = total_transform * face.point2;
		const auto point3 = total_transform * face.point3;

		// we have to do this now before we skip faces that wont be displayed
		if (_fill_type == FILL_RANDOM_COLORS)
			assign_rotating_color();
		
		// TODO: do we need to bother with this?
		// min_x, max_x, min_y, max_y and the z-range check accomplish this later...
		if (!point_in_range(point1) && !point_in_range(point2) && !point_in_range(point3))
			continue;

		auto p1 = viewport_to_screen_coordinates(point1.to_vec2_divide_by_w());
		auto p2 = viewport_to_screen_coordinates(point2.to_vec2_divide_by_w());
		auto p3 = viewport_to_screen_coordinates(point3.to_vec2_divide_by_w());

		int min_x = max(0, min3(p1.x, p2.x, p3.x));
		int max_x = min(_width - 1, max3(p1.x, p2.x, p3.x));

		int min_y = max(0, min3(p1.y, p2.y, p3.y));
		int max_y = min(_height - 1, max3(p1.y, p2.y, p3.y));

		// we do this now once per face
		if (_fill_type == FILL_FLAT)
			setup_lighting(face, modelm, view);

		float total_double_area = double_area(p1, p2, p3);
		for (int y = min_y; y <= max_y; ++y)
		{
			for (int x = min_x; x <= max_x; ++x)
			{
				// for performance reasons we calculate these only if necessary
				const vec2 p(x, y);
				float alpha3 = double_area(p1, p2, p);
				if (alpha3 < 0)
					continue;

				float alpha1 = double_area(p2, p3, p);
				if (alpha1 < 0)
					continue;

				float alpha2 = double_area(p3, p1, p);
				if (alpha2 < 0)
					continue;

				alpha1 /= total_double_area;
				alpha2 /= total_double_area;
				alpha3 /= total_double_area;

				assert(total_double_area >= 0);
				assert(alpha1 >= 0);
				assert(alpha2 >= 0);
				assert(alpha3 >= 0);

				float z =  alpha1 * (point1.z / point1.w) +
						   alpha2 * (point2.z / point2.w) +
						   alpha3 * (point3.z / point3.w);

				if (_zbuffer[ZINDEX(_width, x, y)] <= z)
				{
					continue;
				}

				// TODO: I think this is correct
				// This can happen if one point of the triangle is outside of the camera's range... I think
				if (z < -1 || z > 1)
					continue;

				_zbuffer[ZINDEX(_width, x, y)] = z;
				// try to visualize z-buffer
				z = clamp(z, -1, 1);
				if (_fill_type == FILL_ZBUFFER)
					set_color(1-z, 1-z, 1-z);
				draw_point(x, y);
			}
		}
	}
}

void Renderer::draw_model_wireframe(Model* model)
{
	_rotating_color = 1;
	mat4 projection = _camera->get_projection_matrix();
	mat4 modelview = _camera->get_view_matrix() * model->get_transforms();
	mat4 total_transform = projection * modelview;
	set_color(1, 1, 1);
	for (int i = 0; i < model->get_faces()->size(); i++)
	{
		if (_fill_type == FILL_RANDOM_COLORS)
			assign_rotating_color();
		const auto& face = model->get_faces()->at(i);
		const auto point1 = total_transform * face.point1;
		const auto point2 = total_transform * face.point2;
		const auto point3 = total_transform * face.point3;

		draw_line(point1, point2);
		draw_line(point2, point3);
		draw_line(point3, point1);
	}
}

void Renderer::draw_face_normals(Model* model)
{
	mat4 total_transform = _camera->get_projection_matrix() * _camera->get_view_matrix() * model->get_transforms();

	for (const auto& face : *model->get_faces())
	{
		auto vector1 = face.point2 - face.point1;
		auto vector2 = face.point3 - face.point2;
		auto normal = vec4(normalize(cross(vector1, vector2)), 0);
		auto center_of_triangle = (face.point1 + face.point2 + face.point3) / 3;
		auto start = total_transform * center_of_triangle;
		auto end = total_transform * (center_of_triangle + normal * 0.2f);
		draw_line(start, end);
		//auto s = viewport_to_screen_coordinates(start.to_vec2_divide_by_w());
		//draw_point(s.x, s.y);
	}
}

float Renderer::get_aspect_ratio()
{
	return (float) _width / _height;
}

// draws a line, first performing viewport transformation
void Renderer::draw_line(vec4 point1, vec4 point2)
{
	// TODO: this isn't really correct - we just do it for performance reasons to 
	// avoid drawing really long and almost infinite lines
	// we need to find the point where an infinite line intercepts the screen 
	// and start drawing from that point
	if (!point_in_range(point1) || !point_in_range(point2))
	{
		return;
	}

	auto p1 = viewport_to_screen_coordinates(point1.to_vec2_divide_by_w());
	auto p2 = viewport_to_screen_coordinates(point2.to_vec2_divide_by_w());
	draw_line(p1, p2);
}

void Renderer::draw_line(vec2 point1, vec2 point2)
{
	bool inverted = false;
	
	// we draw from left to right, so start from the leftmost point
	// horizontal line
	if (fabs(point2.y - point1.y) == 0.0)
	{
		if (point1.x > point2.x)
		{
			swap(point1, point2);
		}
		for (int x = point1.x; x <= point2.x; x++)
		{
			draw_point(x, point1.y);
		}
		return;
	}
	// vertical line
	if (fabs(point2.x - point1.x) == 0.0 )
	{
		if (point1.y > point2.y)
		{
			swap(point1, point2);
		}
		for( int y = point1.y ; y <= point2.y ; y++)
		{
			draw_point(point1.x, y);
		}
		return;
	}
	
	if (fabs((point2.y-point1.y)/(point2.x-point1.x)) > 1)
	{
		point1 = vec2(point1.y, point1.x);
		point2 = vec2(point2.y, point2.x);
		inverted = true;	
	}

	draw_line_implementation(point1, point2, inverted);
}

void Renderer::clear_screen()
{
	for (int y = 0; y< _height; y++)
	{
		for(int x =0 ; x<_width ; x++)
		{
			_buffer[INDEX(_width, x, y, 0)] = 0;
			_buffer[INDEX(_width, x, y, 1)] = 0;
			_buffer[INDEX(_width, x, y, 2)] = 0;
			_zbuffer[ZINDEX(_width, x, y)] = FLT_MAX;
		}
	}
}

void Renderer::set_color(float r, float g, float b)
{
	_r = r;
	_g = g;
	_b = b;
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
				draw_point(left + x, bottom + 8 - y);
			}
		}
	}
}

void Renderer::draw_letter(char letter, vec4 point)
{
	if (!point_in_range(point))
	{
		return;
	}
	auto fixed_point = viewport_to_screen_coordinates(point.to_vec2_divide_by_w());
	draw_letter(letter, fixed_point.x - 4, fixed_point.y - 4);
}

void Renderer::assign_rotating_color()
{
	float r = 0.1 * (_rotating_color % 10);
	float g = 0.1 * ((_rotating_color / 10) % 10);
	float b = 0.1 * ((_rotating_color / 100) % 10);
	++_rotating_color;

	set_color(r, g, b);
	return;
}

// TODO: we can optimize away a lot of these matrix calculations
// TODO: add gui for changing light parameters
// TODO: add gui for changing material parameters
// TODO: add rgb vector
// TODO: emissive colors
void Renderer::setup_lighting(const Face& face, const mat4& model, const mat4& view)
{
	if (_lights.size() == 0)
	{
		set_color(0, 0, 0);
		return;
	}

	vec4 light = view * _lights[0]->get_origin_in_world_coordinates();

	vec4 vector1 = (view * model) * (face.point2 - face.point1);
	vec4 vector2 = (view * model) * (face.point3 - face.point2);
	vec4 center_of_triangle =  view * model * ((face.point1 + face.point2 + face.point3) / 3);

	vec3 N = normalize(cross(vector1, vector2));
	vec3 L = normalize((light - center_of_triangle).to_vec3());
	vec3 V = normalize(center_of_triangle.to_vec3()); // TODO: is this correct? Should we try to draw it?
	
	float k_a = 0.2; // ambient light for this material
	float k_d = 0.6; // diffuse light for this material

	float L_a = 0.2; // overall ambient light
	float L_d = 1; // overall diffuse light

	// TODO: specular
	float I_a = max(0, k_a * L_a);
	float I_d = max(0, k_d * (dot(L, N)) * L_d);

	float final = I_d + I_a;
	final = max(0, min(1, final));
	set_color(final, final, final);
}

/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::InitOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, _width, _height);
}

void Renderer::swap_buffers()
{
	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGB, GL_FLOAT, _buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}