#include <Book/AudioSettingsState.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RenderWindow.hpp>


AudioSettingsState::AudioSettingsState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	sf::Vector2f windowSize(context.window->getSize());


	// auto musicTheme = std::make_shared<GUI::Button>(context);
	// musicTheme->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y);
	// musicTheme->setText("Choose music theme");
	// musicTheme->setCallback([this]()
	// {
	// 		requestStackPush(States::ChooseMusicTheme);
	// });

	auto musicMission = std::make_shared<GUI::Button>(context);
	musicMission->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 75);
	musicMission->setText("Choose music mission");
	musicMission->setCallback([this]()
	{
			requestStackPush(States::ChooseMusicMission);
	});

	auto volumeButton = std::make_shared<GUI::Button>(context);
	volumeButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 125);
	volumeButton->setText("Volume settings");
	volumeButton->setCallback([this]()
	{
			requestStackPush(States::volumeSettings);
	});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 175);
	backButton->setText("Back");
	backButton->setCallback([this]()
		{
			requestStackPop();
		});
	//mGUIContainer.pack(musicTheme);
	mGUIContainer.pack(musicMission);
	mGUIContainer.pack(volumeButton);
	mGUIContainer.pack(backButton);
}

void AudioSettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool AudioSettingsState::update(sf::Time)
{
	return true;
}

bool AudioSettingsState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);

	return false;
}