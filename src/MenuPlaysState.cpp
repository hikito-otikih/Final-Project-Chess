#include <Book/MenuPlaysState.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuPlaysState::MenuPlaysState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	sf::Vector2f windowSize(context.window->getSize());
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	auto newButton = std::make_shared<GUI::Button>(context);
	newButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	newButton->setText("New game");
	newButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::NewGame);
		});

	auto savedButton = std::make_shared<GUI::Button>(context);
	savedButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	savedButton->setText("Load saved game");
	savedButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::Loadgame);
		});

	auto mainmenuButton = std::make_shared<GUI::Button>(context);
	mainmenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
	mainmenuButton->setText("Back to main menu");
	mainmenuButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::Title);
		});


	mGUIContainer.pack(newButton);
	mGUIContainer.pack(savedButton);
	mGUIContainer.pack(mainmenuButton);

	getContext().music->setPaused(true);
}

MenuPlaysState::~MenuPlaysState()
{
	getContext().music->setPaused(false);
}

void MenuPlaysState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuPlaysState::update(sf::Time)
{
	return false;
}

bool MenuPlaysState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
