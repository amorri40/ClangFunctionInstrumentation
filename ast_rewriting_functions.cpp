//
//  ast_rewriting_functions.c
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include <stdio.h>
#include "clang/Lex/Lexer.h"
#include "clang/Basic/SourceManager.h"

#include "MyRecursiveASTVisitor.h"

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int get_length_of_token_at_location(clang::SourceLocation Loc, Rewriter &rewriter) {
    return clang::Lexer::MeasureTokenLength(
                                            Loc, rewriter.getSourceMgr(), rewriter.getLangOpts());
}

bool replace_text_at_location(Rewriter &rewriter, clang::SourceLocation start, int end, const char* new_text) {
    return rewriter.ReplaceText(start, end, new_text);
}

/*std::string get_location_to_string(Rewriter &rewriter, clang::SourceManager* sm, clang::SourceLocation start, clang::SourceLocation end) {
    clang::SourceLocation b(start), _e(end);
    clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, *sm, rewriter.getLangOpts()));
    return std::string(sm->getCharacterData(b),
                       sm->getCharacterData(e)-sm->getCharacterData(b));
}*/

std::string get_location_to_string(Rewriter &rewriter, clang::SourceManager* sm, clang::SourceLocation start, clang::SourceLocation end) {
    //start.dump(*sm);
    //clang::SourceLocation _begin(start.), _end(end.getLocEnd());
    return std::string(sm->getCharacterData(start),
                       sm->getCharacterData(end));//-sm->getCharacterData(start));
}

std::string convert_decl_to_str(Rewriter &rewriter, clang::FunctionDecl *d, clang::SourceManager* sm) {
    clang::SourceLocation b(d->getLocStart()), _e(d->getLocEnd());
    clang::SourceLocation e(clang::Lexer::getLocForEndOfToken(_e, 0, *sm, rewriter.getLangOpts()));
    return std::string(sm->getCharacterData(b),
                       sm->getCharacterData(e)-sm->getCharacterData(b));
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