LLVM_SRC_PATH := /home/yzg/llvm/include
LLVM_BUILD_PATH := /home/yzg/llvm
LLVM_BIN_PATH 	:= $(LLVM_BUILD_PATH)/bin

$(info -----------------------------------------------)
$(info Using LLVM_SRC_PATH = $(LLVM_SRC_PATH))
$(info Using LLVM_BUILD_PATH = $(LLVM_BUILD_PATH))
$(info Using LLVM_BIN_PATH = $(LLVM_BIN_PATH))
$(info -----------------------------------------------)

CXX := /home/yzg/llvm/bin/clang++
CXXFLAGS := -fno-rtti -O0 -g
PLUGIN_CXXFLAGS := -fpic

LLVM_CXXFLAGS := `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS := `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`
LLVM_LDFLAGS_NOLIBS := `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS := -shared

CLANG_INCLUDES := \
	-I$(LLVM_SRC_PATH)/include/clang \
	-I$(LLVM_BUILD_PATH)/include/clang

CLANG_LIBS := \
	-Wl,--start-group \
	-lclangAST \
	-lclangASTMatchers \
	-lclangAnalysis \
	-lclangBasic \
	-lclangDriver \
	-lclangFrontend \
	-lclangFrontendTool \
	-lclangLex \
	-lclangParse \
	-lclangSema \
	-lclangEdit \
	-lclangRewrite \
	-lclangRewriteFrontend \
	-lclangIndex \
	-lclangSerialization \
	-Wl,--end-group


DIR_SRCS := src
BUILDDIR := build

.PHONY: all
all:$(BUILDDIR)/myASTVisitor
	

$(BUILDDIR)/myASTVisitor: $(DIR_SRCS)/myASTVisitor.cpp $(DIR_SRCS)/PointerChecker/pointerChecker.cpp $(DIR_SRCS)/SwitchChecker/switchChecker.cpp $(DIR_SRCS)/SpaceChecker/spaceChecker.cpp $(DIR_SRCS)/SwitchChecker/switchChecker.cpp $(DIR_SRCS)/SpaceChecker/spaceChecker.cpp $(DIR_SRCS)/SwitchChecker/switchChecker.cpp $(DIR_SRCS)/SpaceChecker/spaceChecker.cpp $(DIR_SRCS)/FunctionChecker/functionChecker.cpp $(DIR_SRCS)/SlowMemoryChecker/slowMemoryChecker.cpp 
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(CLANG_INCLUDES) $^ \
		$(CLANG_LIBS) $(LLVM_LDFLAGS) -o $@

