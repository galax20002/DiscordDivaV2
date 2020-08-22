#pragma once

#include <winsock2.h>
#include <exception>
#include <iostream>
#include <ws2tcpip.h>
#include <stdio.h>
#include "discord.h"
#include "difficulty.h"
#include <time.h>
#include "difficulty.h"
#include "config.h"
#include "PluginConfigApi.h"

struct SongData {
	int songID;
	char unknown[4];
	char songName[24];
	char isLong;
};
void GetSongName(char* buffer, rsize_t bufferSize);
void GetSongName(SongData song, char* buffer, rsize_t bufferSize);
SongData GetSongData();

signed __int64(__fastcall* divaSongStart)(__int64 a1, __int64 a2) = (__int64(__fastcall*)(__int64 a1, __int64 a2))0x1400DF820;
signed __int64(__fastcall* divaSongEnd)(__int64 a1) = (__int64(__fastcall*)(__int64 a1))0x1400DFE80;
void(__fastcall* divaPaused)() = (void(__fastcall*)())0x1401295C0;
void(__fastcall* divaUnpaused)() = (void(__fastcall*)())0x140129590;
//void(__fastcall* divaPauseThunk)() = (void(__fastcall*)())0x14013C910;