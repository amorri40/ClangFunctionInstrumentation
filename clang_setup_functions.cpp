//
//  clang_setup_functions.cpp
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include "clang_setup_functions.h"
using namespace clang;

void setup_diagostics(CompilerInstance& compiler) {
    DiagnosticOptions* diagnosticOptions = new DiagnosticOptions();
    
    TextDiagnosticPrinter *pTextDiagnosticPrinter =
    new TextDiagnosticPrinter(
                              llvm::outs(),
                              diagnosticOptions,
                              false);
    
    compiler.createDiagnostics(pTextDiagnosticPrinter);
}