#pragma once
#include <vector>
#include <memory>

class Texture
{
public:
	explicit Texture(const std::string& filename);
	~Texture();
	void activate();

private:
	std::vector<unsigned char> _buffer;
	GLuint _texture_id;

private:
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) noexcept = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) noexcept = delete;
};

typedef std::shared_ptr<Texture> TexturePtr;

