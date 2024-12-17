#include <Book/StaleMateState.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


#include <Book/PauseState.hpp>
#include <Book/Button.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


StaleMateState::StaleMateState(StateStack& stack, Context context)
	: State(stack, context)
	, mPausedText()
{
	mShowText = true;
	mTextEffectTime = sf::Time::Zero;
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f windowSize(context.window->getSize());

	mPausedText.setFont(font);
	mPausedText.setString("Stalemate!");
	mPausedText.setColor(sf::Color::White);
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	mText.setFont(font);
	mText.setString("Press any key to continue");
	mText.setColor(sf::Color::White);
	mText.setCharacterSize(30);
	centerOrigin(mText);
	mText.setPosition(sf::Vector2f(context.window->getSize() / 2u));

	mResult.setFont(font);
	mResult.setString("Draw");
	mResult.setColor(sf::Color::White);
	mResult.setCharacterSize(70);
	centerOrigin(mResult);
	mResult.setPosition(0.5f * windowSize.x, 0.6f * windowSize.y);
}

StaleMateState::~StaleMateState()
{
}

void StaleMateState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 100));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	if (mShowText) window.draw(mText);
	window.draw(mResult);
}

bool StaleMateState::update(sf::Time dt)
{
	mTextEffectTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool StaleMateState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		requestStackPop();
		//requestStackPush(States::Menu);
	}
	return false;
}