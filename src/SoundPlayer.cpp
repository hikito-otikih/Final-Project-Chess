#include <Book/SoundPlayer.hpp>

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds()
{
	mSoundBuffers.load(SoundEffect::Button,			"Media/Sound/Button.wav");
	mSoundBuffers.load(SoundEffect::movechess,		"Media/Sound/movechess.wav");
	mSoundBuffers.load(SoundEffect::capturemove,	"Media/Sound/capturemove.wav");
	mSoundBuffers.load(SoundEffect::movecheck,		"Media/Sound/movecheck.wav");
	mSoundBuffers.load(SoundEffect::gameend,		"Media/Sound/gameend.wav");
	mSoundBuffers.load(SoundEffect::gamedraw,		"Media/Sound/gamedraw.wav");

	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
	mVolume = 50.f;
}

void SoundPlayer::play(SoundEffect::ID effect)
{
	play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
	mSounds.push_back(sf::Sound());
	sf::Sound& sound = mSounds.back();

	sound.setBuffer(mSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);

	sound.setVolume(mVolume);
	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([] (const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}
