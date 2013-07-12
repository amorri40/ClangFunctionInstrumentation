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

using namespace clang;
#include "clang_setup_functions.h"
#include "MyASTConsumer.h"

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
  CompilerInvocation::CreateFromArgs(*invocation, argv + 1, argv + argc,
                                      compiler.getDiagnostics());
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

void setup_header_info(CompilerInstance& compiler) {
  HeaderSearchOptions &headerSearchOptions = compiler.getHeaderSearchOpts();

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
  headerSearchOptions.AddPath("/usr/include/c++/4.6",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/include/c++/4.6/i686-linux-gnu",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/include/c++/4.6/backward",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/local/include",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/local/lib/clang/3.3/include",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/include/i386-linux-gnu",
          clang::frontend::Angled,
          false,
          false);
  headerSearchOptions.AddPath("/usr/include",
          clang::frontend::Angled,
          false,
          false);
  // </Warning!!> -- End of Platform Specific Code
}

void setup_language_options_cxx(CompilerInvocation* Invocation) {
  // Allow C++ code to get rewritten
    LangOptions langOpts;
    langOpts.GNUMode = 1; 
    langOpts.CXXExceptions = 1; 
    langOpts.RTTI = 1; 
    langOpts.Bool = 1; 
    langOpts.CPlusPlus = 1; 
    Invocation->setLangDefaults(langOpts,
                                clang::IK_CXX,
                                clang::LangStandard::lang_cxx0x);
}

/*
 Load fileName into the compiler object
 */
void load_cxx_file(CompilerInstance& compiler, std::string fileName) {
  const FileEntry *pFile = compiler.getFileManager().getFile(fileName);
  compiler.getSourceManager().createMainFileID(pFile);
  Preprocessor& preprocessor = compiler.getPreprocessor();
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
    outName.insert(ext, "_out");
    return outName;
}
//

int main(int argc, char **argv)
{
  

  if (argc < 2)
  {
     llvm::errs() << "Usage: CIrewriter <options> <filename>\n";
     return 1;
  }

  // Get filename
  std::string fileName(argv[argc - 1]);
  make_sure_file_exists(fileName);
  

  CompilerInstance compiler;
  setup_diagostics(compiler);

  CompilerInvocation *Invocation = pass_flags_to_preprocessor(compiler, argc, argv);

  set_default_target(compiler);

  compiler.createFileManager();
  compiler.createSourceManager(compiler.getFileManager());

  setup_header_info(compiler);

  setup_language_options_cxx(Invocation);
  printf("%s\n", "setup language options");

  compiler.createPreprocessor();
  compiler.getPreprocessorOpts().UsePredefines = false;

  compiler.createASTContext();
  printf("%s\n", "created AST Context");

  // Initialize rewriter
  Rewriter Rewrite;
  Rewrite.setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
  printf("%s\n", "created rewriter");

  load_cxx_file(compiler, fileName);
  printf("%s\n", "Opened input file");

  

  std::string outName = get_outputfilename_for_filename(fileName);

  llvm::errs() << "Output to: " << outName << "\n";
  std::string OutErrorInfo;
  llvm::raw_fd_ostream outFile(outName.c_str(), OutErrorInfo, 0);

  if (!OutErrorInfo.empty()) {
     llvm::errs() << "Cannot open " << outName << " for writing\n";
  }
  printf("%s\n", "Opened output file");

    MyASTConsumer astConsumer(Rewrite); // create the ast consumer
    // Parse the AST wtith out astConsumer
    ParseAST(compiler.getPreprocessor(), &astConsumer, compiler.getASTContext());
    printf("%s\n", "After parseAST");
    //compiler.getDiagnosticClient().EndSourceFile();

    printf("%s\n", "before write to start of file");
    // Output some #ifdefs
    outFile << "#define L_AND(a, b) a && b\n";
    outFile << "#define L_OR(a, b) a || b\n\n";

    // Now output rewritten source code
    const RewriteBuffer *RewriteBuf =
      Rewrite.getRewriteBufferFor(compiler.getSourceManager().getMainFileID());
      printf("%s\n", "After getRewriteBufferFor");
    outFile << std::string(RewriteBuf->begin(), RewriteBuf->end());
    

  outFile.close();
  
    int compile_status = execv("clang", argv);
    printf("%s %d\n", "End of program", compile_status);

  return 0;
}

