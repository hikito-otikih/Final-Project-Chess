#include <Book/SettingOnlyAudio.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/MusicPlayer.hpp>


#include <SFML/Graphics/RenderWindow.hpp>


SettingOnlyAudio::SettingOnlyAudio(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	auto audioButton = std::make_shared<GUI::Button>(context);
	audioButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	audioButton->setText("Audio");
	audioButton->setCallback([this]()
		{
			requestStackPush(States::AudioSettingsStateOnlyMusicTheme);
		});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	mGUIContainer.pack(audioButton);
	mGUIContainer.pack(backButton);
	//getContext().music->setPaused(true);
}

SettingOnlyAudio::~SettingOnlyAudio()
{
	//getContext().music->setPaused(false);
}

void SettingOnlyAudio::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingOnlyAudio::update(sf::Time)
{
	return true;
}

bool SettingOnlyAudio::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	return false;
}