#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <BOOK/State.hpp>
#include <BOOK/SoundPlayer.hpp>
#include <BOOK/chess.hpp>

#include <string>

class Piece
{
public:
	typedef std::unique_ptr<Piece> Ptr;
	Piece(std::string newPos, sf::Texture texture, bool Type, State::Context context);
	void setPosition(std::string newPos, int moveSound = 0);
	void setPosition(sf::Vector2f newPos);
	void draw(sf::RenderWindow& window);
	sf::Vector2f getPosition() { return mSprite.getPosition(); }
	void setSpritePosition(sf::Vector2f newPos) { mSprite.setPosition(newPos); }
	bool getType() { return type; }
	void setTexture(sf::Texture texture) { mTexture = texture; mSprite.setTexture(mTexture); }
	void update(sf::Time dt);
private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
	sf::Vector2f targetPosition;
	bool type;
	int playSound;
	SoundPlayer& mSoundplayer;
};

#endif // PIECE_HPP