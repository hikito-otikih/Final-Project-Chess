#include <Book/SettingsState.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/MusicPlayer.hpp>


#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	auto audioButton = std::make_shared<GUI::Button>(context);
	audioButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	audioButton->setText("Audio");
	audioButton->setCallback([this]()
	{
			requestStackPush(States::AudioSettings);
	});

	auto boardButton = std::make_shared<GUI::Button>(context);
	boardButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	boardButton->setText("Board");
	boardButton->setCallback([this]()
	{
			requestStackPush(States::ChooseBoard);
	});

	auto piecesButton = std::make_shared<GUI::Button>(context);
	piecesButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 100);
	piecesButton->setText("Pieces");
	piecesButton->setCallback([this]()
	{
			requestStackPush(States::ChoosePieces);
	});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 150);
	backButton->setText("Back");
	backButton->setCallback([this]()
	{
		requestStackPop();
	});
	mGUIContainer.pack(audioButton);
	mGUIContainer.pack(boardButton);
	mGUIContainer.pack(piecesButton);
	mGUIContainer.pack(backButton);
	//getContext().music->setPaused(true);
}

SettingsState::~SettingsState()
{
	//getContext().music->setPaused(false);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}