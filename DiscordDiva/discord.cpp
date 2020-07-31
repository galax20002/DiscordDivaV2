#include "stdafx.h"
#include "discord.h"
#include  <locale.h>  		// setlocale()
#include <fcntl.h>

#pragma execution_character_set( "utf-8" )

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()

struct IDiscordCore* core = nullptr;
struct IDiscordActivityManager* activities = nullptr;
std::mutex coreMutex;

void DiscordThread(void*) {
	for (;;)
	{
		coreMutex.lock();
		if (core != nullptr)
		{
			core->run_callbacks(core);
			coreMutex.unlock();
		}
		else
		{
			coreMutex.unlock();
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

}
void SetupDiscord() {
	EDiscordResult result;
	struct DiscordCreateParams params;

	// Logs (Params List)
	std::cout << "[";
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << DISCORD_PREFIX_;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "] ";
	std::cout << "Creating params list" << std::endl;
	SetConsoleTextAttribute(hConsole, 7);

	DiscordCreateParamsSetDefault(&params);
	params.client_id = 737853290611736606;
	params.flags = DiscordCreateFlags_NoRequireDiscord;

	// Logs (Creating Client)
	std::cout << "[";
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << DISCORD_PREFIX_;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "] ";
	std::cout << "Creating client" << std::endl;
	SetConsoleTextAttribute(hConsole, 7);

	result = DiscordCreate(DISCORD_VERSION, &params, &core);
	if (result != DiscordResult_Ok)
	{
		// Logs (Discord Client Error)
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "] ";
		std::cout << "Failed to create Discord client. Reason: ";
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << result << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
		core = nullptr;
		return;
	}
	activities = core->get_activity_manager(core);
	_beginthread(DiscordThread, 20, NULL);
}

void StopDiscord() {
	coreMutex.lock();
	core->destroy(core);
	core = nullptr;
	coreMutex.unlock();
}

void UpdateActivityCallback(void* data, enum EDiscordResult result)
{
	if (result == DiscordResult_Ok)
	{
		// Logs (Updated)
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "] ";
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "Activity updated!" << std::endl << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}
	else
	{
		// Logs (Update Error)
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "] ";
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "Couldn't update activity: ";
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << result << std::endl << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}

}

void ChangeActivity(int isPlaying, char* songName, int isPV, Difficulty difficulty, long long timeSinceStart) {
	
	int idOfSong = *(int*)0x140CDD8E0;
	// info about the 1st singer
	int currentChars = *(int*)0x1411B76D8;
	int currentModule = *(int*)0x1411A8A10;

	// Logs (Updating)
	std::cout << "[";
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << DISCORD_PREFIX_;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "] ";
	SetConsoleTextAttribute(hConsole, 8);
	std::cout << "Updating activity" << std::endl;
	SetConsoleTextAttribute(hConsole, 7);


	struct DiscordActivity activity;
	memset(&activity, 0, sizeof(activity));

	// Game Icon by Module
	switch (currentChars)
	{
	case 0:	// Hatsune Miku

			switch (currentModule)
			{
			case 54:	// White One Piece
				strcpy_s(activity.assets.large_image, "miku-white-gown");
				break;

			case 55:	// Natural
				strcpy_s(activity.assets.large_image, "miku-natural");
				break;

			case 79:	// Miku Hood
				strcpy_s(activity.assets.large_image, "miku-little-red");
				break;

			case 89:	// ∞
				strcpy_s(activity.assets.large_image, "miku-infinity");
				break;

			case 137:	// Type 2020
				strcpy_s(activity.assets.large_image, "miku-type-2020");
				break;

			case 219:	// Ribbon Girl
				strcpy_s(activity.assets.large_image, "miku-ribbon-girl");
				break;

			case 268:	// Deep Sea Girl
				strcpy_s(activity.assets.large_image, "miku-deep-sea-girl");
				break;

			case 269:	// Honey Whip
				strcpy_s(activity.assets.large_image, "miku-honey-whip");
				break;

			case 299:	// hatsune Miku V3
				strcpy_s(activity.assets.large_image, "miku-v3");
				break;

			default:
				strcpy_s(activity.assets.large_image, "miku");
				break;
			}
			strcpy_s(activity.assets.large_text, "Hatsune Miku");
			break;
	

	case 1: // Kagamine Rin

			switch (currentModule)
			{
			case 72:	// Reactor
				strcpy_s(activity.assets.large_image, "rin-reactor");
				break;

			case 171:	// Kagamine Rin Raspberry Fox
				strcpy_s(activity.assets.large_image, "rin-raspberry-fox");
				break;

			case 274:	// Kagamine Rin Future Style
				strcpy_s(activity.assets.large_image, "rin-future");
				break;

			case 322:	// Faker
				strcpy_s(activity.assets.large_image, "rin-faker");
				break;

			case 325:	// Fairy Dress
				strcpy_s(activity.assets.large_image, "rin-fairy-dress");
				break;

			default:
				strcpy_s(activity.assets.large_image, "rin");
				break;
			}		
			strcpy_s(activity.assets.large_text, "Kagamine Rin");
			break;
	

	case 2: // Kagamine Len

			switch (currentModule)
			{
			case 73:	// Punkish
				strcpy_s(activity.assets.large_image, "len-punkish");
				break;

			case 148:	// Blue Moon
				strcpy_s(activity.assets.large_image, "len-blue-moon");
				break;

			case 150:	// Strangely Dark
				strcpy_s(activity.assets.large_image, "len-strangely-dark");
				break;

			case 275:	// Trickster
				strcpy_s(activity.assets.large_image, "len-trickster");
				break;

			case 330:	// White Edge
				strcpy_s(activity.assets.large_image, "len-white-edge");
				break;

			default:
				strcpy_s(activity.assets.large_image, "len");
				break;
			}
			strcpy_s(activity.assets.large_text, "Kagamine Len");
			break;


	case 3:	// Megurine Luka

			switch (currentModule)
			{
			case 170:	// Megurine Luka Conflict
				strcpy_s(activity.assets.large_image, "luka-conflict");
				break;

			case 249:	// After School Mode
				strcpy_s(activity.assets.large_image, "luka-after-school");
				break;

			case 332:	// Temptation
				strcpy_s(activity.assets.large_image, "luka-temptation");
				break;

			case 383:	// Megurine Luka V4X
				strcpy_s(activity.assets.large_image, "luka-v4x");
				break;

			default:
				strcpy_s(activity.assets.large_image, "luka");
				break;
			}
			strcpy_s(activity.assets.large_text, "Megurine Luka");
			break;


	case 4:	// Akita Neru

			strcpy_s(activity.assets.large_image, "neru");
			strcpy_s(activity.assets.large_text, "Akita Neru");
			break;


	case 5:	// Yowane Haku

			strcpy_s(activity.assets.large_image, "haku");
			strcpy_s(activity.assets.large_text, "Yowane Haku");
			break;


	case 6:	// KAITO
		
			switch (currentModule)
			{
			case 256:	// School Uniform★Parka
				strcpy_s(activity.assets.large_image, "kaito-school-uniform-parka");
				break;

			case 338:	// Originator
				strcpy_s(activity.assets.large_image, "kaito-originator");
				break;

			case 367:	// Diamond Dust
				strcpy_s(activity.assets.large_image, "kaito-diamond-dust");
				break;

			default:
				strcpy_s(activity.assets.large_image, "kaito");
				break;
			}
			strcpy_s(activity.assets.large_text, "KAITO");
			break;


	case 7:	// MEIKO

			switch (currentModule)
			{
			case 163:	// Autumn Leaf
				strcpy_s(activity.assets.large_image, "meiko-momiji");
				break;

			case 258:	// Blue Crystal
				strcpy_s(activity.assets.large_image, "meiko-blue-crystal");
				break;

			case 341:	// Marine Ribbons
				strcpy_s(activity.assets.large_image, "meiko-marine-ribbons");
				break;

			default:
				strcpy_s(activity.assets.large_image, "meiko");
				break;
			}
			strcpy_s(activity.assets.large_text, "MEIKO");
			break;


	case 8:	// Sakine MEIKO

			strcpy_s(activity.assets.large_image, "sakine");
			strcpy_s(activity.assets.large_text, "Sakine MEIKO");
			break;


	case 9:	// Kasane Teto

			strcpy_s(activity.assets.large_image, "teto");
			strcpy_s(activity.assets.large_text, "Kasane Teto");
			break;


	default:

			strcpy_s(activity.assets.large_image, "icon0");
			strcpy_s(activity.assets.large_text, "");
			break;
	}




	/*strcpy_s(activity.assets.large_image, "icon");*/
	/*strcpy_s(activity.assets.large_text, "Hatsune Miku: Project DIVA Arcade Future Tone");*/
	if (isPlaying)
	{
		// song name
		strcpy_s(activity.details, songName);

		// Logs (name song)
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, 11);
		std::cout << DISCORD_PREFIX_;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << "] ";
		std::cout << "ID: ";
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << idOfSong;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << " - Chars: ";
		SetConsoleTextAttribute(hConsole, 8);
		std::cout << currentChars;
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << " - Song: ";
		SetConsoleTextAttribute(hConsole, 8);
		SetConsoleOutputCP(65001);
		printf("%s", songName);
		/*std::wcout << songName;*/
		SetConsoleTextAttribute(hConsole, 7);
		std::cout << " on ";

		if (isPV) {
			strcpy_s(activity.state, "Watching a PV");
			strcpy_s(activity.assets.small_text, "PV Mode");
			strcpy_s(activity.assets.small_image, "pv");
		}
		else {
			// Difficulty
			sprintf_s(activity.state, "Playing at %s", DifficultyToString(difficulty));
			sprintf_s(activity.assets.small_text, "%s", DifficultyToString(difficulty));

			// if to show the color circule of the Difficulty
			if (DifficultyToString(difficulty) == "Easy")
			{
				strcpy_s(activity.assets.small_image, "d-easy");
				
				SetConsoleTextAttribute(hConsole, 11);
				std::cout << "Easy";
				SetConsoleTextAttribute(hConsole, 7);

			}
			else if (DifficultyToString(difficulty) == "Normal")
			{
				strcpy_s(activity.assets.small_image, "d-normal");

				SetConsoleTextAttribute(hConsole, 10);
				std::cout << "Normal";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (DifficultyToString(difficulty) == "Hard")
			{
				strcpy_s(activity.assets.small_image, "d-hard");

				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "Hard";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (DifficultyToString(difficulty) == "Extreme")
			{
				strcpy_s(activity.assets.small_image, "d-extreme");

				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Extreme";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else if (DifficultyToString(difficulty) == "EX Extreme")
			{
				strcpy_s(activity.assets.small_image, "d-ex-extreme");

				SetConsoleTextAttribute(hConsole, 13);
				std::cout << "EX Extreme";
				SetConsoleTextAttribute(hConsole, 7);
			}
			else {
				strcpy_s(activity.assets.small_image, "None");
			}

			std::cout << std::endl;
			SetConsoleTextAttribute(hConsole, 7);

			activity.timestamps.start = timeSinceStart;
		}
	}
	else
	{
		strcpy_s(activity.assets.large_image, "icon0");
		strcpy_s(activity.assets.large_text, "");
		strcpy_s(activity.state, "Waiting for a song!");
		strcpy_s(activity.assets.small_image, "None");

	}
	coreMutex.lock();
	if (activities == nullptr || core == nullptr)
	{
		coreMutex.unlock();
		return;
	}
	activities->update_activity(activities, &activity, NULL, UpdateActivityCallback);
	coreMutex.unlock();
}