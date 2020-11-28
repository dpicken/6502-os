cpu := 65c02

SRC_EXT_CXX := c

CXX.MKDEPS := $(CXX.MKDEPS)
CXX.MKDEPS.CXXFLAGS=

CXX = cl65
CXXFLAGS +=
CXXFLAGS += --target none
CXXFLAGS += --cpu $(cpu)

AS = ca65
ASFLAGS += --target none
ASFLAGS += --cpu $(cpu)

AR = ar65
AR.MKLIB = $(AR) r
