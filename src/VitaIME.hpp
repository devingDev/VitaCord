#ifndef VITAIME_HPP
#define VITAIME_HPP


#include <stdio.h>
#include <string>


#define SCE_IME_DIALOG_MAX_TITLE_LENGTH	(128)
#define SCE_IME_DIALOG_MAX_TEXT_LENGTH	(512)

#define IME_DIALOG_RESULT_NONE 0
#define IME_DIALOG_RESULT_RUNNING 1
#define IME_DIALOG_RESULT_FINISHED 2
#define IME_DIALOG_RESULT_CANCELED 3

#define LEN SCE_IME_DIALOG_MAX_TEXT_LENGTH


class VitaIME{
	public: 
		VitaIME();
		std::string getUserText(char title[]);
		std::string getUserText(char title[], char showtext[]);
		int checkArray(char string[] , int length);
		
	private:
	



		void oslOskGetText(char text[]);
		void initImeDialog(char *title, char *initial_text, int max_text_length);
		void utf16_to_utf8(uint16_t *src, uint8_t *dst);
		void utf8_to_utf16(uint8_t *src, uint16_t *dst);
	
	
};

#endif

