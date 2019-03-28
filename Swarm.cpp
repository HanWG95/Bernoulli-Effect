#include "Swarm.h"
#include <iostream>
#include <algorithm>

using namespace std;
namespace HW {

	Swarm::Swarm() :lastTime(0)
	{
		for (int i = 0; i < NPARTICLES; i++) {
			Particle m_pParticles2;
			m_pParticles2.m_id = vecBalls.size();//N-particles will be included in the system
			vecBalls.emplace_back(m_pParticles2);//log all particles into vecBalls vector for collision check
		}
		m_pParticles = vecBalls.data();

		Wall wall;//set start point, end point and thickness of walls
		//top left
		wall.createWall(0.0f, 0.0f, 400.0f, 0.0f, 10.0f);
		vecWalls.push_back(wall);
		//top mid
		wall.createWall(400.0f, 0.0f, 600.0f, 200.0f, 10.0f);
		vecWalls.push_back(wall);
		//top right
		wall.createWall(600.0f, 200.0f, 900.0f, 200.0f, 10.0f);
		vecWalls.push_back(wall);
		//bottom left
		wall.createWall(00.0f, 600.0f, 400.0f, 600.0f, 10.0f);
		vecWalls.push_back(wall);
		//bottom mid
		wall.createWall(400.0f, 600.0f, 600.0f, 400.0f, 10.0f);
		vecWalls.push_back(wall);
		//bottom right
		wall.createWall(600.0f, 400.0f, 900.0f, 400.0f, 10.0f);
		vecWalls.push_back(wall);

		m_pWalls = vecWalls.data();
	}

	Swarm::~Swarm()
	{
		delete[] m_pParticles;
		delete[] & m_pParticles2;
		delete[] m_pWalls;
	}

	void Swarm::update(int elapsed) {

		int interval = elapsed - lastTime;
		//cout << "swarm update" << endl;
		//cout << endl;				

		for (int i = 0; i < Swarm::vecBalls.size(); i++) {
			//cout << "Updating Particle No. " << i << "  :" << endl;
			vecBalls.at(i).update(interval);

			//m_pParticles->update(interval);
			auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
			{
				//square both side to get rid of square root at the left hand side
				return fabs((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) <= (r1 + r2)*(r1 + r2);
			};

			vector<pair<Particle*, Particle*>> vecCollidingPairs;//pointers for first and second element in the pair vectors
			vector<Particle*> fakeBalls;//pointer to indicate the fakeball for wall collision
			vector<Particle*> vecPressure1;
			vector<Particle*> vecPressure3;

			//collision check and update the new speed 
			for (auto &ball : vecBalls)
			{		
				//log zone 1 ball and zone 3 ball into seperate vector
				if (100 < ball.m_x && ball.m_x < 400) {
					vecPressure1.push_back(&ball);
				}
				
				///////////////////////////////////////////////////////////
				if (600 < ball.m_x && ball.m_x < 900) {
					vecPressure3.push_back(&ball);
				}
				
				/////////////////////////////////////////////////////
				//cout << "Wall collision" << endl;	

					for (auto &edge : vecWalls)
					{	//Wall collision	
						// Check that line formed by velocity vector, intersects with line segment
						float fLineX1 = edge.ex - edge.sx;
						float fLineY1 = edge.ey - edge.sy;

						float fLineX2 = ball.m_x - edge.sx;
						float fLineY2 = ball.m_y - edge.sy;

						float fEdgeLength = fLineX1 * fLineX1 + fLineY1 * fLineY1;

						// This is nifty - It uses the DP of the line segment vs the line to the object, to work out
						// how much of the segment is in the "shadow" of the object vector. The min and max clamp
						// this to lie between 0 and the line segment length, which is then normalised. We can
						// use this to calculate the closest point on the line segment
						float t = (std::max(0.0f, std::min(fEdgeLength, (fLineX1 * fLineX2 + fLineY1 * fLineY2)))) / fEdgeLength;

						// Which we do here
						float fClosestPointX = edge.sx + t * fLineX1;
						float fClosestPointY = edge.sy + t * fLineY1;

						// And once we know the closest point, we can check if the ball has collided with the segment in the
						// same way we check if two balls have collided
						float fDistance = sqrtf((ball.m_x - fClosestPointX)*(ball.m_x - fClosestPointX) + (ball.m_y - fClosestPointY)*(ball.m_y - fClosestPointY));

						if (fDistance <= (ball.m_radius + edge.l_radius))
						{
							// Collision has occurred - treat collision point as a ball that cannot move. To make this
							// compatible with the dynamic resolution code below, we add a fake ball with equal mass
							// so it behaves like a "mirror" to the target ball when the momentum calculations are performed
							Particle *fakeball = new Particle();
							fakeball->m_radius = edge.l_radius;
							fakeball->m_mass = ball.m_mass;
							fakeball->m_x = fClosestPointX;
							fakeball->m_y = fClosestPointY;
							fakeball->xspeed = -ball.xspeed;	
							fakeball->yspeed = -ball.yspeed;	

							// Store Fake Ball
							fakeBalls.push_back(fakeball);

							// Add collision to vector of collisions for dynamic resolution
							vecCollidingPairs.push_back({ &ball, fakeball });

							// Calculate displacement required
							float fOverlap = 1.0f * (fDistance - ball.m_radius - fakeball->m_radius);

							// Displace Current Ball away from collision
							ball.m_x -= fOverlap * (ball.m_x - fakeball->m_x) / fDistance;
							ball.m_y -= fOverlap * (ball.m_y - fakeball->m_y) / fDistance;
												
						}
					}
				
				//static collision
				for (auto &target : vecBalls)
				{
					if (ball.m_id != target.m_id)
					{	//ball id can't be the same as target id, particle will be colliding with itself if this is not included
						if (DoCirclesOverlap(ball.m_x, ball.m_y, ball.m_radius, target.m_x, target.m_y, target.m_radius))
						{	// statement is true, particle colliding with target particle
							// Collision has occured
							vecCollidingPairs.push_back({ &ball, &target });

							// Distance between ball centers
							float fDistance = sqrtf((ball.m_x - target.m_x)*(ball.m_x - target.m_x) + (ball.m_y - target.m_y)*(ball.m_y - target.m_y));

							// Calculate displacement required
							float fOverlap = 0.5f * (fDistance - ball.m_radius - target.m_radius);

							// Displace Current Ball away from collision
							ball.m_x -= fOverlap * (ball.m_x - target.m_x) / fDistance;
							ball.m_y -= fOverlap * (ball.m_y - target.m_y) / fDistance;

							// Displace Target Ball away from collision
							target.m_x += fOverlap * (ball.m_x - target.m_x) / fDistance;
							target.m_y += fOverlap * (ball.m_y - target.m_y) / fDistance;
							//cout << "Particles collision " << endl;
						}
					}
				}
			}

			//dynamic collision
			for (auto c : vecCollidingPairs)
			{
				Particle *b1 = c.first;
				Particle *b2 = c.second;

				//cout << "Particle 1 position: x = " << b1->m_x << " , y = " << b1->m_y << endl;
				//cout << "Particle 1 speed: vx = " << b1->xspeed << " , vy = " << b1->yspeed << endl;

				//cout << "Particle 2 position: x = " << b2->m_x << " , y = " << b2->m_y << endl;
				//cout << "Particle 2 speed: vx = " << b2->xspeed << " , vy = " << b2->yspeed << endl;

				// Distance between balls
				float fDistance = sqrtf((b1->m_x - b2->m_x)*(b1->m_x - b2->m_x) + (b1->m_y - b2->m_y)*(b1->m_y - b2->m_y));

				// Normal unit vector
				float nx = (b2->m_x - b1->m_x) / fDistance;
				float ny = (b2->m_y - b1->m_y) / fDistance;

				// Tangent unit vector
				float tx = -ny;
				float ty = nx;

				// Dot Product Tangent (scalar)
				float dpTan1 = b1->xspeed * tx + b1->yspeed * ty;
				float dpTan2 = b2->xspeed * tx + b2->yspeed * ty;

				// Dot Product Normal (scalar)
				float dpNorm1 = b1->xspeed * nx + b1->yspeed * ny;
				float dpNorm2 = b2->xspeed * nx + b2->yspeed * ny;

				// Conservation of momentum in 1D
				float m1 = (dpNorm1 * (b1->m_mass - b2->m_mass) + 2.0f * b2->m_mass * dpNorm2) / (b1->m_mass + b2->m_mass);
				float m2 = (dpNorm2 * (b2->m_mass - b1->m_mass) + 2.0f * b1->m_mass * dpNorm1) / (b1->m_mass + b2->m_mass);

				// Update ball velocities(scalar -> vector -> sum of normal and tangent)
				b1->xspeed = tx * dpTan1 + nx * m1;
				b1->yspeed = ty * dpTan1 + ny * m1;
				b2->xspeed = tx * dpTan2 + nx * m2;
				b2->yspeed = ty * dpTan2 + ny * m2;

				//cout << endl;
				//cout << "Particle 1 position: x = " << b1->m_x << " , y = " << b1->m_y << endl;
				//cout << "Particle 1 speed: vx = " << b1->xspeed << " , vy = " << b1->yspeed << endl;

				//cout << "Particle 2 position: x = " << b2->m_x << " , y = " << b2->m_y << endl;
				//cout << "Particle 2 speed: vx = " << b2->xspeed << " , vy = " << b2->yspeed << endl;
			
				//cout << "Dynamic collision " << endl;

				//An alternate way of solving the same question
				//Wikipedia Version - Maths is smarter but same
				//float kx = (b1->vx - b2->vx);
				//float ky = (b1->vy - b2->vy);
				//float p = 2.0 * (nx * kx + ny * ky) / (b1->mass + b2->mass);
				//b1->vx = b1->vx - p * b2->mass * nx;
				//b1->vy = b1->vy - p * b2->mass * ny;
				//b2->vx = b2->vx + p * b1->mass * nx;
				//b2->vy = b2->vy + p * b1->mass * ny;
			}
			
			int zoneOneNum = vecPressure1.size();
			float zoneOneAvg = 0.0;
			for (int i = 0; i < zoneOneNum; i++) {
				zoneOneAvg += abs(vecPressure1[i]->yspeed); //y speed cal for Pressure calculation
				//zoneOneAvg += vecPressure1[i]->xspeed; //xspeed average calculation
			}
			//zoneOneAvg = zoneOneAvg / zoneOneNum; 
			//cout << zoneOneAvg << endl; //xspeed/yspeed cal

			//double zoneOneDensity = zoneOneNum / 0.18;
			//cout  << zoneOneDensity << endl;
			//logger (zoneOneAvg);

			int zoneThreeNum = vecPressure3.size();
			float zoneThreeAvg = 0.0;
			for (int i = 0; i < zoneThreeNum; i++) {
				zoneThreeAvg += abs(vecPressure3[i]->yspeed);
				//zoneThreeAvg += vecPressure3[i]->xspeed;
			}
			//zoneThreeAvg = zoneThreeAvg / zoneThreeNum;	
			//cout << zoneThreeAvg << endl; //xspeed/yspeed cal

			double zoneThreeDensity = zoneThreeNum / 0.06;
			cout << zoneThreeDensity << endl;

			//remove collision pair
			vecCollidingPairs.clear();
			//remove fake balls
			vecPressure1.clear();
			vecPressure3.clear();
			for (auto &b : fakeBalls) delete b;
			fakeBalls.clear();
			//SDL_Delay(1000);
			//cout << "Particle No. " << i << " updated" << endl;
		}		
		lastTime = elapsed;//update time elapsed
	}
}
