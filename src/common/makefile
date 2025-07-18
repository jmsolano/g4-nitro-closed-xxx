TOP      = $(shell pwd)

# OS Name (Linux or Darwin)
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# Flags to detect 32-bit or 64-bit OS platform
OS_SIZE = $(shell uname -m | sed -e "s/i.86/32/" -e "s/x86_64/64/")
OS_ARCH = $(shell uname -m | sed -e "s/i386/i686/")

# Flags to detect either a Linux system (linux) or Mac OSX (darwin)
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

# OS-specific build flags
ifneq ($(DARWIN),)
    CXXFLAGS   := -arch $(OS_ARCH)
else
  ifeq ($(OS_SIZE),32)
    CXXFLAGS   := -m32
  else
    CXXFLAGS   := -m64
  endif
endif

MYMAKEFLAGS := 

# COMPILERS OPTIONS
ifneq ($(DARWIN),)
  CXX          := g++-12
else
  CXX          := g++
endif

CXXFLAGS       += -std=c++11 -fPIC -Wall -pedantic
OPTIMFLAGS      = -O2 -funroll-loops -falign-loops=8 #-ffast-math

ARCHIVE      := ar
ARCHFLAG     := -rc

# Debug build flags
DEBUGVERSION := 0
ifeq ($(DEBUGVERSION),1)
  CXXFLAGS     += -DDEBUG=1 -ggdb -W -Wno-long-long
  MYMAKEFLAGS += DEBUGVERSION=1
else
   ifeq ($(PROFILEVERSION),1)
      CXXFLAGS     += -DDEBUG=1 -g -pg -Wall -pedantic $(OPTIMFLAGS)
      MYMAKEFLAGS += PROFILEVERSION=1
   else
      CXXFLAGS     += -DDEBUG=0 -Wall -pedantic $(OPTIMFLAGS)
   endif
endif

INCDEFS        := -include globaldefs.h

# FILES
CPPOBJS=$(patsubst %.cpp,%.o,$(wildcard *.cpp))
OBJS= $(CPPOBJS)
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.cpp)
LIBOBJS=$(shell echo $(OBJS))
COMMONSTATICLIBNAME=$(shell cd ../..; pwd | sed -e 's;\(.*\)/\(.*\);\2;')
STATICLIB="lib$(COMMONSTATICLIBNAME).a"

# -L: FOLDER LIBRARY
	LFLAGS+=#-L/usr/lib
	LFLAGS+=#-L.

# -l: LIBRARY
	lLIBS+=#-lgfnl_util
	lLIBS+= #-lapr-1 -laprutil-1 -llog4cxx

# -I: INCLUDES
	IFLAGS+= $(INCDEFS)

TARGET=target

$(TARGET): globaldefs.h $(OBJS) $(HEADERS) $(SOURCES)
	@echo "\033[32mArchiving $(STATICLIB)\\033[m"
	@$(ARCHIVE) $(ARFLAGS) $(STATICLIB) $(LIBOBJS)

$(CPPOBJS): %.o: %.cpp
	@echo "\033[32m   Compiling $@\\033[m"
	$(CXX) $(CXXFLAGS) $(IFLAGS) -c $<  -o $@  

.PHONY: clean
clean:
	$(info CLEANING ALL)
	@$(RM) -f $(TARGET) $(OBJS) $(STATICLIB) 2>/dev/null || true


globaldefs.h: 
	@echo "//Here you can define the macros that are used WITHIN"  > $@
	@echo "//this directory. Notice that the macros defined here" >> $@
	@echo "//affect ALL and every single *cc and *cpp source." >> $@
	@echo "#ifndef _GLOBALDEFS_H_" >> $@
	@echo "#define _GLOBALDEFS_H_" >> $@
	@echo "" >> $@
	@echo "#endif /* _GLOBALDEFS_H_ */" >> $@


list:
	@echo $(OBJS)
	@echo $(HEADERS)

listobjs:
	@echo $(LIBOBJS)

