#ifndef INSUFFICIENT_MATERIAL_HPP
#define INSUFFICIENT_MATERIAL_HPP

#include <Book/State.hpp>

#include <SFML/Graphics/Text.hpp>


class INSUFFICIENTMATERIAL : public State
{
public:
	INSUFFICIENTMATERIAL(StateStack& stack, Context context);
	~INSUFFICIENTMATERIAL();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Text			mPausedText, mText, mResult;
	sf::Time			mTextEffectTime;
	bool mShowText;
};

#endif // INSUFFICIENT_MATERIAL_HPP