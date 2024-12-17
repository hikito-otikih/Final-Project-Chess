#ifndef THREEFOLD_REPETITION_HPP
#define THREEFOLD_REPETITION_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class THREEFOLDREPETITION : public State
{
public:
	THREEFOLDREPETITION(StateStack& stack, Context context);
	~THREEFOLDREPETITION();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // THREEFOLD_REPETITION_HPP