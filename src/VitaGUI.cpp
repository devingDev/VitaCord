#include "VitaGUI.hpp"
#include "log.hpp"
#include <istream>
#include <sstream>
#include <iterator>
#include <psp2/io/dirent.h>
#include <psp2/power.h>
#include <psp2/rtc.h>
#include <debugnet.h>


#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void VitaGUI::NextFont(){
	// REMOVED 
	
	//currentFont ++;
	//if(currentFont > 4){
	//	currentFont = 0;
	//}else if(currentFont < 0){
	//	currentFont = 0;
	//}
	//
	//switch(currentFont){
	//	case 0:
	//		vita2dFont = vita2dFontSymbola;
	//		break;
	//	case 1:
	//		vita2dFont = vita2dFontSymbolaHint;
	//		break;
	//	case 2:
	//		vita2dFont = vita2dFontSeguiemEmoji;
	//		break;
	//	case 3:
	//		vita2dFont = vita2dFontLastResort;
	//		break;	
	//	case 4:
	//		vita2dFont = vita2dFontOpenSansEmoji;
	//		break;	
	//	default:
	//		vita2dFont = vita2dFontOpenSansEmoji;
	//		break;
	//}
}

VitaGUI::VitaGUI(){
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	//pgf = vita2d_load_default_pgf();
	
	//pgf = vita2d_load_custom_pgf("app0:assets/font/seguiemj.pgf");
	//vita2dFontSymbola = vita2d_load_font_file("app0:assets/font/symbola.ttf");
	//vita2dFontSymbolaHint = vita2d_load_font_file("app0:assets/font/symbolahint.ttf");
	//vita2dFontSeguiemEmoji = vita2d_load_font_file("app0:assets/font/seguiemj.ttf");
	//vita2dFontLastResort = vita2d_load_font_file("app0:assets/font/lastresort.ttf");
	//vita2dFontOpenSansEmoji = vita2d_load_font_file("app0:assets/font/opensansemoji.ttf");
	
	for(int f = 0; f < MAX_FONT_SIZE ; f++){
		vita2dFont[f] = vita2d_load_font_file("app0:assets/font/droidsans.ttf");
	}
	
	vita2dFontSmall = vita2d_load_font_file("app0:assets/font/droidsans.ttf");
	vita2dFontNormal = vita2d_load_font_file("app0:assets/font/droidsans.ttf");
	vita2dFontBig = vita2d_load_font_file("app0:assets/font/droidsans.ttf");
	
	std::string bgPath = "app0:assets/images/Vitacord-Background-8BIT.png";
	backgroundImage = vita2d_load_PNG_file(bgPath.c_str());
	loginFormImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-LoginForm-8BIT.png");
	loadingImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-Loading-8BIT.png");
	guildsBGImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-GuildsBG-8BIT.png");
	dmIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-DMIcon-8BIT.png");
	statbarIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-icon.png");
	statbarBatteryImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-battery.png");
	statbarBatteryChargeImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-statbar-battery-charge.png");
	sidepanelStateIconImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-sidebar-default-usericon.png");
	messageInputImage = vita2d_load_PNG_file("app0:assets/images/Vitacord-messager-input.png");
	
	loginInputs.clear();
	
	inputbox emailI;
	emailI.x = 431;
	emailI.y = 139;
	emailI.w = 375;
	emailI.h = 50;
	loginInputs.push_back(emailI);
	
	inputbox passwordI;
	passwordI.x = 431;
	passwordI.y = 219;
	passwordI.w = 375;
	passwordI.h = 50;
	loginInputs.push_back(passwordI);
	
	inputbox loginI;
	loginI.x = 449;
	loginI.y = 335;
	loginI.w = 349;
	loginI.h = 58;
	loginInputs.push_back(loginI);
	
	loginTexts.clear();
	loginTexts.push_back(" ");
	loginTexts.push_back(" ");
	loginTexts.push_back(" ");
	
	
	inputboxMessageInput.x = 230;
	inputboxMessageInput.y = 473;
	inputboxMessageInput.w = 730;
	inputboxMessageInput.h = 71;
	
	// L8R
	
	std::string filename = "app0:assets/images/loading/thumb0";
	std::string completeName = "";
	for(int fraL = 0 ; fraL < FRAMES_LOADING_IMAGE ; fraL++){
		if(fraL < 9){
			completeName = filename + "0"+ std::to_string(fraL+1) + ".png";
			loadingAnim[fraL] = vita2d_load_PNG_file(completeName.c_str());
		}else{
			completeName = filename + std::to_string(fraL+1) + ".png";
			loadingAnim[fraL] = vita2d_load_PNG_file(completeName.c_str());
		}

	}
	
	
	
	loadEmojiFiles();
	
	
	
}
void VitaGUI::loadEmojiFiles(){
	// EMOJI LOADER:
	emojis.clear();
	emojis.push_back(emoji_icon());
}

VitaGUI::~VitaGUI(){
	vita2d_fini();
	vita2d_free_texture(backgroundImage);
	vita2d_free_texture(loginFormImage);
	vita2d_free_texture(loadingImage);
	vita2d_free_font(vita2dFontSmall);
	vita2d_free_font(vita2dFontNormal);
	vita2d_free_font(vita2dFontBig);
	//vita2d_free_pgf(pgf);
}
void VitaGUI::updateBoxes(){
	
	
}


void VitaGUI::DrawStatusBar() {
	
	vita2d_draw_rectangle(0, 0, 960, 30, RGBA8(174, 85, 44, 255));
	vita2d_draw_texture(statbarIconImage, 10, 7);
	
	// Battery
	float batteryX = (949 - vita2d_texture_get_width(statbarBatteryImage));
	float percent = scePowerGetBatteryLifePercent();
	float width = ((29.0f * percent) / 100.0f);
	
	if (scePowerIsBatteryCharging()) {
		batteryChargeCycle += 0.1;
		if (batteryChargeCycle > width)
			batteryChargeCycle = 0;
		float min = min(width, batteryChargeCycle);
		vita2d_draw_rectangle((947.0f - min), 6, min, 16, RGBA8(91, 223, 38, 255));
		vita2d_draw_texture(statbarBatteryChargeImage, batteryX, 4);
		}
	else {
		if (scePowerIsLowBattery())
			vita2d_draw_rectangle((947.0f - width), 6, width, 16, RGBA8(255, 48, 48, 255));
		else
			vita2d_draw_rectangle((947.0f - width), 6, width, 16, RGBA8(91, 223, 38, 255));
		vita2d_draw_texture(statbarBatteryImage, batteryX, 4);
		}
	
	// Date & time
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime(&time);
	char dateString[16];
	char timeString[24];
	char string[64];
	sprintf(dateString, "%02d/%02d/%04d", time.day, time.month, time.year);
	sprintf(timeString, "%02d:%02d %s", (time.hour > 12) ? (time.hour - 12) : ((time.hour == 0) ? 12 : time.hour), time.minute, time.hour >= 12 ? "PM" : "AM");
	sprintf(string, "%s  %s", dateString, timeString);
	vita2d_font_draw_text(vita2dFontSmall, 875 - vita2d_font_text_width(vita2dFontSmall, 20, string), 22, RGBA8(255, 255, 255, 255), 20, string);
	}


void VitaGUI::Draw(){
	
	//COMMENT debugNetPrintf(DEBUG, "Draw()\n");
	
	if(state == 2){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetGuildBoxes()\n");
		setGuildBoxes();
	} else if(state == 3){
		//COMMENT debugNetPrintf(DEBUG, "Call SetChannelBoxes()\n");
		setChannelBoxes();
	}else if(state == 4){
		//COMMENT debugNetPrintf(DEBUG, "Call SetChannel+MessageBoxes()\n");
		setChannelBoxes();
		setMessageBoxes();
	}else if(state == 6){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetDMBoxes()\n");
		setDirectMessageBoxes();
	}else if(state == 7){
		
		//COMMENT debugNetPrintf(DEBUG, "Call SetDM+MsgBoxes()\n");
		setDirectMessageBoxes();
		setDirectMessageMessagesBoxes();
	}
		
		//COMMENT debugNetPrintf(DEBUG, "All boxes set!\n");
	
	
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	//COMMENT debugNetPrintf(DEBUG, "Check states\n");
	
	if(state == 0){
		
		DrawLoginScreen();
		
	}else if(state == 1){
		// BG
		
		
		

		
		framePassed++;
		if(framePassed >= 2){
			
			loadingImageFrame++;
			framePassed = 0;
		}
		
		if(loadingImageFrame >= FRAMES_LOADING_IMAGE ){
			loadingImageFrame = 0;
		}else if(loadingImageFrame < 0){
			loadingImageFrame = 0;
		}
		if(loadingAnim[loadingImageFrame] != NULL){
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(39, 43, 47, 255));
			// frame of anim
			vita2d_draw_texture(loadingAnim[loadingImageFrame], loadingAnimX , loadingAnimY);
		
		}else{
			vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(95, 118, 198, 255));
			vita2d_draw_texture_rotate(loadingImage, 480 , 247, loadingImageAngle);
			loadingImageAngle += 0.25f;
		}
		// text
		vita2d_font_draw_text(vita2dFont[25] , 300, 355, RGBA8(255,255,255,255), 25, loadingString.c_str());
		

		
	}else if(state == 2){
		
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background

		
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		// GUILDS
		DrawGuildsOnSidebar();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[18], 70, 514, RGBA8(255, 255, 255, 255), 18, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[15], 70, 530, RGBA8(255, 255, 255, 255), 15, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawStatusBar();
		
	}else if(state == 3){
		
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		// CHANNELS
		logSD("loop draw channel boxes");
		DrawChannelsOnSidebar();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[18], 70, 514, RGBA8(255, 255, 255, 255), 18, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[15], 70, 530, RGBA8(255, 255, 255, 255), 15, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON  
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawStatusBar();
		
	}else if(state == 4){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		
		
		//CHANNELS AND AFTER THAT MESSAGES
		
		DrawChannelsOnSidebar();
		
		// MESSAGES
		DrawMessages();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[18], 70, 514, RGBA8(255, 255, 255, 255), 18, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[15], 70, 530, RGBA8(255, 255, 255, 255), 15, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawStatusBar();
		
		// MESSAGEINPUT
		vita2d_draw_texture(messageInputImage, 230, 473);
		
	}else if(state == 6){

		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
		
		
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		DrawDirectMessageUsersOnSidebar();
		
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[18], 70, 514, RGBA8(255, 255, 255, 255), 18, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[15], 70, 530, RGBA8(255, 255, 255, 255), 15, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_rectangle(146, 30, 84, 69, RGBA8(66, 70, 77, 225));
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawStatusBar();
		
	}else if(state == 7){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(54, 57, 62, 255)); // Background
			
			
		/// SIDEPANEL
		// BG
		vita2d_draw_rectangle(0, 30, 230, 449, RGBA8(46, 49, 54, 255));
		
		DrawDirectMessageUsersOnSidebar();
		
		DrawDirectMessageMessages();
		
		// TOP sidepanel to hide guilds underneath
		vita2d_draw_rectangle(0, 0, 230, 100, RGBA8(46, 49, 54, 255));
		vita2d_draw_rectangle(0, 99, 230, 1, RGBA8(5, 5, 6, 255));
		
		// BOTTOM SIDEPANEL
		vita2d_draw_rectangle(0, 479, 230, 1, RGBA8(5, 5, 6, 255));
		vita2d_draw_rectangle(0, 480, 230, 64, RGBA8(40, 43, 48, 255));
		vita2d_draw_texture(sidepanelStateIconImage, 14, 493); // sidepanelStateIconImage = user icon or Vitacord-default-usericon.png
		vita2d_font_draw_text(vita2dFont[18], 70, 514, RGBA8(255, 255, 255, 255), 18, panelUsername.c_str());
		vita2d_font_draw_text(vita2dFont[15], 70, 530, RGBA8(255, 255, 255, 255), 15, panelUserDiscriminator.c_str()); // create a vita2dfont for each font-size or your font will get messed up.
		
		vita2d_draw_rectangle(146, 30, 84, 69, RGBA8(66, 70, 77, 225));
		vita2d_draw_texture(dmIconImage, 166, 41); // DM ICON 
		
		
		// maybe add something on the big right 
		
		/// STATBAR
		DrawStatusBar();
		
		
		// MESSAGEINPUT
		vita2d_draw_texture(messageInputImage, 230, 473);
		
	}else if(state == 9){
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(114, 137, 217, 255));
		unsigned int i = 0;
		for( i = 0; i < rectangles.size() ; i++){
			vita2d_draw_rectangle(rectangles[i].x, rectangles[i].y, rectangles[i].w, rectangles[i].h, rectangles[i].color);
		}
	}

	
	//COMMENT debugNetPrintf(DEBUG, "End of check states\n");
	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	
	//COMMENT debugNetPrintf(DEBUG, "Ended drawing and swapped buffers\n");
}

int VitaGUI::scroll(int x , int y , int posx , int posy){
	//COMMENT debugNetPrintf(DEBUG, "Scroll GUI\n");
	if(state == 2){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			guildScrollX = 0;
			guildScrollY += y;
			if(guildScrollY < guildScrollYMin)
				guildScrollY = guildScrollYMin;
			else if(guildScrollY > guildScrollYMax )
				guildScrollY = guildScrollYMax;
			
		}
		

		
		return 0;
	}else if(state == 3){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			channelScrollX = 0;
			channelScrollY += y;

			
			if(channelScrollY < channelScrollYMin)
				channelScrollY = channelScrollYMin;
			else if(channelScrollY > channelScrollYMax )
				channelScrollY = channelScrollYMax;
		}
		

		
		return 0;
	}else if(state==4){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			channelScrollX = 0;
			channelScrollY += y;
			
			
			if(channelScrollY < channelScrollYMin)
				channelScrollY = channelScrollYMin;
			else if(channelScrollY > channelScrollYMax )
				channelScrollY = channelScrollYMax;
			


		}else if (posx > 230){
			messageScrollX = 0;
			messageScrollY += y;

			
			if(messageScrollY < messageScrollYMin)
				messageScrollY = messageScrollYMin;
			else if(messageScrollY > messageScrollYMax )
				messageScrollY = messageScrollYMax;
		}
		

		return 0;
	}else if(state==6){
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			directMessageScrollX = 0;
			directMessageScrollY += y;
			
			if(directMessageScrollY < directMessageScrollYMin)
				directMessageScrollY = directMessageScrollYMin;
			else if(directMessageScrollY > directMessageScrollYMax )
				directMessageScrollY = directMessageScrollYMax;
		}

		return 0;
	}else if(state==7){
		
		if(posx < 230 && posx > 0 && posy < 522 && posy > 22){
			directMessageScrollX = 0;
			directMessageScrollY += y;
			
			if(directMessageScrollY < directMessageScrollYMin)
				directMessageScrollY = directMessageScrollYMin;
			else if(directMessageScrollY > directMessageScrollYMax )
				directMessageScrollY = directMessageScrollYMax;
		}
		if(posx> 230){
			directMessageMessagesScrollX = 0;
			directMessageMessagesScrollY += y;
			
			
			if(directMessageMessagesScrollY < directMessageMessagesScrollYMin)
				directMessageMessagesScrollY = directMessageMessagesScrollYMin;
			else if(directMessageMessagesScrollY > directMessageMessagesScrollYMax )
				directMessageMessagesScrollY = directMessageMessagesScrollYMax;
		}
		
		return 0;
	}
	return -1;
}


int VitaGUI::analogScrollRight(int x , int y){
	
	
	if(state == 4){
		messageScrollX = 0;
		messageScrollY += y;

		
		if(messageScrollY < messageScrollYMin)
			messageScrollY = messageScrollYMin;
		else if(messageScrollY > messageScrollYMax )
			messageScrollY = messageScrollYMax;
		
	}else if(state == 7){
		directMessageMessagesScrollX = 0;
		directMessageMessagesScrollY += y;
		
		
		if(directMessageMessagesScrollY < directMessageMessagesScrollYMin)
			directMessageMessagesScrollY = directMessageMessagesScrollYMin;
		else if(directMessageMessagesScrollY > directMessageMessagesScrollYMax )
			directMessageMessagesScrollY = directMessageMessagesScrollYMax;
	
	}
	return 1;
	
}

int VitaGUI::analogScrollLeft(int x , int y){
	
	
	if(state == 1){
		loadingAnimX += x;
		loadingAnimY -= y;
	}
	else if (state==2){
		guildScrollX = 0;
		guildScrollY += y;
		if(guildScrollY < guildScrollYMin)
			guildScrollY = guildScrollYMin;
		else if(guildScrollY > guildScrollYMax )
			guildScrollY = guildScrollYMax;
		
	}else if(state == 3){
		
		channelScrollX = 0;
		channelScrollY += y;

		
		if(channelScrollY < channelScrollYMin)
			channelScrollY = channelScrollYMin;
		else if(channelScrollY > channelScrollYMax )
			channelScrollY = channelScrollYMax;
	}else if(state == 4){
		channelScrollX = 0;
		channelScrollY += y;
		
		
		if(channelScrollY < channelScrollYMin)
			channelScrollY = channelScrollYMin;
		else if(channelScrollY > channelScrollYMax )
			channelScrollY = channelScrollYMax;
	}else if(state== 6){
		
		directMessageScrollX = 0;
		directMessageScrollY += y;
		
		if(directMessageScrollY < directMessageScrollYMin)
			directMessageScrollY = directMessageScrollYMin;
		else if(directMessageScrollY > directMessageScrollYMax )
			directMessageScrollY = directMessageScrollYMax;
		
	}else if(state == 7){
		directMessageScrollX = 0;
		directMessageScrollY += y;
		
		if(directMessageScrollY < directMessageScrollYMin)
			directMessageScrollY = directMessageScrollYMin;
		else if(directMessageScrollY > directMessageScrollYMax )
			directMessageScrollY = directMessageScrollYMax;
	}
	return 1;
	
}


int VitaGUI::click(int x , int y){
	//COMMENT debugNetPrintf(DEBUG, "Click GUI\n");
	if(state == 0){
		for(unsigned int i = 0 ; i < loginInputs.size() ; i++){
			if( x > loginInputs[i].x && x < loginInputs[i].x + loginInputs[i].w){
				if( y > loginInputs[i].y && y < loginInputs[i].y + loginInputs[i].h){
					return i;
				}
			}
		}
	}else if(state == 2){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < guildBoxes.size() ; i++){
				if( x  > guildBoxes[i].x && x  < guildBoxes[i].x + guildBoxes[i].w){
					if( y > guildBoxes[i].y && y  < guildBoxes[i].y + guildBoxes[i].h){
						return guildBoxes[i].guildIndex;
					}
				}
			}
		}
		
	}else if(state == 3){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < channelBoxes.size() ; i++){
				if( x  > channelBoxes[i].x && x  < channelBoxes[i].x + channelBoxes[i].w){
					if( y  > channelBoxes[i].y && y  < channelBoxes[i].y + channelBoxes[i].h){
						return channelBoxes[i].channelIndex;
					}
				}
			}
		}
	}else if(state == 4){
		
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < channelBoxes.size() ; i++){
				if( x  > channelBoxes[i].x && x  < channelBoxes[i].x + channelBoxes[i].w){
					if( y  > channelBoxes[i].y && y  < channelBoxes[i].y + channelBoxes[i].h){
						return channelBoxes[i].channelIndex;
					}
				}
			}
		}
		
		if( x > inputboxMessageInput.x && y < inputboxMessageInput.x + inputboxMessageInput.w){
			if( y > inputboxMessageInput.y && y < inputboxMessageInput.y + inputboxMessageInput.h){
				return CLICKED_MESSAGE_INPUT;
			}
		}
		
		
	}else if(state == 6){
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			return CLICKED_DM_ICON;
		}
		
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < directMessageBoxes.size() ; i++){
				if( x  > directMessageBoxes[i].x && x  < directMessageBoxes[i].x + directMessageBoxes[i].w){
					if( y  > directMessageBoxes[i].y && y  < directMessageBoxes[i].y + directMessageBoxes[i].h){
						return directMessageBoxes[i].dmIndex;
					}
				}
			}
		}
	}else if(state == 7){
		
		//COMMENT debugNetPrintf(DEBUG, "state 7 check click %d  and %d \n"  , x , y);
		
		if(x > DMICONX && x < DMICONX2 && y > DMICONY && y < DMICONY2){
			//COMMENT debugNetPrintf(DEBUG, "clicked dmicon\n");
			return CLICKED_DM_ICON;
		}
		if( y < 515  &&  y > 99){
			for(unsigned int i = 0 ; i < directMessageBoxes.size() ; i++){
				if( x  > directMessageBoxes[i].x && x  < directMessageBoxes[i].x + directMessageBoxes[i].w){
					if( y  > directMessageBoxes[i].y && y  < directMessageBoxes[i].y + directMessageBoxes[i].h){
						return directMessageBoxes[i].dmIndex;
					}
				}
			}
		}
		
		if( x > inputboxMessageInput.x && y < inputboxMessageInput.x + inputboxMessageInput.w){
			if( y > inputboxMessageInput.y && y < inputboxMessageInput.y + inputboxMessageInput.h){
				//COMMENT debugNetPrintf(DEBUG, "clicked mniput\n");
				return CLICKED_MESSAGE_INPUT;
			}
		}
		
		// ? messages
	}
	return -1;
}

void VitaGUI::AddRectangle(float nx , float ny , float nw , float nh , unsigned int ncolor){
	rectangle r;
	r.x = nx;
	r.y = ny;
	r.w = nw;
	r.h = nh;
	r.color = ncolor;
	rectangles.push_back(r);
}
void VitaGUI::RemoveRectangle(unsigned int index){
	
	if(index < rectangles.size()){
		rectangles.erase(rectangles.begin()+index);
	}
	
}

int VitaGUI::GetState(){
	return state;
}
void VitaGUI::SetState(int s){
	lastState = state;
	state = s;
	//COMMENT debugNetPrintf(DEBUG, "SetState : %d\n" , state);
	
	if(state == 4){
		messageScrollY = 0;
		directMessageMessagesScrollY = 0;
		directMessageScrollY = 0;
		setMessageBoxes();
		messageScrollSet = false;
	}else if(state == 7 && lastState == 6){
		messageScrollY = 0;
		directMessageMessagesScrollY = 0;
		setDirectMessageMessagesBoxes();
		directMessageMessagesScrollSet = false;
		
	}else if(lastState == 7 && state == 7){
		
		directMessageMessagesScrollY = 0;
		setDirectMessageMessagesBoxes();
		directMessageMessagesScrollSet = false;
		
		
	}else{
		messageScrollY = 0;
		channelScrollY = 0;
		directMessageMessagesScrollY = 0;
		directMessageScrollY = 0;
	}
	

	
}
void VitaGUI::SetStateToLastState(){
	state = lastState;
}

void VitaGUI::passDiscordPointer(Discord *ptr){
	discordPtr = ptr;
}
void VitaGUI::setGuildBoxes(){
	guildBoxes.clear();
	for(unsigned int i = 0; i < discordPtr->guilds.size() ; i++){
		guildBox boxG;
		boxG.x = guildScrollX ;
		boxG.y = 100 + guildScrollY + i * GUILD_HEIGHT;
		boxG.w = 230;
		boxG.h = GUILD_HEIGHT;
		boxG.guildIndex = i;
		boxG.name = discordPtr->guilds[i].name;
		guildBoxes.push_back(boxG);
	}
	guildScrollYMin = -((guildBoxes.size()-1)*GUILD_HEIGHT - 100);
	guildScrollYMax = 100;
}
void VitaGUI::setChannelBoxes(){
	channelBoxes.clear();
	int amount = 0;
	for(unsigned int i = 0; i < discordPtr->guilds[discordPtr->currentGuild].channels.size() ; i++){
		if(discordPtr->guilds[discordPtr->currentGuild].channels[i].type == "text" && discordPtr->guilds[discordPtr->currentGuild].channels[i].readallowed){
			channelBox boxC;
			boxC.x = channelScrollX ;
			boxC.y = 100 + channelScrollY + amount * CHANNEL_HEIGHT;
			boxC.w = 230;
			boxC.h = CHANNEL_HEIGHT;
			boxC.channelIndex = i;
			boxC.name = discordPtr->guilds[discordPtr->currentGuild].channels[i].name;
			channelBoxes.push_back(boxC);
			amount++;
		}
	}
	channelScrollYMin = -((amount-1)*CHANNEL_HEIGHT - 100) ;
	channelScrollYMax = 100;
}
bool VitaGUI::setMessageBoxes(){
		
	int topMargin = 45;
	int bottomMargin = 18;
	int textHeight = 0;
	int allHeight = 0;
	if(!discordPtr->refreshingMessages && discordPtr->refreshedMessages){
		discordPtr->refreshedMessages = false;
		messageBoxes.clear();
		for(unsigned int i = 0; i < discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages.size() ; i++){
			messagebox boxC;
			boxC.x = messageScrollX + 230;
			boxC.y = messageScrollY + i * 64;
			boxC.username = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].author.username;
			boxC.content = "";
			boxC.lineCount = wordWrap( discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content , 80 , boxC.content);
			textHeight = boxC.lineCount * vita2d_font_text_height(vita2dFont[15], 15, boxC.content.c_str());
			boxC.messageHeight = max(64, textHeight + topMargin + bottomMargin);
			allHeight += boxC.messageHeight;
			
			
			boxC.w = 730;
			boxC.h = boxC.messageHeight;
			
			
			messageBoxes.push_back(boxC);
		}
		messageScrollYMin =  -( allHeight )  + 430; //-( allHeight )
		messageScrollYMax = 400;
		
		if(!messageScrollSet){
			messageScrollSet = true;
			messageScrollY = messageScrollYMin;
		}
		
		return true;
	}
	return false;
}


int VitaGUI::wordWrap(std::string str, unsigned int maxCharacters, std::string &out) {
	if(str.length() < maxCharacters ) {
		out = str;
		return 1; // 1 or 0 ?
		
	}
	out = "";
	int breaks = str.length() / maxCharacters;
	for(int i = 0 ; i < breaks+1; i++){
		out += str.substr(i*maxCharacters, maxCharacters) + '\n';
		
	}
	
	return breaks;
}

/*int VitaGUI::wordWrap(std::string str, int width, std::string &out) {
   
    unsigned int curWidth = width;
    int lineCount = 1;
    while (curWidth < str.length()) {
        std::string::size_type spacePos = str.rfind(' ', curWidth);
        if (spacePos == std::string::npos){
            spacePos = str.find(' ', curWidth);
		}
        if (spacePos != std::string::npos) {
			str[spacePos] = '\n';
			lineCount++;
			curWidth = spacePos + width + 1;
		}
	}
    out = str.substr(0, str.size());
    return lineCount;

}*/
/*int VitaGUI::wordWrap(std::string stringToWrap ,  int availableWidth , std::string &out ){
	
	int wordWidth = 0;
	int lineBreaks = 0;
	
	out="";
	std::istringstream iss{stringToWrap};
	// Read tokens from stream into vector (split at whitespace).
	std::vector<std::string> words{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};
	
	
	
	int spaceLeft = availableWidth;
	for(unsigned int i = 0 ; i < words.size() ; i++){
		
		//wordWidth = vita2d_font_text_width(vita2dFontNormal, MESSAGE_CONTENT_TEXT_SIZE_PIXEL, words[i].c_str());
		wordWidth = words[i].length();
	
		if(wordWidth > spaceLeft){
			int wordBreaks = wordWidth / availableWidth;
			for(int wB = 0; wB < wordBreaks; wB++){
				words[i] = '\n' + words[i].substr(
				
				
			}
			
			
		}else if(wordWidth + 1  > spaceLeft){
			words[i] = '\n' + words[i];
			lineBreaks++;
			spaceLeft = availableWidth - words[i].length();
		}else{
			spaceLeft = spaceLeft - ( words[i].length() + 1 );
		}
		out += words[i];
		if(words[i] != " " && words[i].length() > 0){
			out += " ";
		}
		
	}
	
	
	return lineBreaks;
	
}*/


void VitaGUI::setDirectMessageBoxes(){
	directMessageBoxes.clear();
	for(unsigned int i = 0; i < discordPtr->directMessages.size() ; i++){
		dmBox boxDM;
		boxDM.x = directMessageScrollX ;
		boxDM.y = 100 + directMessageScrollY + i * GUILD_HEIGHT;
		boxDM.w = 230;
		boxDM.h = GUILD_HEIGHT;
		boxDM.name = discordPtr->directMessages[i].recipients[0].username;
		boxDM.dmIndex = i;
		directMessageBoxes.push_back(boxDM);
	}
	directMessageMessagesScrollYMin = -((directMessageBoxes.size()-1)*CHANNEL_HEIGHT - 100) ;
	directMessageMessagesScrollYMax = 100;
	//directMessageScrollYMin = -((discordPtr->directMessages.size()-1)*GUILD_HEIGHT -300) ;
	
}

void VitaGUI::setDirectMessageMessagesBoxes(){
	
	//discordPtr->directMessages[discordPtr->currentDirectMessage].messages[i].content
	
	
	
	int topMargin = 45;
	int bottomMargin = 18;
	int textHeight = 0;
	int allHeight = 0;
	// TODO CHECK FOR REFRESHED AND REFRESHING DMSGS + PTHREAD THE refreshDm() in Discord.cpp
	//if(!discordPtr->refreshingMessages && discordPtr->refreshedMessages){
		//discordPtr->refreshedMessages = false;
		directMessageMessagesBoxes.clear();
		for(unsigned int i = 0; i < discordPtr->directMessages[discordPtr->currentDirectMessage].messages.size() ; i++){
			messagebox boxC;
			boxC.x = directMessageMessagesScrollX + 230;
			boxC.y = directMessageMessagesScrollY + i * 64;
			boxC.username = discordPtr->directMessages[discordPtr->currentDirectMessage].messages[i].author.username;
			boxC.content = "";
			boxC.lineCount = wordWrap( discordPtr->directMessages[discordPtr->currentDirectMessage].messages[i].content , 80 , boxC.content);
			textHeight = boxC.lineCount * vita2d_font_text_height(vita2dFont[15], 15, boxC.content.c_str());
			boxC.messageHeight = max(64, textHeight + topMargin + bottomMargin);
			allHeight += boxC.messageHeight;
			
			
			
			boxC.w = 730;
			boxC.h = boxC.messageHeight;
			
			
			directMessageMessagesBoxes.push_back(boxC);
		}
		directMessageMessagesScrollYMin =  -( allHeight  ) + 430; //-( allHeight )
		directMessageMessagesScrollYMax =   100; 
		
		
		if(!directMessageMessagesScrollSet){
			directMessageMessagesScrollSet = true;
			directMessageMessagesScrollY = directMessageMessagesScrollYMin;
		}
		

	//}
	

	
}


void VitaGUI::setUserInfo(){
	
	panelUsername = discordPtr->username;
	panelUserDiscriminator = "#" + discordPtr->discriminator;
}


void VitaGUI::showLoginCue(){
	//COMMENT debugNetPrintf(DEBUG, "Show login cue\n");
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	DrawLoginScreen();
	
	vita2d_draw_rectangle(455 , 335, 330 , 58 , RGBA8(0xFF , 0xFF , 0xFF , 0x24));
	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	//COMMENT debugNetPrintf(DEBUG, "Finish show login cue\n");
}

void VitaGUI::unshowLoginCue(){
	//COMMENT debugNetPrintf(DEBUG, "unshwo login cue\n");
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	DrawLoginScreen();
	
	vita2d_end_drawing();
	vita2d_swap_buffers();
	
	//COMMENT debugNetPrintf(DEBUG, "finish unshow login cue\n");
}

void VitaGUI::DrawLoginScreen(){
	
	
		vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(114, 137, 217, 255));
		vita2d_draw_texture( loginFormImage , 161, 53 );
		//vita2d_pgf_draw_text(pgf, 420, 154, RGBA8(255,255,255,255), 2.0f, loginTexts[0].c_str());
		//vita2d_pgf_draw_text(pgf, 420, 250, RGBA8(255,255,255,255), 2.0f, loginTexts[1].c_str());
		vita2d_font_draw_text(vita2dFont[18] , 438, 181, RGBA8(255,255,255,255), 18, loginTexts[0].c_str());
		vita2d_font_draw_text(vita2dFont[18] , 438, 261, RGBA8(255,255,255,255), 18, loginTexts[1].c_str());
		vita2d_font_draw_text(vita2dFont[18] , 750, 261, RGBA8(255,0,0,255), 18, loginTexts[2].c_str());
	
}

void VitaGUI::DrawGuildsOnSidebar(){
	int height = 0;
	//COMMENT debugNetPrintf(DEBUG, "Start of DrawGuilds \n" );
	for(unsigned int i = 0 ; i < guildBoxes.size() ; i++){
		height = guildScrollY + i * GUILD_HEIGHT;
		//COMMENT debugNetPrintf(DEBUG, "Checking guild text height : %d \n" , height);
		if(height < MAX_DRAW_HEIGHT && height  > MIN_DRAW_HEIGHT){
			//vita2d_draw_texture( guildsBGImage , guildScrollX + 230 , guildScrollY + i * 128);
			//vita2d_draw_rectangle(guildScrollX + 4, 100 + guildScrollY + i * GUILD_HEIGHT, 222 , GUILD_HEIGHT, RGBA8(48, 50, 55, 255));
			//vita2d_pgf_draw_text(pgf, guildScrollX + 256, guildScrollY + i * 128 + 96, RGBA8(255,255,255,255), 3.0f, discordPtr->guilds[i].name.c_str());
			//COMMENT debugNetPrintf(DEBUG, "Draw Guild Sidebar text\n");
			vita2d_font_draw_text(vita2dFont[18] , guildScrollX + 8, 100 + guildScrollY + i * GUILD_HEIGHT + 40, RGBA8(255,255,255,255), GUILD_TITLE_TEXT_SIZE_PIXEL, guildBoxes[i].name.c_str());
			//COMMENT debugNetPrintf(DEBUG, "Drew Guild Sidebar text \n" );
		}
	}
	//COMMENT debugNetPrintf(DEBUG, "End of DrawGuilds \n" );
	
}

void VitaGUI::DrawChannelsOnSidebar(){
	int channelsAmount = static_cast<int>(channelBoxes.size());
	for(int i = 0 ; i < channelsAmount ; i++){
		if(channelScrollY + i * CHANNEL_HEIGHT < MAX_DRAW_HEIGHT && channelScrollY + i * CHANNEL_HEIGHT > MIN_DRAW_HEIGHT){
			logSD("adding channelbox");
			if(discordPtr->currentChannel == channelBoxes[i].channelIndex && discordPtr->inChannel){
				vita2d_draw_rectangle(channelScrollX , 100 + channelScrollY + i * CHANNEL_HEIGHT, 215 , CHANNEL_HEIGHT, RGBA8(40, 43, 48, 255));
				vita2d_draw_rectangle(channelScrollX , 100 + channelScrollY + i * CHANNEL_HEIGHT, 4 , CHANNEL_HEIGHT, RGBA8(95, 118, 198, 255));
			}
			//vita2d_pgf_draw_text(pgf, channelScrollX + 256, channelScrollY + i * 128 + 64, RGBA8(255,255,255,255), 3.0f, discordPtr->guilds[discordPtr->currentGuild].channels[i].name.c_str());
			logSD("Channelname:");
			vita2d_font_draw_text(vita2dFont[18] , channelScrollX + 8, 100 + channelScrollY + i * CHANNEL_HEIGHT + 42  , RGBA8(255,255,255,255), CHANNEL_TITLE_TEXT_SIZE_PIXEL, channelBoxes[i].name.c_str());
			//vita2d_pgf_draw_text(pgf, channelScrollX + 256, channelScrollY + i * 128 + 96, RGBA8(255,255,255,255), 1.0f, discordPtr->guilds[discordPtr->currentGuild].channels[i].topic.c_str());
			//logSD("topic:");
			//logSD(discordPtr->guilds[discordPtr->currentGuild].channels[i].topic);
			//std::string channelTopic = discordPtr->guilds[discordPtr->currentGuild].channels[i].topic;
			//vita2d_font_draw_text(vita2dFont[14] , channelScrollX + 180, channelScrollY + i * 128 + 96, RGBA8(255,255,255,255), CHANNEL_TOPIC_TEXT_SIZE_PIXEL, channelTopic.c_str());
		}
	}
	
}

void VitaGUI::DrawMessages(){
	
	int yPos = 0,height;
	unsigned int messageBoxesAmount = messageBoxes.size();
	
	//for(int i =  messageBoxes.size() ; i >= 0  ; i--){
	yPos = messageScrollY + 40;
	for(unsigned int i =  0 ; i < messageBoxesAmount ; i++){
		
		//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 1\n");
		height = messageBoxes[i].messageHeight;
		
		//COMMENT debugNetPrintf(DEBUG, "calculating %d of %d\n", i, messageBoxesAmount);
		if(yPos < MAX_DRAW_HEIGHT && yPos > MIN_DRAW_HEIGHT){
			
			if(i == messageBoxesAmount-1){
				
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(120, 115, 120, 255)); // after last message
				vita2d_draw_rectangle(240, yPos + height , 710, 1, RGBA8(100, 100, 100, 255)); // 
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(120, 115, 120, 255)); // 
			}else{
				//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 2\n");
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(62, 65, 70, 255)); // two small lines to outline the message panel
				//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 3\n");
				vita2d_draw_rectangle(240, yPos + height, 710, 1, RGBA8(51, 53, 55, 255)); // no need for a panel image
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(62, 65, 70, 255)); // two small lines to outline the message panel
				
			}
			
			
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 4\n");
				vita2d_font_draw_text(vita2dFont[15], 283, yPos + 26, RGBA8(255, 255, 255, 255), 15, messageBoxes[i].username.c_str());
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 5\n");
				vita2d_font_draw_text(vita2dFont[15], 293, yPos + 50, RGBA8(255, 255, 255, 255), 15, messageBoxes[i].content.c_str());
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 6\n");
			//vita2d_draw_texture( guildsBGImage , messageScrollX + 128 , messageScrollY + i * 128);
			//vita2d_pgf_draw_text(pgf, messageScrollX + 256, messageScrollY + i * 128 + 96, RGBA8(255,255,255,255), 1.0f, discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content.c_str());
			//vita2d_font_draw_text(vita2dFont , messageScrollX + 150, messageScrollY + i * 128 + 32, RGBA8(255,255,255,255), MESSAGE_AUTHOR_TEXT_SIZE_PIXEL, discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].author.username.c_str());
			//vita2d_font_draw_text(vita2dFont , messageScrollX + 160, messageScrollY + i * 128 + 96, RGBA8(255,255,255,255), MESSAGE_CONTENT_TEXT_SIZE_PIXEL, discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content.c_str());
			//vita2d_font_draw_text(vita2dFont[15] , messageScrollX + 150, messageScrollY + i * 128 + 32, RGBA8(255,255,255,255), MESSAGE_AUTHOR_TEXT_SIZE_PIXEL, messageBoxes[i].username.c_str());
			//vita2d_font_draw_text(vita2dFont[30] , messageScrollX + 160, messageScrollY + i * 128 + 96, RGBA8(255,255,255,255), MESSAGE_CONTENT_TEXT_SIZE_PIXEL, messageBoxes[i].content.c_str());
		}
		
		//for(int emo = 0; emo < discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis.size() ; emo++){
		//	int in = discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[emo].index ;
		//	if(emojis[in].icon != NULL){
		//		if(emojis[in].id == discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[emo].codepoint){
		//			
		//			vita2d_draw_texture(emojis[in].icon , messageScrollX + 160 + MESSAGE_CONTENT_TEXT_SIZE_PIXEL * discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].emojis[emo].x , messageScrollY + i * 128 + 96);
		//			
		//		}
		//	}
		//}
		
		yPos += height; // add message height to yPos
		//COMMENT debugNetPrintf(DEBUG, "Cycle complete.\n");
	}

		
}


void VitaGUI::DrawDirectMessageUsersOnSidebar(){
	
	
	//COMMENT debugNetPrintf(DEBUG, "DrawDirectMessageUsers\n");
	int dmBoxes = static_cast<int>(directMessageBoxes.size());
	for(int i = 0 ; i < dmBoxes ; i++){
		if(directMessageScrollY + i * CHANNEL_HEIGHT < MAX_DRAW_HEIGHT && directMessageScrollY + i * CHANNEL_HEIGHT > MIN_DRAW_HEIGHT){
			
			if(discordPtr->currentDirectMessage == directMessageBoxes[i].dmIndex && discordPtr->inDirectMessageChannel){
				vita2d_draw_rectangle(directMessageScrollX , 100 + directMessageScrollY + i * CHANNEL_HEIGHT, 215 , CHANNEL_HEIGHT, RGBA8(40, 43, 48, 255));
				vita2d_draw_rectangle(directMessageScrollX , 100 + directMessageScrollY + i * CHANNEL_HEIGHT, 4 , CHANNEL_HEIGHT, RGBA8(95, 118, 198, 255));
			}
			
			//vita2d_draw_texture( guildsBGImage , directMessageScrollX + 128 , directMessageScrollY + i * 128);
			//vita2d_pgf_draw_text(pgf, directMessageScrollX + 256, directMessageScrollY + i * 128 + 96, RGBA8(255,255,255,255), 1.0f, discordPtr->guilds[discordPtr->currentGuild].channels[discordPtr->currentChannel].messages[i].content.c_str());

			vita2d_font_draw_text(vita2dFont[18] , directMessageScrollX + 8, 100 + directMessageScrollY + i * CHANNEL_HEIGHT + 42, RGBA8(255,255,255,255), CHANNEL_TITLE_TEXT_SIZE_PIXEL, directMessageBoxes[i].name.c_str());
			
		}
	}

	
}

void VitaGUI::DrawDirectMessageMessages(){

	
	int yPos = 0,height;
	unsigned int messageBoxesAmount = directMessageMessagesBoxes.size();

	//for(int i =  messageBoxes.size() ; i >= 0  ; i--){
	yPos = directMessageMessagesScrollY + 40;
	for(unsigned int i =  0 ; i < messageBoxesAmount ; i++){
		
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 1\n");
			height = directMessageMessagesBoxes[i].messageHeight;
		
		//COMMENT debugNetPrintf(DEBUG, "calculating %d of %d\n", i, messageBoxesAmount);
		if(yPos < MAX_DRAW_HEIGHT && yPos > MIN_DRAW_HEIGHT){
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 2\n");
			if(i == messageBoxesAmount-1){
				
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(120, 115, 120, 255)); // after last message
				vita2d_draw_rectangle(240, yPos + height , 710, 1, RGBA8(100, 100, 100, 255)); // 
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(120, 115, 120, 255)); // 
			}else{
				//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 2\n");
				vita2d_draw_rectangle(242, yPos + height - 1, 706, 1, RGBA8(62, 65, 70, 255)); // two small lines to outline the message panel
				//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 3\n");
				vita2d_draw_rectangle(240, yPos + height, 710, 1, RGBA8(51, 53, 55, 255)); // no need for a panel image
				vita2d_draw_rectangle(242, yPos + height + 1, 706, 1, RGBA8(62, 65, 70, 255)); // two small lines to outline the message panel
				
			}
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 4\n");
				vita2d_font_draw_text(vita2dFont[15], 283, yPos + 26, RGBA8(255, 255, 255, 255), 15, directMessageMessagesBoxes[i].username.c_str());
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 5\n");
				vita2d_font_draw_text(vita2dFont[15], 293, yPos + 50, RGBA8(255, 255, 255, 255), 15, directMessageMessagesBoxes[i].content.c_str());
			//COMMENT debugNetPrintf(DEBUG, "MESSAGE STEP 6\n");
		}

		
		yPos += height; // add message height to yPos
		//COMMENT debugNetPrintf(DEBUG, "Cycle complete.\n");
	}
	
}




