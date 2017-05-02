#include "AnimController.h"
#include "InputManager.h"

#include "MrAnimHandler.h"

#include "GLM\gtc\quaternion.hpp"

AnimController::AnimController()
{

}


AnimController::~AnimController()
{
}

void AnimController::Update(GLuint locations)
{
	Transform transform;

	InputManager * input = InputManager::Get();

	float x =  input->GetAxisDirection(CONTROLLER_AXIS_RIGHTX);
	float y = input->GetAxisDirection(CONTROLLER_AXIS_RIGHTY);

	KeyFrame * kf = new KeyFrame;
	kf->localTx = new glm::mat4[m_skel->GetNumJoints()];
	if (y < 0)
	{
		for (uint32_t i = 0; i < m_skel->GetNumJoints(); i++)
		{
			glm::quat rot1(glm::quat_cast(m_keyBase->localTx[i]));
			glm::quat rot2(glm::quat_cast(m_keyUp->localTx[i]));
			glm::quat rot3 = glm::lerp(rot1, rot2, abs(y));
			kf->localTx[i] = glm::mat4(rot3);
			kf->localTx[i][3] = m_keyBase->localTx[i][3];

		}
	}
	else
	{
		if (abs(y) < 0.5f)
		{
			for (uint32_t i = 0; i < m_skel->GetNumJoints(); i++)
			{
				glm::quat rot1(glm::quat_cast(m_keyBase->localTx[i]));
				glm::quat rot2(glm::quat_cast(m_keyDown->localTx[i]));
				glm::quat rot3 = glm::lerp(rot1, rot2, abs(y * 2));
				kf->localTx[i] = glm::mat4(rot3);
				kf->localTx[i][3] = m_keyBase->localTx[i][3];
			}
		}
		else
		{
			for (uint32_t i = 0; i < m_skel->GetNumJoints(); i++)
			{
				glm::quat rot1(glm::quat_cast(m_keyDown->localTx[i]));
				glm::quat rot2(glm::quat_cast(m_keyDown2->localTx[i]));
				glm::quat rot3 = glm::lerp(rot1, rot2, abs((y - 0.5f) * 2));
				kf->localTx[i] = glm::mat4(rot3);
				kf->localTx[i][3] = m_keyBase->localTx[i][3];
			}
		}
	}

	m_skel->Update(kf, true, 1, 11);

	m_clips[m_currClip]->Update();

	m_skel->Update(m_clips[m_currClip]->GetCurrentKeyFrame(), false, 12);
//
//

//	m_skel->Update(m_clips[m_currClip]->GetCurrentKeyFrame(), false, 3, 3);
//
//	transform.SetRotation(glm::vec3(0.0f, angle - 90, -90.0f));
//
//	m_skel->SetJointRotation(transform, 4);
//
//	m_skel->Update(m_clips[m_currClip]->GetCurrentKeyFrame(), false, 5, 6);


	

	glUniformMatrix4fv(locations, m_skel->GetNumJoints(), GL_FALSE, &m_skel->GetSkinnedTx()[0][0][0]);
}

bool AnimController::SwitchAnim(const char * anim)
{


	return true;
}


bool AnimController::SwitchAnimAt(uint32_t index)
{
	m_clips[m_currClip]->Stop();

	m_currClip = index;

	return false;
}


void AnimController::AddAnimation(AnimClip * clip)
{
	m_clips.push_back(clip);
}


//::.. SET FUNCTIONS ..:://
void AnimController::SetSkeleton(AnimSkeleton * skel)
{
	m_skel = skel;

	MrAnimHandler * animHandler1 = new MrAnimHandler;

	animHandler1->Import(".\\Assets\\Animations\\Player@Up.mranim");

	m_keyUp = new KeyFrame[animHandler1->GetNumKeyFramedJoints()];
	m_keyUp->localTx = new glm::mat4[m_skel->GetNumJoints()];

	for (uint32_t j = 0; j < m_skel->GetNumJoints(); j++)
	{
		m_keyUp->localTx[j] = animHandler1->GetKeyFramedJoints()[j].matrix[0];
	}

	MrAnimHandler * animHandler = new MrAnimHandler;

	animHandler->Import(".\\Assets\\Animations\\Player@Run.mranim");

	m_keyBase = new KeyFrame[animHandler->GetNumKeyFramedJoints()];
	m_keyBase->localTx = new glm::mat4[m_skel->GetNumJoints()];

	for (uint32_t j = 0; j < m_skel->GetNumJoints(); j++)
	{
		m_keyBase->localTx[j] = animHandler->GetKeyFramedJoints()[j].matrix[0];
	}

	MrAnimHandler * animHandler2 = new MrAnimHandler;

	animHandler2->Import(".\\Assets\\Animations\\Player@Down_01.mranim");

	m_keyDown = new KeyFrame[animHandler->GetNumKeyFramedJoints()];
	m_keyDown->localTx = new glm::mat4[m_skel->GetNumJoints()];

	for (uint32_t j = 0; j < m_skel->GetNumJoints(); j++)
	{
		m_keyDown->localTx[j] = animHandler2->GetKeyFramedJoints()[j].matrix[0];
	}


	MrAnimHandler * animHandler3 = new MrAnimHandler;
	animHandler3->Import(".\\Assets\\Animations\\Player@Down_02.mranim");

	m_keyDown2 = new KeyFrame[animHandler->GetNumKeyFramedJoints()];
	m_keyDown2->localTx = new glm::mat4[m_skel->GetNumJoints()];

	for (uint32_t j = 0; j < m_skel->GetNumJoints(); j++)
	{
		m_keyDown2->localTx[j] = animHandler3->GetKeyFramedJoints()[j].matrix[0];
	}

}
