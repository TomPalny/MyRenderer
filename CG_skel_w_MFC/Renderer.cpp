#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "font8x8_basic.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

Renderer::Renderer(int width, int height) :m_width(width), m_height(height), m_outBuffer(NULL)
{
	InitOpenGLRendering();
	ChangeWindowSize(width,height);
}

Renderer::~Renderer(void)
{
}

void Renderer::ChangeWindowSize(int width, int height)
{
	if (m_outBuffer != NULL)
	{
		delete m_outBuffer;
	}
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
}

float* Renderer::GetBuffer()
{
	return m_outBuffer;
}

int Renderer::get_width()
{
	return m_width;
}

int Renderer::get_height()
{
	return m_height;
}

void Renderer::draw_point(const vec2 point)
{
	draw_point(point.x, point.y);
}

void Renderer::draw_point(int x, int y)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
	{
		return;
	}
	m_outBuffer[INDEX(m_width, x, y, 0)] = 1.0f;
	m_outBuffer[INDEX(m_width, x, y, 1)] = 1.0f;
	m_outBuffer[INDEX(m_width, x, y, 2)] = 1.0f;
}

void Renderer::draw_line_old(vec2 point1, vec2 point2)
{
	bool flipped = false;
	// we need a line equation - so swap x and y if necessary
	if (fabs(point2.x - point1.x) < 1)
	{
		// if this is a point and not a line
		if (fabs(point2.y - point1.y) < 1)
		{
			//draw_point(point1);
			return;
		}
		point1 = vec2(point1.y, point1.x);
		point2 = vec2(point2.y, point2.x);
		flipped = true;
	}

	// we draw from left to right, so start from the leftmost point
	if (point1.x >= point2.x)
	{
		swap(point1, point2);
	}

	const float m = (point2.y - point1.y) / (point2.x - point1.x);
	const float b = point1.y - m*point1.x;
	for (int x = point1.x; x <= point2.x; x++)
	{
		const int y = m*x + b;
		if (flipped)
		{
			draw_point(vec2(y, x));
		}
		else
		{
			draw_point(vec2(x, y));
		}
	}
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
	for (int y = 0; y< m_height; y++)
	{
		for(int x =0 ; x<m_width ; x++)
		{
			m_outBuffer[INDEX(m_width, x, y, 0)] = 0;
			m_outBuffer[INDEX(m_width, x, y, 1)] = 0;
			m_outBuffer[INDEX(m_width, x, y, 2)] = 0;
		}
	}
}

void Renderer::SetDemoBuffer()
{
	//vertical line
	for(int i=0; i<m_height; i++)
	{
		m_outBuffer[INDEX(m_width,m_width/2,i,0)]=0.5f;	
		m_outBuffer[INDEX(m_width,m_width/2,i,1)]=0.5f;	
		m_outBuffer[INDEX(m_width,m_width/2,i,2)]=0.5f;

	}
	//horizontal line
	for(int i=0; i<m_width; i++)
	{
		m_outBuffer[INDEX(m_width,i, m_height/2, 0)]=1;	
		m_outBuffer[INDEX(m_width,i, m_height/2, 1)]=0;
		m_outBuffer[INDEX(m_width,i, m_height/2 ,2)]=1;
	}
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
	int top = bottom + 40;
	for (int y = 0; y<8; y++)
	{
		for (int x = 0; x<8; x++)
		{
			if (letter_data[y] & 1 << (x))
			{
				draw_point(vec2(left + x, top - y));
			}
		}
	}
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}

void Renderer::SwapBuffers()
{

	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}