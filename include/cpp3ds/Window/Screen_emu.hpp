#ifndef CPP3DS_SCREEN_HPP
#define CPP3DS_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <cpp3ds/Emulator/Emulator.hpp>
#include <cpp3ds/Graphics/Color.hpp>

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

		virtual void _display();

		template<class... T>
		friend void display(Screen& screen, T&... rest);
		friend void display();

		virtual void draw(Drawable& obj, float x = 0, float y = 0, bool use3D = false);
	};
}

#endif
