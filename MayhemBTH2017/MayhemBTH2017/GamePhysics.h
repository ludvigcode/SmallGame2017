#ifndef __GAMEPHYSICS_H__
#define __GAMEPHYSICS_H__


#include "Collider2D.h"
#include "Player.h"
#include "Weapon.h"
#include "Contact.h"
#include "MyContactListener.h"
#include "PowerUpHandler.h"
#include "Texture.h"
#include "TextureHandler.h"


class GamePhysics
{
public:

	GamePhysics();
	virtual ~GamePhysics();
	void EnterWorld(std::string levelName);
	void Update();
	void Render(Camera camera);
	glm::vec3 GetPosition();

	//::.. SET FUNCTIONS ..:://
	void SetNrOfPlayers(int nrOf);

	void Free();

private:
	b2World				m_world;
	Box					m_newBox;
	Box					m_newBox2;
	Box					m_powerUpBox;
	Player				m_player[4];
	Collider2D			gameFloor;
	Texture m_texture;
	TextureHandler m_textureHandler;


	Level				m_level;

	TimeManager *		m_time;
	GLfloat				m_jumpTimer = 0.0f;
	bool				m_isMidAir;
	bool				m_loadWorld;
	GLfloat				m_playerScaleX;
	GLfloat				m_playerScaleY;
	GLfloat				m_playerPosX;
	GLfloat				m_playerPosY;

	GLfloat				m_powerUpScaleX;
	GLfloat				m_powerUpScaleY;
	GLfloat				m_powerUpPosX;
	GLfloat				m_powerUpPosY;

	ParticleSystem		m_particles;

	Sprite				m_playerSprite;
	Sprite				m_powerUpSprite;
	Transform			m_transform;
	Collider2D			m_floorCollider;
	Sprite				m_firesprites[10];
	Sprite				m_shot;
	//Temporary
	b2FixtureDef		m_powerUpFixture;

	static MyContactListener * m_contactListener;

	PowerUpHandler m_powerupHandler;


	Transform m_transf;
	int m_nrOfPlayers;

	enum _entityCategory {
		BOUNDARY = 0x0001,
		PLAYER1 = 0x0002,
		PROJECTILE1 = 0x0004,
		POWERUP = 0x0008,
		PLAYER2 = 0x0016,
		PROJECTILE2 = 0x0032,
	};

};

#endif // !__GAMEPHYSICS_H__