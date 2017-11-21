// CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define FILE_OPEN 1
#define MAIN_DEMO 2
#define MAIN_ABOUT 3
#define TRANSLATE_MODEL 4
#define SCALE_MODEL 5
#define ROTATE_MODEL 6
#define ADD_PYRAMID 7


Scene *scene;
Renderer *renderer;

int last_x=0,last_y=0;
bool lb_down,rb_down,mb_down;

//----------------------------------------------------------------------------
// Callbacks

void display( void )
{
	scene->draw();
}

void reshape( int width, int height )
{
	renderer->set_window_size(width, height);
	scene->redraw_necessary();
}

void keyboard( unsigned char key, int x, int y )
{
	scene->keyboard(key, x, y);
}

void keyboard_special(int key, int x, int y)
{
	scene->keyboard_special(key, x, y);
}

void mouse(int button, int state, int x, int y)
{
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}
	
	//set down flags
	switch(button) {
		case GLUT_LEFT_BUTTON:
			lb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_RIGHT_BUTTON:
			rb_down = (state==GLUT_UP)?0:1;
			break;
		case GLUT_MIDDLE_BUTTON:
			mb_down = (state==GLUT_UP)?0:1;	
			break;
	}
	
}

void motion(int x, int y)
{
	int dx = x - last_x;
	int dy = y - last_y;
	
	last_x = x;
	last_y = y;
}

void menu_callback(int id)
{
	switch (id)
	{
	case FILE_OPEN:
		scene->open_file();
		break;
	case ADD_PYRAMID:
		scene->add_pyramid_model();
		break;
	case MAIN_DEMO:
		scene->draw_demo();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;
	case SCALE_MODEL:
		scene->set_operation_mode(SCALE_MODE);
		break;
	case TRANSLATE_MODEL:
		scene->set_operation_mode(TRANSLATE_MODE);
		break;
	case ROTATE_MODEL:
		scene->set_operation_mode(ROTATE_MODE);
		break;
	}
}

void objects_menu_callback(const int id)
{
	scene->switch_active_model(id);
}

void normals_menu_callback(const int normal_type)
{
	scene->set_normals_type((NormalType)normal_type);
}

void init_menu()
{
	// TODO: do we leak the old menu every time that this is called?
	const int file_menu = glutCreateMenu(menu_callback);
	glutAddMenuEntry("Choose Object File:",FILE_OPEN);
	glutAddMenuEntry("Pyramid", ADD_PYRAMID);

	const int objects_menu = glutCreateMenu(objects_menu_callback);
	scene->add_objects_to_menu();

	const int normals_menu = glutCreateMenu(normals_menu_callback);
	glutAddMenuEntry("None", NO_NORMALS);
	glutAddMenuEntry("Vertex", VERTEX_NORMALS);
	glutAddMenuEntry("Face", FACE_NORMALS);

	glutCreateMenu(menu_callback);
	glutAddSubMenu("File",file_menu);
	glutAddSubMenu("Object To Control", objects_menu);
	glutAddSubMenu("Normals", normals_menu);
	glutAddMenuEntry("Translate", TRANSLATE_MODEL);
	glutAddMenuEntry("Scale", SCALE_MODEL);
	glutAddMenuEntry("Rotate", ROTATE_MODEL);
	glutAddMenuEntry("Demo",MAIN_DEMO);
	glutAddMenuEntry("About",MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int my_main( int argc, char **argv )
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE);
	glutInitWindowSize( 512, 512 );
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutCreateWindow( "CG" );
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	renderer = new Renderer(512,512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc(keyboard_special);
	glutMouseFunc( mouse );
	glutMotionFunc ( motion );
	glutReshapeFunc( reshape );
	init_menu();

	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main( int argc, char **argv )
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv );
	}
	
	return nRetCode;
}
