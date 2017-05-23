#include "GameSystem.h"



GameSystem::GameSystem()
{
	m_input = InputManager::Get();
	m_soundManager = SoundManager::Get();
	m_numPlayers = 0;
	m_currState = GAME_SETUP;
	m_playerReadyUI = nullptr;

	for (uint32_t i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerReady[i] = false;
	}

	m_camera.SetPosition(glm::vec3(((84 / 2)), ((48 / 2)), -51.2f));
	m_gameSettings = new GameSettings;
	m_gameSettings->CreateUI();
	m_selectorImage.SetTexture(".\\Assets\\Textures\\arredal.jpg");
	m_selectorImage.SetSize(50, 50);

}


GameSystem::~GameSystem()
{
}

void GameSystem::Load(uint32_t gameMode)
{
	m_gameMode = gameMode;
}

void GameSystem::Free()
{
}


void GameSystem::Update()
{
	switch (m_currState)
	{
	case GAME_SETUP:
		GameSetup();
		break;

	case INIT_PLAYER_READY:
		InitPlayerReady();
		break;

	case PLAYER_READY:
		PlayerReady();
		break;

	case INIT_PLAY:
		InitPlay();
		break;

	case START_PLAY:
		StartPlay();
		break;

	case PLAY:
		Play();
		break;

	case SELECT_LEVEL:
		SelectLevel();
		break;

	case LOAD_NEXT_LEVEL:
		LoadNextLevel();
		break;

	case GAME_OVER:
		GameOver();
		break;

	default:
		break;
	}
}


void GameSystem::GameSetup()
{
	m_pressToCont.SetPosition(0, -180);
	m_pressToCont.SetColor(255, 255, 255, 255);
	m_pressToCont.SetText("Press S to continue");

	m_pressToCont.Render();

	m_gameSettings->Update();
	m_gameSettings->Render();

	if (m_input->GetButtonDown(CONTROLLER_BUTTON_START))
	{
		m_currState = INIT_PLAYER_READY;
	}
}

void GameSystem::InitPlayerReady()
{
	VideoManager * vm = VideoManager::Get();
	m_playerReadyUI = new PlayerReadyUI[MAX_PLAYERS];


	// FIX POS!
	m_playerReadyUI[0].playerName.SetPosition((-0.75f * vm->GetWidth()) / 2, 20);
	m_playerReadyUI[1].playerName.SetPosition((-0.25f * vm->GetWidth()) / 2, 20);
	m_playerReadyUI[2].playerName.SetPosition((0.25f * vm->GetWidth()) / 2, 20);
	m_playerReadyUI[3].playerName.SetPosition((0.75f * vm->GetWidth()) / 2, 20);

	m_playerReadyUI[0].playerName.SetText("PLAYER 1");
	m_playerReadyUI[1].playerName.SetText("PLAYER 2");
	m_playerReadyUI[2].playerName.SetText("PLAYER 3");
	m_playerReadyUI[3].playerName.SetText("PLAYER 4");

	m_playerReadyUI[0].playerReady.SetPosition((-0.75f * vm->GetWidth()) / 2, -20);
	m_playerReadyUI[1].playerReady.SetPosition((-0.25f * vm->GetWidth()) / 2, -20);
	m_playerReadyUI[2].playerReady.SetPosition((0.25f * vm->GetWidth()) / 2, -20);
	m_playerReadyUI[3].playerReady.SetPosition((0.75f * vm->GetWidth()) / 2, -20);

	m_playerReadyUI[0].r = 252;
	m_playerReadyUI[0].g = 61;
	m_playerReadyUI[0].b = 73;
	m_playerReadyUI[0].a = 255;

	m_playerReadyUI[1].r = 255;
	m_playerReadyUI[1].g = 147;
	m_playerReadyUI[1].b = 73;
	m_playerReadyUI[1].a = 255;

	m_playerReadyUI[2].r = 61;
	m_playerReadyUI[2].g = 212;
	m_playerReadyUI[2].b = 66;
	m_playerReadyUI[2].a = 255;

	m_playerReadyUI[3].r = 43;
	m_playerReadyUI[3].g = 166;
	m_playerReadyUI[3].b = 160;
	m_playerReadyUI[3].a = 255;


	for (uint32_t i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerReadyUI[i].playerReady.SetText("PRESS A");
	}

	for (uint32_t i = 0; i < MAX_PLAYERS; i++)
	{
		m_playerReadyUI[i].playerName.SetColor(170, 170, 170, 255);
		m_playerReadyUI[i].playerReady.SetColor(170, 170, 170, 255);
	}

	m_pressToCont.SetPosition(0, -180);
	m_pressToCont.SetColor(255, 255, 255, 255);
	m_pressToCont.SetText("Press S to battle!");

	m_currState = PLAYER_READY;
}


void GameSystem::PlayerReady()
{
	for (uint32_t i = 0; i < MAX_PLAYERS; i++)
	{
		if (m_input->GetButtonDown(CONTROLLER_BUTTON_A, i))
		{
			m_playerReady[i] = !m_playerReady[i];

			if (m_playerReady[i])
			{
				m_playerReadyUI[i].playerReady.SetColor(m_playerReadyUI[i].r,
					m_playerReadyUI[i].g, m_playerReadyUI[i].b, 255);
				m_playerReadyUI[i].playerName.SetColor(m_playerReadyUI[i].r,
					m_playerReadyUI[i].g, m_playerReadyUI[i].b, 255);


				m_playerReadyUI[i].playerReady.SetText("READY!");
			}
			else
			{
				m_playerReadyUI[i].playerName.SetColor(170, 170, 170, 255);
				m_playerReadyUI[i].playerReady.SetColor(170, 170, 170, 255);

				m_playerReadyUI[i].playerReady.SetText("PRESS A");
			}
		}
	}

	if (m_input->GetButtonDown(CONTROLLER_BUTTON_START))
	{
		int num = 0;
		for (uint32_t i = 0; i < MAX_PLAYERS; i++)
		{
			if (m_playerReady[i])
			{
				++num;
			}

			// TEMP CHANGE TO TWO
			if (num >= 1)
			{
				// INIT PLAY
				TransitionManager::StartFadingOut();
				m_currState = SELECT_LEVEL;
				return;
			}
		}
	}

	for (uint32_t i = 0; i < 4; i++)
	{
		m_playerReadyUI[i].playerReady.Render();
		m_playerReadyUI[i].playerName.Render();
		m_pressToCont.Render();

	}
}


void GameSystem::InitPlay()
{
	for (uint32_t i = 0; i < MAX_PLAYERS; ++i)
	{
		if (m_playerReady[i])
		{
			++m_numPlayers;
		}
	}

	for (uint32_t i = 0; i < 4; i++)
	{
		m_playerReadyUI[i].playerReady.Render();
		m_playerReadyUI[i].playerName.Render();
	}

	if (TransitionManager::GetIsBlack())
	{
		if (m_playerReadyUI != nullptr)
		{
			delete[] m_playerReadyUI;
			m_playerReadyUI = nullptr;
		}

		m_world = new GamePhysics;
		m_world->EnterWorld(m_levelQueue.at(m_currentLevel));
		m_currState = START_PLAY;

		TransitionManager::StartFadingIn();
		TimeManager::ResetDeltaTime();
	}
	m_soundManager->FadeInNewMusic("bensound-epic", 3, 10);
	TransitionManager::Update();
}

void GameSystem::StartPlay()
{
	Camera camera;
	camera.SetPosition(glm::vec3(((84 / 2)), ((48 / 2)), -51.2f));

	m_world->Update(); // enterplay?
	m_world->Render(camera);

	if (!TransitionManager::GetIsFadingIn())
	{
		m_currState = PLAY;
		m_timer.SetTimer(m_gameSettings->GetGameLenght(), true, false);
	}

	TransitionManager::Update();
}

void GameSystem::Play()
{
	m_timer.Update();
	m_world->Update();
	m_world->Render(m_camera);

	m_atomicBomb.Update(m_camera);

	if (TransitionManager::GetIsBlack())
	{
		m_currState = LOAD_NEXT_LEVEL;
	}
	else if (m_timer.GetElapsed() < 0.001f)
	{
		TransitionManager::StartFadingOut();
	}
	else
	{
		m_gameUI.Update(static_cast<float>(m_gameSettings->GetGameLenght()) - m_timer.GetElapsed());
		m_gameUI.Render();
	}


	TransitionManager::Update();
}

void GameSystem::SelectLevel()
{
	m_levelHandler.GetLevelNames(m_levelText);
	int texLen = m_levelText.at(m_levelSelector).size();
	m_selectorImage.SetPosition(-80 - (10 * texLen), 200 - (50 * m_levelSelector));
	m_selectorImage.Render();

	for (int i = 0; i < m_levelHandler.GetNumLevels(); i++)
	{
		m_levelChoice[i].levelText.SetText(m_levelText.at(i).c_str());
		m_levelChoice[i].levelText.SetPosition(0, 200 - (50 * i));

		if (m_levelChoice[i].isSelect == true)
			m_levelChoice[i].levelText.SetColor(0, 255, 0, 255);
		else
			m_levelChoice[i].levelText.SetColor(255, 0, 0, 255);

		m_levelChoice[i].levelText.Render();
	}


	if (m_input->GetButtonDown(CONTROLLER_BUTTON_DPAD_UP))
	{
		if (m_levelSelector - 1 >= 0)
		{
			m_levelSelector--;
		}
	}

	else if (m_input->GetButtonDown(CONTROLLER_BUTTON_DPAD_DOWN))
	{
		if (m_levelSelector + 1 < m_levelHandler.GetNumLevels())
		{
			m_levelSelector++;
		}
	}

	if (m_input->GetButtonDown(CONTROLLER_BUTTON_A))
	{

		m_levelChoice[m_levelSelector].isSelect = !m_levelChoice[m_levelSelector].isSelect;

	}

	if (m_input->GetButtonDown(CONTROLLER_BUTTON_START))
	{

		for (int i = 0; i < 10; i++)
		{
			if (m_levelChoice[i].isSelect == true)
			{
				m_levelHandler.Import(m_level, m_levelChoice[i].levelText.GetText());
				m_level.SetName(m_levelChoice[i].levelText.GetText());
				m_levelQueue.push_back(m_level);
			}
		}

		for (int i = 0; i < m_levelQueue.size(); i++)
		{
			m_levelHandler.Import(m_levelQueue.at(i), m_levelQueue.at(i).GetName());
		}
		m_numOfLevels = m_levelQueue.size();
		TransitionManager::StartFadingOut();
		m_currState = INIT_PLAY;

	}

	m_pressToCont.Render();
}

void GameSystem::LoadNextLevel()
{
	TransitionManager::Update();

	m_pressToCont.Render();

	if (m_input->GetButtonDown(CONTROLLER_BUTTON_START))
	{

		if (!(m_currentLevel + 1 == m_numOfLevels))
			m_currentLevel++; // IF TRUE SWITCH TO GAME OVER

		m_world->EnterWorld(m_levelQueue.at(m_currentLevel));
		m_currState = START_PLAY;
		TransitionManager::StartFadingIn();
	}
}

void GameSystem::GameOver()
{

}