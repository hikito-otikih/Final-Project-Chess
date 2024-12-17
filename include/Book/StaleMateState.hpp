#ifndef STALEMATESTATE_HPP
#define STALEMATESTATE_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class StaleMateState : public State
{
public:
	StaleMateState(StateStack& stack, Context context);
	~StaleMateState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // STALEMATESTATE_HPP