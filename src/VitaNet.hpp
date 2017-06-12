#ifndef VITANET_HPP
#define VITANET_HPP

#include <psp2/sysmodule.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>

#include <curl/curl.h>
#include <string>
#include <string.h>

class VitaNet{
	
	
	public:
		typedef struct{
			long httpcode;
			std::string header;
			std::string body;
		} http_response;
		
		typedef struct{
			std::string url;
			std::string postdata;
			std::string authtoken;
		} thread_request;
		
		VitaNet();
		~VitaNet();
		http_response curlDiscordGet(std::string url , std::string authtoken);
		http_response curlDiscordPost(std::string url , std::string postdata , std::string authtoken);



	
	private:
		void init(); 
		void terminate();
		
		
};



#endif




