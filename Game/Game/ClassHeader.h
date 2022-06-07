#ifndef CLASS_HEADER_H_
#define CLASS_HEADER_H_

class Game
{
private:
	sf::IpAddress ip;
	unsigned int port;
	std::string type;
	std::string title;
	sf::TcpSocket socket;
	sf::Music music;
	sf::SoundBufferRecorder recorder;
	sf::SoundBuffer buffer;
	sf::Sound sound;

private:
	void GetData(std::string& string, std::ifstream& stream);
	unsigned int GetPort(std::string& port);
	bool PlayMusic(const char* path);
	void SetText(sf::Text& text, sf::Font& font, float x, float y, const char* title, size_t size, sf::Color color);
	bool ShowVideo(sf::RenderWindow& window, const char* templ, const int i);
	void Receive();
	bool DrawingGame(sf::RenderWindow& window);
public:
	Game();
	~Game();
	bool GetConectingData(const char* path);
	bool Connection();
	bool ShowIntro(sf::RenderWindow& window);
	bool ShowMenu(sf::RenderWindow& window);
	bool Play(sf::RenderWindow& window);
};

#endif
