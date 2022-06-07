#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "ClassHeader.h"

//#define DEBUG "..\\"
#define DEBUG

Game::Game()
{
	port = 0;
}

Game::~Game()
{
	
}

bool Game::GetConectingData(const char* path)
{
	std::ifstream stream;
	std::string s_ip;
	std::string s_port;

	stream.open(path);
	if (!stream.is_open()) 
	{
		return false;
	}

	GetData(type, stream);
	if (!type.compare("client"))
	{
		GetData(s_ip, stream);
	}
	else
	{
		stream.get();
		stream.get();
	}
	GetData(s_port, stream);
	port = GetPort(s_port);
	GetData(title, stream);
	sf::IpAddress t_ip(s_ip);
	ip = t_ip;
	stream.close();
	return true;
}

void Game::GetData(std::string& string, std::ifstream& stream) 
{
	char symbol = 0;
	size_t i = 0;

	while ((symbol = stream.get()) != '\n')
	{
		if (symbol != '\0')
		{
			string.push_back(symbol);
			i++;
		}
	}

	string[i] = '\0';
}

unsigned int Game::GetPort(std::string& port)
{
	unsigned int int_port = 0;
	unsigned int ten = 1;

	for (int i = port.size() - 1; i >= 0; i--)
	{
		int_port += (port[i] - 48) * ten;
		ten *= 10;
	}
	return int_port;
}

bool Game::Connection()
{
	if (!type.compare("client"))
	{
		sf::Time timeout = sf::seconds(60.0f);

		if (socket.connect(ip, port, timeout) != sf::Socket::Done)
		{
			return false;
		}
	}
	else if (!type.compare("server")) 
	{
		sf::TcpListener listener;

		if (listener.listen(port) != sf::Socket::Done) 
		{
			return false;
		}

		if (listener.accept(socket) != sf::Socket::Done) 
		{
			return false;
		}
	}
	else 
	{
		return false;
	}
	return true;
}

bool Game::PlayMusic(const char* path)
{
	if (!music.openFromFile(path))
	{
		false;
	}
	music.setPlayingOffset(sf::seconds(103));
	music.play();
	music.setLoop(true);
	return true;
}

bool Game::ShowIntro(sf::RenderWindow& window)
{
	if (!PlayMusic(DEBUG "..\\Multimedia\\music\\song.ogg"))
	{
		return false;
	}

	for (int i = 1; i < 100; i++)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (!ShowVideo(window, DEBUG "..\\Multimedia\\intro\\(x).jpg", i))
		{
			return false;
		}
		else
		{
			window.display();
		}
	}
	return true;
}

bool Game::ShowVideo(sf::RenderWindow& window, const char* templ, const int i)
{
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	char number[3];
	size_t j;

	char temp[100];
	sprintf(temp, "%s", templ);

	for (j = strlen(temp); temp[j] != '('; j--)
		continue;
	temp[j + 1] = '\0';
	_itoa(i, number, 10);
	strcat(temp, number);
	strcat(temp, ").jpg");
	image.loadFromFile(temp);

	if (!texture.loadFromImage(image))
	{
		return false;
	}
	sprite.setTexture(texture);
	window.draw(sprite);
	return true;
}

void Game::SetText(sf::Text& text, sf::Font& font, float x, float y, const char* title, size_t size, sf::Color color)
{
	text.setFont(font);
	text.setString(title);
	text.setCharacterSize(size);
	text.setFillColor(color);
	text.setStyle(sf::Text::Bold);
	text.setPosition(x, y);
}

bool Game::ShowMenu(sf::RenderWindow& window)
{
	sf::Text author, play, quit, title;
	size_t i = 1;
	music.setVolume(100.0f);

	sf::Font font;
	if (!font.loadFromFile(DEBUG "..\\Multimedia\\fonts\\MR ROBOT.ttf"))
	{
		return false;
	}

	while (window.isOpen())
	{
		SetText(title, font, 650.0f, 50.0f, "WHO \t AM \t I\?", 100, sf::Color::Red);
		SetText(play, font, 850.0f, 400.0f, "PLAY", 75, sf::Color::White);
		SetText(quit, font, 870.0f, 600.0f, "quit", 75, sf::Color::White);
		SetText(author, font, 150.0f, 900.0f, "CREATED \t BY \t DENYS \t SPYS", 100, sf::Color::Red);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed)
			{	
				if (event.mouseButton.button == sf::Mouse::Left && sf::IntRect(870, 600, 250, 85).contains(sf::Mouse::getPosition(window)))
				{
					window.close();
					exit(EXIT_SUCCESS);
				}
				else if (event.mouseButton.button == sf::Mouse::Left && sf::IntRect(850, 400, 250, 85).contains(sf::Mouse::getPosition(window)))
				{
					return true;
				}
			}
		}

		if (sf::IntRect(870, 600, 200, 85).contains(sf::Mouse::getPosition(window)))
		{
			SetText(quit, font, 870.0f, 600.0f, "quit", 75, sf::Color::Green);
		}
		else if (sf::IntRect(850, 400, 250, 85).contains(sf::Mouse::getPosition(window)))
		{
			SetText(play, font, 850.0f, 400.0f, "PLAY", 75, sf::Color::Green);
		}

		if (!ShowVideo(window, DEBUG "..\\Multimedia\\menu\\(x).jpg", i))
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			i++;
		}
		
		if (i > 52)
		{
			i = 1;
		}

		window.draw(title);
		window.draw(play);
		window.draw(quit);
		window.draw(author);
		window.display();
	}
	
	return true;
}

bool Game::Play(sf::RenderWindow& window)
{
	FILE* file = NULL;
	const size_t size = 1000000;
	char* array = NULL;
	size_t read_data = 0;
	size_t sent_bytes = 0;
	size_t received = 0;

	sf::Text result;
	sf::Font font;

	if (!font.loadFromFile(DEBUG "..\\Multimedia\\fonts\\MR ROBOT.ttf"))
	{
		return false;
	}

	music.setVolume(10.0f);
	if ((array = (char*)malloc(size)) == NULL)
	{
		exit(EXIT_FAILURE);
	}

	std::thread thread(&Game::Receive, this);
	thread.detach();

	bool flag_record = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				free(array);
				window.close();
			}

			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Key::Space && flag_record == false)
				{
					recorder.start();
					flag_record = true;
				}
			}

			else if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Key::Space)
				{
					flag_record = false;
					recorder.stop();
					buffer = recorder.getBuffer();
					buffer.saveToFile(DEBUG "..\\Multimedia\\voice.ogg");

					file = fopen(DEBUG "..\\Multimedia\\voice.ogg", "rb");
					read_data = fread(array, 1, size, file);

					socket.send(array, read_data, sent_bytes);
					printf("Read_bytes: %zd\n", read_data);
					printf("Sent bytes: %zd\n", sent_bytes);
					printf("Send!\n");
					fclose(file);
				}
			}

			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left && sf::IntRect(1700, 100, 250, 85).contains(sf::Mouse::getPosition(window)))
				{
					free(array);
					return true;
				}
				else if (event.mouseButton.button == sf::Mouse::Left && sf::IntRect(100, 100, 250, 85).contains(sf::Mouse::getPosition(window)))
				{
					music.setVolume(100.0f);
					window.clear(sf::Color::Black);
					SetText(result, font, 500.0f, 500.0f, "G r e a t     G a m e !", 100, sf::Color::Red);
					window.draw(result);
					window.display();
					sf::sleep(sf::seconds(5));
					free(array);
					return true;
				}
			}
		}

		DrawingGame(window);
	}

	return true;
}

void Game::Receive()
{
	FILE* file = NULL;
	const size_t size = 1000000;
	char* array = NULL;
	size_t received = 0;

	if ((array = (char*)malloc(size)) == NULL)
	{
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		if (socket.receive(array, size, received) == sf::Socket::Done)
		{
			file = fopen("voice.ogg", "wb");
			fwrite(array, 1, received, file);

			buffer.loadFromFile("voice.ogg");
			sound.setBuffer(buffer);
			sound.play();

			fclose(file);
		}
	}
	free(array);
}

bool Game::DrawingGame(sf::RenderWindow& window)
{
	sf::RectangleShape rectangle_hint;
	sf::Image image_rectangle_title;
	sf::Texture texture_rectangle_title;
	sf::Sprite sprite_rectangle_title;

	sf::Text guess, hint, quit, title, result;
	static size_t i = 1;
	sf::Font font;

	if (!font.loadFromFile(DEBUG "..\\Multimedia\\fonts\\MR ROBOT.ttf"))
	{
		exit(EXIT_FAILURE);
	}

	char char_title[100];
	strcpy(char_title, this->title.c_str());

	rectangle_hint.setSize(sf::Vector2f(800.0f, 200.0f));
	rectangle_hint.setPosition(610.0f, 940.0f);
	rectangle_hint.setFillColor(sf::Color::Red);

	image_rectangle_title.loadFromFile(DEBUG "..\\Multimedia\\auxiliary\\frame.jpg");

	if (!texture_rectangle_title.loadFromImage(image_rectangle_title))
	{
		exit(EXIT_FAILURE);
	}
	sprite_rectangle_title.setTexture(texture_rectangle_title);
	sprite_rectangle_title.setPosition(1173.0f, 265.0f);
	sprite_rectangle_title.setScale(1.1f, 1.1f);

	SetText(title, font, 1250.0f, 300.0f, char_title, 40, sf::Color::White);
	SetText(quit, font, 1700.0f, 100.0f, "quit", 70, sf::Color::Red);
	SetText(guess, font, 100.0f, 100.0f, "guess!", 70, sf::Color::Red);
	SetText(hint, font, 700.0f, 960.0f, "\t\t\t\tHold     space\nto     send     voice     message", 40, sf::Color::Black);

	if (sf::IntRect(1700, 100, 250, 85).contains(sf::Mouse::getPosition(window)))
	{
		SetText(quit, font, 1650.0f, 100.0f, "q  u  i  t", 70, sf::Color::Red);
		window.draw(quit);
	}
	else if (sf::IntRect(100, 100, 250, 85).contains(sf::Mouse::getPosition(window)))
	{
		SetText(guess, font, 100.0f, 100.0f, "g u e s s !", 70, sf::Color::Red);
		window.draw(guess);
	}

	if (!ShowVideo(window, DEBUG "..\\Multimedia\\game\\(x).jpg", i))
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		i++;
	}

	if (i > 17)
	{
		i = 1;
	}

	window.draw(rectangle_hint);
	window.draw(sprite_rectangle_title);
	window.draw(title);
	window.draw(quit);
	window.draw(guess);
	window.draw(hint);
	window.display();
	
	return true;
}