#pragma once

#include <Map>
#include <vector>
#include <thread>
#include <string>
#include <Windows.h>

typedef std::map<ULONG, bool> KeyMap;
typedef std::vector<int> KeyCodes;

class TKeys {
public:
	TKeys(void);

	void scanKeys();

	void addHotKey(ULONG vkCode);
	void addHotKey(KeyCodes vkCodes);
	
	bool getHotKeyToggle(ULONG vkCode);
	bool getHotKeyToggle(KeyCodes vkCodes);

	bool getHotKeyPressed(ULONG vkCode);
	bool getHotKeyPressed(KeyCodes vkCodes);

	bool getHotKeyChanged(ULONG vkCode);
	bool getHotKeyChanged(KeyCodes vkCodes);

	void toggleHotKeyState(ULONG vkCode);
	void toggleHotKeyState(KeyCodes vkCodes);

	void KeyPress(UINT kSC);
	void KeyUp(UINT kSC);
	void KeySequence(std::wstring keys);
	void KeyAutomate(std::wstring accName, std::wstring passwd);
	
protected:
	KeyMap keysPressed;
	KeyMap keysToggle;
	KeyMap keysChanged;
};
