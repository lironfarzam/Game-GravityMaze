#pragma once
#include "constants.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// a singleton of all textures/sound/fonts

class Resources {
public:
	~Resources();
	static Resources& instance();

	sf::Texture& getImage(int index);
	sf::Font& getFont();

	void playSound(int type, bool loop, int vol);

private:
	Resources();
	Resources(const Resources&) = default;
	Resources& operator=(const Resources&) = default;

	std::vector<sf::Texture> m_textures;
	std::vector<sf::SoundBuffer> m_soundsBuffer;
	std::vector<sf::Sound> m_sounds;

	sf::Font m_gameFont;
};
