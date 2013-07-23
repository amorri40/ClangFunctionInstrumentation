//
//  MyRecursiveASTVisitor.h
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#ifndef __ClangInstrumentationPlugin__MyRecursiveASTVisitor__
#define __ClangInstrumentationPlugin__MyRecursiveASTVisitor__

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
using namespace clang;

// RecursiveASTVisitor is is the big-kahuna visitor that traverses
// everything in the AST.
class MyRecursiveASTVisitor
: public RecursiveASTVisitor<MyRecursiveASTVisitor>
{
    
public:
    MyRecursiveASTVisitor(Rewriter &R, SourceManager* sm) : rewriter(R), sourceManager(sm) { }
    //void InstrumentStmt(Stmt *s);
    //bool VisitStmt(Stmt *s);
    bool VisitFunctionDecl(FunctionDecl *f);
    //Expr *VisitBinaryOperator(BinaryOperator *op);
    
    Rewriter &rewriter;
    SourceManager* sourceManager;
};

#endif /* defined(__ClangInstrumentationPlugin__MyRecursiveASTVisitor__) */
