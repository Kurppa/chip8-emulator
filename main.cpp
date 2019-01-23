#include <iostream>
#include "chip8.h"
#include <SFML/Graphics.hpp>

unsigned char keymap[16] = {
	sf::Keyboard::Key::X,
	sf::Keyboard::Key::Num1,
	sf::Keyboard::Key::Num2,
	sf::Keyboard::Key::Num3,
	sf::Keyboard::Key::Q,
	sf::Keyboard::Key::W,
	sf::Keyboard::Key::E,
	sf::Keyboard::Key::A,
	sf::Keyboard::Key::S,
	sf::Keyboard::Key::D,
	sf::Keyboard::Key::Z,
	sf::Keyboard::Key::C,
	sf::Keyboard::Key::Num4,
	sf::Keyboard::Key::R,
	sf::Keyboard::Key::F,
	sf::Keyboard::Key::V,
};

int main (int argc, char** argv) {

	Chip8 chip8;

	int width = 1280, height = 640;
	
	float sqr_sz = width / 64;

	if (argc != 2) {
		std::cerr << "Usage: ./chip8 romFile" << std::endl;
		return 1;
	}

	if (!chip8.load_program(argv[1])) {
		std::cerr << "Failed to load the requested rom" << std::endl;
		return 2;
	}
	
	sf::RenderWindow* window = new  sf::RenderWindow(sf::VideoMode(1280,640), "chip8");
	window->setFramerateLimit(60);

	if (window == NULL) {
		std::cerr << "Failed to create a window" << std::endl;
		return 3; 
	}

	sf::RectangleShape pixels[32][64];
	
	for (int y = 0; y < 32; ++y) {
		for (int x = 0; x < 64; ++x) {
			pixels[y][x] = sf::RectangleShape(sf::Vector2f(sqr_sz, sqr_sz));
			pixels[y][x].setFillColor(sf::Color::White);
			pixels[y][x].setPosition(sf::Vector2f(x*sqr_sz, y*sqr_sz));

		}
	}


	while (true) {
		chip8.emulate_cycle();
		
		sf::Event e;
		
		while (window->pollEvent(e)) {
			if (e.type == sf::Event::EventType::Closed) {
				window->close();
				return 0;
			}

			if (e.type == sf::Event::KeyPressed) {
				for (int i = 0; i < 16; ++i) {
					if (e.key.code == keymap[i]) {
						chip8.key[i] = 1;
					}
				}
			} else if (e.type == sf::Event::KeyReleased) {
				for (int i = 0; i < 16; ++i) {
					if (e.key.code == keymap[i]) {
						chip8.key[i] = 0;
					}
				}
			
			}
		}
		
		
		if (chip8.draw_flag) {
			chip8.draw_flag = false;
			
			window->clear();
				
			for (int y = 0; y < 32; ++y) {
				for (int x = 0; x < 64; ++x) {	
					if (chip8.gfx[(y*64)+x] == 1) {
						window->draw(pixels[y][x]);
					}
				}
			}
		
			window->display();
		}

	}

	return 0;

}
