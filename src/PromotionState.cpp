#include<BOOK/PromotionState.hpp>

#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>


PromotionState::PromotionState(StateStack& stack, Context context)
	: State(stack, context)
{
	
	auto queenButton = std::make_shared<GUI::Button>(context);
	queenButton->setPosition(480, 300);
	queenButton->setText("Queen");
	queenButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.promotionStatus = 'q';
			requestStackPop();
		});

	auto knightButton = std::make_shared<GUI::Button>(context);
	knightButton->setPosition(480, 350);
	knightButton->setText("Knight");
	knightButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.promotionStatus = 'n';
			requestStackPop();
		});

	auto bishopButton = std::make_shared<GUI::Button>(context);
	bishopButton->setPosition(480, 400);
	bishopButton->setText("Bishop");
	bishopButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.promotionStatus = 'b';
			requestStackPop();
		});
	auto rookButton = std::make_shared<GUI::Button>(context);
	rookButton->setPosition(480, 450);
	rookButton->setText("Rook");
	rookButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.promotionStatus = 'r';
			requestStackPop();
		});
	mGUIContainer.pack(queenButton);
	mGUIContainer.pack(knightButton);
	mGUIContainer.pack(bishopButton);
	mGUIContainer.pack(rookButton);
	// Play menu theme
}

void PromotionState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mGUIContainer);
}

bool PromotionState::update(sf::Time)
{
	return true;
}

bool PromotionState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;

	return true;
}