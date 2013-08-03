/***   CIrewriter.cpp   ******************************************************
 * This code is licensed under the New BSD license.
 * See LICENSE.txt for details.
 *
 * This tutorial was written by Robert Ankeney.
 * Send comments to rrankene@gmail.com.
 * 
 * This tutorial is an example of using the Clang Rewriter class coupled
 * with the RecursiveASTVisitor class to parse and modify C code.
 *
 * Expressions of the form:
 *     (expr1 && expr2)
 * are rewritten as:
 *     L_AND(expr1, expr2)
 * and expressions of the form:
 *     (expr1 || expr2)
 * are rewritten as:
 *     L_OR(expr1, expr2)
 *
 * Functions are located and a comment is placed before and after the function.
 *
 * Statements of the type:
 *   if (expr)
 *      xxx;
 *   else
 *      yyy;
 *
 * are converted to:
 *   if (expr)
 *   {
 *      xxx;
 *   }
 *   else
 *   {
 *      yyy;
 *   }
 *
 * And similarly for while and for statements.
 *
 * Interesting information is printed on stderr.
 *
 * Usage:
 * CIrewriter <options> <file>.c
 * where <options> allow for parameters to be passed to the preprocessor
 * such as -DFOO to define FOO.
 *
 * Generated as output <file>_out.c
 *
 * Note: This tutorial uses the CompilerInstance object which has as one of
 * its purposes to create commonly used Clang types.
 *****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <vector>
#include <sstream>

#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Frontend/Utils.h"

using namespace clang;
#include "clang_setup_functions.h"
#include "MyASTConsumer.h"

#include "ast_rewriting_functions.h"

/*
 Check if file of fileName exists and if not it exits this program
 */
void make_sure_file_exists(std::string fileName) {
  struct stat sb;

  // Make sure it exists
  if (stat(fileName.c_str(), &sb) == -1)
  {
    perror(fileName.c_str());
    exit(EXIT_FAILURE);
  }
}

CompilerInvocation* pass_flags_to_preprocessor(CompilerInstance& compiler, int argc, char **argv) {
  // Create an invocation that passes any flags to preprocessor
  CompilerInvocation *invocation = new CompilerInvocation;
  CompilerInvocation::CreateFromArgs(*invocation, argv + 1, argv + argc, compiler.getDiagnostics());
    //clang::createInvocationFromCommandLine(ArrayRef<const char *>(argv, argc));
    
  compiler.setInvocation(invocation);
  return invocation;
}

void set_default_target(CompilerInstance& compiler) {
  // Set default target triple
  llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions());
  pto->Triple = llvm::sys::getDefaultTargetTriple();
  llvm::IntrusiveRefCntPtr<TargetInfo>
     pti(TargetInfo::CreateTargetInfo(compiler.getDiagnostics(),
                                      pto.getPtr()));
  compiler.setTarget(pti.getPtr());
}
#include "clang/Basic/Version.h"
void setup_header_info(CompilerInstance& compiler) {
  HeaderSearchOptions &headerSearchOptions = compiler.getHeaderSearchOpts();
  headerSearchOptions.ResourceDir = LLVM_PREFIX "/lib/clang/" CLANG_VERSION_STRING;
  // <Warning!!> -- Platform Specific Code lives here
  // This depends on A) that you're running linux and
  // B) that you have the same GCC LIBs installed that
  // I do.
  // Search through Clang itself for something like this,
  // go on, you won't find it. The reason why is Clang
  // has its own versions of std* which are installed under
  // /usr/local/lib/clang/<version>/include/
  // See somewhere around Driver.cpp:77 to see Clang adding
  // its version of the headers to its include path.
  // To see what include paths need to be here, try
  // clang -v -c test.c
  // or clang++ for C++ paths as used below:
    headerSearchOptions.AddPath("/Users/alasdairmorrison/Dropbox/projects/clangparsing/build/debug+asserts/lib/clang/3.4/include", //was 4.6
                                clang::frontend::System,//clang::frontend::Angled,
                                false,
                                true);
    
    headerSearchOptions.AddPath("/Applications/Xcode5-DP3.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/c++/v1", //added by me
                                clang::frontend::Angled,
                                false,
                                false);
    
  /*headerSearchOptions.AddPath("/usr/include/c++/4.2.1", //was 4.6
          clang::frontend::Angled,
          false,
          false);*/
    
  
  /*headerSearchOptions.AddPath("/usr/include/c++/4.2.1/backward", //was 4.6
          clang::frontend::Angled,
          false,
          false);*/
  /*headerSearchOptions.AddPath("/usr/local/include",
          clang::frontend::Angled,
          false,
          false);*/
  /*headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include",
          clang::frontend::Angled,
          false,
          false);*//*
  headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",
          clang::frontend::Angled,
          false,
          false);*/
  /*headerSearchOptions.AddPath("/usr/include",
          clang::frontend::Angled,
          false,
          false);*/
  headerSearchOptions.AddPath("/Users/alasdairmorrison/Dropbox/projects/clangparsing/build/debug+asserts/lib/clang/3.4/include/",
                                clang::frontend::Angled,
                                true,
                                true);
    //headerSearchOptions.
  // </Warning!!> -- End of Platform Specific Code
}

void setup_language_options_cxx(CompilerInvocation* Invocation) {
  // Allow C++ code to get rewritten
    LangOptions langOpts;
    //langOpts.GNUMode = 1;
    //langOpts.CXXExceptions = 1;
    langOpts.RTTI = 1; 
    langOpts.Bool = 1; 
    langOpts.CPlusPlus = 1;
    langOpts.CPlusPlus11 = 1;
    
    Invocation->setLangDefaults(langOpts,
                                clang::IK_CXX,
                                clang::LangStandard::lang_c11);
    clang::HeaderSearchOptions hso = Invocation->getHeaderSearchOpts();
    //hso.UseStandardCXXIncludes = 1;
    //hso.UseBuiltinIncludes = 1;
    //hso.Verbose = 1;
    //hso.AddPath(<#llvm::StringRef Path#>, <#frontend::IncludeDirGroup Group#>, <#bool IsFramework#>, <#bool IgnoreSysRoot#>)
    std::cout << hso.Sysroot;
}

/*
 Load fileName into the compiler object
 */
void load_cxx_file(CompilerInstance& compiler, std::string fileName) {
  const FileEntry *pFile = compiler.getFileManager().getFile(fileName);
  compiler.getSourceManager().createMainFileID(pFile);
  Preprocessor& preprocessor = compiler.getPreprocessor();
    
    preprocessor.getBuiltinInfo().InitializeBuiltins(preprocessor.getIdentifierTable(),
                                           preprocessor.getLangOpts());
  printf("%s\n", "before begin source file ");
  compiler.getDiagnosticClient().BeginSourceFile(compiler.getLangOpts(),
                                               &preprocessor);
}

/*
 Returns the output filename for file, normally appends a string such as _out before the .cxx extension
 */
std::string get_outputfilename_for_filename(std::string fileName) {
  // Convert <file>.c to <file_out>.c
    std::string outName (fileName);
    size_t ext = outName.rfind(".");
    if (ext == std::string::npos)
       ext = outName.length();
    //outName.insert(ext, "_out");
    outName.append(".debug");
    return outName;
}
//

int run_proper_clang(int argc, char **argv, bool link) {
    std::ostringstream oss;
    if (!link)
    oss << "clang++ -xc++ -I/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime ";
    else
        oss << "clang++ ";
    for (int i=1; i<argc; i++) { //don't include argument0 as it is aliclang executable path
        std::string argument = argv[i];
        replaceAll(argument,".cpp",".cpp.debug");
        //replaceAll(argument,".o",".o.debug");
        oss << argument << " ";
    }
    
    if (link) {
        int compile_status = system("clang++ -c \"/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.cpp\" -o \"/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.o\"");
        compile_status = system("clang -c \"/Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/sqlite3.c\"");
        oss << " /Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/log_functions.o /Users/alasdairmorrison/Dropbox/projects/clangparsing/ClangInstrumentationPlugin/Runtime/sqlite3.o";
    }

    
    llvm::errs() << "about to run clang :" << oss.str().c_str() << "\n";
    int compile_status = system(oss.str().c_str());
    //int compile_status = execlp(oss.str().c_str(),NULL);
    llvm::errs() << "==================================== \n\n";
    return compile_status;
    
    //int compile_status = execv("clang", argv);
    //char* arguments[] = {"-v"};
    //int compile_status = execvp("clang", arguments);
    
    
    //printf("%s %d\n", "Running proper clang: ", compile_status);
}

int main(int argc, char **argv)
{
    //std::cout << "start of program";

  if (argc < 2)
  {
     llvm::errs() << "Usage: CIrewriter <options> <filename>\n";
     return 1;
  }
  

  CompilerInstance compiler;
    
  setup_diagostics(compiler);
    
  setup_header_info(compiler);
  CompilerInvocation *Invocation = pass_flags_to_preprocessor(compiler, argc, argv);
    
  set_default_target(compiler);

  compiler.createFileManager();
  
  compiler.createSourceManager(compiler.getFileManager());

  setup_header_info(compiler);

  setup_language_options_cxx(Invocation);

  compiler.createPreprocessor();
    

  compiler.createASTContext();

  // Initialize rewriter
  Rewriter Rewrite;
  Rewrite.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
    
    clang::FrontendOptions fo = compiler.getFrontendOpts();
    
    std::string fileName = fo.Inputs[0].getFile(); //could loop over multiple input files
    fo.FixAndRecompile = 1;
    
    bool outputfile = fileName.substr(fileName.find_last_of(".") + 1) == "o";
    if (outputfile) return run_proper_clang(argc,argv, true); //link files
    

    make_sure_file_exists(fileName);

  load_cxx_file(compiler, fileName);
  printf("%s %s\n", "Opened input file",fileName.c_str());

  

  std::string outName = get_outputfilename_for_filename(fileName);

  llvm::errs() << "Output to: " << outName << "\n";
  std::string OutErrorInfo;
  llvm::raw_fd_ostream outFile(outName.c_str(), OutErrorInfo, 0);

  if (!OutErrorInfo.empty()) {
     llvm::errs() << "Cannot open " << outName << " for writing\n";
  }
  

    MyASTConsumer astConsumer(Rewrite,&compiler.getSourceManager()); // create the ast consumer
    
    // Parse the AST wtith our astConsumer
    ParseAST(compiler.getPreprocessor(), &astConsumer, compiler.getASTContext());
    
    // Output some #ifdefs
    outFile << "//Debug file auto generated from clanginstrumentation \n";
    outFile << "extern void start_log_function(); extern void end_log_function(); \n#include <log_functions.h> \n";

    // Now output rewritten source code
    const RewriteBuffer *RewriteBuf =
      Rewrite.getRewriteBufferFor(compiler.getSourceManager().getMainFileID());
    if (RewriteBuf == NULL) {
        printf("No changes to the file required %s","\n");
        std::string normal_file = compiler.getSourceManager().getMemoryBufferForFile(compiler.getSourceManager().getFileEntryForID(compiler.getSourceManager().getMainFileID()))->getBuffer();
        outFile << normal_file;
    }
    else outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
      //printf("%s\n", "After getRewriteBufferFor");
    
    
    
    

  outFile.close();
  
    return run_proper_clang(argc,argv, false);

  //return 0;
}

