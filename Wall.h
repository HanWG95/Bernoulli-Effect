//Properties of the wall
#pragma once
using namespace std;
namespace HW {

	class Wall
	{
	public:
		Wall();
		//starting x,y point
		float sx;
		float sy;
		//ending x,y point
		float ex;
		float ey;
		//capsule radius/ thickness
		float l_radius;
		void createWall(float s_x, float s_y, float e_x, float e_y, float lradius);
		virtual ~Wall();
	};
}
