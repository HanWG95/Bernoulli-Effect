#include "Wall.h"

namespace HW 
{
	Wall::Wall() 
	{		
	}

	void Wall::createWall(float s_x, float s_y, float e_x, float e_y, float lradius) {		
		sx = s_x;
		sy = s_y;
		ex = e_x;
		ey = e_y;
		l_radius = lradius;
	}

	Wall::~Wall()
	{
	}
}
