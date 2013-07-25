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
#include <iostream>
#include <sstream>


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
        
        if (f->isOverloadedOperator()) return true;
        if ( f->getKind() == f->ClassScopeFunctionSpecialization) return true;
        
        if (f->isVirtualAsWritten()) return true;
        if (f->isCXXClassMember() || f->isCXXInstanceMember()) return true;
        
        
        // Get name of function
        DeclarationNameInfo dni = f->getNameInfo();
        
        //llvm::errs() << dni.getAsString() << "\n";
        // llvm::errs() << f->getQualifiedNameAsString()<< "\n";
       // DeclarationName dn = dni.getName();
       // std::string fname = dn.getAsString();
        std::string fname = f->getNameAsString();
        llvm::errs() << "Function name:" << fname << "\n";
        
        
        fprintf(stderr, "Visiting Function: %s %p\n", fname.c_str(), (void*)f);
        SourceRange sr = f->getSourceRange();
        Stmt *s = f->getBody();

        
        
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
        std::string new_function_name = fname+"_orig";
        int length_of_function_name = get_length_of_token_at_location(start_of_function_name_token, this->rewriter);
        replace_text_at_location(rewriter, start_of_function_name_token, length_of_function_name, new_function_name.c_str());
        
        
     std::string log_start = "{InstrumentFunctionDB inst_func_db(__FUNCTION__);\n";
        
        size_t position_of_first_curly_bracket = statements.find_first_of('{');
        statements.replace(position_of_first_curly_bracket, 1, log_start);
        
        
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        
        
        std::ostringstream debug_version_of_function;
        debug_version_of_function << " " << return_type_str.c_str() << " " << fname.data() << "_debug";
        debug_version_of_function << " " << func_args_string.c_str();
        debug_version_of_function << " " << statements.c_str();
        //now write the function pointer
        debug_version_of_function << " " << return_type_str.c_str();
        debug_version_of_function << " (* " << fname.data()<< ")";
        debug_version_of_function << " " << func_args_string.c_str();
        debug_version_of_function << " = &" << fname.data() << "_debug; \n";
        
        
        // PUT THIS BAXK IN!sprintf(fc, " %s debug_%s %s %s %s (* %s) %s = &debug_%s; \n", return_type_str.c_str(), fname.data(), func_args_string.c_str(),statements.c_str(), return_type_str.c_str(), fname.data(), func_args_string.c_str(),fname.data());
        rewriter.InsertText(END, debug_version_of_function.str(), true, true);
        printf("End of: %s\n", "VisitFunctionDecl");
    }
    
    return true; // returning false aborts the traversal
}