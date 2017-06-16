#include "DiscordApp.hpp"
#include "log.hpp"
#include "easyencryptor.hpp"
#include <psp2/io/stat.h> 

#include <debugnet.h>
#include <psp2/io/fcntl.h>


void DiscordApp::loadUserDataFromFile ( ) {
	std::string enckey = "Toastie";
	logSD ("sceioopen");
	int fh = sceIoOpen ( "ux0:data/vitacord-userdata.txt", SCE_O_RDONLY , 0777 );
	logSD ( "getfilesize" );
	int filesize = sceIoLseek ( fh, 0, SCE_SEEK_END );
	logSD( "filesize is : " + std::to_string ( filesize ) );
	logSD("sceioseekfront");
	sceIoLseek(fh, 0, SCE_SEEK_SET);
	logSD("char* buffer = malloc(filesize)");
	char* buffer = (char*)malloc(filesize);
	logSD("sceioread");
	int readbytes = sceIoRead(fh, buffer, filesize); 
	logSD("readbytes is : " + std::to_string(readbytes));
	logSD("sceioclose");
	sceIoClose( fh );
	sceIoRemove( "ux0:data/vitacord-userdata.txt" );
	
	
	bool oldEnc = false;
	
	if ( filesize < 4 || readbytes < 4 ) {
		logSD( "file too small no settings loaded" );
		
		return;
	}else{
		oldEnc = true;
	}
	logSD( "declare strings" );
	
	std::string parserString = std::string( buffer , readbytes);
	std::string email = ""  , password = "" , token = "";
	bool getmail= true , getpass = false, gettoken = false;
	unsigned int i = 0;
	logSD( "declare strings" );
	
	
	// new enc
	if ( !oldEnc ) {
		int fhMail = sceIoOpen ( "ux0:/data/vitacord/settings/mail.enc" , SCE_O_RDONLY , 0777 );
		//if(
		int fileSize = sceIoLseek ( fhMail, 0, SCE_SEEK_END );
		sceIoLseek ( fhMail, 0, SCE_SEEK_SET );
		if ( fileSize >= 5 ) {
			char * bufferMail = ( char * ) malloc (fileSize);
			int readBytes = sceIoRead ( fhMail , bufferMail , fileSize );
			std::string encMailStr = std::string ( bufferMail , readBytes );
			email = xorDecrypt ( encMailStr );
		}
		sceIoClose ( fhMail );
		
		int fhPass = sceIoOpen ( "ux0:/data/vitacord/settings/pass.enc" , SCE_O_RDONLY , 0777 );
		fileSize = sceIoLseek ( fhPass, 0, SCE_SEEK_END );
		sceIoLseek ( fhPass, 0, SCE_SEEK_SET );
		if ( fileSize >= 1 ) {
			char * bufferPass = ( char * ) malloc ( fileSize );
			int readBytes = sceIoRead ( fhPass , bufferPass , fileSize );
			std::string encPassStr = std::string ( bufferPass , readBytes );
			password = xorDecrypt ( encPassStr );
		}
		sceIoClose ( fhPass );
		
		int fhTok = sceIoOpen ( "ux0:/data/vitacord/settings/to.enc" , SCE_O_RDONLY , 0777 );
		fileSize = sceIoLseek ( fhTok, 0, SCE_SEEK_END );
		sceIoLseek ( fhTok, 0, SCE_SEEK_SET );
		if ( fileSize >= 5 ) {
			char * bufferToken = ( char * ) malloc ( fileSize );
			int readBytes = sceIoRead ( fhTok , bufferToken , fileSize );
			std::string encTokenStr = std::string ( bufferToken , readBytes );
			token = xorDecrypt ( encTokenStr );
		}
		sceIoClose ( fhTok );
		
	} else if ( oldEnc ) {
		// old enc
		for(i = 4 ; i < parserString.length() ; i++){
			
			
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
		
		email = simpleDecrypt(email);
		password = simpleDecrypt(password);
		token = simpleDecrypt(token);
		
		saveUserDataToFile(email , password , token);
		
	}
	
	
	logSD("set mail");
	discord.setEmail(email);
	logSD("set pass");
	discord.setPassword(password);
	logSD("set token");
	discord.setToken(token);
	
	vitaGUI.loginTexts[0] = discord.getEmail();
	vitaGUI.loginTexts[1] = "";
	for( i = 0 ; i < password.length() ; i++){
		vitaGUI.loginTexts[1] += "*";
	}
}

void DiscordApp::saveUserDataToFile(std::string mail , std::string pass , std::string _tok){
	
	
	mail = xorEncrypt(mail);
	pass = xorEncrypt(pass);
	_tok = xorEncrypt(_tok);
	
	sceIoMkdir("ux0:data/vitacord", 0777);
	sceIoMkdir("ux0:data/vitacord/settings", 0777);
	
	//std::string userdata = mail + "\n" + pass + "\n" + _tok + "\n";
	int fh = sceIoOpen("ux0:data/vitacord/settings/mail.enc", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fh, mail.c_str(), strlen(mail.c_str()));
	sceIoClose(fh);
	fh = sceIoOpen("ux0:data/vitacord/settings/pass.enc", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fh, pass.c_str(), strlen(pass.c_str()));
	sceIoClose(fh);
	fh = sceIoOpen("ux0:data/vitacord/settings/to.enc", SCE_O_WRONLY | SCE_O_CREAT, 0777);
	sceIoWrite(fh, _tok.c_str(), strlen(_tok.c_str()));
	sceIoClose(fh);
}


void DiscordApp::Start(){
	
	
	
	logSD("load userdata file");
	loadUserDataFromFile();
	logSD("pass discord pointer to vitaGUI");
	vitaGUI.passDiscordPointer( &discord );
	vitaGUI.passVITAIMEPointer( &vitaIME );
	logSD("start program loop");
	for(;;){
		
		
		
		
		logSD("vitagui draw");
		// DRAW 
		sceKernelDelayThread(5000);
		vitaGUI.Draw();
		sceKernelDelayThread(5000);
		
		
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
	int loginR = discord.login();
	if(loginR  == 200){
		logSD("Login Success");
		vitaGUI.loadingString = "Loading your stuff , " + discord.getUsername();
		saveUserDataToFile(discord.getEmail() , discord.getPassword() , discord.getToken());
		discord.loadData();
		logSD("Loaded data");
		vitaGUI.SetState(1);
	}else if(loginR == 200000){
		int login2faResponse = discord.submit2facode(vitaIME.getUserText(get2facodeTitle));
		if( login2faResponse == 200){
			logSD("Login (2FA) Success");
			vitaGUI.loadingString = "Wait a second " + discord.getUsername();
			saveUserDataToFile(discord.getEmail() , discord.getPassword() , discord.getToken());
			discord.loadData();
			logSD("Loaded data");
			vitaGUI.SetState(1);
			logSD("Set state");
		}else if(login2faResponse == -15){
			vitaGUI.loginTexts[2] = "2FA code too short! ";
		}else if(login2faResponse == -120){
			vitaGUI.loginTexts[2] = "D> JSON parse error! ";
		}else if(login2faResponse == -125){
			vitaGUI.loginTexts[2] = "D> JSON response was empty! ";
		}else if(login2faResponse == -126){
			vitaGUI.loginTexts[2] = "D> 2FA token was empty! ";
		}else{
			vitaGUI.loginTexts[2] = "Error Code " + std::to_string(login2faResponse);;
			std::string errorStr = "Unknown error 2fa = " + std::to_string(login2faResponse);
			criticalLogSD(errorStr.c_str());
		}
	}else if(loginR == -11){
		vitaGUI.loginTexts[2] = "Email too short!";
	}else if(loginR == -12){
		vitaGUI.loginTexts[2] = "Password too short!";
	}else if(loginR == -120){
		vitaGUI.loginTexts[2] = "D> JSON parse error! ";
	}else if(loginR == -125){
		vitaGUI.loginTexts[2] = "D> JSON response was empty! ";
	}else if(loginR == -127){
		vitaGUI.loginTexts[2] = "D> No token or mfa in JSON.";
	}else if(loginR == -129){
		vitaGUI.loginTexts[2] = "D> MFA ticket was empty !";
	}
	else{
		vitaGUI.loginTexts[2] = "Error Code " + std::to_string(loginR);
		std::string errorStr = "Unknown error = " + std::to_string(loginR);
		criticalLogSD(errorStr.c_str());
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
	discord.setPassword(newpassword);
	vitaGUI.loginTexts[1] = "";
	for(unsigned int i = 0 ; i < newpassword.length() ; i++){
		vitaGUI.loginTexts[1] += "*";
	}
	
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


