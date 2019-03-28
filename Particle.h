//Particles properties and update
#pragma once
#include <iostream>
#include <SDL.h>
#include <string>

namespace HW {

	struct Particle
	{
		double m_x;//x coordinates
		double m_y;//y coordinates
		double m_speed;//velocity
		double m_direction;//angle
		double m_radius;//radius of particle
		double m_mass;//mass of particle
		double xspeed;
		double yspeed;
		double accel =0.0;
		int m_id;// used in collision check		
		double inject_y = 10+(rand() % 580);//reinject the particle back into the system from left hand side
		double inject_direction;
		double inject_speed;

	public:
		Particle();
		virtual ~Particle();
		void update(int interval);			
	};
}
