#ifndef BOOK_SETTINGONLYAUDIO_HPP
#define BOOK_SETTINGONLYAUDIO_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingOnlyAudio : public State
{
public:
	SettingOnlyAudio(StateStack& stack, Context context);
	~SettingOnlyAudio();
	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);



private:
	sf::Sprite											mBackgroundSprite;
	GUI::Container										mGUIContainer;
};

#endif // BOOK_SETTINGONLYAUDIO_HPP
