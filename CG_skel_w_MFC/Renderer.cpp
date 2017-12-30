#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "font8x8_basic.h"
#include "Camera.h"
#include "Utils.h"
#include "Light.h"
#include "Material.h"

#define INDEX(width,x,y,c) (((x)+(y)*(width))*3+(c))
#define ZINDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int width, int height) :
	_buffer(NULL), _width(width), _height(height), _r(1.0), _g(1.0), _b(1.0), _rotating_color(1), 
	_material(Material::get_default()), _fog_enabled(false), _supersampling(false), _blur(false)
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

bool Renderer::get_fog()
{
	return _fog_enabled;
}

bool Renderer::get_antialiasing()
{
	return _supersampling;
}

bool Renderer::get_blur()
{
	return _blur;
}

void Renderer::update_settings(bool fog, bool antialising, bool blur)
{
	_fog_enabled = fog;
	_blur = blur;

	if (_supersampling)
	{
		_width /= 2;
		_height /= 2;
	}

	_supersampling = antialising;
	if (_supersampling)
	{
		_width *= 2;
		_height *= 2;
	}

	set_window_size(_width, _height);
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

		// for gouraud/phong lighting
		// the meaning of these change - for gouraud they are colors, for phong they are normal
		vec3 l1, l2, l3;
		// we do this now once per face
		// TODO: use some Shader object...
		if (_fill_type == FILL_FLAT)
			setup_flat_lighting(face, modelm, view);
		else if (_fill_type == FILL_GOURAUD)
		{
			// TODO: I don't think we're translating the normal right...
			// TODO: we get a cool effect but our vectors are definitely messed up
			l1 = get_gouraud_lighting_for_point(face.point1, face.normal1, modelm, view);
			l2 = get_gouraud_lighting_for_point(face.point2, face.normal2, modelm, view);
			l3 = get_gouraud_lighting_for_point(face.point3, face.normal3, modelm, view);	
		}
		else if (_fill_type == FILL_PHONG)
		{
			l1 = get_phong_normal(face.point1, face.normal1, modelm, view);
			l2 = get_phong_normal(face.point2, face.normal2, modelm, view);
			l3 = get_phong_normal(face.point3, face.normal3, modelm, view);
		}

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

				// TODO: gourard shading
				// we already have the normals as part of the face object
				// make sure those normals are right! they look correct in orthographic as long as I don't do non-uniform scaling


				// TODO: I think this is correct
				// This can happen if one point of the triangle is outside of the camera's range... I think
				if (z < -1 || z > 1)
					continue;

				_zbuffer[ZINDEX(_width, x, y)] = z;
				// try to visualize z-buffer
				z = clamp(z, -1, 1);
				if (_fill_type == FILL_ZBUFFER)
					set_color(1-z, 1-z, 1-z);
				else if (_fill_type == FILL_GOURAUD)
				{
					vec3 color = alpha1 * l1 + alpha2 * l2 + alpha3 * l3;
					set_color(color.x, color.y, color.z);
				}
				else if (_fill_type == FILL_PHONG)
				{
					vec3 normal = alpha1 * l1 + alpha2 * l2 + alpha3 * l3;
					vec4 point = alpha1 * face.point1 + alpha2 * face.point2 + alpha3 * face.point3; // TODO: is this right?
					vec3 color = get_lighting_for_point(point, normal, modelm, view);
					set_color(color.x, color.y, color.z);
				}
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

float Renderer::calculate_color(int channel, MaterialPtr light, float diffuse_modifier, float specular_modifier)
{
	float I_a = max(0, _material->_ambient[channel] * light->_ambient[channel]);
	float I_d = max(0, _material->_diffuse[channel] * light->_diffuse[channel] * diffuse_modifier);
	float I_s = max(0, _material->_specular[channel] * light->_specular[channel] * specular_modifier);

	float result = I_d + I_a + I_s;
	return max(0, min(1, result));
}

vec3 Renderer::get_gouraud_lighting_for_point(vec4 model_point, vec4 model_normal, const mat4& model, const mat4& view)
{
	auto transformed_start = model * view * model_point;
	auto transformed_end = model * view * (model_point + model_normal);
	//auto transformed_normal = normalize((transformed_end - transformed_start).to_vec3());
	auto transformed_normal = normalize((model * view * model_normal).to_vec3());
	// TODO: refactor get_lighting_for_point so that we just pass in the point after transformation
	return get_lighting_for_point(model_point, transformed_normal, model, view);
}

vec3 Renderer::get_phong_normal(vec4 point, vec4 normal, const mat4& model, const mat4& view)
{
	auto transformed_start = model * view * point;
	auto transformed_end = model * view * (point + normal);
	//auto transformed_normal = normalize((transformed_end - transformed_start).to_vec3());
	auto transformed_normal = normalize((model * view * normal).to_vec3());
	return transformed_normal;
}

// TODO: we can optimize away a lot of these matrix calculations
// TODO: add gui for changing light parameters
// TODO: emissive colors
vec3 Renderer::get_lighting_for_point(vec4 point, const vec3& N, const mat4& model, const mat4& view)
{
	vec3 result(0, 0, 0);
	point = view * model * point;
	_material->update_if_special(point);

	for (auto lighto : _lights)
	{
		vec4 light = view * lighto->get_origin_in_world_coordinates();
		vec3 L;
		if (lighto->light_type == POINT_LIGHT)
			L = normalize((light - point).to_vec3());
		else
			L = normalize(view * lighto->get_transforms() * vec4(1, 1, 1, 0)).to_vec3();

		vec3 V = normalize(point.to_vec3());
		// TODO: this is the negative of what the wikipedia page says, but it defintely is wrong the other way
		// Is L backwards?
		vec3 R = normalize(L - 2 * dot(L, N) * N);
		float alpha = 5;
		float diffuse_modifier = dot(L, N);
		float specular_modifier = pow(dot(R, V), alpha);

		result += vec3(calculate_color(0, lighto->_material, diffuse_modifier, specular_modifier),
			calculate_color(1, lighto->_material, diffuse_modifier, specular_modifier),
			calculate_color(2, lighto->_material, diffuse_modifier, specular_modifier));
	}

	result += _material->_emissive;
	if (_fog_enabled)
	{
		// point.z is negative for visible points
		//result += (point.z + 1) * vec3(0.5, 0.5, 0.5);
		result /= pow(fabs(point.z), 1.5f);
	}
	result.x = max(0, min(1, result.x));
	result.y = max(0, min(1, result.y));
	result.z = max(0, min(1, result.z));

	return result;
}

void Renderer::setup_flat_lighting(const Face& face, const mat4& model, const mat4& view)
{
	vec4 vector1 = (view * model) * (face.point2 - face.point1);
	vec4 vector2 = (view * model) * (face.point3 - face.point2);
	vec4 center_of_triangle =  (face.point1 + face.point2 + face.point3) / 3;
	_material->update_if_special(center_of_triangle);
	vec3 N = normalize(cross(vector1, vector2));
	vec3 lighting = get_lighting_for_point(center_of_triangle, N, model, view);
	set_color(lighting.x, lighting.y, lighting.z);
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
	if (_supersampling)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width / 2, _height / 2, 0, GL_RGB, GL_FLOAT, NULL);
		glViewport(0, 0, _width / 2, _height / 2);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
		glViewport(0, 0, _width, _height);
	}
}

float* Renderer::supersampling()
{
	float* buffer = new float[3 * _width * _height / 4];

	for (int x = 0; x<_width / 2; x++)
	{
		for (int y = 0; y<_height / 2; y++)
		{
			for (int c = 0; c<3; c++)
			{
				buffer[INDEX((_width / 2), x, y, c)] = (_buffer[INDEX(_width, x * 2, y * 2, c)] +
					_buffer[INDEX(_width, x * 2 + 1, y * 2, c)] +
					_buffer[INDEX(_width, x * 2 + 1, y * 2 + 1, c)] +
					_buffer[INDEX(_width, x * 2, y * 2 + 1, c)]) / 4.0f;
			}
		}
	}

	return buffer;
}

void Renderer::blur(float* buffer, int width, int height)
{
	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			for (int c = 0; c<3; c++)
			{
				buffer[INDEX((width), x, y, c)] = (
					// first column
					buffer[INDEX(width, x - 1, y - 1, c)] +
					buffer[INDEX(width, x - 1, y - 0, c)] + 
					buffer[INDEX(width, x - 1, y + 1, c)] + 
					// second column
					buffer[INDEX(width, x - 0, y - 1, c)] +
					buffer[INDEX(width, x - 0, y - 0, c)] +
					buffer[INDEX(width, x - 0, y + 1, c)] +
					// third column
					buffer[INDEX(width, x + 1, y - 1, c)] +
					buffer[INDEX(width, x + 1, y - 0, c)] +
					buffer[INDEX(width, x + 1, y + 1, c)]
					) / 9.0f;
			}
		}
	}
}
void Renderer::swap_buffers()
{
	float* buffer;
	int width, height;
	if (_supersampling)
	{
		buffer = supersampling();
		width = _width / 2;
		height = _height / 2;
	}
	else
	{
		buffer = _buffer;
		width = _width;
		height = _height;
	}

	if (_blur)
	{
		blur(buffer, width, height);
	}

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();

	if (_supersampling)
	{
		delete buffer;
	}
}