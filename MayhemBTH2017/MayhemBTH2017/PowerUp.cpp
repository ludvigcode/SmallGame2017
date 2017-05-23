#include "PowerUp.h"



PowerUp::PowerUp()
{

}

PowerUp::~PowerUp()
{

}

void PowerUp::Create(b2World* world, glm::vec2 pos)
{

	m_powerupPrefab = PrefabManager::Instantiate("Player", nullptr, nullptr, 0, "Player2");
	m_powerupPrefab->SetScale(glm::vec3(1.5));

	m_boundingBox.InitDynamic(world, pos, glm::vec2(m_powerupPrefab->GetScale().x, m_powerupPrefab->GetScale().y));
	SetActive(false);

	m_sprite.CreateSprite(glm::vec2(GetBox().getBody()->GetPosition().x - (m_powerupPrefab->GetScale().x / 2), GetBox().getBody()->GetPosition().y - (m_powerupPrefab->GetScale().y / 2)), glm::vec2(m_powerupPrefab->GetScale().x, m_powerupPrefab->GetScale().y));
	m_sprite.Init(".\\Assets\\GLSL\\ColliderShader", 0, 0);

	b2Filter filter;
	filter.categoryBits = POWERUP;
	filter.maskBits = BOUNDARY | PLAYER1 | PLAYER2;
	m_boundingBox.getFixture()->SetFilterData(filter);

	m_boundingBox.getBody()->SetUserData(this);

}

//::.. RENDER ..:://
void PowerUp::Render(Camera camera) {

	Transform transform;
	m_sprite.Bind();
	m_sprite.Update(transform, camera);
	m_sprite.Render();
	//m_powerupPrefab->Render(camera);

}

Box PowerUp::GetBox()
{
	return m_boundingBox;
}

Prefab* PowerUp::GetPrefab()
{
	return m_powerupPrefab;
}

bool PowerUp::GetActive()
{
	return m_active;
}

void PowerUp::CollidedWithPlayer(bool player)
{
	if (player)
	{
		m_collidedPlayer = true;
		return;
	}

	m_collidedPlayer = false;
}

void PowerUp::SetActive(bool active)
{
	m_active = active;
	m_boundingBox.getBody()->SetActive(active);
	m_boundingBox.getBody()->SetAwake(active);
}

void PowerUp::Update()
{

	GLfloat xPos = m_boundingBox.getBody()->GetPosition().x;
	GLfloat yPos = m_boundingBox.getBody()->GetPosition().y;
	GLfloat xScale = m_boundingBox.getScale().x;
	GLfloat yScale = m_boundingBox.getScale().y;

	m_powerupPrefab->SetPosition(glm::vec3(xPos, yPos, 0));

	m_sprite.ModifyPos(glm::vec2(xPos - (m_powerupPrefab->GetScale().x / 2), yPos - (m_powerupPrefab->GetScale().y / 2)), glm::vec2(m_powerupPrefab->GetScale().x, m_powerupPrefab->GetScale().y));

}

void PowerUp::Destroy()
{
	delete m_powerupPrefab;
	m_powerupPrefab = NULL;
}
