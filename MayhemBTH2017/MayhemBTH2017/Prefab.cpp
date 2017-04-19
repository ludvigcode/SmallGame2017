#include "Prefab.h"



Prefab::Prefab()
{

}


Prefab::~Prefab()
{

}


void Prefab::Update()
{

}


void Prefab::Render(const Camera & cam)
{
	if (m_animController != nullptr)
	{
		m_animController->Update();
		m_shaderProg->UpdateAnim();
		m_shaderProg->Bind();
	}
	else if (m_shaderProg != nullptr)
	{ 
		m_shaderProg->Update();
		m_shaderProg->Bind();
	}

	if (m_mesh != nullptr)
	{
		m_mesh->Render();
	}
}


//::.. GET FUNCTIONS ..:://
const char * Prefab::GetName() const
{
	return m_name;
}


const Transform & Prefab::GetTransform() const
{
	return m_transform;
}


Mesh * Prefab::GetMesh() const
{
	return m_mesh;
}

ShaderManager * Prefab::GetShaderProg() const
{
	return m_shaderProg;
}

AnimController * Prefab::GetAnimController() const
{
	return m_animController;
}


//::.. SET FUNCTIONS ..:://
void Prefab::SetName(const char * name)
{
	m_name = name;
}


void Prefab::SetTransform(const Transform & transform)
{
	m_transform = transform;
}


void Prefab::SetMesh(Mesh * mesh)
{
	m_mesh = mesh;
}


void Prefab::SetAnimController(AnimController * animController)
{
	m_animController = animController;
}


//::.. HELP FUNCTIONS ..:://
void Prefab::Init()
{
	m_mesh				= nullptr;
	m_shaderProg		= nullptr;
	m_animController	= nullptr;
}
