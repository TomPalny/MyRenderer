#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"
#include "RenderParameters.h"
#include "Light.h"

using namespace std;

class Scene {
	Renderer *_renderer;
	vector<Model*> _models; // TODO: we need to free these in the dtor
	vector<bool> _bounding_boxes;
	vector<Camera*> _cameras;
	vector<Light*> _lights;
	ptrdiff_t _bounding_box_index = 0;

	// these are all updates using keyboard shortcuts
	Model* _active_model;
	Camera* _active_camera;
	NormalType _normal_type;
	OperationMode _operation_mode;
	TransformMode _transform_mode;
	ProjectionType _projection_type;
	int _fovy = 35;
	FillType _fill_type;

	mat4 get_operation_for_keyboard(int key, int x, int y);
	static int get_reverse_key(int key);

public:
	explicit Scene(Renderer* renderer);

	void draw();
	void draw_status_string();
	void draw_one_model(Model* model, bool draw_bounding_box);
	void load_model_at_center(Model* model, const string name);
	static void redraw_necessary();
	void add_objects_to_menu();
	void change_material();
	void show_settings_window();
	void keyboard(unsigned char key, int x, int y);
	void keyboard_special(int key, int x, int y);

	// right-click menu
	void add_pyramid_model();
	void add_light();
	void switch_active_model(int id);
	void set_normals_type(NormalType normal_type);
	void open_file();
	void set_operation_mode(OperationMode mode);
	void switch_camera(unsigned int number);
};