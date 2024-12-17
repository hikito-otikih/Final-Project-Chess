#include <Book/ChooseMusicMission.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RenderWindow.hpp>


ChooseMusicMission::ChooseMusicMission(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	auto oneButton = std::make_shared<GUI::Button>(context);
	oneButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	oneButton->setText("Option one");
	oneButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.musicMission = - 1;
		});

	auto twoButton = std::make_shared<GUI::Button>(context);
	twoButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	twoButton->setText("Option two");
	twoButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.musicMission = - 2;
		});

	auto threeButton = std::make_shared<GUI::Button>(context);
	threeButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 100);
	threeButton->setText("Option three");
	threeButton->setCallback([this]()
		{
			Player& player = *getContext().player;
			player.musicMission = - 3;
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 150);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	mGUIContainer.pack(oneButton);
	mGUIContainer.pack(twoButton);
	mGUIContainer.pack(threeButton);
	mGUIContainer.pack(backButton);
}

void ChooseMusicMission::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool ChooseMusicMission::update(sf::Time)
{
	return true;
}

bool ChooseMusicMission::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	return false;
}