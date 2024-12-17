#include <Book/volumeSettings.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/SoundPlayer.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

volumeSettings::volumeSettings(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{	
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 150);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	mMusic = getContext().music->mVolume;
	mSound = getContext().sounds->mVolume;
	mGUIContainer.pack(backButton);
	musicSliderbar = sf::RectangleShape(sf::Vector2f(220, 20));
	musicSliderbar.setFillColor(sf::Color::White);
	musicSliderbar.setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	musicSliderbar.setOutlineColor(sf::Color::Magenta);
	musicSliderbar.setOutlineThickness(5);
	musicSliderHandle = sf::RectangleShape(sf::Vector2f(20, 20));
	musicSliderHandle.setFillColor(sf::Color::Green);
	musicSliderHandle.setPosition(0.5f * windowSize.x - 100 + mMusic * 2, 0.4f * windowSize.y);

	soundSliderbar = sf::RectangleShape(sf::Vector2f(220, 20));
	soundSliderbar.setFillColor(sf::Color::White);
	soundSliderbar.setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	soundSliderbar.setOutlineColor(sf::Color::Magenta);
	soundSliderbar.setOutlineThickness(5);
	soundSliderHandle = sf::RectangleShape(sf::Vector2f(20, 20));
	soundSliderHandle.setFillColor(sf::Color::Green);
	soundSliderHandle.setPosition(0.5f * windowSize.x - 100 + mSound * 2, 0.4f * windowSize.y + 50);
	musicSliderIsBeingHeld = soundSliderIsBeingHeld = false;

	mMusicText.setFont(context.fonts->get(Fonts::Main));
	mMusicText.setPosition(0.5f * windowSize.x + 140, 0.4f * windowSize.y - 10);
	mMusicText.setFillColor(sf::Color::Yellow);
	mMusicText.setCharacterSize(30);
	mSoundText.setFont(context.fonts->get(Fonts::Main));
	mSoundText.setPosition(0.5f * windowSize.x + 140, 0.4f * windowSize.y + 50 - 10);
	mSoundText.setFillColor(sf::Color::Yellow);
	mSoundText.setCharacterSize(30);
	mMusicText.setString("Music: " + std::to_string(mMusic));
	mSoundText.setString("Sound: " + std::to_string(mSound));
}

void volumeSettings::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
	window.draw(musicSliderbar);
	window.draw(musicSliderHandle);
	window.draw(soundSliderbar);
	window.draw(soundSliderHandle);
	window.draw(mMusicText);
	window.draw(mSoundText);
}

bool volumeSettings::update(sf::Time)
{
	return true;
}

#include<iostream>

bool volumeSettings::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
			if (musicSliderHandle.getGlobalBounds().contains(mousePos))
			{
				musicSliderIsBeingHeld = true;
			}
			if (soundSliderHandle.getGlobalBounds().contains(mousePos))
			{
				soundSliderIsBeingHeld = true;
			}
		}
	}
	if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			musicSliderIsBeingHeld = false;
			soundSliderIsBeingHeld = false;
		}
	}
	if (musicSliderIsBeingHeld)
	{
		int x = event.mouseMove.x;
		if (event.mouseMove.x == 0) x = event.mouseButton.x;
		if (x < musicSliderbar.getPosition().x)
		{
			musicSliderHandle.setPosition(musicSliderbar.getPosition().x, musicSliderHandle.getPosition().y);
		}
		else if (x > musicSliderbar.getPosition().x + musicSliderbar.getSize().x - musicSliderHandle.getSize().x)
		{
			musicSliderHandle.setPosition(musicSliderbar.getPosition().x + musicSliderbar.getSize().x - musicSliderHandle.getSize().x, musicSliderHandle.getPosition().y);
		}
		else
		{
			musicSliderHandle.setPosition(x, musicSliderHandle.getPosition().y);
		}
	}
	
	if (soundSliderIsBeingHeld)
	{
		int x = event.mouseMove.x;
		if (event.mouseMove.x == 0) x = event.mouseButton.x;
		if (x < soundSliderbar.getPosition().x)
		{
			soundSliderHandle.setPosition(soundSliderbar.getPosition().x, soundSliderHandle.getPosition().y);
		}
		else if (x > soundSliderbar.getPosition().x + soundSliderbar.getSize().x - soundSliderHandle.getSize().x)
		{
			soundSliderHandle.setPosition(soundSliderbar.getPosition().x + soundSliderbar.getSize().x - soundSliderHandle.getSize().x, soundSliderHandle.getPosition().y);
		}
		else
		{
			soundSliderHandle.setPosition(x, soundSliderHandle.getPosition().y);
		}
	}
	mMusic = (musicSliderHandle.getPosition().x - musicSliderbar.getPosition().x) / 2;
	mSound = (soundSliderHandle.getPosition().x - soundSliderbar.getPosition().x) / 2;
	mMusicText.setString("Music: " + std::to_string(mMusic));
	mSoundText.setString("Sound: " + std::to_string(mSound));

	getContext().music->mVolume = mMusic;
	getContext().music->setVolume(mMusic);
	getContext().sounds->mVolume = mSound;

	return false;
}