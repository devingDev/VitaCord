
#include <psp2/ctrl.h>
#include "VitaPad.hpp"
#include <math.h>

VitaPad::VitaPad(){
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
	sceCtrlPeekBufferPositive(0, &vitapad, 1);
	//left_analog_calibration_x = vitapad.lx;
	//left_analog_calibration_y = vitapad.ly;
}

VitaPad::VitaPad(bool initstuff){
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
	sceCtrlPeekBufferPositive(0, &vitapad, 1);
	//left_analog_calibration_x = vitapad.lx;
	//left_analog_calibration_y = vitapad.ly;
}


VitaPad::~VitaPad(){
	
}

void VitaPad::Read(){
	sceCtrlPeekBufferPositive(0, &vitapad, 1);
	
	select = start = up = right = down = left = lefttrigger = righttrigger = triangle = circle = cross = square = volup = voldown = false;
	left_analog_moving = right_analog_moving = false;
	left_analog_calibrated_x =   0;
	left_analog_calibrated_y =   0;
	right_analog_calibrated_x =  0;
	right_analog_calibrated_y =  0;
	
	if(vitapad.buttons & SCE_CTRL_SELECT)
		select = true;
	if(vitapad.buttons & SCE_CTRL_START)
		start = true;
	if(vitapad.buttons & SCE_CTRL_UP)
		up = true;
	if(vitapad.buttons & SCE_CTRL_RIGHT)
		right = true;
	if(vitapad.buttons & SCE_CTRL_DOWN)
		down = true;
	if(vitapad.buttons & SCE_CTRL_LEFT)
		left = true;
	if(vitapad.buttons & SCE_CTRL_LTRIGGER)
		lefttrigger = true;
	if(vitapad.buttons & SCE_CTRL_RTRIGGER)
		righttrigger = true;
	if(vitapad.buttons & SCE_CTRL_L1)
		lefttrigger = true;
	if(vitapad.buttons & SCE_CTRL_R1)
		righttrigger = true;
	if(vitapad.buttons & SCE_CTRL_TRIANGLE)
		triangle = true;
	if(vitapad.buttons & SCE_CTRL_CIRCLE)
		circle = true;
	if(vitapad.buttons & SCE_CTRL_CROSS)
		cross = true;
	if(vitapad.buttons & SCE_CTRL_SQUARE)
		square = true;	
	if(vitapad.buttons & SCE_CTRL_VOLUP)
		volup = true;
	if(vitapad.buttons & SCE_CTRL_VOLDOWN)
		voldown = true;
	
	lx = vitapad.lx;
	ly = vitapad.ly;
	rx = vitapad.rx;
	ry = vitapad.ry;
	 
	
	if(abs(vitapad.lx - left_analog_calibration_x) > DEADZONE){
		left_analog_calibrated_x =  static_cast<int>((vitapad.lx - left_analog_calibration_x)/24);
	}else{
		left_analog_calibrated_x = 0;
	}
	if(abs(vitapad.ly - left_analog_calibration_y) > DEADZONE){
		left_analog_calibrated_y =  static_cast<int>((vitapad.ly - left_analog_calibration_y)/24);
	}else{
		left_analog_calibrated_y = 0;
	}
	if(abs(vitapad.rx - right_analog_calibration_x) > DEADZONE){
		right_analog_calibrated_x =  static_cast<int>((vitapad.rx - right_analog_calibration_x)/24);
	}else{
		right_analog_calibrated_x = 0;
	}
	if(abs(vitapad.ry - right_analog_calibration_y) > DEADZONE){
		right_analog_calibrated_y =  static_cast<int>((vitapad.ry - right_analog_calibration_y) /24);
	}else{
		right_analog_calibrated_y = 0;
	}
	
	
	
	
}



