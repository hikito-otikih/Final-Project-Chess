#ifndef BOOK_MENUPLAYSSTATE_HPP
#define BOOK_MENUPLAYSSTATE_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>

#include <SFML/Graphics/Sprite.hpp>


class MenuPlaysState : public State
{
public:
	MenuPlaysState(StateStack& stack, Context context);
	~MenuPlaysState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	GUI::Container 		mGUIContainer;
	sf::Sprite			mBackgroundSprite;
};

#endif // BOOK_MENUPLAYSSTATE_HPP