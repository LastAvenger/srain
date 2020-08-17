# Makefile
#
# Complie C source codes and output object files to $(BUILD_DIR), then links
# them together to get a executable $(TARGET).

ifeq "$(origin CC)" "default"
	CC = gcc
endif
DEFS = -D PACKAGE='"$(PACKAGE)"' \
	   -D PACKAGE_NAME='"$(PACKAGE_NAME)"' \
	   -D PACKAGE_VERSION='"$(PACKAGE_VERSION)"' \
	   -D PACKAGE_BUILD='"$(PACKAGE_BUILD)"' \
	   -D PACKAGE_APPID='"$(PACKAGE_APPID)"' \
	   -D PACKAGE_AUTHOR='"$(PACKAGE_AUTHOR)"' \
	   -D PACKAGE_EMAIL='"$(PACKAGE_EMAIL)"' \
	   -D PACKAGE_DESC='"$(PACKAGE_DESC)"' \
	   -D PACKAGE_WEBSITE='"$(PACKAGE_WEBSITE)"' \
	   -D PACKAGE_DATA_DIR='"$(PACKAGE_DATA_DIR)"' \
	   -D PACKAGE_CONFIG_DIR='"$(PACKAGE_CONFIG_DIR)"' \
	   -D PACKAGE_COPYRIGHT_DATES='"$(PACKAGE_COPYRIGHT_DATES)"'
GTK3FLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK3LIBS = $(shell pkg-config --libs gtk+-3.0)
LIBCONFIGFLAGS = $(shell pkg-config --cflags libconfig)
LIBCONFIGLIBS = $(shell pkg-config --libs libconfig)
LIBSOUPFLAGS = $(shell pkg-config --cflags libsoup-2.4)
LIBSOUPLIBS = $(shell pkg-config --libs libsoup-2.4)
LIBSSLFLAGS = $(shell pkg-config --cflags openssl)
LIBSSLLIBS = $(shell pkg-config --libs openssl)
LIBSECRETFLAGS =$(shell pkg-config --cflags libsecret-1)
LIBSECRETLIBS =$(shell pkg-config --libs libsecret-1)
LIBAPPINDICATORFLAGS = $(shell pkg-config --cflags appindicator3-0.1)
LIBAPPINDICATORLIBS = $(shell pkg-config --libs appindicator3-0.1)

CFLAGS += -std=gnu99 -O2 -Wall -Iinc -Wno-deprecated-declarations \
		  $(DEFS) \
		  $(DBGFLAGS) \
		  $(GTK3FLAGS) \
		  $(LIBCONFIGFLAGS) \
		  $(LIBSOUPFLAGS) \
		  $(LIBSSLFLAGS) \
		  $(LIBSECRETFLAGS) \
		  $(LIBAPPINDICATORFLAGS)

LDFLAGS += $(DBGFLAGS) \
		   $(GTK3LIBS) \
		   $(LIBCONFIGLIBS) \
		   $(LIBSOUPLIBS) \
		   $(LIBSSLLIBS) \
		   $(LIBSECRETLIBS) \
		   $(LIBAPPINDICATORLIBS)

ifeq ($(OS),Windows_NT)
LDFLAGS += -mwindows
endif

SRCS = $(wildcard *.c */*.c */*/*.c $(BUILD_DIR)/resources.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

default:
	$(MAKE) $(TARGET)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: */%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: */*/%.c
	$(CC) $(CFLAGS) -c $^ -o $@


$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)
