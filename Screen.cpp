#include "Screen.h"
#include <iostream>
using namespace std;

namespace HW {

	Screen::Screen(): m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer(NULL) {

	}

	bool Screen::init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			//cout << "SDL could not initialize" << SDL_GetError() << endl;
			return false;
		}

		m_window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_OPENGL);

		if (m_window == NULL) {
			//cout << "Window not created" << SDL_GetError() << endl;
			return false;

		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);

		if (m_renderer == NULL) {
			//cout << "Renderer not created" << SDL_GetError() << endl;
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		if (m_texture == NULL) {
			//cout << "Texture not created" << SDL_GetError() << endl;
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}

		m_buffer = new Uint32[Width*Height];


		// setting background colour for the screen
		// 0 = black, 255 = white
		memset(m_buffer, 0, Width*Height * sizeof(Uint32));

		//for (int i = 0; i < Width*Height; i++) {
		//	m_buffer[i] = 0xFFFF0000; //RGBA
		//}

		return true;

	}

	bool Screen::processEvents() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
		}
		return true;
	}

	void Screen::update() {
		SDL_UpdateTexture(m_texture, NULL, m_buffer, Width * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
		//SDL_Delay(100);

	}

	void Screen::close() {
		delete[] m_buffer;
		delete[] m_buffer2;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();

	}
	
	/*void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {

		if (x < 0 || x >= Width || y < 0 || y >= Height) {
			return;
		}

		Uint32 color = 0;

		color += red;
		color <<= 8;
		color += green;
		color <<= 8;
		color += blue;
		color <<= 8;
		color += 0xFF;

		m_buffer [(y*Width) + x ] = color;

	}*/


	void Screen::setPixel(int x, int y) {

		if (x < 0 || x >= Width || y < 0 || y >= Height) {
			return;
		}

		Uint32 color = 0;

		//color += red;
		//color <<= 8;
		//color += green;
		//color <<= 8;
		//color += blue;
		//color <<= 8;
		//color += 0xFF;
		//reset data above for other colour

		m_buffer[(y*Width) + x] = 0xFFFFFFFF;//white in color FFFFFFFF = 255

	}



	void Screen::setCircle(int x0, int y0, int radius)
	{
		//Reference: Midpoint circle algorithm
		int x = radius - 1;
		int y = 0;
		int dx = 1;
		int dy = 1;
		int err = dx - (radius << 1); // shifting bits left by 1 effectively
									  // doubles the value. == dx - diameter
		while (x >= y)
		{
			setPixel(x0 + x, y0 + y);
			setPixel(x0 + y, y0 + x);
			setPixel(x0 - y, y0 + x);
			setPixel(x0 - x, y0 + y);
			setPixel(x0 - x, y0 - y);
			setPixel(x0 - y, y0 - x);
			setPixel(x0 + y, y0 - x);
			setPixel(x0 + x, y0 - y);
			//  Each of the following renders an octant of the circle			
			if (err <= 0)
			{
				y++;
				err += dy;
				dy += 2;
			}
			if (err > 0)
			{
				x--;
				dx += 2;
				err += dx - (radius << 1);
			}
		}
	}

	void Screen::setWall(int sx, int sy, int ex, int ey, int radius)
	{
		setCircle(sx, sy, radius); //indicate starting point
		setCircle(ex, ey, radius); //indicate ending point

		int deltaX = ex - sx;
		int deltaY = ey - sy;
		double deltaError;
		double error = 0;
		if (deltaX == 0 || deltaY == 0) {
			deltaError = 0;
		}
		else {
			deltaError = deltaX / deltaY;
		}

		int y;
		if (sy == ey) {
			y = sy;
			//draw horizontal wall
			for (int i = sx; i <= ex; i++) {
				setPixel(i, y - radius);
				setPixel(i, y + radius);
				error = error + deltaError;
				if (error >= 0.5)
				{
					++y;
					error -= 1.0;
				}
			}
		}
		//draw wall with + and - slope
		if (sy < ey) {
			y = sy;
			
			for (int i = sx; i <= ex; i++) {
				setPixel(i, y - radius);
				setPixel(i, y + radius);
				error = error + deltaError;
				if (error >= 0.5)
				{
					++y;
					error -= 1.0;
				}
			}
		}

		if (sy > ey) {
				y = sy;
				for (int i = sx; i <= ex; i++) {
					setPixel(i, y - radius);
					setPixel(i, y + radius);
					error = error - deltaError;
					if (error >= 0.5)
					{
						--y;
						error -= 1.0;
					}
				}
		}		
	}
	
	void Screen::clear(){

		memset(m_buffer, 0, Width*Height * sizeof(Uint32));
		//memset(m_buffer2, 0, Width*Height * sizeof(Uint32));

	}

	

} // namespace HW
