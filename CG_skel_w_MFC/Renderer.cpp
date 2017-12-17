#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "font8x8_basic.h"
#include "Camera.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer(int width, int height) :
	_buffer(NULL), _width(width), _height(height), _r(1.0), _g(1.0), _b(1.0)
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
	}
	_width=width;
	_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	_buffer = new float[3*_width*_height];
}

float* Renderer::get_buffer()
{
	return _buffer;
}

int Renderer::get_width()
{
	return _width;
}

int Renderer::get_height()
{
	return _height;
}

void Renderer::draw_point(const vec2 point)
{
	draw_point(point.x, point.y);
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
	return vec2(_width / 2.0f * (point.x + 1), _height / 2.0f * (point.y + 1));
}

// draws a line, first performing viewport transformation
void Renderer::draw_line_v(vec2 point1, vec2 point2)
{
	point1 = viewport_to_screen_coordinates(point1);
	point2 = viewport_to_screen_coordinates(point2);
	// TODO: this isn't really correct - we just do it for performance reasons to 
	// avoid drawing really long and almost infinite lines
	// we need to find the point where an infinite line intercepts the screen 
	// and start drawing from that point
	/*if (!point_in_range(point1.x, point1.y) || !point_in_range(point2.x, point2.y))
	{
		return;
	}*/
	draw_line(point1, point2);
}


bool Renderer::canonical_point_in_range(vec3 point, CameraMode mode)
{
	if (fabs(point.x) > 1 || fabs(point.y) > 1 || fabs(point.z) > 1)
	{
		return false;
	}
	return true;
}

// draws a line, first performing clipping, w-normalization, and viewport transformation
void Renderer::draw_line_vcw(vec4 point1, vec4 point2, CameraMode mode)
{
	// TODO: this isn't really correct - we just do it for performance reasons to 
	// avoid drawing really long and almost infinite lines
	// we need to find the point where an infinite line intercepts the screen 
	// and start drawing from that point
	if (!canonical_point_in_range(point1.to_vec3_divide_by_w(), mode)
		|| !canonical_point_in_range(point2.to_vec3_divide_by_w(), mode))
	{
		return;
	}
	draw_line_v(point1.to_vec2(), point2.to_vec2());
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
		}
	}
}

void Renderer::set_camera(Camera* camera)
{
	_camera = camera;
}

void Renderer::set_color(float r, float g, float b)
{
	_r = r;
	_g = g;
	_b = b;
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

void Renderer::draw_letter_v(char letter, vec2 point)
{
	auto fixed_point = viewport_to_screen_coordinates(point);
	draw_letter(letter, fixed_point.x - 4, fixed_point.y - 4);
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