#include "easyencryptor.hpp"

std::string simpleEncrypt( std::string msg )
{
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output[i] = msg[i] + 1 ;
	}
	return output;
}
std::string simpleDecrypt( std::string msg )
{
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output[i] = msg[i] - 1 ;
	}
	return output;
}
//"I(p95yP/$o0_2Q!-fg616WT_z1u/(fj8V_6ZF;S3p:_Iri3f3(yG52-LZyk!UKKcX8-.PAdQ2GMl@\"x\\xR1u20I9yP/$o0_,!bCp?16mK)L..-F4798-8_b1JzXlAyMr";
	
std::string xorEncrypt( std::string msg ){
	std::string key = "AVERYLONGKEYNOTAVERYLONGKEYNOTAVERYLONGKEYNOTAVERYLONGKEYNOTANOTVERY";
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output[i] = msg[i] ^ key[i] ;
	}
	return output;
}
std::string xorDecrypt( std::string msg ){
	std::string key = "AVERYLONGKEYNOTAVERYLONGKEYNOTAVERYLONGKEYNOTAVERYLONGKEYNOTANOTVERY";
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output[i] = msg[i] ^ key[i] ;
	}
	return output;
}




