#ifndef CHECKMATEBLACKSTATE_HPP
#define CHECKMATEBLACKSTATE_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class CheckMateBlackState : public State
{
public:
	CheckMateBlackState(StateStack& stack, Context context);
	~CheckMateBlackState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // CHECKMATEBLACKSTATE_HPP