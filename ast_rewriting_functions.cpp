//
//  ast_rewriting_functions.c
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include <stdio.h>
#include "clang/Lex/Lexer.h"

#include "MyRecursiveASTVisitor.h"


int get_length_of_token_at_location(clang::SourceLocation Loc, Rewriter &rewriter) {
    return clang::Lexer::MeasureTokenLength(
                                            Loc, rewriter.getSourceMgr(), rewriter.getLangOpts());
}

bool replace_text_at_location(Rewriter &rewriter, clang::SourceLocation start, int end, char* new_text) {
    return rewriter.ReplaceText(start, end, new_text);
}

std::string get_return_type_from_function_declaration(FunctionDecl *f) {
    // Make a stab at determining return type
    // Getting actual return type is trickier
    QualType q = f->getResultType();
    const Type *typ = q.getTypePtr();
    
    std::string ret;
    if (typ->isVoidType())
        ret = "void";
    else
        if (typ->isIntegerType())
            ret = "integer";
        else
            if (typ->isCharType())
                ret = "char";
            else
                ret = "Other";
    return ret;
}