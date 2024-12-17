#include <Book/ChooseLevel.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

ChooseLevel::ChooseLevel(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mEffectTime = sf::Time::Zero;
	showText = true;
	mText.setFont(context.fonts->get(Fonts::Main));
	mText.setString("Choose level");
	mText.setCharacterSize(70);
	mText.setFillColor(sf::Color::White);
	mText.setOutlineColor(sf::Color::Black);
	mText.setOutlineThickness(2.f);
	centerOrigin(mText);
	mText.setPosition(0.5f * context.window->getSize().x, 0.4f * context.window->getSize().y);


	sf::Vector2f windowSize(context.window->getSize());
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	auto whiteButton = std::make_shared<GUI::Button>(context);
	whiteButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	whiteButton->setText("Easy");
	whiteButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.level = 0;
			requestStackPush(States::ChooseTeamState);
		});

	auto blackButton = std::make_shared<GUI::Button>(context);
	blackButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	blackButton->setText("Medium");
	blackButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.level = 2;
			requestStackPush(States::ChooseTeamState);
		});

	auto randomButton = std::make_shared<GUI::Button>(context);
	randomButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
	randomButton->setText("Hard");
	randomButton->setCallback([this]()
		{
			Player& mPlayer = *getContext().player;
			mPlayer.level = 4;
			requestStackPush(States::ChooseTeamState);
		});

	auto backToMenuPlaysState = std::make_shared<GUI::Button>(context);
	backToMenuPlaysState->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 225);
	backToMenuPlaysState->setText("Back");
	backToMenuPlaysState->setCallback([this]()
		{
			requestStackPop();
		});

	mGUIContainer.pack(whiteButton);
	mGUIContainer.pack(blackButton);
	mGUIContainer.pack(randomButton);
	mGUIContainer.pack(backToMenuPlaysState);

	//getContext().music->setPaused(true);
}

ChooseLevel::~ChooseLevel()
{
	//getContext().music->setPaused(false);
}

void ChooseLevel::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
	if (showText) window.draw(mText);
}

bool ChooseLevel::update(sf::Time dt)
{
	mEffectTime += dt;

	if (mEffectTime >= sf::seconds(0.5f))
	{
		mEffectTime = sf::Time::Zero;
		showText = 1 - showText;
	}

	return false;
}

bool ChooseLevel::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}
