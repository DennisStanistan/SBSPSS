/*=========================================================================

	hbbarrel.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __HAZARD_HBBARREL_H__
#define __HAZARD_HBBARREL_H__

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

class CNpcBouncingBarrelHazard : public CNpcHazard
{
public:
	void				init();
	void				render();
	virtual	CRECT const		*getThinkBBox();
protected:
	void				processMovement( int _frames );

	DVECTOR				m_lastWaypoint;

	s16					m_rotation;
	s16					m_rockRotation;
	u8					m_rockDir;
};

#endif