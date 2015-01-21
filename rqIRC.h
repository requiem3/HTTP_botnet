#ifdef useIRC

#ifndef RQIRC_H
#define RQIRC_H
class rqIRC {

private:
//BEGIN IRC config variables
    std::string IRC_SERVER;
    std::string IRC_CHANNEL;
    std::string IRC_CHANNEL_PASSWORD;
    std::string IRC_NICKNAME;
	std::string IRC_REALNAME;
	std::string IRC_CMD_TRIGGER;
	short IRC_PORT;
//END IRC config variables
	char nick[56];
public:
	//default constructor to initialize all the info
	rqIRC(std::string server, std::string channel, std::string pass, std::string nick, std::string realname, std::string cmdtrigger, int port)
	{
		IRC_SERVER = server;
		IRC_CHANNEL = channel;
		IRC_CHANNEL_PASSWORD = pass;
		IRC_NICKNAME = nick;
		IRC_REALNAME = realname;
		IRC_CMD_TRIGGER = cmdtrigger;
		IRC_PORT = port;
	};
//BEGIN get functions
	std::string get_IRC_SERVER()
	{ return IRC_SERVER; }

	std::string get_IRC_CHANNEL()
	{ return IRC_CHANNEL; }

	std::string get_IRC_CHANNEL_PASSWORD()
	{ return IRC_CHANNEL_PASSWORD; }

	std::string get_IRC_NICKNAME()
	{ return IRC_NICKNAME; }

	std::string get_IRC_REALNAME()
	{ return IRC_REALNAME; }

	std::string get_IRC_CMD_TRIGGER()
	{ return IRC_CMD_TRIGGER; }

	int get_IRC_PORT()
	{ return IRC_PORT; }
//END get functions
	int connect_toIRC(int sock)
	{
		struct sockaddr_in sin;        
		struct hostent *_hostent;

		_hostent = gethostbyname(IRC_SERVER.c_str());

		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = *(long *) _hostent->h_addr;
		sin.sin_port = htons(IRC_PORT);

		connect(sock, (struct sockaddr *) &sin, sizeof(sin));

		std::cout << "[CORE] Connected to host: " << IRC_SERVER << std::endl;

		sendLine_toIRC(sock, "USER " + IRC_NICKNAME + " 127.0.0.1 localhost :" + IRC_REALNAME);
	    sendLine_toIRC(sock, "NICK " + IRC_REALNAME);
		sendLine_toIRC(sock, "JOIN #" + IRC_CHANNEL + " :" + IRC_CHANNEL_PASSWORD);
		return 0;
	}

	int sendLine_toIRC(int sock, std::string text, ...) 
	{
		va_list args;
		char    buffer[512] = {0};

		va_start(args, text);
		_vsnprintf(buffer, sizeof(buffer), text.c_str(), args);
		va_end(args);

		strncat(buffer, "\r\n", (sizeof(buffer) - strlen(buffer)));

		printf("IN: %s", buffer);

		return (send(sock, buffer, strlen(buffer), 0));
	}

	int irc_recv_line(int sock, char *line_p, unsigned int line_size)
	{
		char byte = 0;
   
	    while (byte != '\n' && strlen(line_p) < line_size)
		{
			if (!recv(sock, (char *) &byte, 1, 0))
			{ return (0); }

			if (byte != '\r' && byte != '\n' && byte != '\0')
			{       
				strncat(line_p, (char *) &byte, 1);
			}
		}

		printf("OUT: %s\r\n", line_p);
		return (1);
	}
	//Search for characters
	bool search(char *searchee, char *searcher)
	{
		int len = strlen(searchee);
		int len2 = strlen(searcher);

		for(int i = 0; i<len; i++)
		{
			if(searcher[0] == searchee[i])
			{
				bool found = true;
				for (int x= 1; x < len2; x++)
				{
					if(searchee[i+x] != searcher[x])
					{
						found = false;
					}
				}

				if(found==true)
				{
					return true;
				}
			}
		}
		return 0;
	}

};
#endif
#endif