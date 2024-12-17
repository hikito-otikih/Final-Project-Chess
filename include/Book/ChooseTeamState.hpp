#ifndef BOOK_CHOOSETEAMSTATE_HPP
#define BOOK_CHOOSETEAMSTATE_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>


class ChooseTeamState : public State
{
public:
	ChooseTeamState(StateStack& stack, Context context);
	~ChooseTeamState();

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

#endif // BOOK_CHOOSETEAMSTATE_HPP