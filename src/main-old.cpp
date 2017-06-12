#define VITASDK

#include <vita2d.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/display.h>

#include <psp2/io/fcntl.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <pthread.h>


#include "VitaIME.hpp"
#include "VitaPad.hpp"
#include "net_functions.hpp"
#include "VitaTouch.hpp"

#include "json.hpp"

#define ip_server "192.168.1.85"
#define port_server 18194


#define TOUCH_SCREEN_WIDTH 1920
#define TOUCH_SCREEN_HEIGHT 1088

vita2d_texture *backgroundImage;


int currentGuild = 0;
int currentChannel = 0;
	int cursorGuild = 0;
	int cursorChannel = 0;
	long sleepTimeInputNormal = 1*125*1000; // 125 ms
	long sleepTimeInputScroll = 1*25*100; // 2.5 ms or 2500 microseconds

std::string useremail = "" , userpassword = "";

bool loggedIn = false;
bool inChannel = false , inGuild = false , inMain = true , inApp = true;

//std::string data; //will hold the url's contents



int scrollX = 0 , scrollY = 0;

uint64_t lastFetchTimeMS , currentTimeMS , fetchDelayMS = 2000;

uint64_t osGetTimeMS(void){
	return (sceKernelGetProcessTimeWide() / 1000);
}

void logSD(std::string logData){
	
	const char * data = logData.c_str();
	
	int f = sceIoOpen("ux0:data/vitacord-log.txt", SCE_O_WRONLY | SCE_O_APPEND | SCE_O_CREAT, 0777);
    
    sceIoWrite(f, data, strlen(data));
    
    sceIoClose(f);
	
}

vita2d_pgf *pgf;

VitaPad vitaPad;
VitaIME vitaIME;






void processJsonWebhook(){
	nlohmann::json j_complete = nlohmann::json::parse(data);
	
	char url[1024];
	
	const char urlS[] = "https://discordapp.com/api/webhooks/%s/%s" ;
	
	
	
	snprintf(url , 1024 , urlS , j_complete["id"].get<std::string>().c_str() , j_complete["token"].get<std::string>().c_str());

	
	std::string postdata = "{ \"username\":\"coderx3\" , \"content\":\"hi\" , \"avatar_url\":\"https://cdn.discordapp.com/avatars/206127888309682176/9ebc942fd23843bc3f7c1ee372fe84b6.png\" }";
	
	CURL *curl;
	CURLcode res;
	/* In windows, this will init the winsock stuff */ 
	//curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl) {
		/* First set the URL that is about to receive our POST. This URL can
		   just as well be a https:// URL if that is what should receive the
		   data. */ 
		   
		
		curl_easy_setopt(curl, CURLOPT_URL, url);
		
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
		
		
		/* we pass our 'chunk' struct to the callback function */ 
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
		
		
		   
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		   
		   
		
		/* Now specify the POST data */ 
		
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, userAgentHeader);
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		
		std::string ContentLengthS = "Content-Length: " + std::to_string(strlen(postdata.c_str()));
		headerchunk = curl_slist_append(headerchunk, ContentLengthS.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata.c_str()));
		
		curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent);
		

		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			/* always cleanup */ 
			
		}else{
			
		}
		
		
		curl_easy_cleanup(curl);
	}			
	
}


void loginDiscord(){
	
	postdata = "{ \"email\":\"" + useremail + "\" , \"password\":\"" + userpassword + "\" }";
	
	long httpcode =  curlDiscordPost("https://discordapp.com/api/auth/login" , postdata) ;
	
	if(httpcode == 200){
		//logged in!
		loggedIn = true;
		
		logSD("Success loginDiscord 1 : \n");
		logSD(data);
		logSD("\n");
		
		try{
			nlohmann::json j_complete = nlohmann::json::parse(data);
			
			if(!j_complete.is_null()){
			
				if(!j_complete["mfa"].is_null()){
					if(!j_complete["ticket"].is_null()){
						
						std::string ticket = j_complete["ticket"].get<std::string>();
						
						std::string code2fa = "";
						code2fa = vitaIME.getUserText("Auth 2FA Code");
						
						postdata = "{ \"code\":\"" + code2fa + "\" , \"ticket\":\"" + ticket + "\" }";
						httpcode = curlDiscordPost( "https://discordapp.com/api/v6/auth/mfa/totp" , postdata );
						
						if(httpcode == 200){
							
							logSD("Success loginDiscord 2 : \n");
							logSD(data);
							logSD("\n");
							
							nlohmann::json j_complete2 = nlohmann::json::parse(data);
							if(!j_complete2.is_null()){
								if(!j_complete2["token"].is_null()){
									userToken = j_complete2["token"].get<std::string>();
									
									authorizationHeader = "Authorization: " + userToken;
									
								}
							}
							
						}
						
					}
					
				}else if(!j_complete["token"].is_null()){
					
					userToken = j_complete["token"].get<std::string>();
			
					authorizationHeader = "Authorization: " + userToken;
				}
				
			}
		}catch(const std::exception& e){
			
		}

		
	}else if(httpcode == 400){
		// wrong login info!
		loggedIn = false;
	}
	
}

void getChannels(){
	
	for(int i = 0; i < guildsAmount ; i++){
		
		long httpcode =  curlDiscordGet("https://discordapp.com/api/guilds/" + guilds[i].id + "/channels" ) ;
		if(httpcode == 200){
			try{
				nlohmann::json j_complete = nlohmann::json::parse(data);
				if(!j_complete.is_null()){
					int channelsAmount = j_complete.size();
					for(int c = 0; c < channelsAmount; c++){
						
						guilds[i].channels.push_back(channel());
						
						if(!j_complete[c].is_null()){
							
							if(!j_complete[c]["type"].is_null()){
								guilds[i].channels[c].type = j_complete[c]["type"].get<std::string>();
							}else{
								guilds[i].channels[c].type = "";
							}
							
							if(!j_complete[c]["id"].is_null()){
								guilds[i].channels[c].id = j_complete[c]["id"].get<std::string>();
							}else{
								guilds[i].channels[c].id = "";
							}
							
							if(!j_complete[c]["name"].is_null()){
								guilds[i].channels[c].name = j_complete[c]["name"].get<std::string>();
							}else{
								guilds[i].channels[c].name = "name unavailable";
							}
							
							if(!j_complete[c]["topic"].is_null()){
								guilds[i].channels[c].topic = j_complete[c]["topic"].get<std::string>();
							}else{
								guilds[i].channels[c].topic = "";
							}
						
							if(!j_complete[c]["is_private"].is_null()){
								guilds[i].channels[c].is_private = j_complete[c]["is_private"].get<bool>();
							}else{
								guilds[i].channels[c].is_private = false;
							}
							
							if(!j_complete[c]["last_message_id"].is_null()){
								guilds[i].channels[c].last_message_id = j_complete[c]["last_message_id"].get<std::string>();
							}else{
								guilds[i].channels[c].last_message_id = false;
							}
							
							
							
						}
						
					}
				
				}
			}catch(const std::exception& e){
		
			}
		}
		
	}
	
}

void getGuilds(){
	
	long httpcode =  curlDiscordGet("https://discordapp.com/api/users/@me/guilds" ) ;
	
	if(httpcode == 200){
		try{
			nlohmann::json j_complete = nlohmann::json::parse(data);
			if(!j_complete.is_null()){
				guildsAmount = j_complete.size();
				
				
				for(int i = 0; i < guildsAmount; i++){
					
					guilds.push_back(guild());
					
					if(!j_complete[i].is_null()){
						
						
						if(!j_complete[i]["owner"].is_null()){
							guilds[i].owner = j_complete[i]["owner"].get<bool>();
						}else{
							guilds[i].owner = false;
						}
						
						if(!j_complete[i]["permissions"].is_null()){
							guilds[i].permissions = j_complete[i]["permissions"].get<long>();
						}else{
							guilds[i].permissions = 0;
						}
						
						if(!j_complete[i]["icon"].is_null()){
							guilds[i].icon = j_complete[i]["icon"].get<std::string>();
						}else{
							guilds[i].icon = "";
						}
						
						if(!j_complete[i]["id"].is_null()){
							guilds[i].id = j_complete[i]["id"].get<std::string>();
						}else{
							guilds[i].id = "";
						}
						
						if(!j_complete[i]["name"].is_null()){
							guilds[i].name = j_complete[i]["name"].get<std::string>();
							
						}else{
							guilds[i].name = "";
						}
						
						
						
					}
					
					
				}
			}
		}catch(const std::exception& e){
			
		}
		
	}
	
}

void sendMessage(){
	
	postdata = "{ \"content\":\"" + userMessage + "\" }";
	long httpcode = curlDiscordPost("https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages" , postdata );
	
	if(httpcode == 200){
		
		
	}
	
}

void getMessagesFromChannel(){
	
	
	long httpcode =  curlDiscordGet("https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages" ) ;
	
	if(httpcode == 200){
		nlohmann::json j_complete = nlohmann::json::parse(data);
		int messagesAmount = j_complete.size();
		
		
		if(!j_complete.is_null()){
			guilds[currentGuild].channels[currentChannel].messages.clear();
			
			for( int i = 0 ; i < messagesAmount ; i++){
				guilds[currentGuild].channels[currentChannel].messages.push_back(message());
				
				if(!j_complete[i].is_null()){
					
					if(!j_complete[i]["timestamp"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].timestamp = j_complete[i]["timestamp"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].timestamp = "";
					}
					
					if(!j_complete[i]["id"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].id = "";
					}
					
					if(!j_complete[i]["content"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].content = j_complete[i]["content"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].content = "";
					}
					// author :
					if(!j_complete[i]["author"]["username"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.username = j_complete[i]["author"]["username"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.username = "";
					}
					
					if(!j_complete[i]["author"]["discriminator"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.discriminator = j_complete[i]["author"]["discriminator"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.discriminator = "";
					}
					
					if(!j_complete[i]["author"]["id"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.id = j_complete[i]["author"]["id"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.id = "";
					}
					
					if(!j_complete[i]["author"]["avatar"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.avatar = j_complete[i]["author"]["avatar"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.avatar = "";
					}

				}
			}
		}
	
		lastFetchTimeMS = osGetTimeMS();
	
	}
}


void printChannelsInGuild(){
	
	vita2d_pgf_draw_text(pgf, 16, 16, RGBA8(0,255,0,255), 0.75f, guilds[currentGuild].name.c_str());
	std::string printThis ="";
	for(int i = 0 ; i < guilds[currentGuild].channels.size() ; i++){
		printThis = "";
		if(cursorChannel == i)
			printThis += " > ";
		printThis += guilds[currentGuild].channels[i].name;
		if(guilds[currentGuild].channels[i].type != "text"){
			printThis += " (VOICE)";
		}
		vita2d_pgf_draw_text(pgf, scrollX + 16, scrollY + (32 + 16 * i), RGBA8(0,255,0,255), 0.75f, printThis.c_str());
	}
	
}

void printGuilds(){
	
	vita2d_pgf_draw_text(pgf, 16, 16, RGBA8(0,255,0,255), 1.0f, "Guilds : ");
	std::string printThis ="";
	for(int i = 0 ; i < guilds.size() ; i++){
		printThis = "";
		if(cursorGuild == i)
			printThis += " > ";
		printThis += guilds[i].name;
		vita2d_pgf_draw_text(pgf, scrollX + 16, scrollY + (32 + 16 * i), RGBA8(0,255,0,255), 1.0f, printThis.c_str());
		
	}
}

void printMessages(){
	
	std::string printThis ="";
	vita2d_pgf_draw_text(pgf, 8, 16, RGBA8(0,255,0,255), 1.0f, guilds[currentGuild].channels[currentChannel].name.c_str());
	for(int i =  0 ; i < guilds[currentGuild].channels[currentChannel].messages.size() ; i++){
		
		printThis = "";
		printThis += guilds[currentGuild].channels[currentChannel].messages[i].author.username;
		printThis += " : ";
		printThis += guilds[currentGuild].channels[currentChannel].messages[i].content;
		vita2d_pgf_draw_text(pgf, scrollX + 32, scrollY + (32 + 16 * i), RGBA8(0,255,0,255), 1.0f, printThis.c_str());
	}
	
	
}

void joinFirstTextChannel(){
	
	for(int i = 0 ; i < guildsAmount ;i++){
		
		for(int j = 0 ; j < guilds[i].channels.size() ; j++){
			
			if(guilds[i].channels[j].type == "text"){
				if(guilds[i].channels[j].is_private == false){
				
					currentGuild = i;
					currentChannel = j;
					
					
					i = 999999;
					j = 999999;
					break;
					
				}
			}
			
			
		}
		
	}
	
}


void initializeMain(){
	
	initializeNetworking();
    vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	pgf = vita2d_load_default_pgf();
	std::string bgPath = "app0:assets/images/Vitacord-Background-8BIT.png";
	backgroundImage = vita2d_load_PNG_file(bgPath.c_str());
	
}

void userLoginAndGetData(){
	logSD("Starting login loop\n");
	while(!loggedIn){
		
		logSD("Started App\n");
		logSD("Ask for useremail\n");
		useremail = vitaIME.getUserText("Discord Email");
		logSD("Ask for userpassword\n");
		userpassword = vitaIME.getUserText("Discord Password");
		logSD("Try login\n");
		loginDiscord();
		logSD(data);
		logSD("\n");
	}
	logSD("Ended login loop\n");	
	logSD("Get guilds\n");
	getGuilds();
	logSD(data);
	logSD("\n");
	logSD("Get channels\n");
	getChannels();
	logSD(data);
	logSD("\n");
	logSD("joinFirstTextChannel()\n");
	joinFirstTextChannel();
	
}


extern "C"
{
    unsigned int sleep(unsigned int seconds)
    {
        sceKernelDelayThread(seconds*1000*1000);
        return 0;
    }

    int usleep(useconds_t usec)
    {
        sceKernelDelayThread(usec);
        return 0;
    }

    void __sinit(struct _reent *);
}

__attribute__((constructor(101)))
void pthread_setup(void) 
{
    pthread_init();
    __sinit(_REENT);
}
int main(int argc, char *argv[]) {
	
	logSD("Started App\n");
	
	initializeMain();
	
	
	logSD("vita2d startdrawing\n");
	vita2d_start_drawing();
	logSD("vita2d clear\n");
	vita2d_clear_screen();
	logSD("vita2d drawtext\n");
	vita2d_pgf_draw_text(pgf, 0, 16, RGBA8(0,255,0,255), 5.0f, "Discord");
	logSD("vita2d enddrawing\n");
	vita2d_end_drawing();
	logSD("vita2d swapbuffers\n");
	vita2d_swap_buffers();

	userLoginAndGetData();
	
	logSD("Starting app loop----------\n");
	while(inApp){
		
		
		sceKernelDelayThread(1*10*1000);

		
		
		vitaPad.Read();
		
		currentTimeMS = osGetTimeMS();
		if(inChannel){
			
			
			vita2d_start_drawing();
			vita2d_clear_screen();
			vita2d_draw_texture(backgroundImage , 0 , 0);
			printMessages();
			vita2d_end_drawing();
			vita2d_swap_buffers();
			

			
			if(vitaPad.circle){
				
				inGuild = true;
				inChannel = false;
				inMain = false;
				scrollX = scrollY = 0;
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.cross){
				
				userMessage = vitaIME.getUserText("Message : ");
				sendMessage();
				
				
				scrollX = scrollY = 0;
				
				sceKernelDelayThread(sleepTimeInputNormal);
				
			}else if(vitaPad.righttrigger){
				
				
				scrollY += 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}else if(vitaPad.lefttrigger){
				
				
				scrollY -= 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}
			
			if(currentTimeMS - lastFetchTimeMS > fetchDelayMS ){
				
				
				getMessagesFromChannel();
				
			}
			
		}else if(inGuild){
			
			vita2d_start_drawing();
			vita2d_clear_screen();
			vita2d_draw_texture(backgroundImage , 0 , 0);
			printChannelsInGuild();
			vita2d_end_drawing();
			vita2d_swap_buffers();
			
			if(vitaPad.circle){
				
				inChannel = false;
				inGuild = false;
				inMain = true;
				
				
				scrollX = scrollY = 0;
				
				cursorChannel = 0;
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.cross){
				
				
				if(guilds[currentGuild].channels[cursorChannel].type == "text"){
					
					inChannel = true;
					inGuild = false;
					inMain = false;
					
					
					scrollX = scrollY = 0;
					
					currentChannel = cursorChannel;
					currentGuild = cursorGuild;
					
				}
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.up){
				
				
				cursorChannel--;
				if(cursorChannel < 0){
					cursorChannel = guilds[currentGuild].channels.size() - 1;
				}
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.down){
				
				
				cursorChannel++;
				if(cursorChannel >= guilds[currentGuild].channels.size()){
					cursorChannel = 0;
				}
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.righttrigger){
				
				
				scrollY += 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}else if(vitaPad.lefttrigger){
				
				
				scrollY -= 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}
			
			
		}else if(inMain){
			// show guilds ( servers )
			
			vita2d_start_drawing();
			vita2d_clear_screen();
			vita2d_draw_texture(backgroundImage , 0 , 0);
			printGuilds();
			vita2d_end_drawing();
			vita2d_swap_buffers();
			if(vitaPad.circle){
				
				
				// dont quit ^^
				//inChannel = false;
				//inGuild = false;
				//inMain = false;
				//inApp = false;
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.cross){
				
				
				inChannel = false;
				inGuild = true;
				inMain = false;
				
				
				
				scrollX = scrollY = 0;
				
				currentChannel = cursorChannel;
				currentGuild = cursorGuild;
				
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.up){
				
				
				cursorGuild--;
				if(cursorGuild < 0){
					cursorGuild = guildsAmount - 1;
				}
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.down){
				
				
				cursorGuild++;
				if(cursorGuild >= guildsAmount){
					cursorGuild = 0;
				}
				
				sceKernelDelayThread(sleepTimeInputNormal); // 150 ms
			}else if(vitaPad.righttrigger){
				
				
				scrollY += 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}else if(vitaPad.lefttrigger){
				
				
				scrollY -= 3;
				sceKernelDelayThread(sleepTimeInputScroll);
				
			}
			
			
		}
		

		
	}
	
	logSD("Exited inapp loop\n");
	
	vita2d_fini();
	vita2d_free_texture(backgroundImage);
	
	logSD("curl global cleanup\n");

	
	terminateNetworking();
	logSD("term http\n");
	httpTerm();
	logSD("term net\n");
	netTerm();
	sceKernelDelayThread(5*1000*1000);
	
	sceKernelExitProcess(0);
	return 0;
}

