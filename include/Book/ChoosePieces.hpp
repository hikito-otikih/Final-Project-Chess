#ifndef BOOK_CHOOSEPICES_HPP
#define BOOK_CHOOSEPICES_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class ChoosePieces : public State
{
public:
	ChoosePieces(StateStack& stack, Context context);

	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);



private:
	sf::Sprite											mBackgroundSprite;
	GUI::Container										mGUIContainer;
};

#endif // BOOK_CHOOSEPICES_HPP
