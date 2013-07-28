//
//  ast_rewriting_functions.h
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#ifndef ClangInstrumentationPlugin_ast_rewriting_functions_h
#define ClangInstrumentationPlugin_ast_rewriting_functions_h

void replaceAll(std::string& str, const std::string& from, const std::string& to);

int get_length_of_token_at_location(clang::SourceLocation Loc, Rewriter &rewriter);

bool replace_text_at_location(Rewriter &rewriter, clang::SourceLocation start, int end, const char* new_text);

std::string convert_decl_to_str(Rewriter &rewriter, clang::FunctionDecl *d, clang::SourceManager* sm);
std::string get_location_to_string(Rewriter &rewriter, clang::SourceManager* sm, clang::SourceLocation start, clang::SourceLocation end);
#endif
