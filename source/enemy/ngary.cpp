/*=========================================================================

	ngary.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif


void CNpc::processGaryMovement( int _frames )
{
	s8 multiplier = -1 + ( 2 * m_extension );
	s32 maxHeight = 10;
	s32 fallSpeed = 5;
	s8 yMovement = fallSpeed * _frames;
	s8 groundHeight;

	// check vertical collision

	groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

	if ( groundHeight <= 0 )
	{
		// groundHeight <= 0  indicates either on ground or below ground

		// check horizontal collision

		if ( m_layerCollision->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
		{
			// reverse direction

			m_extension = !m_extension;
		}
		else
		{
			// make sure we are on the ground, not below it

			Pos.vy += groundHeight;

			Pos.vx += multiplier * _frames;
		}
	}
	else
	{
		// above ground

		if ( groundHeight < yMovement )
		{
			// colliding with ground

			Pos.vy += groundHeight;

			if ( m_layerCollision->getHeightFromGround( Pos.vx + ( multiplier * _frames ), Pos.vy ) < -maxHeight )
			{
				// reverse direction

				m_extension = !m_extension;
			}
			else
			{
				Pos.vx += multiplier * _frames;
			}
		}
		else
		{
			Pos.vy += yMovement;
		}
	}
}