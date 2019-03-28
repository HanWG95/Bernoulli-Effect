#include "Particle.h"
#include <stdlib.h>
#include <iostream>
#include "Screen.h"
#include <math.h>

using namespace std;
namespace HW {

	Particle::Particle()
	{
		//set initial position of particle
		m_x = 50 + (rand() % 250);
		m_y = 50 + (rand() % 250);
		
		//random distribution of direction and speed
		m_direction = (2 * M_PI * rand() )/ RAND_MAX;		
		m_speed = (0.5 * rand()) / RAND_MAX;
		
		//x and y velocity from polar to cartesian
		//make sure all particles is moving to the right
		if (xspeed < 0) { xspeed = -xspeed; }
		xspeed = m_speed * cos(m_direction);
		yspeed = m_speed * sin(m_direction);
		
		//radius and mass of particle, mass used in collision(kinetic conservation)		
		m_radius = 5.0;
		m_mass = m_radius * 10.0;		
	}

	Particle::~Particle()
	{
	}

	void Particle::update(int interval) {

		//update the position of particle each frame
		//if (xspeed < 0) { xspeed = -xspeed; }
		m_x += (xspeed * interval  + accel) ;
		m_y += yspeed * interval ;
		//cout << "x: " << m_x << " , y: " << m_y << endl;

		//if (m_x < m_radius + 10) xspeed = -xspeed;
		if (m_x > (Screen::Width) || m_x < m_radius || m_y < m_radius || m_y >(Screen::Height))
		{	//set condition to wrap all particles inside the tube
			m_x = 20;
			m_y = inject_y;

			//reinject the particles into the tube from left hand side
			//random distribution of direction and speed
			inject_direction = (2 * M_PI * rand()) / RAND_MAX;
			inject_speed = (0.5 * rand()) / RAND_MAX;

			xspeed = inject_speed * cos(inject_direction);
			yspeed = inject_speed * sin(inject_direction);
			if (xspeed < 0) { xspeed = -xspeed; }

			//m_x += (xspeed * interval + accel);
			//m_y += yspeed * interval;
		}		
	}	
}
