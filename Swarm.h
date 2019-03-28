//Set no. of particles, updates for collision and speed of particles
#pragma once
#include"Particle.h"
#include "Screen.h"
#include "Wall.h"

namespace HW {
class Swarm
		{
		private:			
			Particle *m_pParticles;
			Particle m_pParticles2;
			int lastTime;
			Wall *m_pWalls;			
		public:
			Swarm();
			const static int NPARTICLES = 30;//no. of particles inside the system
			std::vector<Particle> vecBalls;//vector to store particles for collision check
			std::vector<Wall> vecWalls;//vector to store walls
			Particle *getParticles() { return m_pParticles;  };
			Wall *getWalls() { return m_pWalls; };
			void update(int elapsed);			
			virtual ~Swarm();
		};
}
