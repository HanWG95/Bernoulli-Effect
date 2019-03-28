//Present the graphics and update of graphics each frame
#pragma once
#include<iostream>
#include<SDL.h>
#include <vector>

namespace HW {

	class Screen
	{
	private:
		SDL_Window *m_window;		
		SDL_Texture *m_texture;
		Uint32 *m_buffer;
		Uint32 *m_buffer2;

	public:
		SDL_Renderer *m_renderer;
		const static int Width = 900;
		const static int Height = 600;
		Screen();
		bool init();
		void update();
		//void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
		void setPixel(int x, int y);
		void setCircle(int x, int y, int radius);
		void setWall(int sx, int sy, int ex, int ey, int radius);
		bool processEvents();
		void close();
		void clear();
	};

} // namespace HW
