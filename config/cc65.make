cpu := 65c02

SRC_EXT_CXX := c

CXX.MKDEPS := $(CXX.MKDEPS)
CXX.MKDEPS.CXXFLAGS =
CXX.MKDEPS.CXXFLAGS += -I/usr/local/share/cc65/include
CXX.MKDEPS.CXXFLAGS += -Wno-deprecated

CXX = cl65
CXXFLAGS +=
CXXFLAGS += --target none
CXXFLAGS += --cpu $(cpu)

CXXFLAGS.OPTIMIZATIONS :=
CXXFLAGS.OPTIMIZATIONS += -O
CXXFLAGS.OPTIMIZATIONS += -Oi
CXXFLAGS.OPTIMIZATIONS += -Or
CXXFLAGS.OPTIMIZATIONS += -Os
# Note: make all locals static locals: ~15% performance improvement
CXXFLAGS.OPTIMIZATIONS += -Cl

CXXFLAGS += $(CXXFLAGS.OPTIMIZATIONS)

AS = ca65
ASFLAGS += --target none
ASFLAGS += --cpu $(cpu)

AR = ar65
AR.MKLIB = $(AR) r
