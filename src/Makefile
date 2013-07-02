# defined in environment variables
#DEBUG:=yes
MACOS:=yes

ifeq ($(MACOS),yes)
	CC:=clang++
else
	CC:=g++
endif

AR:=ar -r 
PROJECT_LOC:=..
WORKSPACE_LOC:=$(PROJECT_LOC)/..

TARGET_NAME:= libhts-core

ifeq ($(DEBUG),yes)
	CONFIG_NAME:=debug
	TARGET_NAME:= $(TARGET_NAME)d
	CFLAGS:=$(SFLAGS) -O0 -g3 -D_DEBUG
	#-std=c++11
else
	CONFIG_NAME:=release
	CFLAGS:=$(SFLAGS) -O3 -DNDEBUG
endif

#------------------------------------------------------------------------

TARGET_NAME:= $(TARGET_NAME).a
TARGET_DIR:= $(PROJECT_LOC)/lib
TARGET:= $(TARGET_DIR)/$(TARGET_NAME)

#------------------------------------------------------------------------

SRC_DIR:= $(PROJECT_LOC)/src
OBJ_DIR:= $(PROJECT_LOC)/.tmp/$(CONFIG_NAME)

SRC:= $(wildcard $(SRC_DIR)/*.cpp)
OBJS:= $(subst $(SRC_DIR)/, $(OBJ_DIR)/, $(SRC:.cpp=.o)) 
HEADERS:=  $(wildcard $(SRC_DIR)/*.hxx) $(wildcard $(SRC_DIR)/*.h) $(wildcard $(PROJECT_LOC)/include/hts-core/*.h)

#------------------------------------------------------------------------

INCLUDES:= -I$(WORKSPACE_LOC)/deps/include
LIBS:=$(WORKSPACE_LOC)/deps/lib

CFLAGS:= $(CFLAGS) -W -Wall $(INCLUDES)

#------------------------------------------------------------------------

all: $(TARGET)

distclean: clean
	rm -rf $(TARGET_DIR)

clean:
	rm -rf $(OBJ_DIR)

$(TARGET):  $(OBJS) 
	@mkdir -p $(TARGET_DIR)
	$(AR) $(TARGET) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) Makefile
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $< 
	

	