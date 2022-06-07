#define _CRT_SECURE_NO_WARNINGS

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "ClassHeader.h"

//#define DEBUG "..\\"
#define DEBUG

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Who am I\?", sf::Style::Fullscreen);
	Game game;

	if (!game.ShowIntro(window))
	{
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		if (!game.ShowMenu(window))
		{
			exit(EXIT_FAILURE);
		}

		if (!game.GetConectingData(DEBUG "..\\File\\data.txt"))
		{
			exit(EXIT_FAILURE);
		}
		
		if (!game.Connection())
		{
			exit(EXIT_FAILURE);
		}

		if (!game.Play(window))
		{
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}




