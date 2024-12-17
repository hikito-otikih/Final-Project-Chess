#include <Book/Loadgame.hpp>
#include <Book/Utility.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/ExtraButton.hpp>
#include <Book/Button.hpp>
#include <Book/Player.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <sstream>


std::vector<std::string> getFilesInDirectory(const std::string& directory)
{
	std::vector<std::string> files;
	for (const auto &entry : std::filesystem::directory_iterator(directory))
		if (entry.is_regular_file() && entry.path().extension() == ".json")
			files.push_back(entry.path().filename().string());
	return files;
}

// std::tm stringToDate(const std::string& date_str) {
// 	std::string str = "";
// 	for (int i = 11; i < date_str.size(); i++)
// 		if (date_str[i] == '.') break;
// 		else str += date_str[i];

// 	std::time_t epoch_time = std::stoll(str);
// 	std::tm* TM = std::localtime(&epoch_time);
// 	return *TM;
// }

/*
		"Game at " + toString(1900 + dates[i].tm_year) + " - "
		+ toString(1 + dates[i].tm_mon) + " - "
		+ toString(dates[i].tm_mday) + " - "
		+ toString(dates[i].tm_hour) + " - "
		+ toString(dates[i].tm_min) + " - "
		+ toString(dates[i].tm_sec)
*/

#include <iostream>
#include <cstring>

Loadgame::Loadgame(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	sf::Vector2f windowSize(context.window->getSize());
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));
	files.clear();
	dates.clear();
	pos.clear();
	buttons.clear();
	curOffset = 0;
	files = getFilesInDirectory(".");
	// for (const auto& file : files)
	// 	dates.push_back(stringToDate(file));
	for (int i = 0; i < (int) files.size(); i++)
	{
		pos.push_back(sf::Vector2f(windowSize.x / 2 - 250, windowSize.y / 2 - 125 + 50 * i));
		auto button = std::make_shared<GUI::ExtraButton>(context);
		button->setPosition(pos[i]);
		button->setText(files[i]);

		button->setCallback([this, i]()
			{
				Player& mPlayer = *getContext().player;
				std::ifstream file(files[i]);
				nlohmann::json j;
				file >> j;
				mPlayer.piecesType = j["piecesType"];
				mPlayer.boardType = j["boardType"];
				mPlayer.team = j["team"];
				mPlayer.RealTEAM = j["RealTeam"];
				mPlayer.level = j["level"];
				mPlayer.mode = j["mode"];
				mPlayer.fenCode = j["Board"];
				mPlayer.turn = j["Turn"];
				file.close();
				requestStackPop();
				if (mPlayer.mode == 0) requestStackPush(States::ChessGame2Players);
				if (mPlayer.mode == 1) requestStackPush(States::ChessGame1Player);
			});
		buttons.push_back(button);
		if (i < 5) mGUIContainer.pack(buttons[i]);
	}
	UP = windowSize.y / 2 - 125;
	DOWN = windowSize.y / 2 + 125;
	mouseScroll = 0.f;
	auto backBut = std::make_shared<GUI::Button>(context);
	backBut->setPosition(100.f, 100.f);
	backBut->setText("Back");
	backBut->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::MenuPlaysState);
		});
	backButton = backBut;
	mGUIContainer.pack(backButton);
}

Loadgame::~Loadgame()
{
}

void Loadgame::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);

}

bool Loadgame::update(sf::Time)
{
	//std::cerr << "Hello\n";
	if (mouseScroll == 0.f) return false;
	mGUIContainer.changeComponent();
	float totalHeight = (int)files.size() * 50;
	float visibleHeight = 250;
	float scrollOffset = 0;
	if (totalHeight > visibleHeight)
	{
		scrollOffset = -(totalHeight - visibleHeight);
	}
	if (mouseScroll > 0) curOffset = std::min(0.f, curOffset + mouseScroll);
	if (mouseScroll < 0) curOffset = std::max(scrollOffset, curOffset + mouseScroll);
	mouseScroll = 0;
	bool hasSelection = false;
	int loop = 0;
	for (int i = 0; i < files.size(); i++)
	{
		buttons[i]->setPosition(pos[i].x, pos[i].y + curOffset);
		if (buttons[i]->getPosition().y >= UP && buttons[i]->getPosition().y + 50 <= DOWN)
		{
			mGUIContainer.pack(buttons[i]);
			if (buttons[i]->isSelected())
			{
				hasSelection = true;
				mGUIContainer.changeSelectedChild(loop);
			}
			loop++;
		}
		else
			buttons[i]->deselect();
	}
	if (!hasSelection)
	{
		mGUIContainer.ressetSelectedChild();
	}
	mGUIContainer.pack(backButton);
	return false;
}

bool Loadgame::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		if (event.mouseWheelScroll.delta > 0) mouseScroll += 50.f;
		if (event.mouseWheelScroll.delta < 0) mouseScroll -= 50.f;
	}
	return false;
}
