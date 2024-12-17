#include <Book/NewGameState.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

NewGameState::NewGameState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	sf::Vector2f windowSize(context.window->getSize());
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	auto newButton = std::make_shared<GUI::Button>(context);
	newButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	newButton->setText("1 Player");
	newButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer = Player();
			mPlayer.mode = 1;
			requestStackPush(States::ChooseLevel);
		});

	auto savedButton = std::make_shared<GUI::Button>(context);
	savedButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	savedButton->setText("2 Players");
	savedButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer = Player();
			mPlayer.mode = 0;
			requestStackPop();
			requestStackPush(States::ChessGame2Players);
		});

	auto mainmenuButton = std::make_shared<GUI::Button>(context);
	mainmenuButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
	mainmenuButton->setText("Back to play menu");
	mainmenuButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::MenuPlaysState);
		});


	mGUIContainer.pack(newButton);
	mGUIContainer.pack(savedButton);
	mGUIContainer.pack(mainmenuButton);

	getContext().music->setPaused(true);
}

NewGameState::~NewGameState()
{
	getContext().music->setPaused(false);
}

void NewGameState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool NewGameState::update(sf::Time)
{
	return false;
}

bool NewGameState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
