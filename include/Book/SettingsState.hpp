#ifndef BOOK_SETTINGSSTATE_HPP
#define BOOK_SETTINGSSTATE_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SettingsState : public State
{
	public:
										SettingsState(StateStack& stack, Context context);
										~SettingsState();
		virtual void					draw();
		virtual bool					update(sf::Time dt);
		virtual bool					handleEvent(const sf::Event& event);



	private:
		sf::Sprite											mBackgroundSprite;
		GUI::Container										mGUIContainer;
};

#endif // BOOK_SETTINGSSTATE_HPP
