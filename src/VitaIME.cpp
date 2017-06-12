#include <stdio.h>



#include <vita2d.h>
#include <psp2/appmgr.h>
#include <psp2/apputil.h>
#include <psp2/types.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/message_dialog.h>
#include <psp2/ime_dialog.h>
#include <psp2/display.h>
#include <psp2/apputil.h>
#include <psp2/types.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/message_dialog.h>
#include <psp2/ime_dialog.h>
#include <psp2/display.h>
#include <psp2/apputil.h>
#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>


#include <string.h>
#include <stdbool.h>

#include "VitaIME.hpp"


static uint16_t ime_title_utf16[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
static uint16_t ime_initial_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH];
static uint16_t ime_input_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
static uint8_t ime_input_text_utf8[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1];
		
void VitaIME::utf16_to_utf8(uint16_t *src, uint8_t *dst) {
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

void VitaIME::utf8_to_utf16(uint8_t *src, uint16_t *dst) {
	int i;
	for (i = 0; src[i];) {
		if ((src[i] & 0xE0) == 0xE0) {
			*(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
			i += 3;
		} else if ((src[i] & 0xC0) == 0xC0) {
			*(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
			i += 2;
		} else {
			*(dst++) = src[i];
			i += 1;
		}
	}

	*dst = '\0';
}
 
void VitaIME::initImeDialog(char *title, char *initial_text, int max_text_length) {
    // Convert UTF8 to UTF16
	utf8_to_utf16((uint8_t *)title, ime_title_utf16);
	utf8_to_utf16((uint8_t *)initial_text, ime_initial_text_utf16);
 
    SceImeDialogParam param;
	sceImeDialogParamInit(&param);

	param.sdkVersion = 0x03150021,
	param.supportedLanguages = 0x0001FFFF;
	param.languagesForced = SCE_TRUE;
	param.type = SCE_IME_TYPE_BASIC_LATIN;
	param.title = ime_title_utf16;
	param.maxTextLength = max_text_length;
	param.initialText = ime_initial_text_utf16;
	param.inputTextBuffer = ime_input_text_utf16;

	//int res = 
	sceImeDialogInit(&param);
	return ;
}

void VitaIME::oslOskGetText(char *text){
	// Convert UTF16 to UTF8
	utf16_to_utf8(ime_input_text_utf16, ime_input_text_utf8);
	strcpy(text,(char*)ime_input_text_utf8);
}


int VitaIME::checkArray(char string[], int length) {
    int i;

    // This next line could as well be a call to `strlen`.
    // But you shouldn't let it access `string` when `i` is >= `length`. 
    for(i=0; string[i] != '\0'; i++);

    // This should be `>=`.  If `i` is 15, you accessed (in the loop above)
    // `string[15]`, which is past the end of the array.
    if(i > length) {
        return 1;
    }

    return 0;
}

std::string VitaIME::getUserText(char title[] ){
	char emptytext[] = "";
	return getUserText(title , emptytext);
}



std::string VitaIME::getUserText(char title[] , char showtext[]) {
    bool shown_dial = false;
   
    char userText[512];
    strcpy(userText, showtext);
   
   
   
    while (1) {
        vita2d_start_drawing();
        vita2d_clear_screen();
       
        if (!shown_dial) {
            initImeDialog(title, userText, 128);
            shown_dial = true;
            }
       
        SceCommonDialogStatus status = sceImeDialogGetStatus();
       
        if (status == (SceCommonDialogStatus)IME_DIALOG_RESULT_FINISHED) {
            SceImeDialogResult result;
            memset(&result, 0, sizeof(SceImeDialogResult));
            sceImeDialogGetResult(&result);
 
            if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE)
                status = (SceCommonDialogStatus)IME_DIALOG_RESULT_CANCELED;
            else
                oslOskGetText(userText);
 
            sceImeDialogTerm();
            shown_dial = 0;
            break;
		}
       
        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart();
	}
    return userText;
}





/*std::string VitaIME::getUserText(char title[] , char showtext[]){
	int shown_dial = 0;
	
	char userText[512] ;
	strncpy(userText , showtext , 512);
	userText[512] = 0;
	
	std::string userTextString = "";
	
    while (1) {
        if(!shown_dial){
            initImeDialog(title, userText, 128);
			shown_dial=1;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();
       
        SceCommonDialogStatus status = sceImeDialogGetStatus();
       
		if (status == IME_DIALOG_RESULT_FINISHED) {
			SceImeDialogResult result;
			memset(&result, 0, sizeof(SceImeDialogResult));
			sceImeDialogGetResult(&result);

			if (result.button == SCE_IME_DIALOG_BUTTON_CLOSE) {
				status = IME_DIALOG_RESULT_CANCELED;
				vita2d_end_drawing();
				vita2d_common_dialog_update();
				vita2d_swap_buffers();
				sceDisplayWaitVblankStart();
				sceImeDialogTerm();
				shown_dial = 0;
				return userTextString;
				break;
			} else {
				oslOskGetText(userText);
				status = IME_DIALOG_RESULT_CANCELED;
				vita2d_end_drawing();
				vita2d_common_dialog_update();
				vita2d_swap_buffers();
				sceDisplayWaitVblankStart();
				sceImeDialogTerm();
				shown_dial = 0;
				userTextString = userText;
				return userTextString;
				break;
			}

			sceImeDialogTerm();
			shown_dial = 0;
			
			//call uri
			//sceAppMgrLaunchAppByUri(0x20000, userText); //launch flag 0x40000 | open flag 0x20000
			
			if(checkArray( userText , 1) == 1){
				vita2d_end_drawing();
				vita2d_common_dialog_update();
				vita2d_swap_buffers();
				sceDisplayWaitVblankStart();
				userTextString = userText;
				return userTextString;
			}else{
				vita2d_end_drawing();
				vita2d_common_dialog_update();
				vita2d_swap_buffers();
				sceDisplayWaitVblankStart();
				return userTextString;
			}
			
		}
       
        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
        sceDisplayWaitVblankStart();
    }
	return userTextString;
}*/


VitaIME::VitaIME(){
	
	
	
	sceAppUtilInit(&(SceAppUtilInitParam){}, &(SceAppUtilBootParam){});
	sceCommonDialogSetConfigParam(&(SceCommonDialogConfigParam){});
 

}

