#pragma once
#include "mat.h"
#include <memory>
#include <unordered_map>

class Material;
typedef std::shared_ptr<Material> MaterialPtr;

class Material
{
public:
	Material();
	Material(std::string name, vec3 ambient, vec3 diffuse, vec3 specular, float shininess);
	~Material();

	static void load_csv();
	static MaterialPtr get_default();
	static MaterialPtr get_default_white_light();
	static MaterialPtr get_from_csv(std::string name);
	static void load_csv_if_necessary();
	static std::vector<std::string> get_csv_material_names();

	std::string _name;
	vec3 _ambient;
	vec3 _diffuse;
	vec3 _specular;
	vec3 _emissive;
	float _shininess;

private:
	static std::unordered_map<std::string, MaterialPtr> _csv_materials;
};
