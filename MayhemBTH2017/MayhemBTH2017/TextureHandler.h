#ifndef __TEXTUREHANDLER_H__
#define __TEXTUREHANDLER_H__

#define STB_IMAGE_IMPLEMENTATION

#include "Texture.h"

#include <cstdint>
#include <cstdlib>
#include <glew.h>


class TextureHandler
{
public:
	TextureHandler();
	virtual ~TextureHandler();

	Texture& Import(const char* filepath);
};


#endif // __TEXTUREIMPORTER_H__