#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

class Light {

};

enum NormalType
{
	NO_NORMALS,
	VERTEX_NORMALS,
	FACE_NORMALS,
	NUMBER_OF_NORMAL_TYPES
};

enum OperationMode
{
	TRANSLATE_MODE,
	ROTATE_MODE,
	SCALE_MODE
};

class Scene {
	Model* _active_model;
	Camera* _active_camera;
	vector<Model*> _models; // TODO: we need to free these in the dtor
	vector<bool> _bounding_boxes;
	vector<Camera*> _cameras;
	vector<Light*> _lights;
	Renderer *_renderer;
	NormalType _normal_type;
	OperationMode _operation_mode;
	TransformMode _transform_mode;
	CameraMode _camera_mode;
	ptrdiff_t _bounding_box_index = 0;
	bool _wireframe_mode;

	mat4 get_operation_for_keyboard(int key, int x, int y);
	static int get_reverse_key(int key);


public:
	explicit Scene(Renderer* renderer);
	void set_normals_type(NormalType normal_type);
	void open_file();;

	void add_pyramid_model();
	void load_model_at_center(Model* model, const string name);

	void draw_status_string();
	void draw();
	void switch_active_model(int id);
	static void redraw_necessary();
	void draw_one_model(Model* model, bool draw_bounding_box);
	void add_objects_to_menu();
	void set_operation_mode(OperationMode mode);

	void switch_camera(unsigned int number);
	void keyboard(unsigned char key, int x, int y);
	void keyboard_special(int key, int x, int y);

	int activeLight;
};