#ifndef GUIELEMENTS_HPP
#define GUIELEMENTS_HPP

#include <vita2d.h>
#include <string>

typedef struct{
	float x,y,w,h;
	unsigned int color;
}rectangle;

typedef struct{
	std::string name;
	
}image;


typedef struct{
	int x;
	int y;
	int w;
	int h;
	
}inputbox;


typedef struct{
	int x;
	int y;
	int w;
	int h;
	
}box;

typedef struct{
	int x;
	int y;
	int w;
	int h;
	int lineCount;
	int messageHeight;
	std::string username;
	std::string content;
}messagebox;

typedef struct{
	int x;
	int y;
	int w;
	int h;
	int dmIndex;
	std::string name;
}dmBox;

typedef struct{
	int x;
	int y;
	int w;
	int h;
	int channelIndex;
	std::string name;
}channelBox;


typedef struct{
	int x;
	int y;
	int w;
	int h;
	int guildIndex;
	std::string name;
}guildBox;


#endif

