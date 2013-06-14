CXX := clang++
LLVMCOMPONENTS := cppbackend
RTTIFLAG := -fno-rtti
LLVMCONFIG := llvm-config

CXXFLAGS := $(RTTIFLAG) -D__STDC_LIMIT_MACROS=1 -D__STDC_CONSTANT_MACROS=1 -I"../llvm/include/" -I"../llvm/tools/clang/include/" -I"../build/tools/clang/include/"
LLVMLDFLAGS := -v -L"../build/Debug+Asserts/lib/" -L"../build/Debug+Asserts/lib/clang/3.4/lib/darwin/" -L"../build/tools/clang/lib/" #$(shell $(LLVMCONFIG) --ldflags --libs $(LLVMCOMPONENTS))

SOURCES = CIrewriter.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXES = $(OBJECTS:.o=)
CLANGLIBS = \
				-lclangTooling\
				-lclangFrontendTool\
				-lclangFrontend\
				-lclangDriver\
				-lclangSerialization\
				-lclangCodeGen\
				-lclangParse\
				-lclangSema\
				-lclangStaticAnalyzerFrontend\
				-lclangStaticAnalyzerCheckers\
				-lclangStaticAnalyzerCore\
				-lclangAnalysis\
				-lclangARCMigrate\
				-lclangRewriteFrontend\
				-lclangRewriteCore\
				-lclangEdit\
				-lclangAST\
				-lclangLex\
				-lclangBasic\
				-lLLVMTableGen -lLLVMMCJIT -lLLVMRuntimeDyld -lLLVMObject -lLLVMMCDisassembler -lLLVMLinker -lLLVMipo -lLLVMInterpreter -lLLVMInstrumentation -lLLVMJIT -lLLVMExecutionEngine -lLLVMDebugInfo -lLLVMBitWriter -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMX86Desc -lLLVMSelectionDAG -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMX86Info -lLLVMAsmPrinter -lLLVMMCParser -lLLVMCodeGen -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAsmParser -lLLVMArchive -lLLVMBitReader -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport
				

all: $(OBJECTS) $(EXES)

%: %.o
	$(CXX) -o $@ $< $(LLVMLDFLAGS) $(CLANGLIBS)

clean:
	-rm -f $(EXES) $(OBJECTS) *~
