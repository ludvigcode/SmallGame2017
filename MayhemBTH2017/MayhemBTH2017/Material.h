#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "ShaderProg.h"
#include "Texture.h"
#include "SDL.h"


class Material
{
public:
	Material();
	virtual ~Material();

	void SetMaterial(const char * filepath);

private:
	ShaderProg	m_shaderProg;
	Texture		m_texture;

};

#endif // !__MATERIAL_H__