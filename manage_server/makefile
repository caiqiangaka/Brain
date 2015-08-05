

CORE_OBJ_DIR	= obj
CORE_SRC_DIR	= core
CORE_H			= h
CORE_LIB_DIR	= lib

CORE_LIB	= $(CORE_LIB_DIR)/core_msg.lib
CORE_INC	= -I$(CORE_H)
TARGET		= manage_server


##############   chose product  ###################
ENABLE_IBCP_REV_CHECK     ?= true
CORSS_COMPILER			  ?= X86


ifeq ($(CORSS_COMPILER),ARM)
PREFIX=/opt/at91/Buildroot/buildroot-2011.05/output/host/usr/bin/arm-unknown-linux-uclibcgnueabi-
endif

ifeq ($(CORSS_COMPILER),X86)
PREFIX=
endif



############ compiler and utils ###################
CC=$(PREFIX)gcc
CCC=$(PREFIX)g++
AS=$(PREFIX)as
LD=$(CC)
AR=$(PREFIX)ar


###################################################
#CFLAGS  = -Wall -Os -fshort-enums
CFLAGS  = 
GDB		= 

ifeq ($(ENABLE_IBCP_REV_CHECK),true)
CFLAGS	+= -DIBCP_REV_CKECK_SEL
endif	

CORE_OBJ    = $(CORE_OBJ_DIR)/host_cmd_if_lib.o	\
	$(CORE_OBJ_DIR)/host_command.o	\
	$(CORE_OBJ_DIR)/msg_proto_lib.o		\
	$(CORE_OBJ_DIR)/network_commun.o		\
	$(CORE_OBJ_DIR)/unix_domain_socket.o	\
	$(CORE_OBJ_DIR)/socket_udp.o	\
	$(CORE_OBJ_DIR)/system_level.o \
	$(CORE_OBJ_DIR)/poll_msg_if_lib.o	\
	$(CORE_OBJ_DIR)/poll_command.o	\
	$(CORE_OBJ_DIR)/type_conversion_cmd.o  \
	$(CORE_OBJ_DIR)/console.o	\
	$(CORE_OBJ_DIR)/logs.o	\
	$(CORE_OBJ_DIR)/usr_module.o	\
	$(CORE_OBJ_DIR)/msg_service.o	\
	$(CORE_OBJ_DIR)/cJSON.o	\
	$(CORE_OBJ_DIR)/get_pid_lib.o	\
	$(CORE_OBJ_DIR)/config_module.o	\
	$(CORE_OBJ_DIR)/uart_lib.o	\
	$(CORE_OBJ_DIR)/crc.o	\
	$(CORE_OBJ_DIR)/task.o \
	$(CORE_OBJ_DIR)/test_demo.o

############   output #############################
$(TARGET):$(CORE_LIB)
	$(CC) -static $(CORE_LIB) -lpthread -lm -lcrypt -o $@

#############   include and obj ###################
ALL_INC = $(CORE_INC)
ALL_OBJ = $(CORE_OBJ)	


#############  rules for core msg files  ##########
$(CORE_OBJ_DIR)/%.o : $(CORE_SRC_DIR)/%.c
	@echo "	Compiling core msg management ......" $<
	@$(CC)	$(GDB) -c $(CFLAGS) $(ALL_INC)  $< -o $@
	
############   core msg library          ##############
$(CORE_LIB):	$(ALL_OBJ)
	@echo "	Linking core msg management......"
	@$(AR) rc $@  $(ALL_OBJ)

help:
	@echo '      CORSS_COMPILER            -------- Corss Compiler  default:X86 	CORSS_COMPILER=(X86|ARM)'
	@echo '      ENABLE_IBCP_REV_CHECK     -------- Enable recv check default:true ENABLE_IBCP_REV_CHECK=(true|false)'

.PHONY: clean
clean:
	-rm -f $(CORE_OBJ_DIR)/*.o
	-rm -f $(CORE_LIB_DIR)/*.lib
	-rm -f $(TARGET)
