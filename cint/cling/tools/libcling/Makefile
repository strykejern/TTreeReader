##===- tools/libcling/Makefile -----------------------------*- Makefile -*-===##
# 
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
# 
##===----------------------------------------------------------------------===##

CLING_LEVEL = ../..
LIBRARYNAME = cling

EXPORTED_SYMBOL_FILE = $(PROJ_SRC_DIR)/libcling.exports

LINK_LIBS_IN_SHARED = 1
SHARED_LIBRARY = 1

LINK_COMPONENTS := analysis archive asmparser asmprinter bitreader bitwriter codegen core debuginfo engine executionengine instcombine instrumentation interpreter ipa ipo jit linker mc mcdisassembler mcjit mcparser native nativecodegen object runtimedyld scalaropts selectiondag support tablegen target transformutils
USEDLIBS = clingUserInterface.a clingMetaProcessor.a clingInterpreter.a clingUITextInput.a clangFrontend.a clangSerialization.a clangDriver.a clangCodeGen.a clangParse.a clangSema.a clangAnalysis.a clangRewrite.a clangAST.a clangLex.a clangBasic.a clangEdit.a LLVMSupport.a
LLVMLIBS = LLVMSupport.a
include $(CLING_LEVEL)/Makefile

# Inform the sources about their location
CPPFLAGS += -I$(PROJ_SRC_DIR)/../../include -I$(PROJ_SRC_DIR)/../../../clang/include \
-I$(PROJ_SRC_DIR)/../../../clang/lib -I$(PROJ_OBJ_DIR)/../../include \
-I$(PROJ_OBJ_DIR)/../../../clang/include \
'-DCLANG_SYS_HEADERS="-I$(PROJ_SRC_DIR)../../../clang/lib/Headers"' \
'-DCLING_INSTDIR_INCL="$(DESTDIR)$(PROJ_includedir)"' \
'-DCLING_SRCDIR_INCL="$(LLVM_SRC_ROOT)/tools/cling/include"'


##===----------------------------------------------------------------------===##
# FIXME: This is copied from the 'lto' makefile.  Should we share this?
##===----------------------------------------------------------------------===##

ifeq ($(HOST_OS),Darwin)
    LLVMLibsOptions += -Wl,-compatibility_version,1

    # Set dylib internal version number to submission number.
    ifdef LLVM_SUBMIT_VERSION
        LLVMLibsOptions += -Wl,-current_version \
                           -Wl,$(LLVM_SUBMIT_VERSION).$(LLVM_SUBMIT_SUBVERSION)
    endif

    # Extra options to override libtool defaults.
    LLVMLibsOptions += -Wl,-dead_strip -Wl,-seg1addr,0xE0000000 

    # Mac OS X 10.4 and earlier tools do not allow a second -install_name on command line
    DARWIN_VERS := $(shell echo $(TARGET_TRIPLE) | sed 's/.*darwin\([0-9]*\).*/\1/')
    ifneq ($(DARWIN_VERS),8)
       LLVMLibsOptions += -Wl,-install_name \
                          -Wl,"@rpath/lib$(LIBRARYNAME)$(SHLIBEXT)"
    endif
endif
