#include "easyencryptor.hpp"

std::string encrypt(std::string msg)
{
	std::string output = msg;
	for(unsigned int i = 0; i< msg.size() ; i++){
		output[i] = msg[i]+1;
	}
	return output;
   
}
std::string decrypt(std::string msg)
{
	std::string output = msg;
	for(unsigned int i = 0; i< msg.size() ; i++){
		output[i] = msg[i]-1;
	}
	return output;
	
}

