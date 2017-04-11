#include "TKeys.h"

TKeys::TKeys(void){
}

void TKeys::scanKeys(){
	for (KeyMap::iterator it = keysPressed.begin(); it != keysPressed.end(); ++it) {
		ULONG curKey = it->first;
		bool pressed = (GetAsyncKeyState(curKey % 0x100) & 0x8000) != 0; // Single Key

		while (curKey > 0xFF) { // iterate remaining bytes
			curKey = curKey >> 8;
			pressed = pressed && ((GetAsyncKeyState(curKey % 0x100) & 0x8000) != 0);
		}

		if (pressed) { // Key is Pressed
			if (!(it->second)) { //If not Pressed Previously
				it->second = true;
				keysToggle.find(it->first)->second = !(keysToggle.find(it->first)->second);
				keysChanged.find(it->first)->second = true;
			}
			else {
				keysChanged.find(it->first)->second = false;
			}
		}
		else { //Key is not Pressed
			if (it->second) {
				it->second = false;
				keysChanged.find(it->first)->second = true;
			}
			else {
				keysChanged.find(it->first)->second = false;
			}
		}
	}
}

void TKeys::addHotKey(ULONG vkCode){
	KeyMap::iterator it = keysToggle.find(vkCode);
	if (it == keysToggle.end()) {
		keysPressed.insert(std::pair<ULONG, bool>(vkCode, false));
		keysToggle.insert(std::pair<ULONG, bool>(vkCode, false));
		keysChanged.insert(std::pair<ULONG, bool>(vkCode, false));
	}
}

void TKeys::addHotKey(KeyCodes vkCodes){
	ULONG comboVK = 0;
	for (size_t i = 0; i < vkCodes.size(); ++i) {
		comboVK += vkCodes.at(i) << (i * 8);
	}
	addHotKey(comboVK);
}

bool TKeys::getHotKeyToggle(ULONG vkCode){
	KeyMap::iterator it = keysToggle.find(vkCode);
	if (it != keysToggle.end()) {
		return it->second;
	}
	return false;
}

bool TKeys::getHotKeyToggle(KeyCodes vkCodes){
	ULONG comboVK = 0;
	for (size_t i = 0; i < vkCodes.size(); ++i) {
		comboVK += vkCodes.at(i) << (i * 8);
	}
	return getHotKeyToggle(comboVK);
}

bool TKeys::getHotKeyPressed(ULONG vkCode){
	KeyMap::iterator it = keysPressed.find(vkCode);
	if (it != keysPressed.end()) {
		return it->second;
	}
	return false;
}

bool TKeys::getHotKeyPressed(KeyCodes vkCodes) {
	ULONG comboVK = 0;
	for (size_t i = 0; i < vkCodes.size(); ++i) {
		comboVK += vkCodes.at(i) << (i * 8);
	}
	return getHotKeyPressed(comboVK);
}

bool TKeys::getHotKeyChanged(ULONG vkCode) {
	KeyMap::iterator it = keysChanged.find(vkCode);
	if (it != keysChanged.end()) {
		return it->second;
	}
	return false;
}

bool TKeys::getHotKeyChanged(KeyCodes vkCodes) {
	ULONG comboVK = 0;
	for (size_t i = 0; i < vkCodes.size(); ++i) {
		comboVK += vkCodes.at(i) << (i * 8);
	}
	return getHotKeyChanged(comboVK);
}

void TKeys::toggleHotKeyState(ULONG vkCode){
	KeyMap::iterator it = keysToggle.find(vkCode);
	if (it != keysToggle.end()) {
		it->second = !it->second;
	}
}

void TKeys::toggleHotKeyState(KeyCodes vkCodes){
	ULONG comboVK = 0;
	for (size_t i = 0; i < vkCodes.size(); ++i) {
		comboVK += vkCodes.at(i) << (i * 8);
	}
	toggleHotKeyState(comboVK);
}

#pragma region Keys_related

void TKeys::KeyPress(UINT kSC) {
	INPUT kinp;
	kinp.type = INPUT_KEYBOARD;
	kinp.ki.time = 0;
	kinp.ki.wVk = 0;
	kinp.ki.dwExtraInfo = 0;

	kinp.ki.dwFlags = KEYEVENTF_SCANCODE;
	kinp.ki.wScan = kSC;

	SendInput(1, &kinp, sizeof(INPUT));
}

void TKeys::KeyUp(UINT kSC) {
	INPUT kinp;
	kinp.type = INPUT_KEYBOARD;
	kinp.ki.time = 0;
	kinp.ki.wVk = 0;
	kinp.ki.dwExtraInfo = 0;

	kinp.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	kinp.ki.wScan = kSC;

	SendInput(1, &kinp, sizeof(INPUT));
}

void TKeys::KeySequence(std::wstring keys) {
	std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 100));
	for (size_t j = 0; j < keys.size(); ++j) {

		short kVK = VkKeyScan(keys.at(j));
		bool upperCase = HIBYTE(kVK) == 1;
		UINT kSC = MapVirtualKey(LOBYTE(kVK), MAPVK_VK_TO_VSC);

		if (upperCase) { // Press shift
			KeyPress(MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC));
			std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 100));
		}
		KeyPress(kSC);
		KeyUp(kSC);
		if (upperCase) { // Release shift
			KeyUp(MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC));
			std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 100));
		}else{
			std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 50));
		}
	}
}

void TKeys::KeyAutomate(std::wstring accName, std::wstring passwd) {

	std::this_thread::sleep_for(std::chrono::milliseconds(600 + rand() % 150));

	KeySequence(accName);

	std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 150));

	UINT kSC = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);
	KeyPress(kSC);
	KeyUp(kSC);

	std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 150));

	KeySequence(passwd);

	std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 150));

	KeyPress(kSC);
	KeyUp(kSC);

	return;
}

#pragma endregion Keys_Related 