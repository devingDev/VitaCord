#include "DiscordApp.hpp"
#include "log.hpp"
#include "easyencryptor.hpp"

#include <debugnet.h>
#include <psp2/io/fcntl.h>


void DiscordApp::loadUserDataFromFile(){
	std::string enckey = "Toastie";
	logSD("sceioopen");
	int fh = sceIoOpen("ux0:data/vitacord-userdata.txt", SCE_O_RDONLY , 0777);
	logSD("getfilesize");
	int filesize = sceIoLseek(fh, 0, SCE_SEEK_END);
	logSD("filesize is : " + std::to_string(filesize));
	logSD("sceioseekfront");
	sceIoLseek(fh, 0, SCE_SEEK_SET);
	logSD("char* buffer = malloc(filesize)");
	char* buffer = (char*)malloc(filesize);
	logSD("sceioread");
	int readbytes = sceIoRead(fh, buffer, filesize); 
	logSD("readbytes is : " + std::to_string(readbytes));
	logSD("sceioclose");
	sceIoClose(fh);
	
	if(filesize < 4 || readbytes < 4){
		logSD("file too small no settings loaded");
		return;
	}
	logSD("declare strings");
	
	std::string parserString = std::string( buffer , readbytes);
	std::string email = ""  , password = "" , token = "";
	bool getmail= true , getpass = false, gettoken = false;
	unsigned int i = 0;
	logSD("declare strings");
	for(i = 0 ; i < parserString.length() ; i++){
		
		if(parserString[i] == '\n'){
			if(getmail){
				logSD("newline . switching to getpass");
				getmail = false;
				getpass = true;
			}else if(getpass){
				logSD("newline . switching to token");
				getpass = false;
				gettoken = true;
			}else if(gettoken){
				gettoken = false;
				i = 9999;
				break;
			}
		}else if(parserString[i] == '\r'){
			
		}else if(parserString[i] == '\0'){
			logSD("NIL character");
		}else{
			if(getmail){
				email += parserString[i];
			}else if(getpass){
				password += parserString[i];
			}else if(gettoken){
				token += parserString[i];
			}
		}
		
	}
	
	email = decrypt(email);
	password = decrypt(password);
	token = decrypt(token);
	
	logSD("set mail");
	discord.setEmail(email);
	logSD("set pass");
	discord.setPassword(password);
	logSD("set token");
	discord.setToken(token);
	
	vitaGUI.loginTexts[0] = discord.getEmail();
	vitaGUI.loginTexts[1] = "********";
}

void DiscordApp::saveUserDataToFile(std::string mail , std::string pass , std::string _tok){
	
	
	mail = encrypt(mail);
	pass = encrypt(pass);
	_tok = encrypt(_tok);
	
	std::string userdata = mail + "\n" + pass + "\n" + _tok + "\n";
	int fh = sceIoOpen("ux0:data/vitacord-userdata.txt", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fh, userdata.c_str(), strlen(userdata.c_str()));
	sceIoClose(fh);
}


void DiscordApp::Start(){
	
	criticalLogSD("____App started!_____");
	
	logSD("load userdata file");
	loadUserDataFromFile();
	logSD("pass discord pointer to vitaGUI");
	vitaGUI.passDiscordPointer( &discord );
	logSD("start program loop");
	for(;;){
		
		
		
		
		logSD("vitagui draw");
		vitaGUI.Draw();
		
		logSD("vitapad read");
		vitaPad.Read();
		logSD("vitatouch read");
		vitaTouch.readTouch();
		
		
		vitaGUI.analogScrollRight( vitaPad.right_analog_calibrated_x , -vitaPad.right_analog_calibrated_y );
		vitaGUI.analogScrollLeft( vitaPad.left_analog_calibrated_x , -vitaPad.left_analog_calibrated_y );
		
		if(vitaTouch.clicking){
			logSD("clicking check");
			clicked = vitaGUI.click(vitaTouch.lastClickPoint.x , vitaTouch.lastClickPoint.y );
		}else{
			clicked = -1;
		}
		if(vitaTouch.scrolling){
			logSD("scolling check");
			scrolled = vitaGUI.scroll(vitaTouch.scrollDirX , vitaTouch.scrollDirY , vitaTouch.lastTouchPoint.x, vitaTouch.lastTouchPoint.y);
		}else{
			scrolled = -1;
		}
		vitaState = vitaGUI.GetState();
		if(vitaState == 0){
			switch(clicked){
				case 0:
					getUserEmailInput();
					break;
					
				case 1:
					getUserPasswordInput();
					break;
					
				case 2:
					doLogin();
					break;
			}
		}else if(vitaState == 1){
			if(discord.loadingData){

			}else{
				vitaGUI.SetState(2);
				sceKernelDelayThread(SLEEP_CLICK_NORMAL);
			}
		}else if(vitaState == 2){

			switch(clicked){
				case -1:
					break;
				case CLICKED_DM_ICON:
					vitaGUI.SetState(6);
					break;
				default:
					logSD("join guild");
					discord.JoinGuild(clicked);
					logSD("vitagui setstate(3)");
					vitaGUI.SetState(3);
					sceKernelDelayThread(SLEEP_CLICK_NORMAL);
					break;
				
			}
		}else if(vitaState == 3){
			if(vitaPad.circle){
				vitaGUI.SetState(2);
				sceKernelDelayThread(SLEEP_CLICK_NORMAL);
			}
			switch(clicked){
				case -1:
					break;
				case CLICKED_DM_ICON:
					vitaGUI.SetState(6);
					break;
				default:
					JoinChannel(clicked);
					break;
				
			}
			
		}else if(vitaState == 4){
			if(vitaPad.cross){
				SendChannelMessage();
			}else if(vitaPad.circle){
				discord.LeaveChannel();
				vitaGUI.SetState(2);
			}else{
				
			}
			
			switch(clicked){
				case -1:
					break;
				case CLICKED_DM_ICON:
					discord.LeaveChannel();
					vitaGUI.SetState(6);
					break;
					
				case CLICKED_MESSAGE_INPUT:
					SendChannelMessage();
					break;
				
				default:
					JoinChannel(clicked);
					break;
				
			}
			
			
		}else if(vitaState == 6){
			
			
			if(vitaPad.circle){
				LeaveDMChannel();
			}
			
			switch(clicked){
				case -1:
					break;
				case CLICKED_DM_ICON:
					LeaveDMChannel();
					break;
					
				default:
					JoinDMChannel(clicked);
					break;
				
			}
			discord.refreshDirectMessages();
			
		}else if(vitaState == 7){
			
			
			if(vitaPad.cross){
				SendDirectMessage();
			}else if(vitaPad.circle){
				LeaveDMChannel();
			}
			
			switch(clicked){
				case -1:
					break;
					
				case CLICKED_DM_ICON:
					LeaveDMChannel();
					break; 
					
				case CLICKED_MESSAGE_INPUT:
					SendDirectMessage();
					break;
					
				default:
					JoinDMChannel(clicked);
					break;
				
			}
			discord.refreshCurrentDirectMessages();
			
		}
		
	}
	
}

void DiscordApp::doLogin(){
	
	vitaGUI.showLoginCue();
	debugNetPrintf(DEBUG  , "(Check)Login with email : %s   and password : %s!\n" , discord.getPassword().c_str() , discord.getEmail().c_str());
	int loginR = discord.login();
	if(loginR  == 200){
		logSD("Login Success");
		vitaGUI.loadingString = "Loading your stuff , " + discord.getUsername();
		saveUserDataToFile(discord.getEmail() , discord.getPassword() , discord.getToken());
		discord.loadData();
		logSD("Loaded data");
		vitaGUI.SetState(1);
	}else if(loginR == 200000){
		if( discord.submit2facode(vitaIME.getUserText(get2facodeTitle)) == 200){
			logSD("Login (2FA) Success");
			vitaGUI.loadingString = "Wait a second " + discord.getUsername();
			saveUserDataToFile(discord.getEmail() , discord.getPassword() , discord.getToken());
			discord.loadData();
			logSD("Loaded data");
			vitaGUI.SetState(1);
			logSD("Set state");
		}else{
			vitaGUI.loginTexts[2] = "Wrong 2FA code ?";
		}
	}else{
		vitaGUI.loginTexts[2] = "Wrong password ?";
	}
	
	vitaGUI.unshowLoginCue();
	vitaGUI.setUserInfo();
	
	sceKernelDelayThread(SLEEP_CLICK_EXTENDED);
	
	
}



void DiscordApp::getUserEmailInput(){
	vitaGUI.loginTexts[2] = "";
	
	std::string newemail = vitaIME.getUserText(emailTitle , (char *)discord.getEmail().c_str() );
	discord.setEmail(newemail);
	vitaGUI.loginTexts[0] = newemail;
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
}

void DiscordApp::getUserPasswordInput(){
	vitaGUI.loginTexts[2] = "";
	
	std::string newpassword = vitaIME.getUserText(passwordTitle );
	debugNetPrintf(DEBUG  , "New password is : %s\n" , newpassword.c_str());
	discord.setPassword(newpassword);
	debugNetPrintf(DEBUG  , "(Check)New password is : %s\n" , discord.getPassword().c_str());
	vitaGUI.loginTexts[1] = "********";
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
}


void DiscordApp::SendChannelMessage(){
	
	std::string userMessage = vitaIME.getUserText(messageTitle);
	discord.sendMessage(userMessage);
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
	
}

void DiscordApp::SendDirectMessage(){
	
	std::string userMessage = vitaIME.getUserText(messageTitle);
	discord.sendDirectMessage(userMessage);
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
}

void DiscordApp::JoinDMChannel(int index){
	
	discord.LeaveDirectMessageChannel();	
	discord.JoinDirectMessageChannel(index);
	vitaGUI.SetState(7);
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
}

void DiscordApp::LeaveDMChannel(){
	discord.LeaveDirectMessageChannel();
	vitaGUI.SetState(2);
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
	
}

void DiscordApp::JoinChannel(int index){

	discord.LeaveChannel();
	discord.JoinChannel(index);
	vitaGUI.SetState(4);
	sceKernelDelayThread(SLEEP_CLICK_NORMAL);
}


