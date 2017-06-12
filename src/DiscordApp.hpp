#ifndef DISCORDAPP_HPP
#define DISCORDAPP_HPP


#include <psp2/kernel/processmgr.h>

#include "VitaTouch.hpp"
#include "VitaPad.hpp"
#include "VitaIME.hpp"
#include "Discord.hpp"
#include "VitaGUI.hpp"


class DiscordApp{
	
	public:
		void Start();
		void loadUserDataFromFile();
		void saveUserDataToFile(std::string m , std::string p , std::string t);
		
	private:
		char emptyMessage[1] = "";
		char emailTitle[14] = "Discord Email";
		char passwordTitle[17] = "Discord Password";
		char messageTitle[8] = "Message";
		char get2facodeTitle[30] = "Enter your 2Factor Auth Code!";
		VitaGUI vitaGUI;
		Discord discord;
		VitaIME vitaIME;
		VitaPad vitaPad;
		VitaTouch vitaTouch;
		int clicked = -1;
		int scrolled = -1;
		int vitaState = 0;
		
		void doLogin();
		void getUserEmailInput();
		void getUserPasswordInput();
		
		
		
		void SendChannelMessage();
		void SendDirectMessage();
		void JoinDMChannel(int index);
		void LeaveDMChannel();
		void JoinChannel(int index);
	
};



#endif




