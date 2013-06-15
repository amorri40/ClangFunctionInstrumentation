//
//  ast_rewriting_functions.h
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#ifndef ClangInstrumentationPlugin_ast_rewriting_functions_h
#define ClangInstrumentationPlugin_ast_rewriting_functions_h

int get_length_of_token_at_location(clang::SourceLocation Loc, Rewriter &rewriter);

bool replace_text_at_location(Rewriter &rewriter, clang::SourceLocation start, int end, char* new_text);
#endif
