cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

set(SHORT_NAME vita_cord)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "VitaCord")
set(VITA_TITLEID  "VTCD00001")
set(VITA_VERSION  "01.05")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive -std=gnu11 -Wall ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -std=c++11 -Wall ")


find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(
  ../common
)

add_executable(${SHORT_NAME}.elf
  src/VitaGUI.cpp
  src/Discord.cpp
  src/easyencryptor.cpp
  src/DiscordApp.cpp
  src/VitaNet.cpp
  src/main.cpp
  src/log.cpp
  src/VitaIME.cpp
  src/VitaPad.cpp
  src/VitaTouch.cpp
)

target_link_libraries(${SHORT_NAME}.elf


  debugnet
  vita2d
  SceNet_stub
  ScePgf_stub
  ScePvf_stub
  SceGxm_stub
  SceHttp_stub
  SceCtrl_stub
  SceTouch_stub
  SceNetCtl_stub
  SceAppUtil_stub
  SceDisplay_stub
  SceLibKernel_stub
  SceSysmodule_stub
  ScePower_stub
  SceCommonDialog_stub
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  c
  m
  z
  png
  jpeg
  pthread
  freetype
)


vita_create_self(${SHORT_NAME}.self ${SHORT_NAME}.elf)

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/frame1.png sce_sys/livearea/contents/frame1.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE assets/font/droidsans.ttf assets/font/droidsans.ttf
  FILE assets/images/Vitacord-Background-8BIT.png assets/images/Vitacord-Background-8BIT.png
  FILE assets/images/Vitacord-DMIcon-8BIT.png assets/images/Vitacord-DMIcon-8BIT.png
  FILE assets/images/Vitacord-GuildsBG-8BIT.png assets/images/Vitacord-GuildsBG-8BIT.png
  FILE assets/images/Vitacord-Loading-8BIT.png assets/images/Vitacord-Loading-8BIT.png
  FILE assets/images/Vitacord-LoginForm-8BIT.png assets/images/Vitacord-LoginForm-8BIT.png
  FILE assets/images/Vitacord-messager-input.png assets/images/Vitacord-messager-input.png
  FILE assets/images/Vitacord-sidebar-default-usericon.png assets/images/Vitacord-sidebar-default-usericon.png
  FILE assets/images/Vitacord-statbar-icon.png assets/images/Vitacord-statbar-icon.png
  FILE assets/images/loading/thumb001.png assets/images/loading/thumb001.png
  FILE assets/images/loading/thumb002.png assets/images/loading/thumb002.png
  FILE assets/images/loading/thumb003.png assets/images/loading/thumb003.png
  FILE assets/images/loading/thumb004.png assets/images/loading/thumb004.png
  FILE assets/images/loading/thumb005.png assets/images/loading/thumb005.png
  FILE assets/images/loading/thumb006.png assets/images/loading/thumb006.png
  FILE assets/images/loading/thumb007.png assets/images/loading/thumb007.png
  FILE assets/images/loading/thumb008.png assets/images/loading/thumb008.png
  FILE assets/images/loading/thumb009.png assets/images/loading/thumb009.png
  FILE assets/images/loading/thumb010.png assets/images/loading/thumb010.png
  FILE assets/images/loading/thumb011.png assets/images/loading/thumb011.png
  FILE assets/images/loading/thumb012.png assets/images/loading/thumb012.png
  FILE assets/images/loading/thumb013.png assets/images/loading/thumb013.png
  FILE assets/images/loading/thumb014.png assets/images/loading/thumb014.png
  FILE assets/images/loading/thumb015.png assets/images/loading/thumb015.png
  FILE assets/images/loading/thumb016.png assets/images/loading/thumb016.png
  FILE assets/images/loading/thumb017.png assets/images/loading/thumb017.png
  FILE assets/images/loading/thumb018.png assets/images/loading/thumb018.png
  FILE assets/images/loading/thumb019.png assets/images/loading/thumb019.png
  FILE assets/images/loading/thumb020.png assets/images/loading/thumb020.png
  FILE assets/images/loading/thumb021.png assets/images/loading/thumb021.png
  FILE assets/images/loading/thumb022.png assets/images/loading/thumb022.png
  FILE assets/images/loading/thumb023.png assets/images/loading/thumb023.png
  FILE assets/images/loading/thumb024.png assets/images/loading/thumb024.png
  FILE assets/images/loading/thumb025.png assets/images/loading/thumb025.png
  FILE assets/images/loading/thumb026.png assets/images/loading/thumb026.png
  FILE assets/images/loading/thumb027.png assets/images/loading/thumb027.png
  FILE assets/images/loading/thumb028.png assets/images/loading/thumb028.png
  FILE assets/images/loading/thumb029.png assets/images/loading/thumb029.png
  FILE assets/images/loading/thumb030.png assets/images/loading/thumb030.png
  FILE assets/images/loading/thumb031.png assets/images/loading/thumb031.png
  FILE assets/images/loading/thumb032.png assets/images/loading/thumb032.png
  FILE assets/images/loading/thumb033.png assets/images/loading/thumb033.png
  FILE assets/images/loading/thumb034.png assets/images/loading/thumb034.png
  FILE assets/images/Vitacord-statbar-battery.png assets/images/Vitacord-statbar-battery.png
  FILE assets/images/Vitacord-statbar-battery-charge.png assets/images/Vitacord-statbar-battery-charge.png
)

