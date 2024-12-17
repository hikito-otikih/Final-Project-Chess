#ifndef BOOK_CHESSGAME2PLAYERSSTATE_HPP
#define BOOK_CHESSGAME2PLAYERSSTATE_HPP

#include <Book/State.hpp>
#include <Book/chess.hpp>
#include <Book/Piece.hpp>
#include <Book/ResourceIdentifiers.hpp>
#include <Book/ContainerOnlyClick.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <set>


class ChessGame2PlayersState : public State
{
public:
	ChessGame2PlayersState(StateStack& stack, Context context);
	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);
	void addLegalMoves();
	void MAKEMOVE(int x, int y);
	bool getType(int x) { return (x >= 6); };
	void drawMakedMoves(sf::RenderWindow& window);
	sf::Texture getTexture(int type);
	void newBoard();
	std::string newPosition(int i, int j);
	std::string localPosition(int i, int j);
private:
	sf::Text			mText[20];
	sf::RectangleShape	mSquare[10][10];
	sf::Sprite			mBackgroundSprite;
	Piece::Ptr			mPieces[10][10];
	bool isDragging, selected, turn;
	sf::Vector2f offset;
	sf::Vector2i selectedPiece, lastPiece;
	std::set<std::pair<int, int>> legalMoves[10][10], promotion[10][10];
	chess::Board board;
	GUI::ContainerOnlyClick mGUIContainer;
	std::string newPos;
	std::vector<std::string> moveds, removeds;
	std::vector<std::string> Smoveds, Sremoveds;
	bool showText;
	sf::Time mEffectTime;
	float mouseScroll, curOffset;
	bool resetCurOffset;
	Player& mPlayer;
	std::vector<sf::Text> texts;
	sf::Font mFont;
	sf::Vector2i pre1, pre2;

};

#endif // BOOK_CHESSGAME2PLAYERSSTATE_HPP