//
//  clang_plugin.c
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include <stdio.h>
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/AST.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "MyASTConsumer.h"
#include "clang_setup_functions.h"

using namespace clang;

namespace {

class PrintFunctionNamesAction : public PluginASTAction {
protected:
    ASTConsumer *CreateASTConsumer(CompilerInstance &compiler, llvm::StringRef) {
        //setup_diagostics(compiler);
        //compiler.createFileManager();
        //compiler.createSourceManager(compiler.getFileManager());
        printf("%s\n", "run");
        Rewriter* rewriter = new Rewriter();
        rewriter->setSourceMgr(compiler.getSourceManager(), compiler.getLangOpts());
        //printf("%s\n", "created rewriter");
        llvm::errs() << "Hello world" << "\n";
        return new MyASTConsumer(*rewriter);
    }
    
    bool shouldEraseOutputFiles () {
        printf("don't erase");
        return false; }
    
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string>& args) {
        return true;
        //printf("%s\n", "prse args");
        for (unsigned i = 0, e = args.size(); i != e; ++i) {
            llvm::errs() << "PrintFunctionNames arg = " << args[i] << "\n";
            
            // Example error handling.
            if (args[i] == "-an-error") {
                DiagnosticsEngine &D = CI.getDiagnostics();
                unsigned DiagID = D.getCustomDiagID(
                                                    DiagnosticsEngine::Error, "invalid argument '" + args[i] + "'");
                D.Report(DiagID);
                return false;
            }
        }
        if (args.size() && args[0] == "help")
            PrintHelp(llvm::errs());
        
        return true;
    }
    void PrintHelp(llvm::raw_ostream& ros) {
        ros << "Help for PrintFunctionNames plugin goes here\n";
    }
    
};
}


static FrontendPluginRegistry::Add<PrintFunctionNamesAction>
X("ali", "instrument code");