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

using namespace clang;

namespace {

class PrintFunctionNamesAction : public PluginASTAction {
protected:
    ASTConsumer *CreateASTConsumer(CompilerInstance &CI, llvm::StringRef) {
        printf("%s\n", "run");
        Rewriter rewriter;
        rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        printf("%s\n", "created rewriter");
        
        return new MyASTConsumer(rewriter);
    }
    
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string>& args) {
        printf("%s\n", "prse args");
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