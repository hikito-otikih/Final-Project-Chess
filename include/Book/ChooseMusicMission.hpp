#ifndef BOOK_CHOOSEMUSICMISSION_HPP
#define BOOK_CHOOSEMUSICMISSION_HPP

#include <Book/State.hpp>
#include <Book/Container.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>


class ChooseMusicMission : public State
{
public:
	ChooseMusicMission(StateStack& stack, Context context);

	virtual void					draw();
	virtual bool					update(sf::Time dt);
	virtual bool					handleEvent(const sf::Event& event);



private:
	sf::Sprite											mBackgroundSprite;
	GUI::Container										mGUIContainer;
};

#endif // BOOK_CHOOSEMUSICMISSION_HPP
