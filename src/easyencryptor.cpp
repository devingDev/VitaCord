#include "easyencryptor.hpp"

std::string simpleEncrypt( std::string msg )
{
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output += msg[i] + 1 ;
	}
	return output;
}
std::string simpleDecrypt( std::string msg )
{
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output += msg[i] - 1 ;
	}
	return output;
}
//
	

/*std::string xorEncrypt(std::string nString)
{
	const int KEY = 23;                       // Key used for XOR operation
	int strLen = (nString.length());          // Grab string length for iterating thru string
	char *cString = (char*)(nString.c_str()); // Convert string to C string so we can actually iterate through it(std::string will treat it as one full object :()
	
	for (int i = 0; i < strLen; i++)          // time to iterate thru string and XOR each individual char.
	{
	 *(cString+i) = (*(cString+i) ^ KEY);     // ^ is the binary operator for XOR
	}
	
	std::string out = std::string(cString , strLen);
	return out;
}

std::string xorDecrypt(std::string nString)
{
	const int KEY = 23;                       // Key used for XOR operation
	int strLen = (nString.length());          // Grab string length for iterating thru string
	char *cString = (char*)(nString.c_str()); // Convert string to C string so we can actually iterate through it(std::string will treat it as one full object :()
	
	for (int i = 0; i < strLen; i++)          // time to iterate thru string and XOR each individual char.
	{
	 *(cString+i) = (*(cString+i) ^ KEY);     // ^ is the binary operator for XOR
	}
	
	std::string out = std::string(cString , strLen);
	return out;
}*/

std::string xorEncrypt( std::string msg ){
	std::string key = "I(p95yP/$o0_2Q!-fg61{?.56WT_z1u/(fj8V_6ZF;S3p:_.Iri3f3(yG52-L_I_[UjoNi2zyPC4%*5r=GBTJV8}c@Tr,:8{^m}lx43N-.PAdQ2GMl@\"x\\xRX1u20I9yP/$o0_,!bCp?16mK)L..-F4798-8_b81JzXlA_yMr16W0-T_z1u/(fj8V_6ZF;S3bCp?16_5mK)L.@.p:_Iri3f3(yG52-zl6p401u/(]}LOZ0h-84l=MI?CoYx\"e,nl!yrDt1*N6@Ytg$.Lo5q2m^^F4W0gTMlyKlOt:Ti&<3$67Kti31ßvdkslß3ßv32]\\"; // -4
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output += msg[i] ^ key[i] ;
	}
	return output;
}

std::string xorDecrypt( std::string msg ){
std::string key = "I(p95yP/$o0_2Q!-fg61{?.56WT_z1u/(fj8V_6ZF;S3p:_.Iri3f3(yG52-L_I_[UjoNi2zyPC4%*5r=GBTJV8}c@Tr,:8{^m}lx43N-.PAdQ2GMl@\"x\\xRX1u20I9yP/$o0_,!bCp?16mK)L..-F4798-8_b81JzXlA_yMr16W0-T_z1u/(fj8V_6ZF;S3bCp?16_5mK)L.@.p:_Iri3f3(yG52-zl6p401u/(]}LOZ0h-84l=MI?CoYx\"e,nl!yrDt1*N6@Ytg$.Lo5q2m^^F4W0gTMlyKlOt:Ti&<3$67Kti31ßvdkslß3ßv32]\\"; // -4
	std::string output = "";
	for( unsigned int i = 0; i < msg.size() ; i++ ){
		output += msg[i] ^ key[i] ;
	}
	return output;
}




