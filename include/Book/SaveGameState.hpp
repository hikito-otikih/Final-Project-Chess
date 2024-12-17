#ifndef BOOK_SAVEGAMESTATE_HPP
#define BOOK_SAVEGAMESTATE_HPP

#include <Book/State.hpp>
#include <Book/ContainerOnlyClick.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class SaveGameState : public State
{
	public:
										SaveGameState(StateStack& stack, Context context);
										~SaveGameState();
		virtual void					draw();
		virtual bool					update(sf::Time dt);
		virtual bool					handleEvent(const sf::Event& event);



	private:
		GUI::ContainerOnlyClick								mGUIContainer;
		std::string 										fileName;
		bool 												showText;
		sf::Time 											mEffectTime;
};

#endif // BOOK_SAVEGAMESTATE_HPP