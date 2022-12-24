////////////////////////////////////////////////////////////////////////////////
//	Module		:	cta_game_artefact.cpp
//	Created		:	19.12.2007
//	Modified	:	19.12.2007
//	Autor		:	Alexander Maniluk
//	Description	:	Artefact object for Capture The Artefact game mode
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "cta_game_artefact_activation.h"

#include "PhysicsShell.h"
#include "PhysicsShellHolder.h"
#include "game_cl_base.h"

#include "../xrRender/Public/Kinematics.h"
#include "../xrRender/Public/KinematicsAnimated.h"

#include "inventory.h"
#include "level.h"
#include "ai_object_location.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "phworld.h"
#include "restriction_space.h"
#include "../xrEngine/IGame_Persistent.h"

CtaArtefactActivation::CtaArtefactActivation(CArtefact* af, u32 owner_id) :
	SArtefactActivation(af, owner_id)
{
}

CtaArtefactActivation::~CtaArtefactActivation()
{
}

void CtaArtefactActivation::UpdateActivation()
{
	if (!IsInProgress())
		return;

	VERIFY(!ph_world->Processing());
	m_cur_state_time				+=	Device->fTimeDelta;
	if(m_cur_state_time				>=	m_activation_states[int(m_cur_activation_state)].m_time){
		m_cur_activation_state		=	(EActivationStates)(int)(m_cur_activation_state+1);
		
		if(m_cur_activation_state == eMax){
			m_cur_activation_state = eNone;
			//m_af->processing_deactivate			();
			//m_af->DestroyObject();
		}

		m_cur_state_time	= 0.0f;
		ChangeEffects				();


	if(m_cur_activation_state==eSpawnZone && OnServer())
		SpawnAnomaly	();

	}
	UpdateEffects				();
}

void CtaArtefactActivation::Load()
{
	inherited::Load();
}

void CtaArtefactActivation::Start()
{
	inherited::Start();
}

void CtaArtefactActivation::Stop()
{
	inherited::Stop();
}

void CtaArtefactActivation::ChangeEffects()
{
	//inherited::ChangeEffects();
}

void CtaArtefactActivation::UpdateEffects()
{
	inherited::UpdateEffects();
}

void CtaArtefactActivation::SpawnAnomaly()
{
	inherited::SpawnAnomaly();
}

void CtaArtefactActivation::PhDataUpdate(dReal step)
{
	inherited::PhDataUpdate(step);
}