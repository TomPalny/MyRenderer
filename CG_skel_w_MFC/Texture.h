#pragma once
#include <vector>
#include <memory>

enum TEXTURE_UNITS {
	NORMAL_TEXTURE_UNIT = 0,
	TURBULENCE_TEXTURE_UNIT = 1
};

class Texture
{
public:
	explicit Texture(const std::string& filename, int texture_unit=NORMAL_TEXTURE_UNIT);
	~Texture();
	void activate();

private:
	std::vector<unsigned char> _buffer;
	GLuint _texture_id;
	int _texture_unit;

private:
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) noexcept = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) noexcept = delete;
};

typedef std::shared_ptr<Texture> TexturePtr;

