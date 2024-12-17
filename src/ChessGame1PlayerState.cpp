#include <Book/ChessGame1PlayerState.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/Button.hpp>
#include <nlohmann/json.hpp>
#include <ctime>
#include <fstream>
#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

ChessGame1PlayerState::ChessGame1PlayerState(StateStack& stack, Context context)
	: State(stack, context), mPlayer(*context.player)
{
	mFont = context.fonts->get(Fonts::Main);
	auto undoButton = std::make_shared<GUI::Button>(context);
	undoButton->setPosition(850, 570);
	undoButton->setText("Undo");
	undoButton->setCallback([this]()
		{
			mPlayer.Undo = 1;
		});

	auto redoButton = std::make_shared<GUI::Button>(context);
	redoButton->setPosition(850, 620);
	redoButton->setText("Redo");
	redoButton->setCallback([this]()
		{
			mPlayer.Redo = 1;
		});
	auto resetGame = std::make_shared<GUI::Button>(context);
	resetGame->setPosition(850, 670);
	resetGame->setText("Reset game");
	resetGame->setCallback([this]()
		{
			mPlayer.Reset = 1;
		});
	auto backToMenu = std::make_shared<GUI::Button>(context);
	backToMenu->setPosition(850, 720);
	backToMenu->setText("New game");
	backToMenu->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::NewGame);
		});
	auto saveGame = std::make_shared<GUI::Button>(context);
	saveGame->setPosition(850, 770);
	saveGame->setText("Save game");
	saveGame->setCallback([this]()
		{
			// nlohmann::json JSON;
			// JSON["piecesType"] = mPlayer.piecesType;
			// JSON["boardType"] = mPlayer.boardType;
			// JSON["team"] = mPlayer.team;
			// JSON["RealTeam"] = mPlayer.RealTEAM;
			// JSON["level"] = mPlayer.level;
			// JSON["mode"] = mPlayer.mode;
			// JSON["Board"] = board.getFen();
			// JSON["Turn"] = turn;
			// ///
			// std::time_t now = std::time(nullptr);
			// std::stringstream ss;
			// ss << "Game_State_" << now << ".json";
			// std::ofstream fout(ss.str());
			// ///
			// fout << JSON.dump(4);
			// fout.close();
			// requestStackPop();
			// requestStackPush(States::Title);
			mPlayer.fenCode = board.getFen();
			mPlayer.turn = turn;
			requestStackPush(States::Savegame);
		});
	mGUIContainer.pack(undoButton);
	mGUIContainer.pack(redoButton);
	mGUIContainer.pack(resetGame);
	mGUIContainer.pack(backToMenu);
	mGUIContainer.pack(saveGame);

	moveds.clear();
	removeds.clear();
	Smoveds.clear();
	Sremoveds.clear();
	mBackgroundSprite.setTexture(context.textures->get(Textures::WoodenBackground));
	if (context.player->musicMission == 1) context.music->play(Music::MusicMission1);
	if (context.player->musicMission == 2) context.music->play(Music::MusicMission2);
	if (context.player->musicMission == 3) context.music->play(Music::MusicMission3);
	//context.music->stop();
	isDragging = false;
	selected = false;
	selectedPiece = sf::Vector2i(8, 8);
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			mSquare[i][j] = sf::RectangleShape(sf::Vector2f(80, 80));
			mSquare[i][j].setPosition((i + 1) * 80 + 5 * i, (j + 1) * 80 + 5 * j);
			if ((i + j) % 2 == 0)
			{
				mSquare[i][j].setFillColor(sf::Color(235, 236, 208));
			}
			else
			{
				mSquare[i][j].setFillColor(sf::Color(115, 149, 82));
			}
			mSquare[i][j].setOutlineColor(sf::Color(136, 81, 41));
			mSquare[i][j].setOutlineThickness(5);
		}
	}
	
	board.setFen(mPlayer.fenCode);
	turn = mPlayer.turn;
	newBoard();
	moveds.push_back(board.getFen());
	addLegalMoves();
	showText = 1;
	mEffectTime = sf::Time::Zero;
	mouseScroll = 0;
	curOffset = 0;
	resetCurOffset = 0;
	texts.clear();
	bot = Bot(mPlayer.level);
	promotionFlag = 0;
	callBot = sf::Time::Zero;
	pre1 = sf::Vector2i(8, 8);
	pre2 = sf::Vector2i(8, 8);
}

#include <iostream>

void ChessGame1PlayerState::addLegalMoves()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			legalMoves[i][j].clear();
			promotion[i][j].clear();
		}
	chess::Movelist moves;
	chess::movegen::legalmoves(moves, board);
	for (const auto& move : moves)
	{
		std::string moveStr = chess::uci::moveToUci(move);
		int OriginRow = '8' - moveStr[1], OringinCow = moveStr[0] - 'a';
		int DestRow = '8' - moveStr[3], DestCow = moveStr[2] - 'a';
		if (mPlayer.team == 1)
		{
			OriginRow = moveStr[1] - '1';
			OringinCow = 'h' - moveStr[0];
			DestRow = moveStr[3] - '1';
			DestCow = 'h' - moveStr[2];
		}
		legalMoves[OriginRow][OringinCow].insert(std::make_pair(DestRow, DestCow));
		if (moveStr.size() == 5)
			promotion[OriginRow][OringinCow].insert(std::make_pair(DestRow, DestCow));
	}
	moves.clear();
}

void ChessGame1PlayerState::drawMakedMoves(sf::RenderWindow& window)
{
	sf::RectangleShape scrollArea;
	scrollArea.setSize(sf::Vector2f(300, 540));
	scrollArea.setPosition(800, 10);
	scrollArea.setFillColor(sf::Color::Transparent);
	scrollArea.setOutlineColor(sf::Color::White);
	scrollArea.setOutlineThickness(5);
	window.draw(scrollArea);
	///
	float totalHeight = (moveds.size() + 2) / 2 * 35;
	float visibleHeight = scrollArea.getSize().y;
	float scrollOffset = 0;
	if (totalHeight > visibleHeight)
	{
		scrollOffset = -(totalHeight - visibleHeight);
	}
	if (mouseScroll > 0) curOffset = std::min(0.f, curOffset + mouseScroll);
	if (mouseScroll < 0) curOffset = std::max(scrollOffset, curOffset + mouseScroll);
	mouseScroll = 0;
	if (resetCurOffset)
	{
		curOffset = scrollOffset;
		resetCurOffset = 0;
	}
	texts.clear();
	for (int i = 0; i < Smoveds.size(); i++)
	{
		sf::Text text;
		text.setFont(mFont);
		text.setString(Smoveds[i]);
		text.setCharacterSize(30);
		text.setFillColor(sf::Color::White);
		if (i & 1)
			text.setFillColor(sf::Color::Black);
		text.setPosition(810 + i % 2 * 150, 20 + i / 2 * 35 + curOffset);
		texts.push_back(text);
	}
	window.draw(scrollArea);
	for (int i = 0; i < texts.size(); i++)
		if (texts[i].getPosition().y >= scrollArea.getPosition().y && texts[i].getPosition().y + 30 <= scrollArea.getPosition().y + scrollArea.getSize().y)
			window.draw(texts[i]);
}

//#include <SFML/Graphics/Texture.hpp>

sf::Texture ChessGame1PlayerState::getTexture(int type)
{
	if (type == 0)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEPAWN);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEPAWN);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEPAWN);
	}
	if (type == 1)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEKNIGHT);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEKNIGHT);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEKNIGHT);
	}
	if (type == 2)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEBISHOP);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEBISHOP);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEBISHOP);
	}
	if (type == 3)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEROOK);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEROOK);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEROOK);
	}
	if (type == 4)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEQUEEN);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEQUEEN);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEQUEEN);
	}
	if (type == 5)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::WHITEKING);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernWHITEKING);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonWHITEKING);
	}
	if (type == 6)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKPAWN);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKPAWN);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKPAWN);
	}
	if (type == 7)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKKNIGHT);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKKNIGHT);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKKNIGHT);
	}
	if (type == 8)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKBISHOP);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKBISHOP);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKBISHOP);
	}
	if (type == 9)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKROOK);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKROOK);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKROOK);
	}
	if (type == 10)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKQUEEN);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKQUEEN);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKQUEEN);
	}
	if (type == 11)
	{
		if (mPlayer.piecesType == 1) return getContext().textures->get(Textures::BLACKKING);
		if (mPlayer.piecesType == 2) return getContext().textures->get(Textures::ModernBLACKKING);
		if (mPlayer.piecesType == 3) return getContext().textures->get(Textures::CartoonBLACKKING);
	}
	return sf::Texture();
}

std::string ChessGame1PlayerState::newPosition(int i, int j)
{
	std::string ans = "";
	ans += char(j + 'a');
	ans += char(7 - i + '1');
	if (mPlayer.team == 1)
	{
		ans[0] = char(7 - j + 'a');
		ans[1] = char(i + '1');
	}
	return ans;
}

std::string ChessGame1PlayerState::localPosition(int i, int j)
{
	std::string ans = "";
	ans += char(i + 'a');
	ans += char(j + '1');
	return ans;
}

void ChessGame1PlayerState::newBoard()
{
	for (int i = 0; i < 8; i++)
	{
		int k = mPlayer.team == 0 ? i : 7 - i;
		mText[i].setFont(mFont);
		mText[i].setString(char(i + 'A'));
		mText[i].setCharacterSize(50);
		mText[i].setFillColor(sf::Color::Black);
		mText[i].setPosition((k + 1) * 80 + 5 * k + 40, 760 + 40);
	}
	for (int i = 0; i < 8; i++)
	{
		int k = mPlayer.team == 0 ? i : 7 - i;
		mText[i + 8].setFont(mFont);
		mText[i + 8].setString(std::to_string(i + 1));
		mText[i + 8].setCharacterSize(50);
		mText[i + 8].setFillColor(sf::Color::Black);
		int j = 7 - k;
		mText[i + 8].setPosition(40, (j + 1) * 80 + 5 * j + 40);
	}

	for (int i = 0; i < 16; i++)
	{
		sf::FloatRect textRect = mText[i].getLocalBounds();
		mText[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int type = board.at(chess::Square(newPosition(i, j)));
			if (type == 12) mPieces[i][j] = nullptr;
			else
				mPieces[i][j] = std::make_unique<Piece>(localPosition(i, j), getTexture(type), getType(type), getContext());
		}
}

void ChessGame1PlayerState::draw()
{
	if (mPlayer.musicMission == -1) getContext().music->play(Music::MusicMission1);
	if (mPlayer.musicMission == -2) getContext().music->play(Music::MusicMission2);
	if (mPlayer.musicMission == -3) getContext().music->play(Music::MusicMission3);
	mPlayer.musicMission = abs(mPlayer.musicMission);
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	drawMakedMoves(window);
	window.draw(mGUIContainer);
	sf::Texture FlipButtonTexture = getContext().textures->get(Textures::FLIPBUTTON);
	sf::Sprite FlipButton(FlipButtonTexture);
	FlipButton.setPosition(770, 570);
	window.draw(FlipButton);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((i + j) % 2 != 0)
			{
				if (mPlayer.boardType == 1) mSquare[i][j].setFillColor(sf::Color(115, 149, 82));
				if (mPlayer.boardType == 2) mSquare[i][j].setFillColor(sf::Color(195, 73, 57));
				if (mPlayer.boardType == 3) mSquare[i][j].setFillColor(sf::Color(102, 102, 224));
			}
			window.draw(mSquare[i][j]);
		}
	}
	if (selected)
	{
		for (const auto& move : legalMoves[selectedPiece.x][selectedPiece.y])
		{
			sf::RectangleShape Square(sf::Vector2f(80, 80));
			Square.setPosition((move.second + 1) * 80 + 5 * move.second, (move.first + 1) * 80 + 5 * move.first);
			Square.setFillColor(sf::Color(0, 0, 0, 100));
			Square.setOutlineColor(sf::Color::Yellow);
			Square.setOutlineThickness(5);
			window.draw(Square);
		}
		sf::RectangleShape Square(sf::Vector2f(80, 80));
		Square.setPosition((selectedPiece.y + 1) * 80 + 5 * selectedPiece.y, (selectedPiece.x + 1) * 80 + 5 * selectedPiece.x);
		Square.setFillColor(sf::Color(0, 0, 0, 100));
		Square.setOutlineColor(sf::Color::White);
		Square.setOutlineThickness(5);
		window.draw(Square);
	}
	if (pre1 != sf::Vector2i(8, 8))
	{
		sf::CircleShape Circle(40);
		Circle.setFillColor(sf::Color(0, 0, 0, 0));
		Circle.setOutlineColor(sf::Color(255,40,40));
		Circle.setOutlineThickness(5);
		Circle.setPosition((pre1.y + 1) * 80 + 5 * pre1.y, (pre1.x + 1) * 80 + 5 * pre1.x);
		window.draw(Circle);
		sf::CircleShape Circle2(40);
		Circle2.setFillColor(sf::Color(0, 0, 0, 0));
		Circle2.setOutlineColor(sf::Color(255, 94, 0));
		Circle2.setOutlineThickness(5);
		Circle2.setPosition((pre2.y + 1) * 80 + 5 * pre2.y, (pre2.x + 1) * 80 + 5 * pre2.x);
		window.draw(Circle2);
	}
	for (int i = 0; i < 16; i++)
	{
		window.draw(mText[i]);
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			if (i != selectedPiece.x || j != selectedPiece.y)
			{
				if (mPieces[i][j])
				{
					mPieces[i][j]->draw(window);
				}
			}
	}
	if (selectedPiece != sf::Vector2i(8, 8))
	{
		mPieces[selectedPiece.x][selectedPiece.y]->draw(window);
	}
	sf::Text text;
	text.setPosition(100, 0);
	if (turn == 0) { text.setString("White's turn"); text.setFillColor(sf::Color::Black); }
	else { text.setString("Black's turn"); text.setFillColor(sf::Color::White); }
	text.setFont(mFont);
	text.setCharacterSize(60);
	window.draw(text);
	if (board.inCheck())
	{
		sf::Text inCheck;
		inCheck.setPosition(450, 20);
		if (turn == 0) { inCheck.setString("White is in check"); inCheck.setFillColor(sf::Color::Red); }
		else { inCheck.setString("Black is in check"); inCheck.setFillColor(sf::Color::Red); }
		inCheck.setFont(mFont);
		inCheck.setCharacterSize(40);
		if (showText) window.draw(inCheck);
	}
}

bool ChessGame1PlayerState::update(sf::Time dt)
{
	std::cerr << "update\n";
	mEffectTime += dt;
	if (mEffectTime >= sf::seconds(0.5f))
	{
		mEffectTime = sf::Time::Zero;
		showText = 1 - showText;
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (mPieces[i][j] != nullptr)
				mPieces[i][j]->update(dt);

	if (mPlayer.piecesType < 0)
	{
		mPlayer.piecesType = -mPlayer.piecesType;
		newBoard();
	}
	if (mPlayer.promotionStatus != '\0' && mPlayer.promotionStatus != '.')
	{
		board.unmakeMove(chess::uci::uciToMove(board, newPos));
		moveds.pop_back();
		newPos += mPlayer.promotionStatus;
		board.makeMove(chess::uci::uciToMove(board, newPos));
		moveds.push_back(newPos);
		int i = lastPiece.x, j = lastPiece.y;
		if (mPlayer.promotionStatus == 'q')
		{
			if (turn == 1) 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::WHITEQUEEN));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernWHITEQUEEN));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonWHITEQUEEN));
			}
			else
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::BLACKQUEEN));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernBLACKQUEEN));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonBLACKQUEEN));
			}
		}
		if (mPlayer.promotionStatus == 'n')
		{
			if (turn == 1) 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::WHITEKNIGHT));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernWHITEKNIGHT));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonWHITEKNIGHT));
			}
			else 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::BLACKKNIGHT));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernBLACKKNIGHT));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonBLACKKNIGHT));
			}
		}
		if (mPlayer.promotionStatus == 'b')
		{
			if (turn == 1) 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::WHITEBISHOP));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernWHITEBISHOP));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonWHITEBISHOP));
			}
			else 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::BLACKBISHOP));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernBLACKBISHOP));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonBLACKBISHOP));
			}
		}
		if (mPlayer.promotionStatus == 'r')
		{
			if (turn == 1) 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::WHITEROOK));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernWHITEROOK));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonWHITEROOK));
			}
			else 
			{
				if (mPlayer.piecesType == 1) mPieces[i][j]->setTexture(getContext().textures->get(Textures::BLACKROOK));
				if (mPlayer.piecesType == 2) mPieces[i][j]->setTexture(getContext().textures->get(Textures::ModernBLACKROOK));
				if (mPlayer.piecesType == 3) mPieces[i][j]->setTexture(getContext().textures->get(Textures::CartoonBLACKROOK));
			}
		}

		moveds.pop_back();
		moveds.push_back(board.getFen());
		mPlayer.promotionStatus = '\0';
		promotionFlag = 0;
		addLegalMoves();
		auto [resultReason, resultValue] = board.isGameOver();
		if (resultValue == chess::GameResult::LOSE)
		{
			if (turn == 1)
				requestStackPush(States::CheckMateWhite);
			else
				requestStackPush(States::CheckMateBlack);
		}
		if (resultValue == chess::GameResult::DRAW)
		{
			if (resultReason == chess::GameResultReason::STALEMATE)
			{
				requestStackPush(States::StaleMate);
			}
			if (resultReason == chess::GameResultReason::FIFTY_MOVE_RULE)
			{
				requestStackPush(States::FIFTY_MOVE_RULE);
			}
			if (resultReason == chess::GameResultReason::INSUFFICIENT_MATERIAL)
			{
				requestStackPush(States::INSUFFICIENT_MATERIAL);
			}
			if (resultReason == chess::GameResultReason::THREEFOLD_REPETITION)
			{
				requestStackPush(States::THREEFOLD_REPETITION);
			}
		}
		std::cerr << "Promotion\n";
	}
	
	if (turn != mPlayer.RealTEAM && promotionFlag == 0)
	{
		callBot += dt;
		if (callBot.asSeconds() > 0.75f)
		{
			callBot = sf::Time::Zero;
			std::cerr << bot.MAXDEPTH << '\n';
			std::cerr << "call bot 1\n";
			chess::Move move = bot.move(board);
			std::cerr << "call bot 2\n";
			std::cerr << chess::uci::moveToUci(move) << '\n';
			if (move != chess::Move())
			{
				MAKEMOVE(move);
				selectedPiece = sf::Vector2i(8, 8);
				turn = 1 - turn;
				addLegalMoves();
			}
		}
	}

	if (mPlayer.Reset == 1)
	{
		board = chess::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		removeds.clear();
		Sremoveds.clear();
		moveds.clear();
		Smoveds.clear();
		moveds.push_back(board.getFen());
		newBoard();
		addLegalMoves();
		turn = 0;
		mPlayer.Reset = 0;
		selectedPiece = sf::Vector2i(8, 8);
		selected = false;
		pre1 = sf::Vector2i(8, 8);
		pre2 = sf::Vector2i(8, 8);
	}
	if (mPlayer.Undo == 1)
	{
		if (Smoveds.size() > 0)
		{
			removeds.push_back(moveds.back());
			Sremoveds.push_back(Smoveds.back());
			moveds.pop_back();
			Smoveds.pop_back();
			board.setFen(moveds.back());
			turn = 1 - turn;
			newBoard();
			addLegalMoves();
		}
		mPlayer.Undo = 0;
		selectedPiece = sf::Vector2i(8, 8);
		selected = false;
		pre1 = sf::Vector2i(8, 8);
		pre2 = sf::Vector2i(8, 8);
	}
	if (mPlayer.Redo == 1)
	{
		if (removeds.size() > 0)
		{
			board.setFen(removeds.back());
			moveds.push_back(removeds.back());
			Smoveds.push_back(Sremoveds.back());
			removeds.pop_back();
			Sremoveds.pop_back();
			turn = 1 - turn;
			newBoard();
			addLegalMoves();
		}
		mPlayer.Redo = 0;
		selectedPiece = sf::Vector2i(8, 8);
		selected = false;
		pre1 = sf::Vector2i(8, 8);
		pre2 = sf::Vector2i(8, 8);
	}
	std::cerr << "end update\n";
	return true;
}

void ChessGame1PlayerState::MAKEMOVE(chess::Move move)
{
	resetCurOffset = 1;
	removeds.clear();
	Sremoveds.clear();
	std::string moveStr = chess::uci::moveToUci(move);
	int i, j;
	if (mPlayer.team == 0)
	{
		i = '8' - moveStr[3], j = moveStr[2] - 'a';
		selectedPiece = sf::Vector2i('8' - moveStr[1], moveStr[0] - 'a');
	}
	else
	{
		i = moveStr[3] - '1', j = 'h' - moveStr[2];
		selectedPiece = sf::Vector2i(moveStr[1] - '1', 'h' - moveStr[0]);
	}
	pre1 = selectedPiece;
	pre2 = sf::Vector2i(i, j);
	mPieces[i][j] = std::move(mPieces[selectedPiece.x][selectedPiece.y]);
	mPieces[selectedPiece.x][selectedPiece.y] = nullptr;
	newPos = newPosition(selectedPiece.x, selectedPiece.y) + newPosition(i, j);
	int makeSomeNoise = 1;
	if (board.isCapture(chess::uci::uciToMove(board, newPos))) makeSomeNoise = 2;

	if (moveStr.size() == 5)
	{
		mPlayer.promotionStatus = moveStr[4];
	}
	
	chess::Square enPassantSq = board.enpassantSq();
	board.makeMove(chess::uci::uciToMove(board, newPos));
	moveds.push_back(board.getFen());
	Smoveds.push_back(newPos);
	lastPiece = sf::Vector2i(i, j);
	std::string Pos = newPos;
	Pos.erase(0, 2);
	if (enPassantSq == chess::Square(Pos))
		mPieces[7 + '1' - newPos[1]][newPos[2] - 'a'] = nullptr;
	if (newPos == "e1g1")
	{
		if (mPlayer.team == 0)
		{
			mPieces[7][5] = std::move(mPieces[7][7]);
			mPieces[7][7] = nullptr;
			mPieces[7][5]->setPosition("h6");
		}
		else
		{
			mPieces[0][2] = std::move(mPieces[0][0]);
			mPieces[0][0] = nullptr;
			mPieces[0][2]->setPosition("a3");
		}
	}
	if (newPos == "e1c1")
	{
		if (mPlayer.team == 0)
		{
			mPieces[7][3] = std::move(mPieces[7][0]);
			mPieces[7][0] = nullptr;
			mPieces[7][3]->setPosition("h4");
		}
		else
		{
			mPieces[0][4] = std::move(mPieces[0][7]);
			mPieces[0][7] = nullptr;
			mPieces[0][4]->setPosition("a5");
		}
	}
	if (newPos == "e8g8")
	{
		if (mPlayer.team == 0)
		{
			mPieces[0][5] = std::move(mPieces[0][7]);
			mPieces[0][7] = nullptr;
			mPieces[0][5]->setPosition("a6");
		}
		else
		{
			mPieces[7][2] = std::move(mPieces[7][0]);
			mPieces[7][0] = nullptr;
			mPieces[7][2]->setPosition("h3");
		}
	}
	if (newPos == "e8c8")
	{
		if (mPlayer.team == 0)
		{
			mPieces[0][3] = std::move(mPieces[0][0]);
			mPieces[0][0] = nullptr;
			mPieces[0][3]->setPosition("a4");
		}
		else
		{
			mPieces[7][4] = std::move(mPieces[7][7]);
			mPieces[7][7] = nullptr;
			mPieces[7][4]->setPosition("h5");

		}
	}
	if (board.inCheck()) makeSomeNoise = 3;
	auto [resultReason, resultValue] = board.isGameOver();
	if (resultValue == chess::GameResult::LOSE)
	{
		if (turn == 0)
			requestStackPush(States::CheckMateWhite);
		else
			requestStackPush(States::CheckMateBlack);
		makeSomeNoise = 4;
	}
	if (resultValue == chess::GameResult::DRAW)
	{
		if (resultReason == chess::GameResultReason::STALEMATE)
		{
			requestStackPush(States::StaleMate);
		}
		if (resultReason == chess::GameResultReason::FIFTY_MOVE_RULE)
		{
			requestStackPush(States::FIFTY_MOVE_RULE);
		}
		if (resultReason == chess::GameResultReason::INSUFFICIENT_MATERIAL)
		{
			requestStackPush(States::INSUFFICIENT_MATERIAL);
		}
		if (resultReason == chess::GameResultReason::THREEFOLD_REPETITION)
		{
			requestStackPush(States::THREEFOLD_REPETITION);
		}
		makeSomeNoise = 5;
	}
	mPieces[i][j]->setPosition(localPosition(i, j), makeSomeNoise);
}


void ChessGame1PlayerState::MAKEMOVE(int i, int j)
{
	resetCurOffset = 1;
	removeds.clear();
	Sremoveds.clear();
	mPieces[i][j] = std::move(mPieces[selectedPiece.x][selectedPiece.y]);
	mPieces[selectedPiece.x][selectedPiece.y] = nullptr;
	newPos = newPosition(selectedPiece.x, selectedPiece.y) + newPosition(i, j);
	pre1 = selectedPiece;
	pre2 = sf::Vector2i(i, j);
	int makeSomeNoise = 1;
	if (board.isCapture(chess::uci::uciToMove(board, newPos))) makeSomeNoise = 2;

	if (promotion[selectedPiece.x][selectedPiece.y].find(std::make_pair(i, j)) != promotion[selectedPiece.x][selectedPiece.y].end())
	{
		mPlayer.promotionStatus = '.';
		promotionFlag = 1;
		requestStackPush(States::PromotionState);
	}
	chess::Square enPassantSq = board.enpassantSq();
	board.makeMove(chess::uci::uciToMove(board, newPos));
	moveds.push_back(board.getFen());
	Smoveds.push_back(newPos);
	lastPiece = sf::Vector2i(i, j);
	std::string Pos = newPos;
	Pos.erase(0, 2);
	if (enPassantSq == chess::Square(Pos))
		mPieces[7 + '1' - newPos[1]][newPos[2] - 'a'] = nullptr;

	if (newPos == "e1g1")
	{
		if (mPlayer.team == 0)
		{
			mPieces[7][5] = std::move(mPieces[7][7]);
			mPieces[7][7] = nullptr;
			mPieces[7][5]->setPosition("h6");
		}
		else
		{
			mPieces[0][2] = std::move(mPieces[0][0]);
			mPieces[0][0] = nullptr;
			mPieces[0][2]->setPosition("a3");
		}
	}
	if (newPos == "e1c1")
	{
		if (mPlayer.team == 0)
		{
			mPieces[7][3] = std::move(mPieces[7][0]);
			mPieces[7][0] = nullptr;
			mPieces[7][3]->setPosition("h4");
		}
		else
		{
			mPieces[0][4] = std::move(mPieces[0][7]);
			mPieces[0][7] = nullptr;
			mPieces[0][4]->setPosition("a5");
		}
	}
	if (newPos == "e8g8")
	{
		if (mPlayer.team == 0)
		{
			mPieces[0][5] = std::move(mPieces[0][7]);
			mPieces[0][7] = nullptr;
			mPieces[0][5]->setPosition("a6");
		}
		else
		{
			mPieces[7][2] = std::move(mPieces[7][0]);
			mPieces[7][0] = nullptr;
			mPieces[7][2]->setPosition("h3");
		}
	}
	if (newPos == "e8c8")
	{
		if (mPlayer.team == 0)
		{
			mPieces[0][3] = std::move(mPieces[0][0]);
			mPieces[0][0] = nullptr;
			mPieces[0][3]->setPosition("a4");
		}
		else
		{
			mPieces[7][4] = std::move(mPieces[7][7]);
			mPieces[7][7] = nullptr;
			mPieces[7][4]->setPosition("h5");

		}
	}
	if (board.inCheck()) makeSomeNoise = 3;
	auto [resultReason, resultValue] = board.isGameOver();
	if (resultValue == chess::GameResult::LOSE)
	{
		if (turn == 0)
			requestStackPush(States::CheckMateWhite);
		else
			requestStackPush(States::CheckMateBlack);
		makeSomeNoise = 4;
	}
	if (resultValue == chess::GameResult::DRAW)
	{
		if (resultReason == chess::GameResultReason::STALEMATE)
		{
			requestStackPush(States::StaleMate);
		}
		if (resultReason == chess::GameResultReason::FIFTY_MOVE_RULE)
		{
			requestStackPush(States::FIFTY_MOVE_RULE);
		}
		if (resultReason == chess::GameResultReason::INSUFFICIENT_MATERIAL)
		{
			requestStackPush(States::INSUFFICIENT_MATERIAL);
		}
		if (resultReason == chess::GameResultReason::THREEFOLD_REPETITION)
		{
			requestStackPush(States::THREEFOLD_REPETITION);
		}
		makeSomeNoise = 5;
	}
	mPieces[i][j]->setPosition(localPosition(i, j), makeSomeNoise);
}

bool ChessGame1PlayerState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
			if (mousePos.x >= 770 && mousePos.x <= 830 && mousePos.y >= 570 && mousePos.y <= 630)
			{
				mPlayer.team = 1 - mPlayer.team;
				selectedPiece = sf::Vector2i(8, 8);
				selected = false;
				pre1 = sf::Vector2i(8, 8);
				pre2 = sf::Vector2i(8, 8);
				newBoard();
				addLegalMoves();
			}
		}
	}
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.delta > 0) mouseScroll += 35.f;
		if (event.mouseWheelScroll.delta < 0) mouseScroll -= 35.f;
	}
	else mouseScroll = 0;
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStackPush(States::Pause);
	else
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
						if (mPieces[i][j] != nullptr && mPieces[i][j]->getType() == turn && turn == mPlayer.RealTEAM)
						{
							sf::Vector2f piecePos = sf::Vector2f((j + 1) * 80 + j * 5, (i + 1) * 80 + i * 5);
							if (piecePos.x <= mousePos.x && mousePos.x <= piecePos.x + 80)
								if (piecePos.y <= mousePos.y && mousePos.y <= piecePos.y + 80)
								{
									isDragging = true;
									selectedPiece = sf::Vector2i(i, j);
									selected = true;
								}
						}
				}
			}
		}
	if (event.type == sf::Event::MouseButtonReleased && isDragging == false && selected)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					sf::Vector2f piecePos = sf::Vector2f((j + 1) * 80 + j * 5, (i + 1) * 80 + i * 5);
					if (piecePos.x <= mousePos.x && mousePos.x <= piecePos.x + 80)
						if (piecePos.y <= mousePos.y && mousePos.y <= piecePos.y + 80)
						{
							if (legalMoves[selectedPiece.x][selectedPiece.y].find(std::make_pair(i, j)) != legalMoves[selectedPiece.x][selectedPiece.y].end())
							{
								MAKEMOVE(i, j);
								std::cerr << "promotion status" << mPlayer.promotionStatus << '\n';
								//
								selectedPiece = sf::Vector2i(8, 8);
								selected = false;
								turn = 1 - turn;
								addLegalMoves();
							}
						}

				}
			}
		}
	}
	if (event.type == sf::Event::MouseButtonReleased && isDragging == true)
	{
		isDragging = false;
		bool isLegal = false;
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				sf::Vector2f piecePos = sf::Vector2f((j + 1) * 80 + j * 5, (i + 1) * 80 + i * 5);
				if (piecePos.x <= mousePos.x && mousePos.x <= piecePos.x + 80)
					if (piecePos.y <= mousePos.y && mousePos.y <= piecePos.y + 80)
					{
						if (legalMoves[selectedPiece.x][selectedPiece.y].find(std::make_pair(i, j)) != legalMoves[selectedPiece.x][selectedPiece.y].end())
						{
							MAKEMOVE(i, j);
							//
							selectedPiece = sf::Vector2i(8, 8);
							isLegal = true;
							selected = false;
							turn = 1 - turn;
							addLegalMoves();
						}
					}

			}
		}
		if (!isLegal)
			if (mPieces[selectedPiece.x][selectedPiece.y] != nullptr)
				mPieces[selectedPiece.x][selectedPiece.y]->setPosition(localPosition(selectedPiece.x, selectedPiece.y));
	}

	if (isDragging)
	{
		sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
		if (event.type == sf::Event::MouseMoved) mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
		mousePos.x = std::min(mousePos.x, 760.f);
		mousePos.x = std::max(mousePos.x, 80.f);
		mousePos.y = std::min(mousePos.y, 760.f);
		mousePos.y = std::max(mousePos.y, 80.f);
		if (mPieces[selectedPiece.x][selectedPiece.y] != nullptr)
		{
			mPieces[selectedPiece.x][selectedPiece.y]->setSpritePosition(mousePos);
			mPieces[selectedPiece.x][selectedPiece.y]->setPosition(mousePos);
		}
	}
	/*if (turn != mPlayer.team && promotionFlag == 0)
	{
		std::cerr << bot.MAXDEPTH << '\n';
		std::cerr << "call bot 1\n";
		chess::Move move = bot.move(board);
		std::cerr << "call bot 2\n";
		std::cerr << chess::uci::moveToUci(move) << '\n';
		MAKEMOVE(move);
		selectedPiece = sf::Vector2i(8, 8);
		turn = 1 - turn;
		addLegalMoves();
	}*/

	std::cerr << "end process\n";
	return true;
}