//
//  MyRecursiveASTVisitor.cpp
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include "MyRecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "ast_rewriting_functions.h"
#include "clang/Basic/SourceManager.h"


bool MyRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f)
{
    
    //f->dump();
    //DeclarationName new_name =
    //f->setDeclName (DeclarationName N)
    //f->getLocation();
    //if (f->isFromASTFile()) return true;
    if (f->hasBody())
    {
        if (f->isMain()) return true;
        
        SourceLocation sl = f->getLocation();
        
        
        //fprintf(stderr, "filename: %s \n",rewriter.getSourceMgr().getFilename(sl).str().c_str());
        if (rewriter.getSourceMgr().getFileID(sl) !=
            rewriter.getSourceMgr().getMainFileID()) {
            return true;
        }
        
        fprintf(stderr, "Visiting Function: %s %p\n", std::string(f->getName()).c_str(), (void*)f);
        SourceRange sr = f->getSourceRange();
        Stmt *s = f->getBody();
        
        //SourceManager* sm = rewriter.getSourceMgr();
        //rewriter.
        //SourceManager smm = *sourceManager;
        
        
        //sl.isFileID()
        
        
        // Get name of function
        DeclarationNameInfo dni = f->getNameInfo();
        DeclarationName dn = dni.getName();
        std::string fname = dn.getAsString();
        llvm::errs() << "Function name:" << fname << "\n";
        
        
        std::string statements = rewriter.ConvertToString(s);
        QualType return_type = f->getResultType();
        std::string return_type_str = return_type.getAsString();
        
        SourceLocation end_of_func_name = clang::Lexer::getLocForEndOfToken(dni.getEndLoc(), 0, rewriter.getSourceMgr(), rewriter.getLangOpts());
        SourceLocation start_of_stmts =clang::Lexer::GetBeginningOfToken(s->getLocStart(), rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        std::string func_args_string = get_location_to_string(rewriter, &rewriter.getSourceMgr(), end_of_func_name, start_of_stmts);
        
        llvm::errs() << func_args_string << "\n";
        
        rewriter.setSourceMgr(rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        // Point to start of function name
        SourceLocation start_of_function_name_token = dni.getLoc();
        std::string new_function_name = "orig_"+fname;
        int length_of_function_name = get_length_of_token_at_location(start_of_function_name_token, this->rewriter);
        replace_text_at_location(rewriter, start_of_function_name_token, length_of_function_name, new_function_name.c_str());
        
        // Add comment
        char fc[256];
        //sprintf(fc, "// Begin function %s returning %s\n", fname.data(), ret.data());
        //rewriter.InsertText(start_of_function_decl_tokens, fc, true, true);
        
        if (f->isMain())
            llvm::errs() << "Found main()\n";
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        sprintf(fc, " %s debug_%s %s %s %s (* %s) %s = &orig_%s; \n", return_type_str.c_str(), fname.data(), func_args_string.c_str(),statements.c_str(),
                return_type_str.c_str(), fname.data(), func_args_string.c_str(),fname.data());
        rewriter.InsertText(END, fc, true, true);
        printf("End of: %s\n", "VisitFunctionDecl");
    }
    
    return true; // returning false aborts the traversal
}