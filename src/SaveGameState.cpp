#include <Book/SaveGameState.hpp>
#include <Book/Utility.hpp>
#include <Book/ResourceHolder.hpp>
#include <Book/MusicPlayer.hpp>
#include <Book/Player.hpp>

#include <nlohmann/json.hpp>
#include <ctime>
#include <fstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

std::tm stringToDate(const std::string& date_str) {
	std::string str = "";
	for (int i = 11; i < date_str.size(); i++)
		if (date_str[i] == '.') break;
		else str += date_str[i];

	std::time_t epoch_time = std::stoll(str);
	std::tm* TM = std::localtime(&epoch_time);
	return *TM;
}


SaveGameState::SaveGameState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	sf::Vector2f windowSize(context.window->getSize());
	showText = true;
	/// GETTIME
	std::time_t now = std::time(nullptr);
	std::stringstream ss;
	ss << "Game_State_" << now;
	tm date = stringToDate(ss.str());
	fileName = "Game at " + toString(1900 + date.tm_year) + " - "
			+ toString(1 + date.tm_mon) + " - "
			+ toString(date.tm_mday) + " - "
			+ toString(date.tm_hour) + " - "
			+ toString(date.tm_min) + " - "
			+ toString(date.tm_sec);
	///
	auto saveButton = std::make_shared<GUI::Button>(context);
	saveButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 50);
	saveButton->setText("Save");
	saveButton->setCallback([this]()
	{
        Player& mPlayer = *getContext().player;
        nlohmann::json JSON;
        JSON["piecesType"] = mPlayer.piecesType;
        JSON["boardType"] = mPlayer.boardType;
        JSON["team"] = mPlayer.team;
        JSON["RealTeam"] = mPlayer.RealTEAM;
        JSON["level"] = mPlayer.level;
        JSON["mode"] = mPlayer.mode;
        JSON["Board"] = mPlayer.fenCode;
        JSON["Turn"] = mPlayer.turn;
        ///
        // std::time_t now = std::time(nullptr);
        // std::stringstream ss;
        // ss << "Game_State_" << now << ".json";
        // std::ofstream fout(ss.str());
		std::ofstream fout(fileName + ".json");
        ///
        fout << JSON.dump(4);
        fout.close();
        requestStackPop();
		requestStackPop();
        requestStackPush(States::Title);
	});

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(0.5f * windowSize.x - 100, 0.4f * windowSize.y + 100);
	backButton->setText("Back to Game");
	backButton->setCallback([this]()
	{
            requestStackPop();
	});

	mGUIContainer.pack(saveButton);
	mGUIContainer.pack(backButton);
	//getContext().music->setPaused(true);
}

SaveGameState::~SaveGameState()
{
	//getContext().music->setPaused(false);
}

void SaveGameState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	if (showText) fileName.push_back('_');
	sf::Text text(fileName, getContext().fonts->get(Fonts::Main), 30);
	text.setFillColor(sf::Color::White);
	text.setPosition(0.5f * window.getSize().x - 100, 0.4f * window.getSize().y);
	if (showText) fileName.pop_back();

	window.draw(backgroundShape);
	window.draw(mGUIContainer);
	window.draw(text);
}

bool SaveGameState::update(sf::Time dt)
{
	mEffectTime += dt;
	if (mEffectTime >= sf::seconds(0.5f))
	{
		mEffectTime = sf::Time::Zero;
		showText = !showText;
	}
	return true;
}

bool SaveGameState::handleEvent(const sf::Event& event)
{
	mGUIContainer.handleEvent(event);
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode == 8 && fileName.size() > 0)
		{
			if (!fileName.empty()) fileName.pop_back();
		}
		else if (event.text.unicode < 128 && event.text.unicode != 13 && event.text.unicode != 8)
		{
			if ((int)fileName.size() < 36) fileName.push_back(static_cast<char>(event.text.unicode));
		}
	}
	return false;
}