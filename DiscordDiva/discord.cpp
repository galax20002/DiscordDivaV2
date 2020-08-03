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
				strcpy_s(activity.assets.large_text, "Hatsune Miku: White One Piece");
				break;

			case 55:	// Natural
				strcpy_s(activity.assets.large_image, "miku-natural");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Natural");
				break;

			case 79:	// Miku Hood
				strcpy_s(activity.assets.large_image, "miku-little-red");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Hood");

				break;

			case 89:	// ∞
				strcpy_s(activity.assets.large_image, "miku-infinity");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: ∞");

				break;

			case 137:	// Type 2020
				strcpy_s(activity.assets.large_image, "miku-type-2020");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Type 2020");

				break;

			case 219:	// Ribbon Girl
				strcpy_s(activity.assets.large_image, "miku-ribbon-girl");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Ribbon Girl");
				break;

			case 268:	// Deep Sea Girl
				strcpy_s(activity.assets.large_image, "miku-deep-sea-girl");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Deep Sea Girl");
				break;

			case 269:	// Honey Whip
				strcpy_s(activity.assets.large_image, "miku-honey-whip");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: Honey Whip");
				break;

			case 299:	// hatsune Miku V3
				strcpy_s(activity.assets.large_image, "miku-v3");
				strcpy_s(activity.assets.large_text, "Hatsune Miku: V3");
				break;

			default:
				strcpy_s(activity.assets.large_image, "miku");
				strcpy_s(activity.assets.large_text, "Hatsune Miku");
				break;
			}
			break;
	

	case 1: // Kagamine Rin

			switch (currentModule)
			{
			case 72:	// Reactor
				strcpy_s(activity.assets.large_image, "rin-reactor");
				strcpy_s(activity.assets.large_text, "Kagamine Rin: Reactor");
				break;

			case 171:	// Kagamine Rin Raspberry Fox
				strcpy_s(activity.assets.large_image, "rin-raspberry-fox");
				strcpy_s(activity.assets.large_text, "Kagamine Rin: Raspberry Fox");
				break;

			case 274:	// Kagamine Rin Future Style
				strcpy_s(activity.assets.large_image, "rin-future");
				strcpy_s(activity.assets.large_text, "Kagamine Rin: Future Style");
				break;

			case 322:	// Faker
				strcpy_s(activity.assets.large_image, "rin-faker");
				strcpy_s(activity.assets.large_text, "Kagamine Rin: Faker");
				break;

			case 325:	// Fairy Dress
				strcpy_s(activity.assets.large_image, "rin-fairy-dress");
				strcpy_s(activity.assets.large_text, "Kagamine Rin: Fairy Dress");
				break;

			default:
				strcpy_s(activity.assets.large_image, "rin");
				strcpy_s(activity.assets.large_text, "Kagamine Rin");
				break;
			}		
			break;
	

	case 2: // Kagamine Len

			switch (currentModule)
			{
			case 73:	// Punkish
				strcpy_s(activity.assets.large_image, "len-punkish");
				strcpy_s(activity.assets.large_text, "Kagamine Len: Punkish");
				break;

			case 148:	// Blue Moon
				strcpy_s(activity.assets.large_image, "len-blue-moon");
				strcpy_s(activity.assets.large_text, "Kagamine Len: Blue Moon");
				break;

			case 150:	// Strangely Dark
				strcpy_s(activity.assets.large_image, "len-strangely-dark");
				strcpy_s(activity.assets.large_text, "Kagamine Len: Strangely Dark");
				break;

			case 275:	// Trickster
				strcpy_s(activity.assets.large_image, "len-trickster");
				strcpy_s(activity.assets.large_text, "Kagamine Len: Trickster");
				break;

			case 330:	// White Edge
				strcpy_s(activity.assets.large_image, "len-white-edge");
				strcpy_s(activity.assets.large_text, "Kagamine Len: White Edge");
				break;

			default:
				strcpy_s(activity.assets.large_image, "len");
				strcpy_s(activity.assets.large_text, "Kagamine Len");
				break;
			}
			break;


	case 3:	// Megurine Luka

			switch (currentModule)
			{
			case 170:	// Megurine Luka Conflict
				strcpy_s(activity.assets.large_image, "luka-conflict");
				strcpy_s(activity.assets.large_text, "Megurine Luka: Conflict");
				break;

			case 249:	// After School Mode
				strcpy_s(activity.assets.large_image, "luka-after-school");
				strcpy_s(activity.assets.large_text, "Megurine Luka: After School Mode");
				break;

			case 332:	// Temptation
				strcpy_s(activity.assets.large_image, "luka-temptation");
				strcpy_s(activity.assets.large_text, "Megurine Luka: Temptation");
				break;

			case 383:	// Megurine Luka V4X
				strcpy_s(activity.assets.large_image, "luka-v4x");
				strcpy_s(activity.assets.large_text, "Megurine Luka: V4X");
				break;

			default:
				strcpy_s(activity.assets.large_image, "luka");
				strcpy_s(activity.assets.large_text, "Megurine Luka");
				break;
			}
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
				strcpy_s(activity.assets.large_text, "KAITO: School Uniform★Parka");
				break;

			case 338:	// Originator
				strcpy_s(activity.assets.large_image, "kaito-originator");
				strcpy_s(activity.assets.large_text, "KAITO: Originator");
				break;

			case 367:	// Diamond Dust
				strcpy_s(activity.assets.large_image, "kaito-diamond-dust");
				strcpy_s(activity.assets.large_text, "KAITO: Diamond Dust");
				break;

			default:
				strcpy_s(activity.assets.large_image, "kaito");
				strcpy_s(activity.assets.large_text, "KAITO");
				break;
			}
			break;


	case 7:	// MEIKO

			switch (currentModule)
			{
			case 163:	// Autumn Leaf
				strcpy_s(activity.assets.large_image, "meiko-momiji");
				strcpy_s(activity.assets.large_text, "MEIKO: Autumn Leaf");
				break;

			case 258:	// Blue Crystal
				strcpy_s(activity.assets.large_image, "meiko-blue-crystal");
				strcpy_s(activity.assets.large_text, "MEIKO: Blue Crystal");
				break;

			case 341:	// Marine Ribbons
				strcpy_s(activity.assets.large_image, "meiko-marine-ribbons");
				strcpy_s(activity.assets.large_text, "MEIKO: Marine Ribbons");
				break;

			default:
				strcpy_s(activity.assets.large_image, "meiko");
				strcpy_s(activity.assets.large_text, "MEIKO");

				break;
			}
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
		
		switch (currentChars)
		{
		case 0:	// Hatsune Miku
			std::cout << "Hatsune Miku";
			break;

		case 1: // Kagamine Rin
			std::cout << "Kagamine Rin";
			break;

		case 2: // Kagamine Len
			std::cout << "Kagamine Len";
			break;

		case 3:	// Megurine Luka
			std::cout << "Megurine Luka";
			break;

		case 4:	// Akita Neru
			std::cout << "Akita Neru";
			break;

		case 5:	// Yowane Haku
			std::cout << "Yowane Haku";
			break;


		case 6:	// KAITO
			std::cout << "KAITO";
			break;

		case 7:	// MEIKO
			std::cout << "MEIKO";
			break;

		case 8:	// Sakine MEIKO
			std::cout << "Sakine MEIKO";
			break;

		case 9:	// Kasane Teto
			std::cout << "Kasane Teto";
			break;

		default:
			std::cout << "Unknown";
			break;
		}

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