#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include <sim3ds/sim/Simulator.h>
#include <cpp3ds/Color.h>

namespace cpp3ds {

	class Drawable; // Forward-declaration for cyclic reference

	class Screen {
	private:
		void setPixelAddress(int addr, Color color);
	protected:
		sf::Image pixelImage;
		int x, y, width, height;
	public:
		Screen(int x, int y, int width, int height): x(x), y(y), width(width), height(height) {
			pixelImage.create(width, height, sf::Color::Transparent);
		}
		int getWidth(){ return width; }
		int getHeight(){ return height; }
		void setPixel(int x, int y, Color color);
		void clear(Color color = {0,0,0});
		void display();
		virtual void draw(Drawable& obj, float x = 0, float y = 0, bool use3D = false);
	};
}

#endif
