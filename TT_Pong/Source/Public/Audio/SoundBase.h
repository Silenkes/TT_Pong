#pragma once

#include <SFML/Audio.hpp>
#include <iostream>

enum class ESoundType : uint8_t
{
	None,
	BoundaryHit,
	BotHit,
	PlayerHit,
	BotGoal,
	PlayerGoal,
};

class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	void Play(ESoundType SoundType);
	std::string TypeToString(ESoundType SoundType);

private:
	std::unordered_map<ESoundType, sf::SoundBuffer> SoundBuffers;
	std::unordered_map<ESoundType, std::unique_ptr<sf::Sound>> Sounds;
};