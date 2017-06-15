#include "VitaNet.hpp"
#include "log.hpp"
#include <psp2/io/fcntl.h> 
#include <psp2/io/stat.h> 


VitaNet::VitaNet(){
	init();
}

VitaNet::~VitaNet(){
	terminate();
}

struct stringcurl {
  char *ptr;
  size_t len;
}; 
void init_string(struct stringcurl *s) {
  s->len = 0;
  s->ptr = (char*)malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = (char*)realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

VitaNet::http_response VitaNet::curlDiscordDelete(std::string url , std::string authtoken){
	
	VitaNet::http_response resp;
	std::string authorizationHeader = "Authorization: " + authtoken;
	
	//DBG
	//url = "http://jaynapps.com/psvita/httpdump.php";
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &resp.httpcode);
		
		resp.header = std::string(header.ptr , header.len);
		resp.body = std::string(body.ptr , body.len);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			
		}else{
			
		}
		
		
	}else{
		
	}
	curl_easy_cleanup(curl);
	
	return resp;
}

VitaNet::http_response VitaNet::curlDiscordGet(std::string url , std::string authtoken){
	
	VitaNet::http_response resp;
	std::string authorizationHeader = "Authorization: " + authtoken;
	
	//DBG
	//url = "http://jaynapps.com/psvita/httpdump.php";
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &resp.httpcode);
		
		resp.header = std::string(header.ptr , header.len);
		resp.body = std::string(body.ptr , body.len);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			
		}else{
			
		}
		
		
	}else{
		
	}
	curl_easy_cleanup(curl);
	
	return resp;
}

VitaNet::http_response VitaNet::curlDiscordPost(std::string url , std::string postdata , std::string authtoken){
	
	logSD("curlDiscordPost");
	
	VitaNet::http_response resp;
	std::string authorizationHeader = "Authorization: " + authtoken;
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		logSD(authorizationHeader);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		std::string ContentLengthS = "Content-Length: " + std::to_string(strlen(postdata.c_str()));
		headerchunk = curl_slist_append(headerchunk, ContentLengthS.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(postdata.c_str()));
		
		
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &resp.httpcode);
				
		resp.header = std::string(header.ptr , header.len);
		resp.body = std::string(body.ptr , body.len);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			/* always cleanup */ 
			
		}else{
			
		}
		
		
	}
	curl_easy_cleanup(curl);
	
	return resp;
}

VitaNet::http_response VitaNet::curlDiscordPatch(std::string url , std::string patchData , std::string authtoken){
	
	VitaNet::http_response resp;
	std::string authorizationHeader = "Authorization: " + authtoken;
	
	//DBG
	//url = "http://jaynapps.com/psvita/httpdump.php";
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		std::string ContentLengthS = "Content-Length: " + std::to_string(strlen(patchData.c_str()));
		headerchunk = curl_slist_append(headerchunk, ContentLengthS.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, patchData.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(patchData.c_str()));
		
		
		
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &resp.httpcode);
		
		resp.header = std::string(header.ptr , header.len);
		resp.body = std::string(body.ptr , body.len);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			
		}else{
			
		}
		
		
	}else{
		
	}
	curl_easy_cleanup(curl);
	
	return resp;
	
}


static size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = sceIoWrite(   *(int*) stream , ptr , size*nmemb);
  return written;
}


VitaNet::http_response  VitaNet::curlDiscordDownloadImage(std::string url , std::string authtoken , std::string file){
	// file will be like "ux0:data/folder/imagename.jpg";
	VitaNet::http_response resp;
	std::string authorizationHeader = "Authorization: " + authtoken;
	
	
	sceIoMkdir("ux0:data/vitacord", 0777);
	sceIoMkdir("ux0:data/vitacord/attachments", 0777);
	sceIoMkdir("ux0:data/vitacord/attachments/thumbnails", 0777);
	sceIoMkdir("ux0:data/vitacord/attachments/images", 0777);
	sceIoMkdir("ux0:data/vitacord/attachments/other", 0777);
	int imageFD = sceIoOpen( file.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!imageFD){
		resp.httpcode = -99;  // could not open file
		return resp;
	}
	
	//DBG
	//url = "http://jaynapps.com/psvita/httpdump.php";
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFD);
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		headerchunk = curl_slist_append(headerchunk, authorizationHeader.c_str());
		//headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &resp.httpcode);
		
		resp.header = std::string(header.ptr , header.len);
		resp.body = "";
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

			
		}else{
			
		}
		
		
	}else{
		
	}
	sceIoClose(imageFD);
	curl_easy_cleanup(curl);
	
	return resp;
}




void VitaNet::init(){
	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	SceNetInitParam netInitParam;
	int size = 4*1024*1024;
	netInitParam.memory = malloc(size);
	netInitParam.size = size;
	netInitParam.flags = 0;
	sceNetInit(&netInitParam);
	sceNetCtlInit();
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTP);
	sceHttpInit(4*1024*1024);
	curl_global_init(CURL_GLOBAL_ALL);
}
void VitaNet::terminate(){
	curl_global_cleanup();
	sceHttpTerm();	
	sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTP);
	sceNetCtlTerm();
	sceNetTerm();
	sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
}



