/*=========================================================================

	hdbarrel.cpp

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HDBARREL_H__
#include "hazard\hdbarrel.h"
#endif

#ifndef __LAYER_COLLISION_H__
#include "level\layercollision.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatformBarrelHazard::init()
{
	CNpcHazard::init();

	m_npcPath.setPathType( CNpcPath::SINGLE_USE_PATH );

	m_rotation = 0;
	m_rotationDir = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatformBarrelHazard::processMovement( int _frames )
{
	s32 maxHeight = 20;
	s32 distX, distY;
	s32 fallSpeed = 3;
	s8 yMovement = fallSpeed * _frames;
	s32 groundHeight;

	s32 moveX = 0;
	s32 moveY = 0;

	// ignore y component of waypoint, since we are stuck to the ground

	bool pathComplete;

	m_npcPath.thinkFlat( Pos, &pathComplete, &distX, &distY, &m_heading );

	if ( pathComplete )
	{
		// reset

		Pos = m_base;
		m_npcPath.resetPath();

		return;
	}
	else
	{
		// check for collision

		if ( distX )
		{
			distX = distX / abs( distX );
		}

		if ( m_layerCollision->getHeightFromGround( Pos.vx + ( distX * 3 * _frames ), Pos.vy ) < -maxHeight )
		{
			// there is an obstacle in the way, increment the path point (hopefully this will resolve the problem)

			m_npcPath.incPath();
		}
		else
		{
			// check for vertical movement

			groundHeight = m_layerCollision->getHeightFromGround( Pos.vx, Pos.vy, yMovement + 16 );

			if ( groundHeight <= yMovement )
			{
				// groundHeight <= yMovement indicates either just above ground or on or below ground

				moveX = distX * 3 * _frames;
				moveY = groundHeight;
			}
			else
			{
				CNpcPlatform *platform = (CNpcPlatform *) isOnPlatform();

				if ( platform )
				{
					// stick to platform top

					moveY = platform->getHeightFromPlatformAtPosition( Pos.vx, Pos.vy + yMovement );

					if ( !platform->canDrop() )
					{
						// if platform cannot drop any further, move in X

						moveX = distX * 3 * _frames;
					}
				}
				else
				{
					// fall

					moveY = yMovement;
				}
			}
		}
	}

	if ( moveX )
	{
		m_rotationDir = abs( moveX ) / moveX;
	}
	else
	{
		m_rotationDir = 0;
	}

	m_rotation += m_rotationDir * 64 * _frames;
	m_rotation &= 4095;

	Pos.vx += moveX;
	Pos.vy += moveY;

	s32 minY, maxY;

	m_npcPath.getPathYExtents( &minY, &maxY );

	if ( Pos.vy > maxY )
	{
		Pos = m_base;
		m_npcPath.resetPath();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNpcDualPlatformBarrelHazard::render()
{
	CHazardThing::render();

	if (canRender())
	{
		DVECTOR &renderPos=getRenderPos();

		SVECTOR rotation;
		rotation.vx = 0;
		rotation.vy = 0;
		rotation.vz = m_rotation;

		VECTOR scale;
		scale.vx = ONE;
		scale.vy = ONE;
		scale.vz = ONE;

		m_modelGfx->Render(renderPos,&rotation,&scale);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const CRECT *CNpcDualPlatformBarrelHazard::getThinkBBox()
{
	CRECT objThinkBox = getCollisionArea();

	sBBox &thinkBBox = CThingManager::getThinkBBox();
	objThinkBox.x1 = thinkBBox.XMin;
	objThinkBox.x2 = thinkBBox.XMax;
	objThinkBox.y1 = thinkBBox.YMin;
	objThinkBox.y2 = thinkBBox.YMax;

	return &objThinkBox;
}