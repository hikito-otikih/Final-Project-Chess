#ifndef CHECKMATEWHITESTATE_HPP
#define CHECKMATEWHITESTATE_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class CheckMateWhiteState : public State
{
public:
	CheckMateWhiteState(StateStack& stack, Context context);
	~CheckMateWhiteState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // CHECKMATEWHITESTATE_HPP