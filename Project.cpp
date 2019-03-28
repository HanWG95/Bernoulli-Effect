// Project2.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Tutorial to setup SDL2.0 with Visual Studio  -> https://www.youtube.com/watch?v=Sfn7yOiwJLw
#include "pch.h"
#include <iostream>
#include <SDL.h>
#include "Screen.h"
#include <math.h>
#include <time.h>
#include "Swarm.h"
#include "Particle.h"
#include "Wall.h"
#include <fstream>

using namespace std;
using namespace HW;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	Screen screen;
	
	if (screen.init() == false) {
		//report error if SDL failed to initialized
		cout << "Error initializing SDL..." << endl;
	}

	//ofstream myfile;
	//myfile.open("log.csv",ios::binary);

	Swarm swarm;

	while (true) {
		//update particles		
		int elapsed = SDL_GetTicks();// timer
		screen.clear(); // clear screen before update at each frame
			
		const Particle *pParticles = swarm.getParticles();//get particles
		const Wall *pWalls = swarm.getWalls();

		for (int i = 0; i <  Swarm::NPARTICLES ; i++) {
			Particle particle = pParticles[i];			
			screen.setCircle(particle.m_x, particle.m_y, particle.m_radius);			
			//Draw particles			
		}
		
		for (int i = 0; i < 6; i++) {
			Wall wall = pWalls[i];		
			screen.setWall(wall.sx, wall.sy, wall.ex, wall.ey, wall.l_radius);
			//Draw walls
		}

		swarm.update(elapsed);	//update position and properties of particle
		screen.update(); //update graphics
		//Check for messages/events

		//myfile << &swarm.logger << endl;

		if (screen.processEvents() == false) {
			break;
		}		
	}

	screen.close();
	
	return EXIT_SUCCESS;
}



