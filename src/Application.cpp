#include <Book/Application.hpp>
#include <Book/Utility.hpp>
#include <Book/State.hpp>
#include <Book/StateIdentifiers.hpp>
#include <Book/TitleState.hpp>
#include <Book/PauseState.hpp>
#include <Book/SettingsState.hpp>
#include <Book/ChessGame1PlayerState.hpp>
#include <Book/ChessGame2PlayersState.hpp>
#include <Book/PromotionState.hpp>
#include <Book/CheckMateWhiteState.hpp>
#include <Book/CheckMateBlackState.hpp>
#include <Book/StaleMateState.hpp>
#include <Book/FIFTY_MOVE_RULE.hpp>
#include <Book/THREEFOLD_REPETITION.hpp>
#include <Book/INSUFFICIENT_MATERIAL.hpp>
#include <Book/AudioSettingsState.hpp>
#include <Book/ChooseBoard.hpp>
#include <Book/ChoosePieces.hpp>
#include <Book/ChooseMusicTheme.hpp>
#include <Book/ChooseMusicMission.hpp>
#include <Book/volumeSettings.hpp>
#include <Book/MenuPlaysState.hpp>
#include <Book/ChooseTeamState.hpp>
#include <Book/NewGameState.hpp>
#include <Book/ChooseLevel.hpp>
#include <Book/Loadgame.hpp>
#include <Book/SettingOnlyAudio.hpp>
#include <Book/SaveGameState.hpp>
#include <Book/AudioSettingsStateOnlyMusicTheme.hpp>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(1120, 855), "ChessTV", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mMusic()
, mSounds()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer, mMusic, mSounds))
{
	mWindow.setKeyRepeatEnabled(false);
	mWindow.setVerticalSyncEnabled(true);

	mFonts.load(Fonts::Main, 	"Media/Sansation.ttf");

	mTextures.load(Textures::TitleScreen,	"Media/Textures/TitleScreen.png");
	mTextures.load(Textures::Buttons,		"Media/Textures/Buttons.png");
	mTextures.load(Textures::ExtraButtons, "Media/Textures/ExtraButtons.png");
	mTextures.load(Textures::WoodenBackground, "Media/Textures/WoodenBackground.jpg");
	mTextures.load(Textures::FLIPBUTTON, "Media/Textures/FLIPBUTTON.png");
	// Load the textures for the chess pieces (classic)
	mTextures.load(Textures::BLACKBISHOP, "Media/Textures/CLASSIC/BLACKBISHOP.png");
	mTextures.load(Textures::BLACKKING, "Media/Textures/CLASSIC/BLACKKING.png");
	mTextures.load(Textures::BLACKQUEEN, "Media/Textures/CLASSIC/BLACKQUEEN.png");
	mTextures.load(Textures::BLACKKNIGHT, "Media/Textures/CLASSIC/BLACKKNIGHT.png");
	mTextures.load(Textures::BLACKROOK, "Media/Textures/CLASSIC/BLACKROOK.png");
	mTextures.load(Textures::BLACKPAWN, "Media/Textures/CLASSIC/BLACKPAWN.png");
	mTextures.load(Textures::WHITEBISHOP, "Media/Textures/CLASSIC/WHITEBISHOP.png");
	mTextures.load(Textures::WHITEKING, "Media/Textures/CLASSIC/WHITEKING.png");
	mTextures.load(Textures::WHITEQUEEN, "Media/Textures/CLASSIC/WHITEQUEEN.png");
	mTextures.load(Textures::WHITEKNIGHT, "Media/Textures/CLASSIC/WHITEKNIGHT.png");
	mTextures.load(Textures::WHITEROOK, "Media/Textures/CLASSIC/WHITEROOK.png");
	mTextures.load(Textures::WHITEPAWN, "Media/Textures/CLASSIC/WHITEPAWN.png");
	// Load the textures for the chess pieces (modern)
	mTextures.load(Textures::ModernBLACKBISHOP, "Media/Textures/MODERN/BLACKBISHOP.png");
	mTextures.load(Textures::ModernBLACKKING, "Media/Textures/MODERN/BLACKKING.png");
	mTextures.load(Textures::ModernBLACKQUEEN, "Media/Textures/MODERN/BLACKQUEEN.png");
	mTextures.load(Textures::ModernBLACKKNIGHT, "Media/Textures/MODERN/BLACKKNIGHT.png");
	mTextures.load(Textures::ModernBLACKROOK, "Media/Textures/MODERN/BLACKROOK.png");
	mTextures.load(Textures::ModernBLACKPAWN, "Media/Textures/MODERN/BLACKPAWN.png");
	mTextures.load(Textures::ModernWHITEBISHOP, "Media/Textures/MODERN/WHITEBISHOP.png");
	mTextures.load(Textures::ModernWHITEKING, "Media/Textures/MODERN/WHITEKING.png");
	mTextures.load(Textures::ModernWHITEQUEEN, "Media/Textures/MODERN/WHITEQUEEN.png");
	mTextures.load(Textures::ModernWHITEKNIGHT, "Media/Textures/MODERN/WHITEKNIGHT.png");
	mTextures.load(Textures::ModernWHITEROOK, "Media/Textures/MODERN/WHITEROOK.png");
	mTextures.load(Textures::ModernWHITEPAWN, "Media/Textures/MODERN/WHITEPAWN.png");
	// Load the textures for the chess pieces (cartoon)
	mTextures.load(Textures::CartoonBLACKBISHOP, "Media/Textures/CARTOON/BLACKBISHOP.png");
	mTextures.load(Textures::CartoonBLACKKING, "Media/Textures/CARTOON/BLACKKING.png");
	mTextures.load(Textures::CartoonBLACKQUEEN, "Media/Textures/CARTOON/BLACKQUEEN.png");	
	mTextures.load(Textures::CartoonBLACKKNIGHT, "Media/Textures/CARTOON/BLACKKNIGHT.png");
	mTextures.load(Textures::CartoonBLACKROOK, "Media/Textures/CARTOON/BLACKROOK.png");
	mTextures.load(Textures::CartoonBLACKPAWN, "Media/Textures/CARTOON/BLACKPAWN.png");
	mTextures.load(Textures::CartoonWHITEBISHOP, "Media/Textures/CARTOON/WHITEBISHOP.png");
	mTextures.load(Textures::CartoonWHITEKING, "Media/Textures/CARTOON/WHITEKING.png");
	mTextures.load(Textures::CartoonWHITEQUEEN, "Media/Textures/CARTOON/WHITEQUEEN.png");
	mTextures.load(Textures::CartoonWHITEKNIGHT, "Media/Textures/CARTOON/WHITEKNIGHT.png");
	mTextures.load(Textures::CartoonWHITEROOK, "Media/Textures/CARTOON/WHITEROOK.png");
	mTextures.load(Textures::CartoonWHITEPAWN, "Media/Textures/CARTOON/WHITEPAWN.png");

	registerStates();
	mStateStack.pushState(States::Title);

}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<ChessGame1PlayerState>(States::ChessGame1Player);
	mStateStack.registerState<ChessGame2PlayersState>(States::ChessGame2Players);
	mStateStack.registerState<PromotionState>(States::PromotionState);
	mStateStack.registerState<CheckMateWhiteState>(States::CheckMateWhite);
	mStateStack.registerState<CheckMateBlackState>(States::CheckMateBlack);
	mStateStack.registerState<StaleMateState>(States::StaleMate);
	mStateStack.registerState<FIFTYMOVERULE>(States::FIFTY_MOVE_RULE);
	mStateStack.registerState<THREEFOLDREPETITION>(States::THREEFOLD_REPETITION);
	mStateStack.registerState<INSUFFICIENTMATERIAL>(States::INSUFFICIENT_MATERIAL);
	mStateStack.registerState<AudioSettingsState>(States::AudioSettings);
	mStateStack.registerState<ChooseBoard>(States::ChooseBoard);
	mStateStack.registerState<ChoosePieces>(States::ChoosePieces);
	mStateStack.registerState<ChooseMusicTheme>(States::ChooseMusicTheme);
	mStateStack.registerState<ChooseMusicMission>(States::ChooseMusicMission);
	mStateStack.registerState<volumeSettings>(States::volumeSettings);
	mStateStack.registerState<MenuPlaysState>(States::MenuPlaysState);
	mStateStack.registerState<ChooseTeamState>(States::ChooseTeamState);
	mStateStack.registerState<NewGameState>(States::NewGame);
	mStateStack.registerState<ChooseLevel>(States::ChooseLevel);
	mStateStack.registerState<Loadgame>(States::Loadgame);
	mStateStack.registerState<SettingOnlyAudio>(States::SettingOnlyAudio);
	mStateStack.registerState<SaveGameState>(States::Savegame);
	mStateStack.registerState<AudioSettingsStateOnlyMusicTheme>(States::AudioSettingsStateOnlyMusicTheme);
}
