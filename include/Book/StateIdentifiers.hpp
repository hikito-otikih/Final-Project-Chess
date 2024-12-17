#ifndef BOOK_STATEIDENTIFIERS_HPP
#define BOOK_STATEIDENTIFIERS_HPP


namespace States
{
	enum ID
	{
		None,
		Title,
		Pause,
		Settings,
		SettingOnlyAudio,
		ChessGame1Player,
		ChessGame2Players,
		PromotionState,
		CheckMateWhite,
		CheckMateBlack,
		StaleMate,
		INSUFFICIENT_MATERIAL,
		THREEFOLD_REPETITION,
		FIFTY_MOVE_RULE,
		AudioSettings,
		ChooseBoard,
		ChoosePieces,
		ChooseMusicTheme,
		ChooseMusicMission,
		volumeSettings,
		MenuPlaysState,
		ChooseTeamState,
		NewGame,
		ChooseLevel,
		Loadgame,
		Savegame,
		AudioSettingsStateOnlyMusicTheme,
	};
}

#endif // BOOK_STATEIDENTIFIERS_HPP
