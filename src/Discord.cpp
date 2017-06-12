#include "Discord.hpp"
#include "log.hpp"

#include <bitset>
#include <pthread.h>
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>
#include <iconv.h>
#include <wchar.h>
#include <stdlib.h>
#include <algorithm>   // for reverse
#include <debugnet.h>

#include <cctype>
#include "json.hpp"

#include <psp2/kernel/processmgr.h>

uint64_t Discord::osGetTimeMS(){
	return (sceKernelGetProcessTimeWide() / 1000);
}

Discord::Discord(){
	loadedGuilds = false;
	loadingData = false;
}

Discord::~Discord(){
		//?
}
bool Discord::sendDirectMessage(std::string msg){
	debugNetPrintf(DEBUG , "Sending DM\n" );
	std::string postData = "{ \"content\":\"" + msg + "\" }";
	std::string sendDMMessageUrl = "https://discordapp.com/api/v6/channels/" 
							+ directMessages[currentDirectMessage].id + "/messages" ;
	VitaNet::http_response senddmmessageresponse = vitaNet.curlDiscordPost(sendDMMessageUrl , postData , token);
	if(senddmmessageresponse.httpcode == 200){
		debugNetPrintf(DEBUG , "DM SENT!\n" );
		return true;
	}
	
	return false;
}

bool Discord::sendMessage(std::string msg){
	debugNetPrintf(DEBUG , "Sending message\n" );
	std::string postData = "{ \"content\":\"" + msg + "\" }";
	std::string sendMessageUrl = "https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages" ;
	VitaNet::http_response sendmessageresponse = vitaNet.curlDiscordPost(sendMessageUrl , postData , token);
	if(sendmessageresponse.httpcode == 200){
		debugNetPrintf(DEBUG , "Message SENT!\n" );
		
	}
	return true;
}

bool Discord::refreshMessages(){
	
	//debugNetPrintf(DEBUG , "checking time to refresh messages\n" );
	currentTimeMS = osGetTimeMS();
	if(currentTimeMS - lastFetchTimeMS > fetchTimeMS || forceRefreshMessages){
		//debugNetPrintf(DEBUG , "get new messages\n" );
		
		refreshingMessages = true;
		getChannelMessages(currentChannel);
		lastFetchTimeMS = osGetTimeMS();
		refreshedMessages = true;
		refreshingMessages = false;
		forceRefreshMessages = false;
		
	}
	return true;
	
}

void Discord::utf16_to_utf8(uint16_t *src, uint8_t *dst) {
	int i;
	for (i = 0; src[i]; i++) {
		if ((src[i] & 0xFF80) == 0) {
			*(dst++) = src[i] & 0xFF;
		} else if((src[i] & 0xF800) == 0) {
			*(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		} else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
			*(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
			*(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
			*(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
			*(dst++) = (src[i + 1] & 0x3F) | 0x80;
			i += 1;
		} else {
			*(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
			*(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
			*(dst++) = (src[i] & 0x3F) | 0x80;
		}
	}

	*dst = '\0';
}

/*static int strConv(const string &src, wstring &dst) 
{   
    iconv_t cd = iconv_open("UCS-4-INTERNAL", "UTF-8");
    if (cd == (iconv_t)-1)
        return -1;
    
    size_t src_length = strlen(src.c_str());
    int wlen = (int)src_length/3;    
    size_t buf_length = src_length + wlen;
    
    char src_buf[src_length];
    strcpy(src_buf, src.c_str());
    char *buf = new char [buf_length];
    char *inptr = src_buf;
    char *outptr = buf;
    if (iconv(cd, &inptr, &src_length, &outptr, &buf_length) == -1) 
    {
        if (buf!=NULL)
            delete [] buf;
        return -1;
    }
    iconv_close(cd);
    
    dst = wstring(reinterpret_cast<wchar_t*>(buf));
    dst = dst.substr(0, wlen);
    
    if (buf!=NULL)
        delete [] buf;
    
    return wlen;    
}*/

void Discord::parseMessageContentEmoji(message *m , std::string str){
	

	//bool escFound = false;
	//bool uniescFound = false;
	//bool lookingForHi = false;
	//bool lookingForLo = false;
	//bool foundHi = false;
	//bool foundLo = false;
	//int cpCount = 0;
	//int trCount = 0;
	//int hi = 0;
	//int lo = 0;
	//std::string hiStr = "";
	//std::string loStr = "";
	
	//int emojiCount = 0;
	
	// TODO : removeunicode escapes and only keep content + whitesapce
	m->content = str;
	
	std::wstring_convert<std::codecvt_utf8<char32_t>,char32_t> convert;
	std::u32string utf32String = convert.from_bytes(str);;
	//m->contentUTF32 = utf32String;
	
	for(unsigned int i = 0; i < utf32String.length() ; i++){
		
		//for(int emo = 0; emo < emoList.size() ; emo++){
		//	
		//	
		//}
		
		
	}
	

	//bool hiFound = false;
	//bool loFound = false;
	//std::string surrogateStr = "";
	//int surrogateHi = 0;
	//int surrogateLo = 0;
	m->emojis.clear();
	
	
	// old function BAAAD only working if the user writes the unicode himself like : \uXXXX\uXXXX 
	/*
	for(int i = 0; i < str.length(); i ++){
		criticalLogSD("Checking message length:");
		criticalLogSD("i + 11 ? str.length:" + std::to_string(str.length()));
		if(i + 11 < str.length()){  
			criticalLogSD("Checking message for escape sign:");
			criticalLogSD("checking for hi esc : " + str[i]);
			if(str[i] == '\\'){
				criticalLogSD(".esc found:");
				if(str[i+1] == '\\'){
					criticalLogSD("-double esc sign");
					continue;
				}
				criticalLogSD(".checking for hi u : " + str[i+1]);
				if(str[i+1] == 'u'){
					criticalLogSD(".found u sign");
					criticalLogSD("checking for lo esc : " + str[i+6]);
					if(str[i+6] == '\\'){
						criticalLogSD(".found lo esc sign");
						criticalLogSD("checking for lo u : " + str[i+7]);
						if(str[i+7] == 'u'){
							
							criticalLogSD(".found lo u sign");
							
							criticalLogSD("next sign is : " + str[i+2]);
							if(isxdigit(str[i+2])){
								criticalLogSD("1 found hex sign");
								criticalLogSD("next sign is : " + str[i+3]);
								surrogateStr += str[i+2];
								if(isxdigit(str[i+3])){
									criticalLogSD("2 found hex sign");
									criticalLogSD("next sign is : " + str[i+4]);
									surrogateStr += str[i+3];
									if(isxdigit(str[i+4])){
										criticalLogSD("3 found hex sign");
										criticalLogSD("next sign is : " + str[i+5]);
										surrogateStr += str[i+4];
										if(isxdigit(str[i+5])){
											criticalLogSD("4 found hex sign");
											surrogateStr += str[i+5];
											
											criticalLogSD("surrogateStr : " + surrogateStr);
											
											stringStream.str(std::string());
											stringStream.clear();
											stringStream << std::hex << surrogateStr;
											stringStream >> surrogateHi;
											surrogateStr = "";
											
											if (surrogateHi >= 0xD800 && surrogateHi <= 0xDBFF ){
												
												criticalLogSD("surrogateStr was Higher surrogate");
												criticalLogSD("next sign is : " + str[i+8]);
												if(isxdigit(str[i+8])){
													criticalLogSD("5 found hex sign");
													criticalLogSD("next sign is : " + str[i+9]);
													surrogateStr += str[i+8];
													if(isxdigit(str[i+9])){
														criticalLogSD("6 found hex sign");
														criticalLogSD("next sign is : " + str[i+10]);
														surrogateStr += str[i+9];
														if(isxdigit(str[i+10])){
															criticalLogSD("7 found hex sign");
															criticalLogSD("next sign is : " + str[i+11]);
															surrogateStr += str[i+10];
															if(isxdigit(str[i+11])){
																criticalLogSD("8 found hex sign");
																surrogateStr += str[i+11];
																criticalLogSD("surrogateStr : " + surrogateStr);
																
																
																stringStream.str(std::string());
																stringStream.clear();
																stringStream << std::hex << surrogateStr;
																stringStream >> surrogateLo;
																surrogateStr = "";
																if (surrogateLo >= 0xDC00 && surrogateLo <= 0xDFFF ){
																	criticalLogSD("surrogateStr was Lower surrogate");
																	//std::codecvt_utf16<char32_t, 0x10ffffUL, std::codecvt_mode::little_endian> cvt;
																	//mbstate_t state;
																	//char16_t pair[] = { higherSurrogate, lowerSurrogate };
																	//const char16_t *next;
                                                                    //
																	//char32_t u[2];
																	//char32_t *unext;
                                                                    //
																	//cvt.in(state, (const char *)pair, (const char *)(pair + 2),(const char *&)next, u, u + 1, unext);
																	
																	// hi - 0xD800
																	// lo - 0xDC00
																	// unicodepoint => (number which first 10 bits are = hi bits and last 10 bits are lo ) + 0x010000
																	//std::bitset<32> myBit(0);
																	//int myCP = 0;
																	
																	int XTRM = ((surrogateHi - 0xD800) << 10);
																	XTRM += (surrogateLo-0xDC00);
																	XTRM += 0x010000;
																	
																	criticalLogSD("totally found an emoji! : ");
																	criticalLogSD("Position : " + std::to_string(i));
																	criticalLogSD("message id : " + m->id);
																	criticalLogSD(std::to_string(XTRM));
																	
																	m->emojis.push_back(message_emoji());
																	m->emojis[emojiCount].codepoint = XTRM;
																	m->emojis[emojiCount].x = i;
																	emojiCount++;
																	i+=6;
																	surrogateHi = 0;
																	surrogateLo = 0;
																	
																}
														
															}
														
														}
													
													}
													
												}
												
											}
											
										}
									}
								}
							}
						}
					}
					
				}
			}
		}else{
			i = str.length();// no more to get here :P
		}
		surrogateStr = "";
	}
	
	
	*/
	
	
	/*for(int i = 0; i < str.length() ; i++){
		
		
		criticalLogSD("char at i = " + std::to_string(i) + " : " + str[i] + " (int) : " + std::to_string((int)str[i]));
		
		
		//std::wstring messageWString;
		//std::string messageString;
		//strConv(messageString , messageWString);
		//
		//wchar_t widecstr = messageWString.c_str();
		
		
	}*/
	
}

void Discord::getChannelMessages(int channelIndex){
	currentChannel = channelIndex;
	std::string channelMessagesUrl = "https://discordapp.com/api/channels/" + guilds[currentGuild].channels[currentChannel].id + "/messages";
	VitaNet::http_response channelmessagesresponse = vitaNet.curlDiscordGet(channelMessagesUrl , token);
	logSD(channelmessagesresponse.body);
	if(channelmessagesresponse.httpcode == 200){
		nlohmann::json j_complete = nlohmann::json::parse(channelmessagesresponse.body);
		int messagesAmount = j_complete.size();
		
		
		if(!j_complete.is_null()){
			guilds[currentGuild].channels[currentChannel].messages.clear();
			
			for( int i = 0 ; i < messagesAmount ; i++){
				guilds[currentGuild].channels[currentChannel].messages.push_back(message());
				
				if(!j_complete[i].is_null()){
					
					if(!j_complete[i]["timestamp"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].timestamp = j_complete[i]["timestamp"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].timestamp = "0";
					}
					
					if(!j_complete[i]["id"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].id = "0";
					}
					
					if(!j_complete[i]["content"].is_null()){
						
						//std::string str = 
						//char * content = new char [str.length()+1];
						//std::strcpy (content, str.c_str());
						//char * contentUtf8 = new char [str.length()+1];
						//utf16_to_utf8((uint16_t *)content , (uint8_t *) contentUtf8);
						//parseMessageContentEmoji(&guilds[currentGuild].channels[currentChannel].messages[i] , j_complete[i]["content"].get<std::string>() );
						guilds[currentGuild].channels[currentChannel].messages[i].content = j_complete[i]["content"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].content = "";
					}
					// author :
					if(!j_complete[i]["author"]["username"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.username = j_complete[i]["author"]["username"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.username = "N/A";
					}
					
					if(!j_complete[i]["author"]["discriminator"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.discriminator = j_complete[i]["author"]["discriminator"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.discriminator = "N/A";
					}
					
					if(!j_complete[i]["author"]["id"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.id = j_complete[i]["author"]["id"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.id = "0";
					}
					
					if(!j_complete[i]["author"]["avatar"].is_null()){
						guilds[currentGuild].channels[currentChannel].messages[i].author.avatar = j_complete[i]["author"]["avatar"].get<std::string>();
					}else{
						guilds[currentGuild].channels[currentChannel].messages[i].author.avatar = "0";
					}

				}
			}
			
			
			std::reverse(guilds[currentGuild].channels[currentChannel].messages.begin() , guilds[currentGuild].channels[currentChannel].messages.end());
			
		}
	
		lastFetchTimeMS = osGetTimeMS();
		
	}
	
}
void Discord::JoinGuild(int gIndex){
	currentGuild = gIndex;
}
void Discord::JoinChannel(int cIndex){
	inChannel = true;
	currentChannel = cIndex;
	forceRefreshMessages = true;
	refreshMessages();
	forceRefreshMessages = false;
	
	if(!pthreadStarted){
		debugNetPrintf(DEBUG , "Startint pthread refresh Messages\n");
		
		pthreadStarted = true;
		logSD("pthread_create( loadDataThread , NULL , wrapper , 0)");
		debugNetPrintf(DEBUG , "pthread_create coming\n");
		if( int errP = pthread_create(&loadMessagesThread, NULL, &Discord::refreshMessages_wrapper, this) != 0){
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			debugNetPrintf(DEBUG , "PTHREAD_CREATE ERROR : %d\n" , errP);
			pthreadStarted = false;
			
		}else{
			
			debugNetPrintf(DEBUG , "successfully started pthread\n");
			
		}
	}
	//getChannelMessages(currentChannel);
}
void Discord::LeaveChannel(){
	inChannel = false;
	currentChannel = 0;
}
void Discord::setToken(std::string tok){
	token = tok;
}


void * Discord::thread_loadData(void *arg){
	
	Discord *discordPtr = reinterpret_cast<Discord *>(arg);
	logSD("start of thread_loadData");
	discordPtr->loadingData = true;
	while(discordPtr->loadingData){
		if(!discordPtr->loadedGuilds){
			std::string guildsUrl = "https://discordapp.com/api/users/@me/guilds";
			VitaNet::http_response guildsresponse = discordPtr->vitaNet.curlDiscordGet(guildsUrl , token);
			logSD(guildsresponse.body);
			if(guildsresponse.httpcode == 200){
				try{
					nlohmann::json j_complete = nlohmann::json::parse(guildsresponse.body);
					if(!j_complete.is_null()){
						discordPtr->guilds.clear();
						discordPtr->guildsAmount = j_complete.size();
						for(int i = 0; i < guildsAmount; i++){
							
							discordPtr->guilds.push_back(guild());
							
							if(!j_complete[i].is_null()){
								
								
								if(!j_complete[i]["owner"].is_null()){
									discordPtr->guilds[i].owner = j_complete[i]["owner"].get<bool>();
								}else{
									discordPtr->guilds[i].owner = false;
								}
								
								if(!j_complete[i]["permissions"].is_null()){
									discordPtr->guilds[i].permissions = j_complete[i]["permissions"].get<long>();
								}else{
									discordPtr->guilds[i].permissions = 0;
								}
								
								if(!j_complete[i]["icon"].is_null()){
									discordPtr->guilds[i].icon = j_complete[i]["icon"].get<std::string>();
								}else{
									discordPtr->guilds[i].icon = "";
								}
								
								if(!j_complete[i]["id"].is_null()){
									discordPtr->guilds[i].id = j_complete[i]["id"].get<std::string>();
									logSD(discordPtr->guilds[i].id);
								}else{
									discordPtr->guilds[i].id = "";
									logSD(discordPtr->guilds[i].id);
								}
								
								if(!j_complete[i]["name"].is_null()){
									discordPtr->guilds[i].name = j_complete[i]["name"].get<std::string>();
									logSD(discordPtr->guilds[i].name);
									
								}else{
									discordPtr->guilds[i].name = "";
									logSD(discordPtr->guilds[i].name);
								}
								
								
								
							}
							
							
						}
						discordPtr->loadedGuilds = true;
					}
				}catch(const std::exception& e){
					discordPtr->loadedGuilds = true;
				}
				
			}else{
				discordPtr->loadedGuilds = true;
			}
		}else if(discordPtr->loadedGuilds && !discordPtr->loadedChannels){
			
			
			
			
			
			for(int i = 0; i < discordPtr->guildsAmount ; i++){
				
				
				
				std::string myRolesUrl ="https://discordapp.com/api/guilds/" + discordPtr->guilds[i].id + "/members/" + discordPtr->id;
				VitaNet::http_response myRolesResponse = discordPtr->vitaNet.curlDiscordGet(myRolesUrl , token);
				debugNetPrintf(DEBUG , "Roles response %d \n" , i);
				debugNetPrintf(DEBUG , myRolesResponse.body.c_str());
				if(myRolesResponse.httpcode == 200){
					try{
						nlohmann::json j_complete = nlohmann::json::parse(myRolesResponse.body);
						if(!j_complete.is_null()){
								
							if(!j_complete["roles"].is_null()){
								
								discordPtr->guilds[i].myroles.clear();
								int rolesAmount = j_complete["roles"].size();
								for(int rol = 0; rol < rolesAmount ; rol++){
									if(!j_complete["roles"][rol].is_null()){
										std::string role = j_complete["roles"][rol].get<std::string>();
										discordPtr->guilds[i].myroles.push_back(role);
										
									}
									
								}
								
							}
								
						}
					}catch(const std::exception& e){
						// nothing
					}
					
				}
				
				
				
				
				
				
				std::string channelUrl = "https://discordapp.com/api/guilds/" + discordPtr->guilds[i].id + "/channels";
				VitaNet::http_response channelresponse = discordPtr->vitaNet.curlDiscordGet(channelUrl , token);
				debugNetPrintf(DEBUG , "Channel response \n");
				debugNetPrintf(DEBUG , channelresponse.body.c_str());
				logSD(channelresponse.body);
				if(channelresponse.httpcode == 200){
					try{
						nlohmann::json j_complete = nlohmann::json::parse(channelresponse.body);
						if(!j_complete.is_null()){
							discordPtr->guilds[i].channels.clear();
							int channelsAmount = j_complete.size();
							for(int c = 0; c < channelsAmount; c++){
								
								discordPtr->guilds[i].channels.push_back(channel());
								
								if(!j_complete[c].is_null()){
									
									if(!j_complete[c]["type"].is_null()){
										discordPtr->guilds[i].channels[c].type = j_complete[c]["type"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].type);
									}else{
										discordPtr->guilds[i].channels[c].type = "text";
										logSD(discordPtr->guilds[i].channels[c].type);
									}
									
									if(!j_complete[c]["id"].is_null()){
										discordPtr->guilds[i].channels[c].id = j_complete[c]["id"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].id);
									}else{
										discordPtr->guilds[i].channels[c].id = "00000000";
										logSD(discordPtr->guilds[i].channels[c].id);
									}
									
									if(!j_complete[c]["name"].is_null()){
										discordPtr->guilds[i].channels[c].name = j_complete[c]["name"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].name);
									}else{
										discordPtr->guilds[i].channels[c].name = "name unavailable";
										logSD(discordPtr->guilds[i].channels[c].name);
									}
									
									if(!j_complete[c]["topic"].is_null()){
										discordPtr->guilds[i].channels[c].topic = j_complete[c]["topic"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].topic);
									}else{
										discordPtr->guilds[i].channels[c].topic = " ";
										logSD(discordPtr->guilds[i].channels[c].topic);
									}
								
									if(!j_complete[c]["is_private"].is_null()){
										discordPtr->guilds[i].channels[c].is_private = j_complete[c]["is_private"].get<bool>();
										logSD(std::to_string(discordPtr->guilds[i].channels[c].is_private));
									}else{
										discordPtr->guilds[i].channels[c].is_private = false;
										logSD(std::to_string(discordPtr->guilds[i].channels[c].is_private));
									}
									
									if(!j_complete[c]["last_message_id"].is_null()){
										discordPtr->guilds[i].channels[c].last_message_id = j_complete[c]["last_message_id"].get<std::string>();
										logSD(discordPtr->guilds[i].channels[c].last_message_id);
									}else{
										discordPtr->guilds[i].channels[c].last_message_id = "00000000";
										logSD(discordPtr->guilds[i].channels[c].last_message_id);
									}
									
									debugNetPrintf(DEBUG , "Checking permission_overwrites\n");
									if(!j_complete[c]["permission_overwrites"].is_null()){
										
										debugNetPrintf(DEBUG , "permission_overwrites\n");
										
										int p = j_complete[c]["permission_overwrites"].size();
										debugNetPrintf(DEBUG , "amount of permission_overwrites: %d\n" , p);
										discordPtr->guilds[i].channels[c].permission_overwrites.clear();
										for(int per = 0; per < p; per++){
											debugNetPrintf(DEBUG , "Add new po\n");
											discordPtr->guilds[i].channels[c].permission_overwrites.push_back(permission_overwrite());
											debugNetPrintf(DEBUG , "check allow\n");
											if(!j_complete[c]["permission_overwrites"][per]["allow"].is_null()){
												debugNetPrintf(DEBUG , "adding allow\n");
												discordPtr->guilds[i].channels[c].permission_overwrites[per].allow = j_complete[c]["permission_overwrites"][per]["allow"].get<int>();
												debugNetPrintf(DEBUG , "adding allow\n");
											}else{
												debugNetPrintf(DEBUG , "no allow\n");
												discordPtr->guilds[i].channels[c].permission_overwrites[per].allow = 0;
												debugNetPrintf(DEBUG , "no allow\n");
											}
											
											debugNetPrintf(DEBUG , "check type\n");
											if(!j_complete[c]["permission_overwrites"][per]["type"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].type = j_complete[c]["permission_overwrites"][per]["type"].get<std::string>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].type = "role";
											}
											
											
											debugNetPrintf(DEBUG , "check id\n");
											if(!j_complete[c]["permission_overwrites"][per]["id"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].id = j_complete[c]["permission_overwrites"][per]["id"].get<std::string>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].id = "0";
											}
											
											debugNetPrintf(DEBUG , "check deny\n");
											if(!j_complete[c]["permission_overwrites"][per]["deny"].is_null()){
												discordPtr->guilds[i].channels[c].permission_overwrites[per].deny = j_complete[c]["permission_overwrites"][per]["deny"].get<int>();
											}else{
												discordPtr->guilds[i].channels[c].permission_overwrites[per].deny = 0;
											}
											
										}
										
										
										// TODO : LEARN HOW TO REALLY CHECK PERMISSION !!!
										//bool readAllowedForMeOnce = false; // if one role has read rest doesnt matter
										//bool readDeniedForMeOnce = false;
										//
										//discordPtr->guilds[i].channels[c].readallowed = false;
										//
										//for(int permC = 0; permC < discordPtr->guilds[i].channels[c].permission_overwrites.size() ; permC++){
										//	
										//	// check role "@everyone" ( = guildid)
										//	if(discordPtr->guilds[i].channels[c].permission_overwrites[permC].id == discordPtr->guilds[i].id){
										//		if(!(discordPtr->guilds[i].channels[c].permission_overwrites[permC].deny & PERMISSION_READ_MESSAGES)){
										//			readDeniedForMeOnce = true;
										//		}else{
										//			readAllowedForMeOnce = true;
										//		}
										//	}
										//	
										//	// check all roles i have
										//	for(int myR = 0; myR < discordPtr->guilds[i].myroles.size() ; myR++){
										//		if(discordPtr->guilds[i].channels[c].permission_overwrites[permC].id == discordPtr->guilds[i].myroles[myR]){
										//			
										//			if(!(discordPtr->guilds[i].channels[c].permission_overwrites[permC].deny & PERMISSION_READ_MESSAGES)){
										//				readDeniedForMeOnce = true;
										//			}else{
										//				readAllowedForMeOnce = true;
										//			}
										//		}
										//	}
										//	
										//	
										//	
										//	
										//	
										//}
										//
										//if(readAllowedForMeOnce){
										//	discordPtr->guilds[i].channels[c].readallowed = true;
										//}else if(readDeniedForMeOnce){
										//	discordPtr->guilds[i].channels[c].readallowed = false;
										//}else{
										//	discordPtr->guilds[i].channels[c].readallowed = true;
										//}
										
										
									}else{
										debugNetPrintf(DEBUG , "no permission_overwrites\n");
									}
									
									
									
								}
								
							}
						
						}
					}catch(const std::exception& e){
				
						discordPtr->loadedChannels = true;
					}
				}
				
			}
			discordPtr->loadedChannels = true;
		}else if(discordPtr->loadedGuilds && discordPtr->loadedChannels && !discordPtr->loadedDMs){
			
			std::string directMessagesChannelsUrl = "https://discordapp.com/api/v6/users/@me/channels";
			VitaNet::http_response dmChannelsResponse = discordPtr->vitaNet.curlDiscordGet(directMessagesChannelsUrl , token);
			logSD(dmChannelsResponse.body);
			if(dmChannelsResponse.httpcode == 200){
				try{
					nlohmann::json j_complete = nlohmann::json::parse(dmChannelsResponse.body);
					if(!j_complete.is_null()){
						discordPtr->directMessages.clear();
						int dmChannels = j_complete.size();
						logSD("Amount of DM channels : " + std::to_string(dmChannels));
						for(int i = 0; i < dmChannels; i++){
							discordPtr->directMessages.push_back(directMessage());
							logSD("dm channel added.");
							
							if(!j_complete[i]["last_message_id"].is_null()){
								discordPtr->directMessages[i].last_message_id = j_complete[i]["last_message_id"].get<std::string>();
								logSD("last message id : ." + discordPtr->directMessages[i].last_message_id);
							}else{
								discordPtr->directMessages[i].last_message_id = "0000000000000000";
								logSD("last message id : ." + discordPtr->directMessages[i].last_message_id);
							}
							if(!j_complete[i]["type"].is_null()){
								discordPtr->directMessages[i].type = j_complete[i]["type"].get<long>();
								logSD("type : ." + std::to_string(discordPtr->directMessages[i].type));
							}else{
								discordPtr->directMessages[i].type = 1;
								logSD("type : ." + std::to_string(discordPtr->directMessages[i].type));
							}
							if(!j_complete[i]["id"].is_null()){
								discordPtr->directMessages[i].id = j_complete[i]["id"].get<std::string>();
								logSD("last id : ." + discordPtr->directMessages[i].id);
							}else{
								discordPtr->directMessages[i].id = "0000000000000000";
								logSD("last id : ." + discordPtr->directMessages[i].id);
							}
							logSD("checking for recipients");
							if(!j_complete[i]["recipients"].is_null()){
								discordPtr->directMessages[i].recipients.clear();
								int recAmount = j_complete[i]["recipients"].size();
								logSD("Amount of recipients : " + std::to_string(recAmount));
								for(int r = 0; r < recAmount  ; r++){
									// author :
									logSD("Adding recipient ");
									discordPtr->directMessages[i].recipients.push_back(user());
									if(!j_complete[i]["recipients"][r]["username"].is_null()){
										discordPtr->directMessages[i].recipients[r].username = j_complete[i]["recipients"][r]["username"].get<std::string>();
										logSD("username : " + discordPtr->directMessages[i].recipients[r].username);
									}else{
										discordPtr->directMessages[i].recipients[r].username = "";
										logSD("username : " + discordPtr->directMessages[i].recipients[r].username);
									}
									
									if(!j_complete[i]["recipients"][r]["discriminator"].is_null()){
										discordPtr->directMessages[i].recipients[r].discriminator = j_complete[i]["recipients"][r]["discriminator"].get<std::string>();
										logSD("discriminator : " + discordPtr->directMessages[i].recipients[r].discriminator);
									}else{
										discordPtr->directMessages[i].recipients[r].discriminator = "";
										logSD("discriminator : " + discordPtr->directMessages[i].recipients[r].discriminator);
									}
									
									if(!j_complete[i]["recipients"][r]["id"].is_null()){
										discordPtr->directMessages[i].recipients[r].id = j_complete[i]["recipients"][r]["id"].get<std::string>();
										logSD("id : " + discordPtr->directMessages[i].recipients[r].id);
									}else{
										discordPtr->directMessages[i].recipients[r].id = "";
										logSD("id : " + discordPtr->directMessages[i].recipients[r].id);
									}
									
									if(!j_complete[i]["recipients"][r]["avatar"].is_null()){
										discordPtr->directMessages[i].recipients[r].avatar = j_complete[i]["recipients"][r]["avatar"].get<std::string>();
										logSD("avatar : " + discordPtr->directMessages[i].recipients[r].avatar);
									}else{
										discordPtr->directMessages[i].recipients[r].avatar = "";
										logSD("avatar : " + discordPtr->directMessages[i].recipients[r].avatar);
									}
									
									
									logSD("end of adding recipient.");
								}
							}
							
							logSD("end of this DM channel.");
							
							
							
						}
						
					}
					
				}catch(const std::exception& e){
					logSD("/EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD(":EXCEPTION THROWN!!!");
					logSD("\\EXCEPTION THROWN!!!");
				}
				
			}
			discordPtr->loadedDMs = true;
			discordPtr->loadingData = false;
		}
	}
	logSD("end of thread_loadData()");
	pthread_exit(NULL);
	return NULL;
}


void * Discord::thread_refreshMessages(void *arg){
	Discord *discordPtr = reinterpret_cast<Discord *>(arg);
	
	//discordPtr->getChannelMessages(discordPtr->currentChannel);
	while(discordPtr->inChannel){
		discordPtr->refreshMessages();
		sceKernelDelayThread(1000000);
	
	}
	pthreadStarted = false;
	pthread_exit(NULL);
	return NULL;
}

void Discord::LeaveDirectMessageChannel(){
	currentDirectMessage = 0;
	inDirectMessageChannel = false;
	
	
}
void Discord::JoinDirectMessageChannel(int dIndex){
	currentDirectMessage = dIndex;
	inDirectMessageChannel = true;
	loadingDirectMessages = true;
	getCurrentDirectMessages();
	loadingDirectMessages = false;
	
	
}

void Discord::getDirectMessageChannels(){
	std::string directMessagesChannelsUrl = "https://discordapp.com/api/v6/users/@me/channels";
	VitaNet::http_response dmChannelsResponse = vitaNet.curlDiscordGet(directMessagesChannelsUrl , token);
	
	if(dmChannelsResponse.httpcode == 200){
		try{
			nlohmann::json j_complete = nlohmann::json::parse(dmChannelsResponse.body);
			if(!j_complete.is_null()){
				directMessages.clear();
				int dmChannels = j_complete.size();
				for(int i = 0; i < dmChannels; i++){
					directMessages.push_back(directMessage());
					
					if(!j_complete[i]["last_message_id"].is_null()){
						directMessages[i].last_message_id = j_complete[i]["last_message_id"].get<std::string>();
					}else{
						directMessages[i].last_message_id = "0000000000000000";
					}
					if(!j_complete[i]["type"].is_null()){
						directMessages[i].type = j_complete[i]["type"].get<long>();
					}else{
						directMessages[i].type = 1;
					}
					if(!j_complete[i]["id"].is_null()){
						directMessages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						directMessages[i].id = "0000000000000000";
					}
					if(!j_complete[i]["recipients"].is_null()){
						directMessages[i].recipients.clear();
						int recAmount = j_complete[i]["recipients"].size();
						for(int r = 0; r < recAmount  ; r++){
							// author :
							directMessages[i].recipients.push_back(user());
							if(!j_complete[i]["recipients"][r]["username"].is_null()){
								directMessages[i].recipients[r].username = j_complete[i]["recipients"][r]["username"].get<std::string>();
							}else{
								directMessages[i].recipients[r].username = "";
							}
							
							if(!j_complete[i]["recipients"][r]["discriminator"].is_null()){
								directMessages[i].recipients[r].discriminator = j_complete[i]["recipients"][r]["discriminator"].get<std::string>();
							}else{
								directMessages[i].recipients[r].discriminator = "";
							}
							
							if(!j_complete[i]["recipients"][r]["id"].is_null()){
								directMessages[i].recipients[r].id = j_complete[i]["recipients"][r]["id"].get<std::string>();
							}else{
								directMessages[i].recipients[r].id = "";
							}
							
							if(!j_complete[i]["recipients"][r]["avatar"].is_null()){
								directMessages[i].recipients[r].avatar = j_complete[i]["recipients"][r]["avatar"].get<std::string>();
							}else{
								directMessages[i].recipients[r].avatar = "";
							}
							
							
						}
					}
					
					
					
					
				}
				
			}
			
		}catch(const std::exception& e){
			logSD("/EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD(":EXCEPTION THROWN!!!");
			logSD("\\EXCEPTION THROWN!!!");
		}
		
	}
	lastFetchTimeMS = osGetTimeMS();
	
}


bool Discord::refreshDirectMessages(){
	
	currentTimeMS = osGetTimeMS();
	if(currentTimeMS - lastFetchTimeMS > fetchTimeMS){
		lastFetchTimeMS = osGetTimeMS();
		getDirectMessageChannels();
		return true;
	}
	return false;
}
bool Discord::refreshCurrentDirectMessages(){
	
	currentTimeMS = osGetTimeMS();
	if(currentTimeMS - lastFetchTimeMS > fetchTimeMS){
		lastFetchTimeMS = osGetTimeMS();
		getCurrentDirectMessages();
		return true;
	}
	return false;
}

void Discord::getCurrentDirectMessages(){
	std::string dmChannelUrl = "https://discordapp.com/api/v6/channels/" + directMessages[currentDirectMessage].id + "/messages";
	VitaNet::http_response dmChannelResponse = vitaNet.curlDiscordGet(dmChannelUrl , token);
	
	
	
	if(dmChannelResponse.httpcode == 200){
		try{
			nlohmann::json j_complete = nlohmann::json::parse(dmChannelResponse.body);
			if(!j_complete.is_null()){
				directMessages[currentDirectMessage].messages.clear();
				int msgAmount = j_complete.size();
				for(int i = 0; i < msgAmount; i++){
					
					directMessages[currentDirectMessage].messages.push_back(message());
					
					if(!j_complete[i]["timestamp"].is_null()){
						directMessages[currentDirectMessage].messages[i].timestamp = j_complete[i]["timestamp"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].timestamp = "";
					}
					
					if(!j_complete[i]["id"].is_null()){
						directMessages[currentDirectMessage].messages[i].id = j_complete[i]["id"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].id = "";
					}
					
					if(!j_complete[i]["content"].is_null()){
						directMessages[currentDirectMessage].messages[i].content = j_complete[i]["content"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].content = "";
					}
					// author :
					if(!j_complete[i]["author"]["username"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.username = j_complete[i]["author"]["username"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.username = "";
					}
					
					if(!j_complete[i]["author"]["discriminator"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.discriminator = j_complete[i]["author"]["discriminator"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.discriminator = "";
					}
					
					if(!j_complete[i]["author"]["id"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.id = j_complete[i]["author"]["id"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.id = "";
					}
					
					if(!j_complete[i]["author"]["avatar"].is_null()){
						directMessages[currentDirectMessage].messages[i].author.avatar = j_complete[i]["author"]["avatar"].get<std::string>();
					}else{
						directMessages[currentDirectMessage].messages[i].author.avatar = "";
					}
				}
			}
		}catch(const std::exception& e){
			
		} 
		
		
		std::reverse( directMessages[currentDirectMessage].messages.begin() , directMessages[currentDirectMessage].messages.end() );
		
	}
	lastFetchTimeMS = osGetTimeMS();
}

void Discord::loadData(){
	logSD("inside loadData()");
	loadingData = true;
	logSD("pthread_t loadDataThread");
	pthread_t loadDataThread;
	logSD("pthread_create( loadDataThread , NULL , wrapper , 0)");
	pthread_create(&loadDataThread, NULL, &Discord::loadData_wrapper, this);
	logSD("end of loadData()");
	
}

long Discord::fetchUserData(){
	
	logSD("Fetching userdata");
	std::string userDataUrl = "https://discordapp.com/api/users/@me";
	VitaNet::http_response userdataresponse = vitaNet.curlDiscordGet(userDataUrl , token);
	logSD("userdata response : " + userdataresponse.body);
	if(userdataresponse.httpcode == 200){
		// check if Two-Factor-Authentication is activated and needs further user action
		nlohmann::json j_complete = nlohmann::json::parse(userdataresponse.body);
		if(!j_complete.is_null()){
			if(!j_complete["username"].is_null()){
				username = j_complete["username"].get<std::string>();
			}
			if(!j_complete["verified"].is_null()){
				verified = j_complete["verified"].get<bool>();
			}
			if(!j_complete["mfa_enabled"].is_null()){
				mfa_enabled = j_complete["mfa_enabled"].get<bool>();
			}
			if(!j_complete["id"].is_null()){
				id = j_complete["id"].get<std::string>();
			}
			if(!j_complete["phone"].is_null()){
				phone = j_complete["phone"].get<std::string>();
			}
			if(!j_complete["avatar"].is_null()){
				avatar = j_complete["avatar"].get<std::string>();
			}
			if(!j_complete["discriminator"].is_null()){
				discriminator = j_complete["discriminator"].get<std::string>();
			}
		}
		
		
	}
	
	return userdataresponse.httpcode;
	
}

void Discord::getGuilds(){
	std::string guildUrl = "https://discordapp.com/api/users/@me/guilds";
}
void Discord::getChannels(){
	
}
std::string Discord::getUsername(){
	return username;
}
std::string Discord::getEmail(){
	return email;
}
std::string Discord::getPassword(){
	return password;
}
void Discord::setEmail(std::string mail){
	email = mail;
}
void Discord::setPassword(std::string pass){
	password = pass;
}
long Discord::login(){
	return login(email , password);
}
long Discord::login(std::string mail , std::string pass){
	logSD("Login attempt");
	email = mail;
	password = pass;
	
	/*if(token.length() > 20){
		if(fetchUserData() == 200){
			loggedin = true;
			return 200;
		}else{
			token = "";
		}
		
	}*/
	
	if(email.length() < 1){
		return -11;
	}else if(password.length() < 1){
		return -12;
	}
	
	//std::string loginUrl = "http://jaynapps.com/psvita/httpdump.php";  // DBG
	std::string loginUrl = "https://discordapp.com/api/auth/login";
	std::string postData = "{ \"email\":\"" + email + "\" , \"password\":\"" + password + "\" }";
	VitaNet::http_response loginresponse = vitaNet.curlDiscordPost(loginUrl , postData , "");
	if(loginresponse.httpcode == 200){
		// check if Two-Factor-Authentication is activated and needs further user action
		nlohmann::json j_complete = nlohmann::json::parse(loginresponse.body);
		try{
			if(!j_complete.is_null()){
				if(!j_complete["mfa"].is_null()){
					if(!j_complete["ticket"].is_null()){
						// Two factor auth is enabled ! grab ticket
						twoFactorAuthEnabled = true;
						ticket = j_complete["ticket"].get<std::string>();
						logSD("Need 2FA Code");
						loginresponse.httpcode = 200000; // NEED MFA
					}
				}else if(!j_complete["token"].is_null()){
					// Logged in !
					token = j_complete["token"].get<std::string>();
					loggedin = true;
					fetchUserData();
				}
				
			}
		}catch(const std::exception& e){
			
		}
		
		
	}else{
		// login failed >_>
		
	}
	return loginresponse.httpcode;
	
}
long Discord::submit2facode(std::string code){
	logSD("Submit 2FA Attempt");
	code2fa = code;

	std::string postData = "{ \"code\":\"" + code2fa + "\" , \"ticket\":\"" + ticket + "\" }";
	VitaNet::http_response submit2facoderesponse = vitaNet.curlDiscordPost( "https://discordapp.com/api/v6/auth/mfa/totp" , postData , token);
	logSD("Submit 2FA Response:");
	if(submit2facoderesponse.httpcode == 200){
		
		nlohmann::json j_complete2 = nlohmann::json::parse(submit2facoderesponse.body);
		if(!j_complete2.is_null()){
			if(!j_complete2["token"].is_null()){
				token = j_complete2["token"].get<std::string>();
				loggedin = true;
				fetchUserData();
			}
		}
		
	}
	return submit2facoderesponse.httpcode;
	
}
std::string Discord::getToken(){
	return token;
}
std::string Discord::getTicket(){
	return ticket;
}

