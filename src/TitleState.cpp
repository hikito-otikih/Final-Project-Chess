#include <Book/TitleState.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


TitleState::TitleState(StateStack& stack, Context context)
: State(stack, context)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());
	auto playButton = std::make_shared<GUI::Button>(context);
	playButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	playButton->setText("Play");
	playButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::MenuPlaysState);
		});

	auto settingsButton = std::make_shared<GUI::Button>(context);
	settingsButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this]()
		{
			requestStackPush(States::SettingOnlyAudio);
		});

	auto exitButton = std::make_shared<GUI::Button>(context);
	exitButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
	exitButton->setText("Exit");
	exitButton->setCallback([this]()
		{
			requestStackPop();
		});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(settingsButton);
	mGUIContainer.pack(exitButton);

	// Play menu theme
	Player& player = *getContext().player;
	if (player.musicTheme == 1) context.music->play(Music::MusicTheme1);
	if (player.musicTheme == 2) context.music->play(Music::MusicTheme2);
	if (player.musicTheme == 3) context.music->play(Music::MusicTheme3);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
	Player& player = *getContext().player;
	if (player.musicTheme == - 1) getContext().music->play(Music::MusicTheme1);
	if (player.musicTheme == - 2) getContext().music->play(Music::MusicTheme2);
	if (player.musicTheme == - 3) getContext().music->play(Music::MusicTheme3);
	player.musicTheme = abs(player.musicTheme);

}

bool TitleState::update(sf::Time dt)
{
	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;

	return true;
}