#pragma once

#include <SFML/Graphics.hpp>


template<typename T = int>
struct Conf
{
	inline const static sf::Color ANT_COLOR = sf::Color(255, 73, 68);
	inline const static sf::Color FOOD_COLOR = sf::Color(29, 129, 18);
	inline const static sf::Color TO_FOOD_COLOR = sf::Color(119, 211, 109);
	inline const static sf::Color TO_HOME_COLOR = sf::Color(122, 105, 199);
	inline const static sf::Color COLONY_COLOR = sf::Color(67, 46, 163);
	inline const static uint8_t THREAD_COUNT = 1;
	
	inline const static uint32_t WIN_WIDTH = 1920;
	inline const static uint32_t WIN_HEIGHT = 1080;

	inline const static float MAX_CELL_INTENSITY = 100.0f;
	inline const static float COLONY_SIZE = 20.0f;
};

