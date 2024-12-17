#include <Book/ChoosePieces.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>
#include <Book/Button.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

ChoosePieces::ChoosePieces(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	auto classicButton = std::make_shared<GUI::Button>(context);
	classicButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	classicButton->setText("Classic Pieces");
	classicButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.piecesType = - 1;
		});

	auto modernButton = std::make_shared<GUI::Button>(context);
	modernButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	modernButton->setText("Modern Pieces");
	modernButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.piecesType = - 2;
		});

	auto cartoonButton = std::make_shared<GUI::Button>(context);
	cartoonButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 100);
	cartoonButton->setText("Cartoon Pieces");
	cartoonButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.piecesType = - 3;
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 150);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	mGUIContainer.pack(classicButton);
	mGUIContainer.pack(modernButton);
	mGUIContainer.pack(cartoonButton);
	mGUIContainer.pack(backButton);
}

void ChoosePieces::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool ChoosePieces::update(sf::Time)
{
	return true;
}

bool ChoosePieces::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	return false;
}