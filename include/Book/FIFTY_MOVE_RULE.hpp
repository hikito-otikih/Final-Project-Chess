#ifndef FIFTY_MOVE_RULE_HPP
#define FIFTY_MOVE_RULE_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class FIFTYMOVERULE : public State
{
public:
	FIFTYMOVERULE(StateStack& stack, Context context);
	~FIFTYMOVERULE();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // FIFTY_MOVE_RULE_HPP