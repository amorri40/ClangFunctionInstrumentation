//
//  MyASTConsumer.h
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#ifndef __ClangInstrumentationPlugin__MyASTConsumer__
#define __ClangInstrumentationPlugin__MyASTConsumer__

#include <iostream>
#include "clang/AST/ASTConsumer.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "MyRecursiveASTVisitor.h"

/*
 Ast consumer loops over the AST and calls the RecursiveASTVisitor field for each declaration
 */
class MyASTConsumer : public clang::ASTConsumer
{
public:
    
    MyASTConsumer(clang::Rewriter &Rewrite, clang::SourceManager* sm) : rv(Rewrite,sm) { }
    virtual bool HandleTopLevelDecl(clang::DeclGroupRef d);
    
    MyRecursiveASTVisitor rv;
};

#endif /* defined(__ClangInstrumentationPlugin__MyASTConsumer__) */
