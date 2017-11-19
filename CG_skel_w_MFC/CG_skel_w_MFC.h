#pragma once

#include "resource.h"

void display( void );
void reshape( int width, int height );
void keyboard( unsigned char key, int x, int y );
void mouse(int button, int state, int x, int y);
void file_menu(int id);
void menu_callback(int id);
void init_menu();