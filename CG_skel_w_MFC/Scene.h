#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include <filesystem>
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "RenderParameters.h"
#include "Light.h"

using namespace std;

class Scene {
	Renderer *_renderer;
	vector<Model*> _models; // TODO: we need to free these in the dtor
	vector<Camera*> _cameras;

	// these are all updates using keyboard shortcuts
	Model* _active_model;
	Camera* _active_camera;
	OperationMode _operation_mode;
	TransformMode _transform_mode;
	ProjectionType _projection_type;
	Model* _skybox_cube;

	mat4 get_operation_for_keyboard(int key, int x, int y);
	static int get_reverse_key(int key);

public:
	explicit Scene(Renderer* renderer);

	void draw();
	void draw_status_string();
	void load_model_at_center(Model* model, const string name);
	static void redraw_necessary();
	void add_objects_to_menu();
	void show_materials_dialog();
	void show_settings_dialog();
	void show_model_dialog();
	void show_camera_dialog();
	void keyboard(unsigned char key, int x, int y);
	void keyboard_special(int key, int x, int y);

	// right-click menu
	void add_pyramid_model();
	void add_light();
	void switch_active_model(int id);
	void open_file();
	void set_operation_mode(OperationMode mode);
	void switch_camera(unsigned int number);
};