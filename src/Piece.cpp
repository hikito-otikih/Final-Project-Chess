#include <BOOK/Piece.hpp>

Piece::Piece(std::string newPos, sf::Texture texture, bool Type, State::Context context) : mSoundplayer(*context.sounds)
{
	playSound = 0;
	type = Type;
	mTexture = texture;
	mSprite.setTexture(mTexture);
	sf::FloatRect spriteSize = mSprite.getLocalBounds();
	mSprite.setOrigin(spriteSize.width / 2.f, spriteSize.height / 2.f);
	setPosition(newPos);
	mSprite.setPosition(targetPosition);
}

void Piece::setPosition(sf::Vector2f newPos)
{
	targetPosition = newPos;
}

void Piece::setPosition(std::string newPos, int moveSound)
{
	playSound = moveSound;
	targetPosition = sf::Vector2f(80 * (newPos[1] - '1' + 1) + (newPos[1] - '1') * 5 + 40, 80 * (newPos[0] - 'a' + 1) + (newPos[0] - 'a') * 5 + 40);
}

void Piece::update(sf::Time dt)
{
	sf::Vector2f direction = targetPosition - mSprite.getPosition();
	float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	if (distance > 10.0)
	{
		direction = direction / distance;
		mSprite.move(1000.f * direction * dt.asSeconds());
	}
	else
	{
		if (playSound)
		{
			if (playSound == 1) mSoundplayer.play(SoundEffect::movechess);
			else 
				if (playSound == 2) mSoundplayer.play(SoundEffect::capturemove);
				else
					if (playSound == 3) mSoundplayer.play(SoundEffect::movecheck);
					else
						if (playSound == 4) mSoundplayer.play(SoundEffect::gameend);
						else
							if (playSound == 5) mSoundplayer.play(SoundEffect::gamedraw);
			playSound = 0;
		}
		mSprite.setPosition(targetPosition);
	}
}

void Piece::draw(sf::RenderWindow& window)
{
	window.draw(mSprite);
}