#ifndef PROMOTIONSTATE_HPP
#define PROMOTIONSTATE_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class PromotionState : public State
{
public:
	PromotionState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	GUI::Container			mGUIContainer;
};

#endif // PROMOTIONSTATE_HPP
