#ifndef EASYENCRYPTOR_HPP
#define EASYENCRYPTOR_HPP

#include <string>

std::string simpleEncrypt(std::string msg);
std::string simpleDecrypt(std::string msg);

std::string xorEncrypt(std::string msg);
std::string xorDecrypt(std::string msg);


#endif

