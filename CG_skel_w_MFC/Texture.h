#pragma once
#include <vector>
#include <memory>

enum TEXTURE_UNITS {
	PRIMARY_TEXTURE_UNIT = 0,
	BUMP_MAPPING_TEXTURE_UNIT = 1,
};

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture
{
public:
	~Texture();
	void activate();
	static TexturePtr load_texture(const std::string& filename);
	static TexturePtr load_bumpmap(const std::string& filename);
	static TexturePtr load_noise();
	static TexturePtr load_cubemap(const std::string& filename_prefix);
	bool is_cubemap();

private:
	explicit Texture(int texture_unit, int type);
	void add_image(const std::string& filename, const int type, bool flipped);
	GLuint _texture_id;
	int _texture_unit;
	int _type;

private:
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) noexcept = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) noexcept = delete;
};

