#ifndef BOOK_LOADGAME_HPP
#define BOOK_LOADGAME_HPP

#include <Book/State.hpp>
#include <Book/ContainerOnlyClick.hpp>
#include <Book/Component.hpp>

#include <SFML/Graphics/Sprite.hpp>


class Loadgame : public State
{
public:
	Loadgame(StateStack& stack, Context context);
	~Loadgame();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	GUI::ContainerOnlyClick 		mGUIContainer;
	sf::Sprite						mBackgroundSprite;
	float							mouseScroll, UP, DOWN, curOffset;
	std::vector<std::string>		files;
	std::vector<tm>					dates;
	std::vector<sf::Vector2f>		pos;
	std::vector<std::shared_ptr<GUI::Component> > buttons;
	std::shared_ptr<GUI::Component> backButton;
};

#endif // Loadgame