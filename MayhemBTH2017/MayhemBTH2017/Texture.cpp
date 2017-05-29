#include "Texture.h"



Texture::Texture()
{
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

Texture::Texture(const Texture & other)
{
	m_height = other.GetHeight();
	m_texture = other.GetTexture();
	m_width = other.GetWidth();

	*this = other;

}

Texture & Texture::operator=(const Texture & other)
{
	m_width = other.m_width;
	m_texture = other.m_texture;
	m_height = other.m_height;
	return *this;
}




void Texture::LoadTexture(unsigned char * textureData, GLsizei width, GLsizei height, GLuint numComponents)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
		height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Bind(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

//::.. GET FUNCTIONS ..:://
GLuint Texture::GetHeight()const
{
	return m_height;
}

GLuint Texture::GetWidth()const
{
	return m_width;

}

GLuint Texture::GetTexture() const
{
	return m_texture;
}

