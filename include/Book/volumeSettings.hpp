#ifndef BOOK_VOLUMESETTINGS_HPP
#define BOOK_VOLUMESETTINGS_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class volumeSettings : public State
{
public:
	volumeSettings(StateStack& stack, Context context);

	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);



private:
	sf::Sprite											mBackgroundSprite;
	GUI::Container										mGUIContainer;
	sf::RectangleShape musicSliderbar, musicSliderHandle, soundSliderbar, soundSliderHandle;
	bool musicSliderIsBeingHeld, soundSliderIsBeingHeld;
	int 												mMusic, mSound;
	sf::Text											mMusicText, mSoundText;
};

#endif // BOOK_VOLUMESETTINGS_HPP
