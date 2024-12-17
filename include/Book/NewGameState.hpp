#ifndef BOOK_NEWGAMESTATE_HPP
#define BOOK_NEWGAMESTATE_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>

#include <SFML/Graphics/Sprite.hpp>


class NewGameState : public State
{
public:
	NewGameState(StateStack& stack, Context context);
	~NewGameState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	GUI::Container 		mGUIContainer;
	sf::Sprite			mBackgroundSprite;
};

#endif // BOOK_NEWGAMESTATE_HPP