// TKeysDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\TKeys\TKeys.h"

int main(){
	TKeys tK;

	std::vector<ULONG> hotKeys = {
		VK_UP, 
		VK_DOWN, 
		VK_LEFT, 
		VK_RIGHT,
		VK_LMENU | 'K' << 8,
		VK_LMENU | 'I' << 8,
		VK_LMENU | 'X' << 8, 
	};

	std::vector<std::wstring> hotKeyNames = {
		L"Up",
		L"Down",
		L"Left",
		L"Right",
		L"Alt+K",
		L"Alt+I",
		L"Alt+X"
	};

	for (size_t i = 0; i < hotKeys.size(); ++i) {
		tK.addHotKey(hotKeys.at(i));
	}

	//// Alternatively, manually add each one of them
	//tK.addHotKey(VK_UP);
	//tK.addHotKey(VK_DOWN);
	//tK.addHotKey(VK_LEFT);
	//tK.addHotKey(VK_RIGHT);
	//tK.addHotKey({ VK_LMENU,'Z' });
	//tK.addHotKey({ VK_LMENU,'I' });

	while (true) {
		tK.scanKeys(); // Read hotkey 

		for (size_t i = 0; i < hotKeys.size(); ++i) { // iterate through the hotKeys
			if (tK.getHotKeyChanged(hotKeys.at(i))) { // if key state changed
				std::wcout << hotKeyNames.at(i).c_str() << L": [" 
					<< L"State -> "
					<< (tK.getHotKeyPressed(hotKeys.at(i)) ? L"Pressed" : L"Released")
					<< L", Toggle -> "
					<< (tK.getHotKeyToggle(hotKeys.at(i)) ? L"On" : L"Off")
					<< L"]" << std::endl;
			}
		}
		////Alternatively, manually process each of them
		//if (tK.getHotKeyChanged(VK_UP)) {
		//	std::wcout << L"UP:" << (tK.getHotKeyPressed(VK_UP) ? L"Pressed" : L"Released") << std::endl;
		//}
		//if (tK.getHotKeyChanged(VK_DOWN)) {
		//	std::wcout << L"DOWN:" << (tK.getHotKeyPressed(VK_DOWN) ? L"Pressed" : L"Released") << std::endl;
		//}
		//if (tK.getHotKeyChanged(VK_LEFT)) {
		//	std::wcout << L"LEFT:" << (tK.getHotKeyPressed(VK_LEFT) ? L"Pressed" : L"Released") << std::endl;
		//}
		//if (tK.getHotKeyChanged(VK_RIGHT)) {
		//	std::wcout << L"RIGHT:" << (tK.getHotKeyPressed(VK_RIGHT) ? L"Pressed" : L"Released") << std::endl;
		//}

		if (tK.getHotKeyPressed({ VK_LMENU,'K' }) && tK.getHotKeyChanged({ VK_LMENU,'K' })) {
			// Enter key sequences automatically in a separate thread
			std::thread t1(&TKeys::KeySequence, tK,
				L"Some string that will be entered automatically");
			t1.detach();
		}

		if (tK.getHotKeyPressed({ VK_LMENU,'I' }) && tK.getHotKeyChanged({ VK_LMENU,'I' })) {
			// Enter account & password automatically in a separate thread
			std::thread t1(&TKeys::KeyAutomate, tK,
				L"testACC123@Lol.com.jk",
				L"SomePassword?-+=\\|<>,./;:'[]{}\"!@#$%^&*()");
			t1.detach();
		}

		if (tK.getHotKeyToggle({ VK_LMENU,'X' })) { // Toggle initially false, toggles after press
			std::wcout << L"Alt+Z Pressed, Exiting" << std::endl;
			break;
		}

		Sleep(33);
	}
    return 0;
}

