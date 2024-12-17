#ifndef BOOK_CHOOSELEVEL_HPP
#define BOOK_CHOOSELEVEL_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


class ChooseLevel : public State
{
public:
	ChooseLevel(StateStack& stack, Context context);
	~ChooseLevel();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	GUI::Container 		mGUIContainer;
	sf::Sprite			mBackgroundSprite;
	sf::Text 			mText;
	bool showText;
	sf::Time mEffectTime;
};

#endif // BOOK_CHOOSELEVEL_HPP