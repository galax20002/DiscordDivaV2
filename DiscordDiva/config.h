
#pragma once
#include <string>
#include <codecvt> 

const wchar_t* getConfigFilePath();
void GetOverrideName(int songID, char* defaultName, char* buffer, rsize_t bufferSize);
bool GetBooleanValue(wchar_t* setting);