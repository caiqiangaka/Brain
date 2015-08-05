
IPC_MSG_CLI_OBJ_DIR	= obj
IPC_MSG_CLI_SRC_DIR	= .
IPC_MSG_CLI_H	= .
IPC_MSG_CLI_LIB_DIR	= lib
IPC_MSG_CLI_LIB	= $(IPC_MSG_CLI_LIB_DIR)/libophy_ipcmsg.a
IPC_MSG_CLI_INC	= -I$(IPC_MSG_CLI_H)
#LD_LIBRARY = /usr/local/lib/
LD_LIBRARY = /usr/lib/
TARGET	= ipcCliMsg

############ compiler and utils ###################
#PREFIX=arm-none-linux-gnueabi-
#PREFIX=/opt/at91/Buildroot/buildroot-2011.05/output/host/usr/bin/arm-unknown-linux-uclibcgnueabi-
PREFIX=
CC=$(PREFIX)gcc
CCC=$(PREFIX)g++
AS=$(PREFIX)as
LD=$(CC)
AR=$(PREFIX)ar
CP=cp


IPC_MSG_CLI_OBJ    = $(IPC_MSG_CLI_OBJ_DIR)/communicate_lib.o	\
	$(IPC_MSG_CLI_OBJ_DIR)/udp_communicate_driver.o	\
	$(IPC_MSG_CLI_OBJ_DIR)/cJSON.o	\
	$(IPC_MSG_CLI_OBJ_DIR)/unix_communicate_driver.o \
	$(IPC_MSG_CLI_OBJ_DIR)/test_demo.o

TARGET_IPC_TEST_OBJ    = $(IPC_MSG_CLI_OBJ_DIR)/msg_client.o

############   output #############################
$(TARGET):$(IPC_MSG_CLI_LIB) $(TARGET_IPC_TEST_OBJ)
		$(CC) $(IPC_MSG_CLI_INC) $(TARGET_IPC_TEST_OBJ) $(IPC_MSG_CLI_LIB)  -o $@	

#############  rules for IPC Client msg lib files  #################
$(IPC_MSG_CLI_OBJ_DIR)/%.o : $(IPC_MSG_CLI_SRC_DIR)/%.c
	@echo "	Compiling IPC Client msg lib ......" $<
	@$(CC)	-c $(CFLAGS) $(IPC_MSG_CLI_INC) $< -o $@


############   IPC Client msg lib library          #################
$(IPC_MSG_CLI_LIB):	$(IPC_MSG_CLI_OBJ)
	@echo "	Linking IPC Client msg lib......"
	@$(AR) rc $@  $(IPC_MSG_CLI_OBJ)

install:
	@echo " Install IPC Client msg lib to library......"
	$(CP) $(IPC_MSG_CLI_LIB) $(LD_LIBRARY)

.PHONY: clean
clean:
	rm -f $(IPC_MSG_CLI_OBJ_DIR)/*.o
	rm -f $(IPC_MSG_CLI_LIB_DIR)/*.a
	rm -f $(TARGET)
