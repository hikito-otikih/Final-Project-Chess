#include <Book/ChooseBoard.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RenderWindow.hpp>


ChooseBoard::ChooseBoard(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	auto greenButton = std::make_shared<GUI::Button>(context);
	greenButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	greenButton->setText("Green theme");
	greenButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.boardType = 1;
		});

	auto redButton = std::make_shared<GUI::Button>(context);
	redButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	redButton->setText("Red theme");
	redButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.boardType = 2;
		});

	auto blueButton = std::make_shared<GUI::Button>(context);
	blueButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 100);
	blueButton->setText("Blue theme");
	blueButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.boardType = 3;
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 150);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	mGUIContainer.pack(greenButton);
	mGUIContainer.pack(redButton);
	mGUIContainer.pack(blueButton);
	mGUIContainer.pack(backButton);
}

void ChooseBoard::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool ChooseBoard::update(sf::Time)
{
	return true;
}

bool ChooseBoard::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	return false;
}