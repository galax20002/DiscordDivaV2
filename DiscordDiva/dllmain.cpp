// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "dllmain.h"
#include <detours.h>
#pragma comment(lib, "detours.lib")

HANDLE gConsole = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()

char* IS_PLAYING_GAME = (char*)0x140d1e480;
char* IS_PV = (char*)0x14cc53b6d;
char* DIFFICULTY = (char*)0x14cc12444;
char* IS_EXTRA = (char*)0x14cc12448;
bool isGamePaused = false;
bool showAlbum2 = GetBooleanValue(L"show_albuns_art");

Difficulty GetDifficulty() {
	if (*IS_EXTRA) {
		return Difficulty::Extra;
	}
	return (Difficulty)(*DIFFICULTY);
}

SongData GetSongData() {
	SongData sd;
	sd.isLong = *(char*)0x140D0A590;
	memcpy(sd.songName, (void*)0x140D0A578, sizeof(sd.songName));
	sd.songID = *(int*)0x140CDD8E0;
	return sd;
}

void GetSongName(SongData song, char* buffer, rsize_t bufferSize) {
	char* defaultSongName;
	if (song.isLong > 0x0f)
	{
		defaultSongName = *(char**)song.songName;
	}
	else
	{
		defaultSongName = song.songName;
	}
	GetOverrideName(song.songID, defaultSongName, buffer, bufferSize);
}
void GetSongName(char* buffer, rsize_t bufferSize) {
	auto song = GetSongData();
	return GetSongName(song, buffer, bufferSize);
}

char lastState = 0x01;
time_t songStartTime, playTime;

void OnGameStateChange() {
	auto song = GetSongData();
	//Filter out the Dummy stage
	char isPlayingGame = song.songID == 999 ? 0 : *IS_PLAYING_GAME;


		char songName[100];
		lastState = isPlayingGame;

		time_t activityTime;
		if (isGamePaused)
		{
			time_t now; time(&now);
			playTime = now - songStartTime;
			activityTime = now;
		}
		else
		{
			time(&songStartTime);
			songStartTime -= playTime;
			activityTime = songStartTime;
		}

		//Getting the song name has turned out to be quite a challenge. If it isn't required, don't do it.
		if (isPlayingGame)
		{
			GetSongName(song, songName, sizeof(songName));
		}
		if (showAlbum2)
		{
			ChangeActivity2(isPlayingGame, songName, *IS_PV, GetDifficulty(), (long long)activityTime, isGamePaused);
		}
		else
		{
			ChangeActivity(isPlayingGame, songName, *IS_PV, GetDifficulty(), (long long)activityTime, isGamePaused);
		}
}

signed __int64 hookedDivaSongStart(__int64 a1, __int64 a2)
{
	isGamePaused = false;
	divaSongStart(a1, a2);
	playTime = 0;

	auto song = GetSongData();
	char isPlayingGame = song.songID == 999 ? 0 : *IS_PLAYING_GAME;
	if (showAlbum2 && isPlayingGame != lastState)
	{
		StopDiscord();
		SetupDiscord(song.songID);
	}
	OnGameStateChange();
	return 0;
}

signed __int64 hookedDivaSongEnd(__int64 a1)
{
	isGamePaused = false;
	divaSongEnd(a1);
	OnGameStateChange();
	return 0;
}

void hookedDivaPaused()
{
	isGamePaused = true;
	divaPaused();
	OnGameStateChange();
}

void hookedDivaUnpaused()
{
	isGamePaused = false;
	divaUnpaused();
	OnGameStateChange();
}

void hookedDivaPauseThunk() {
	// this replaces a thunk from the game so that calls via it won't trigger the paused hook here
	divaPaused();
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// Logs (Loading DiscordDiva)
		SetConsoleTextAttribute(gConsole, 7);
		std::cout << "[";
		SetConsoleTextAttribute(gConsole, 11);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(gConsole, 7);
		std::cout << "] ";
		std::cout << "Loading DiscordDiva..." << std::endl;
		SetConsoleTextAttribute(gConsole, 7);

		// Logs (Injecting Hooks)
		std::cout << "[";
		SetConsoleTextAttribute(gConsole, 11);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(gConsole, 7);
		std::cout << "] ";
		std::cout << "Injecting hooks" << std::endl;
		SetConsoleTextAttribute(gConsole, 7);

		//InstallHook((void*)0x1400dfaf8, (void*)OnGameStateChange, 17);
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaSongStart, (PVOID)hookedDivaSongStart);
		DetourTransactionCommit();

		//InstallHook((void*)0x1400DFEC8, (void*)OnGameStateChange, 14);
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaSongEnd, (PVOID)hookedDivaSongEnd);
		DetourTransactionCommit();

		//InstallHook to pause and unpause
		DetourTransactionBegin();
		DetourAttach(&(PVOID&)divaPaused, (PVOID)hookedDivaPaused);
		DetourAttach(&(PVOID&)divaUnpaused, (PVOID)hookedDivaUnpaused);
		DetourTransactionCommit();

		// Logs (Connecting to Discord)
		std::cout << "[";
		SetConsoleTextAttribute(gConsole, 11);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(gConsole, 7);
		std::cout << "] ";
		std::cout << "Connecting to Discord" << std::endl;
		SetConsoleTextAttribute(gConsole, 7);

		SetupDiscord(0);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		StopDiscord();
		break;
	}
	return TRUE;
}


const wchar_t* CONFIG_FILE1 = getConfigFilePath();
PluginConfig::PluginConfigOption config[] = {
	{ PluginConfig::CONFIG_BOOLEAN, new PluginConfig::PluginConfigBooleanData{ L"show_rival_id", L"general", CONFIG_FILE1, L"Show Rival ID", L"Show your rival ID in your status.", false } },
	{ PluginConfig::CONFIG_BOOLEAN, new PluginConfig::PluginConfigBooleanData{ L"show_albuns_art", L"general", CONFIG_FILE1, L"Show Albuns Art", L"Change from showing Skins to show the Albuns Art in your status.", false } },
	{ PluginConfig::CONFIG_BOOLEAN, new PluginConfig::PluginConfigBooleanData{ L"change_extra", L"general", CONFIG_FILE1, L"Show Extra Extreme", L"Change from Ex Extreme to Extra Extreme in your status.", false } },
};

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void)
{
	return L"DiscordDiva v2";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void)
{
	return L"DiscordDiva v2 plugin by NevesPT\n(forked from original DiscordDiva by bela333)\n\nEnables Discord rich presence for the game.";
}

extern "C" __declspec(dllexport) PluginConfig::PluginConfigArray GetPluginOptions(void)
{
	return PluginConfig::PluginConfigArray{ _countof(config), config };
}