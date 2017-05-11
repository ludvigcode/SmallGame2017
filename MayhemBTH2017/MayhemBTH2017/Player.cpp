#include "Player.h"

//::..CONSTRUCTORS..:://
Player::Player(b2World* world, glm::vec2 pos, glm::vec2 scale, int controllerID) {

	Init(world, pos, scale, controllerID);
	m_contact = false;

	m_dead = false;

	m_hitByProjectile = -1;

}

Player::Player()
{

}

Player::~Player()
{
}
//::..INITIALIZERS..:://
void Player::Init(b2World* world, glm::vec2 pos, glm::vec2 scale, int controllerID)
{

	//MARTIN TEST SHIT REMOVE
	//m_testCon = new PlayerController;
	//TEST END
	m_input = InputManager::Get();

	//Initiate the players bounding box
	m_contact = false;
	//Load player MESH

	m_world = world;

	m_playerPrefab = new PlayerPrefab(controllerID);

	m_testCon = new PlayerController;

	SetControllerID(controllerID);

	//m_playerPrefab->SetScale(glm::vec3(1.3));

	//SET BOUNDING BOX SIZE 
	m_boundingBox.InitDynamic(world, pos, glm::vec2(m_playerPrefab->GetPlayerPrefab()->GetScale().x + 1, m_playerPrefab->GetPlayerPrefab()->GetScale().y*m_playerPrefab->GetPlayerPrefab()->GetMesh()->GetHeight()));
	//sprite for size of bouding box
	m_playerSprite.Init(".\\Assets\\GLSL\\ColliderShader", 0, 0);
	//Load player shader
	//m_shader.Init(".\\Assets\\GLSL\\ToonShader", 0, 0);

	//GetBox().getFixture()->SetDensity(0.1);
	GetBox().getFixture()->SetFriction(10.0);
	GetBox().getFixture()->SetRestitution(0.0);
	GetBox().getBody()->SetLinearDamping(0.0);
	GetBox().getBody()->ResetMassData();

	b2Filter filter;
	if (m_controllerID == 0)
	{
		filter.categoryBits = PLAYER1;
		filter.maskBits = BOUNDARY | POWERUP | PROJECTILE2;
	}
	else if (m_controllerID == 1)
	{
		filter.categoryBits = PLAYER2;
		filter.maskBits = BOUNDARY | POWERUP | PROJECTILE1;
	}

	GetBox().getFixture()->SetFilterData(filter);

	GetBox().getBody()->SetUserData(this);

	m_collidedPowerUp = false;

	//set weapon


	m_healthBar = PrefabManager::Instantiate("lukas", nullptr, nullptr, 0, "Candle");

	m_healthBar->Create();

	Prefab * projectile = PrefabManager::Instantiate("Candle", nullptr, nullptr, 0, "Candle");

	projectile->SetScale(glm::vec3(1, 1, 1));

	//	m_weapon = Weapon(gun, projectile);
	m_weapon = Weapon(nullptr, projectile, m_controllerID);

	m_weapon.SetProjectileType(0.1f, 1.0f, 0.0f, 0.0f, 5.0f, 10, m_controllerID);
	m_weapon.InitParticleSystem(".\\Assets\\GLSL\\GeometryPass", glm::vec4(1.0, 1.0, 1.0, 1.0), 2.0f, 50, 5.0f);

	m_life = 1.0f;

	m_healthBar->SetScale(glm::vec3(1, 1, m_life * 5));
	m_healthBar->Rotate(glm::vec3(0.0, 90.0, 0.0));
	m_healthBar->SetPosition(glm::vec3(m_boundingBox.getBody()->GetPosition().x + 3, m_boundingBox.getBody()->GetPosition().y + 5, 0.0));

	//Set fixture 

}

void Player::Update() {

	m_healthBar->SetPosition(glm::vec3(m_boundingBox.getBody()->GetPosition().x + 3, m_boundingBox.getBody()->GetPosition().y + 5, 0.0));
	m_healthBar->SetPosition(glm::vec3(m_healthBar->GetPosition().x - m_life * 2.5f, m_healthBar->GetPosition().y, m_healthBar->GetPosition().z));

	if (m_input->GetAxisRaw(CONTROLLER_AXIS_TRIGGERRIGHT, m_controllerID) > 0.0001f)
	{
		if (m_weapon.FireRate(0.15))
		{
			m_weapon.Shoot(100.0f, m_world, glm::vec3(GetPrefab()->GetProjectileSpawnPoint().x, GetPrefab()->GetProjectileSpawnPoint().y, GetPrefab()->GetProjectileSpawnPoint().z), m_controllerID);
		}
	}


	if (m_contact)
	{
		if (m_collidedProjectile)
		{
			ScoreManager::AddHitScore(m_hitByProjectileID);
			m_life -= 0.1f;

			m_healthBar->SetPosition(glm::vec3(m_boundingBox.getBody()->GetPosition().x + 3, m_boundingBox.getBody()->GetPosition().y + 5, 0.0));
			m_healthBar->SetPosition(glm::vec3(m_healthBar->GetPosition().x - m_life * 2.5f, m_healthBar->GetPosition().y, m_healthBar->GetPosition().z));
			m_healthBar->SetScale(glm::vec3(1, 1, m_life * 5));


			std::cout << m_life << std::endl;
			if (m_life <= 0.0f)
			{
				ScoreManager::AddKill(m_hitByProjectileID);
				m_healthBar->SetScale(glm::vec3(1.0, 1.0, 0));
				ScoreManager::AddDeath(m_controllerID);
				m_dead = true;
			}
		}
		if (m_collidedPowerUp)
		{
			m_weapon.SetProjectileType(1.0f, 1.0f, 0.0f, 0.1f, 5.0f, 100, m_controllerID);
			PostProcessingManager::Get()->Update(2);
			m_collidedPowerUp = false;
		}
		m_contact = false;
	}
	else
	{
		PostProcessingManager::Get()->Update(0);
	}

	if (m_dead)
	{
		m_time += TimeManager::Get()->GetDeltaTime();
		Respawn(glm::vec2(70, 70));
		if (Timer(2))
		{
			Respawn(glm::vec2(40, 30));
			m_boundingBox.getBody()->ApplyForce(b2Vec2(1.0, 1.0), m_boundingBox.getBody()->GetWorldCenter(), true);
			m_life = 1.0;
			m_healthBar->SetScale(glm::vec3(1, 1, m_life * 5));
			m_dead = false;
		}
	}




	if (GetBox().getBody()->GetLinearVelocity().y != 0) {
		m_isMidAir = true;


	}
	else {
		m_isMidAir = false;
		m_doubleJump = false;
	}


	//controller input///////////////////////////////////////////


	//PLAYER MOVEMENT
	GLfloat leftVelocity = GetBox().getBody()->GetLinearVelocity().x * InputManager::Get()->GetAxis(CONTROLLER_AXIS_LEFT_X);


	// THIS STUFF WORKS

	if (m_input->GetAxisRaw(CONTROLLER_AXIS_LEFT_X, m_controllerID))
	{
	
		if (m_isMidAir) {

			GetBox().getBody()->ApplyForce(b2Vec2(m_input->GetAxisRaw(CONTROLLER_AXIS_LEFT_X, m_controllerID)*(-300)*TimeManager::Get()->GetDeltaTime(), 0), GetBox().getBody()->GetWorldCenter(), 1);

		}
		if (!m_isMidAir) {

			GetBox().getBody()->SetLinearVelocity(b2Vec2(m_input->GetAxisRaw(CONTROLLER_AXIS_LEFT_X, m_controllerID)*(-300)*TimeManager::Get()->GetDeltaTime(), 0));
		}


	}
	// ** //



	m_playerPrefab->Update(InputManager::Get()->GetAxisRaw(CONTROLLER_AXIS_RIGHT_X, m_controllerID),
		m_input->GetAxisRaw(CONTROLLER_AXIS_RIGHT_Y, m_controllerID),
		m_input->GetAxis(CONTROLLER_AXIS_LEFT_X, m_controllerID));


	if (m_input->GetButtonDown(CONTROLLER_BUTTON_LEFTBUTTON, m_controllerID) != 0.0f)
	{


		if (!m_isMidAir) {

			//First jump
			GetBox().getBody()->ApplyForce(b2Vec2(0, 230), GetBox().getBody()->GetWorldCenter(), 1);
			m_doubleJump = true;


			//m_player.GetBox().getBody()->ApplyLinearImpulse(b2Vec2(0, impulse), m_player.GetBox().getBody()->GetWorldCenter(), 1);
		}
	}

	//DOUBLE JUMP
	if (m_doubleJump && m_input->GetButtonDown(CONTROLLER_BUTTON_LEFTBUTTON, m_controllerID) != 0.0f && m_isMidAir)
	{
		m_doubleJump = false;
		GetBox().getBody()->ApplyForce(b2Vec2(0, 210), GetBox().getBody()->GetWorldCenter(), 1);

	}

	GLfloat xPos = GetBox().getBody()->GetPosition().x;
	GLfloat yPos = GetBox().getBody()->GetPosition().y;
	GLfloat xScale = GetBox().getScale().x;
	GLfloat yScale = GetBox().getScale().y;

	m_playerPrefab->GetPlayerPrefab()->SetPosition(glm::vec3(xPos + 0.5, yPos + GetBox().getScale().y - 6, 0));

	//////////////////////////////////////////////////////////

	m_weapon.Update(GetPrefab()->GetProjectileSpawnPoint(), b2Vec2(1.0, 1.0));

}

void Player::Respawn(glm::vec2 pos)
{
	m_boundingBox.getBody()->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
}

Box Player::GetBox()
{
	return m_boundingBox;
}

PlayerPrefab* Player::GetPrefab()
{
	return m_playerPrefab;
}

void Player::StartContact(bool projectile, bool powerup)
{
	m_contact = true;

	if (projectile)
	{
		m_collidedProjectile = true;
		m_collidedPowerUp = false;
	}
	if (powerup)
	{
		m_collidedPowerUp = true;
		m_collidedProjectile = false;
	}

}

void Player::EndContact()
{
}

int Player::GetControllerID()
{
	return m_controllerID;
}

void Player::UpdateParticles()
{

	m_weapon.UpdateParticles();
}

Prefab * Player::GetHealthBar()
{
	return m_healthBar;
}



//::.. SET FUNCTIONS ..:://
void Player::SetCategoryBits(short CATEGORY) {

	m_fixture.filter.categoryBits = CATEGORY;
}
void Player::SetMaskBits(short MASK) {

	m_fixture.filter.maskBits = MASK;

}

bool Player::Timer(float rate)
{
	if (m_time >= rate)
	{
		m_time = 0;
		return true;
	}
	return false;
}

void Player::SetControllerID(int ID)
{
	m_controllerID = ID;
}

void Player::Hit(int projectileID)
{
	m_hitByProjectileID = projectileID;
}

//::.. GET FUNCTIONS ..:://
uint16 Player::GetCategoryBits() {
	return m_fixture.filter.categoryBits;
}

uint16 Player::GetMaskBits() {

	return m_fixture.filter.maskBits;
}

//void Player::StartContact() {
//	m_contact = true;
//}
//void Player::EndContact() {
//	m_contact = false;
//}

//::.. RENDER ..:://
void Player::Render(Camera camera) {

	//Renders the player and the gun 
	m_playerPrefab->Render(camera);

	//Renders projectiles of a weapon and its particles
	m_weapon.Render(camera);
}