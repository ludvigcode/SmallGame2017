#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "PrefabManager.h"
#include "Box.h"
#include "AShader.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "PlayerPrefab.h"
#include "Collidable.h"
#include "Projectile.h"
#include "Weapon.h"

class Player : public Collidable
{
public:
	//::.. CONSTRUCTORS ..:://
	Player();
	Player(b2World* world, glm::vec2 pos, glm::vec2 scale, int controllerID);
	virtual ~Player();

	//::..INITIALIZER..:://
	void Init(b2World* world, glm::vec2 pos, glm::vec2 scale, int controllerID);
	
	//::..RENDER..:://
	void Render(Camera camera);
	void Update();
	void Respawn(glm::vec2 pos);

	//::..SETTERS..:://
	void SetCategoryBits(short CATEGORY);
	void SetMaskBits(short MASK);
	bool Timer(float rate);
	void SetControllerID(int ID);
	void Hit();
	
	//::..GETTERS..:://
	uint16 GetCategoryBits();
	uint16 GetMaskBits();
	Box GetBox();
	PlayerPrefab* GetPrefab();
	b2Fixture* GetFilter();
	void StartContact(bool projectile, bool powerup);
	void EndContact();
	int GetControllerID();
	void UpdateParticles();
	
private:
private:
	InputManager * m_input;
	b2Fixture* m_filter;
	Box m_boundingBox;
	PlayerPrefab * m_playerPrefab;
	Sprite m_playerSprite;
	Transform m_transf;
	Camera m_cam;
	AShader m_shader;
	AShader m_toonShader;
	bool	m_isMidAir;
	bool m_doubleJump;
	bool m_contact;
	bool m_dead;
	float m_time;
	int m_controllerID;
	bool m_collidedProjectile;
	bool m_hitByProjectile;

	Weapon m_weapon;

	b2World * m_world;

	bool m_collidedPowerUp;

	b2FixtureDef m_fixture;
	enum _entityCategory {
		BOUNDARY = 0x0001,
		PLAYER1 = 0x0002,
		PROJECTILE = 0x0004,
		POWERUP = 0x0008,
		PLAYER2 = 0x0010,
		PLAYER3 = 0x0012,
		PLAYER4 = 0x0014,
	};





	//TEMP DEBUG MARTIN
	PlayerController*	m_testCon;
};

#endif