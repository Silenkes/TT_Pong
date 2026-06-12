#include "Audio/SoundBase.h"

SoundBase::SoundBase()
{
    SoundBuffers[ESoundType::BoundaryHit].loadFromFile("Assets/Audio/BoundaryHit.wav");
    SoundBuffers[ESoundType::BotHit].loadFromFile("Assets/Audio/BotHit.wav");
    SoundBuffers[ESoundType::PlayerHit].loadFromFile("Assets/Audio/PlayerHit.wav");
    SoundBuffers[ESoundType::BotGoal].loadFromFile("Assets/Audio/BotGoal.wav");
    SoundBuffers[ESoundType::PlayerGoal].loadFromFile("Assets/Audio/PlayerGoal.wav");

    for (auto& [Type, Buffer] : SoundBuffers)
    {
        if (Buffer.getSampleCount() > 0)
        {
			Sounds[Type] = std::make_unique<sf::Sound>(Buffer);
		}

        else
        {
			std::cout << "Failed to load sound: " << TypeToString(Type) << std::endl;
        }
	}
}

SoundBase::~SoundBase() = default;

void SoundBase::Play(ESoundType SoundType)
{
    if (SoundType == ESoundType::None)
        return;

    auto Sound = Sounds.find(SoundType);

    if (Sound != Sounds.end())
        Sound->second->play();
}

std::string SoundBase::TypeToString(ESoundType SoundType)
{
    switch (SoundType)
    {
    case ESoundType::BoundaryHit: return "BoundaryHit";
    case ESoundType::BotHit:      return "BotHit";
    case ESoundType::PlayerHit:   return "PlayerHit";
    case ESoundType::BotGoal:     return "BotGoal";
    case ESoundType::PlayerGoal:  return "PlayerGoal";
    default:                      return "None";
    }
}
