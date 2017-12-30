#include "stdafx.h"
#include "Material.h"
#include <fstream>
#include <sstream>
#include <string>


std::unordered_map<std::string, MaterialPtr> Material::_csv_materials;

Material::Material() : _name("Default"), _ambient(0.5, 0.5, 0.5), _diffuse(1.0, 1.0, 1.0), _specular(0.1, 0.1, 0.1), _emissive(0, 0, 0), _shininess(5)
{	
}

Material::Material(std::string name, vec3 ambient, vec3 diffuse, vec3 specular, float shininess) :
	_name(name), _ambient(ambient), _diffuse(diffuse), _specular(specular), _emissive(0,0,0), _shininess(shininess)
{
}

Material::~Material()
{
}

void Material::load_csv()
{
	std::string line;
	std::ifstream file("c:\\Projects\\technion\\graphics\\code\\materials.csv");
	std::getline(file, line); // skip header
	while (std::getline(file, line))
	{
		if (line == "")
			continue;

		std::string name;
		char c;
		float ambient_r, ambient_g, ambient_b, diffuse_r, diffuse_g, diffuse_b, specular_r, specular_g, specular_b, shininess;
		std::istringstream line_parser(line);
		std::getline(line_parser, name, ',');

		line_parser >> c >> ambient_r >> c >> ambient_g >> c >> ambient_b >> c
			>> diffuse_r >> c >> diffuse_g >> c >> diffuse_b >> c
			>> specular_r >> c >> specular_g >> c >> specular_b >> c >> shininess;

		_csv_materials[name] = std::make_shared<Material>(name,
			vec3(ambient_r, ambient_g, ambient_b),
			vec3(diffuse_r, diffuse_g, diffuse_b),
			vec3(specular_r, specular_g, specular_b),
			shininess);
	}
}

MaterialPtr Material::get_default()
{

	return std::make_shared<Material>();
}

MaterialPtr Material::get_default_white_light()
{
	//float L_a = 0.2; // overall ambient light
	//float L_d = 1.0; // overall diffuse light
	//float L_s = 1.0; // overall shininess?
	vec3 ambient(0.2, 0.2, 0.2);
	vec3 diffuse(1, 1, 1);
	vec3 specular(1, 1, 1);
	return std::make_shared<Material>("White Light", ambient, diffuse, specular, 1);
}

void Material::load_csv_if_necessary()
{
	if (_csv_materials.size() == 0)
	{
		load_csv();
	}
}

MaterialPtr Material::get_from_csv(std::string name)
{
	load_csv_if_necessary();
	return _csv_materials[name];
}

std::vector<std::string> Material::get_csv_material_names()
{
	load_csv_if_necessary();
	std::vector<std::string> names;
	for (auto kv : _csv_materials)
	{
		names.push_back(kv.first);
	}
	return names;
}


