#ifndef GUIELEMENTS_HPP
#define GUIELEMENTS_HPP

#include <vita2d.h>
#include <string>
#include <vector>

typedef struct{
	int posX;
	int posY;
	int spriteSheetX;
	int spriteSheetY;
} m_emoji;
typedef struct{
	std::string name;
	int id;
	vita2d_texture *icon;
} emoji_icon;
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
	std::string channelID;
	std::string messageID;
	bool showAttachmentAsImage;
	bool showAttachmentAsBinary;
	std::string attachmentFilename;
	vita2d_texture * attachmentThumbnail;
	int attachmentReadableSize;
	std::string attachmentReadableSizeUnit;
	std::string attachmentFullText;
	
	std::vector<m_emoji> emojis;
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

