#include <Book/ExtraButton.hpp>
#include <Book/Utility.hpp>
#include <Book/SoundPlayer.hpp>
#include <Book/ResourceHolder.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

ExtraButton::ExtraButton(State::Context context)
: mCallback()
, mSprite(context.textures->get(Textures::ExtraButtons))
, mText("", context.fonts->get(Fonts::Main), 16)
, mIsToggle(false)
, mSounds(*context.sounds)
{
	changeTexture(Normal);

	sf::FloatRect bounds = mSprite.getLocalBounds();
	mText.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

void ExtraButton::setCallback(Callback callback)
{
	mCallback = std::move(callback);
}

void ExtraButton::setText(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}

void ExtraButton::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool ExtraButton::isSelectable() const
{
    return true;
}

void ExtraButton::select()
{
	Component::select();

	changeTexture(Selected);
}

void ExtraButton::deselect()
{
	Component::deselect();

	changeTexture(Normal);
}

void ExtraButton::activate()
{
	Component::activate();

    // If we are toggle then we should show that the button is pressed and thus "toggled".
	if (mIsToggle)
		changeTexture(Pressed);

	if (mCallback)
		mCallback();

    // If we are not a toggle then deactivate the button since we are just momentarily activated.
	if (!mIsToggle)
		deactivate();

	//mSounds.play(SoundEffect::movechess);
	mSounds.play(SoundEffect::Button);
}

void ExtraButton::deactivate()
{
	Component::deactivate();

	if (mIsToggle)
	{
        // Reset texture to right one depending on if we are selected or not.
		if (isSelected())
			changeTexture(Selected);
		else
			changeTexture(Normal);
	}
}

void ExtraButton::handleEvent(const sf::Event&)
{
}

void ExtraButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}

void ExtraButton::changeTexture(Type buttonType)
{
	sf::IntRect textureRect(0, 50*buttonType, 500, 50);
	mSprite.setTextureRect(textureRect);
}

sf::FloatRect ExtraButton::getSizeSprite() const
{
	return mSprite.getGlobalBounds();
}

}
