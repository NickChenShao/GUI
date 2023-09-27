
ifeq ($(OS),Windows_NT)
    CURR_DIR_PATH = $(CURDIR)
else
    CURR_DIR_PATH = $(shell pwd)
endif

GCC:=gcc
INC+=-I${CURR_DIR_PATH}/../
SRC+=${CURR_DIR_PATH}/../xmcore_menu_form.c

INC+=-I${CURR_DIR_PATH}/hmi/inc

SRC+=${CURR_DIR_PATH}/xmmcu_demo.c
SRC+=${CURR_DIR_PATH}/hmi/src/mainhmi.c
SRC+=${CURR_DIR_PATH}/hmi/src/hmi_camera.c
SRC+=${CURR_DIR_PATH}/hmi/src/hmi_music.c
SRC+=${CURR_DIR_PATH}/hmi/src/hmi_set.c
SRC+=${CURR_DIR_PATH}/hmi/src/hmi_more_set.c
SRC+=${CURR_DIR_PATH}/hmi/src/hmi_video.c


AIM_NAME:=xmmcu_demo

.PHONY:all
all:
	${GCC} ${INC} ${SRC} -o ${CURR_DIR_PATH}/${AIM_NAME} -std=c99

.PHONY:clean
clean:
	rm -rf ${CURR_DIR_PATH}/${AIM_NAME}