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


/*
 Check it doesn't contain bad statements which would make the debug code not work
 */
bool contains_bad_statements(Stmt *s) {
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        
        Stmt* statement_from_it = *it;
        
        if (statement_from_it == NULL) continue;
        if (!it->children().empty()) {if (contains_bad_statements(*it)) return true;}
        
        
        if (isa<LabelStmt>(*statement_from_it)) {
            return true; //we don't want to debug functions with label statements
        }
    }
    return false; //everything is good
}

void modify_statements(Rewriter* rewriter, Stmt *s) {
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        
        Stmt* statement_from_it = *it;
        
        if (statement_from_it == NULL) continue;
        
        std::ostringstream class_name;
        class_name << " /*" << it->getStmtClassName() << "*/ ";
        
        
        if (it->getLocStart().isInvalid()) continue;
        rewriter->InsertText(it->getLocStart(), class_name.str());
        
        if (!it->children().empty()) {modify_statements(rewriter,*it);}
        
         if (isa<DeclStmt>(*statement_from_it)) {
            DeclStmt *declStatement = cast<DeclStmt>(statement_from_it);
            if (declStatement->isSingleDecl()) {
                Decl* d = declStatement->getSingleDecl();
                d->getDeclKindName();
                rewriter->InsertTextAfter(d->getLocStart(), "/*DECL*/");
            }
        } else if (isa<CompoundStmt>(*statement_from_it)) {
            // basically blocks of statements {}
            
            CompoundStmt *st = cast<CompoundStmt>(statement_from_it);
            rewriter->InsertTextAfter(st->getLocStart(), "/*Compound*/");
        }
        else if (isa<CompoundAssignOperator>(*statement_from_it)) {
        //ignore compound statements
        }
        else if (isa<Expr>(*statement_from_it)) {
            
            Expr *st = cast<Expr>(statement_from_it);
            
            if (isa<clang::BinaryOperator>(st)) {
                
                BinaryOperator* biOp = (BinaryOperator *) st;
                if (biOp->isAssignmentOp()) {
                    // get the lhs and rhs of the operator
                    Expr* lhs = biOp->getLHS();
                    
                    if (isa<clang::ImplicitCastExpr>(lhs)) continue;
                    
                    
                    Expr* rhs = biOp->getRHS();
                    //if (rhs->isLValue()) continue; //current doesn't support val = val = val
                    if (isa<clang::UnaryOperator>(rhs)) continue;
                    if (isa<clang::CompoundAssignOperator>(rhs)) continue;
                    if (isa<clang::BinaryOperator>(rhs)) continue;
                    if (isa<clang::BinaryConditionalOperator>(rhs)) continue;
                    if (isa<clang::ImplicitCastExpr>(rhs)) continue;
                    /*
                     *left hand side
                     */
                    std::string var_name = rewriter->ConvertToString(lhs);
                    replaceAll(var_name,"\"","'");
                    
                    std::ostringstream debug_version_of_lhs;
                    
                    debug_version_of_lhs << "(inst_func_db.log_change_lhs(";
                    debug_version_of_lhs << "\"" << var_name << "\",__LINE__,";
                    //debug_version_of_lhs << rewriter->ConvertToString(lhs);
                    //debug_version_of_lhs << "),";
                    
                    if (lhs->getLocStart().isInvalid()) continue;
                    if (biOp->getOperatorLoc().isInvalid()) continue;
                    if (rhs->getLocStart().isInvalid()) continue;
                    if (biOp->getLocEnd().isInvalid()) continue;
                    
                    rewriter->InsertTextAfter(lhs->getLocStart(), debug_version_of_lhs.str());
                    rewriter->InsertTextAfter(biOp->getOperatorLoc(), "))");
                    
                    /*
                     now do the right hand side
                    */
                    //std::string rhs_name = rewriter->ConvertToString(lhs);
                    //replaceAll(rhs_name,"\"","'");
                    
                    std::ostringstream debug_version_of_rhs;
                    
                    debug_version_of_rhs << "/*RHSValue*/ (inst_func_db.log_change_rhs(";
                    debug_version_of_rhs << "\"" << var_name << "\",__LINE__,";
                    //debug_version_of_rhs << rewriter->ConvertToString(rhs);
                    //debug_version_of_rhs << "),";
                    
                    std::ostringstream class_name;
                    //class_name << "/*" << rhs->getStmtClassName() << "*/";
                    
                    //rewriter->InsertText(rhs->getExprLoc(), class_name.str());
                    
                    rewriter->InsertTextBefore(rhs->getLocStart(), debug_version_of_rhs.str());
                    rewriter->InsertTextAfterToken(biOp->getLocEnd(), "))/*End of RHS*/");
                    llvm::errs() << rhs->getStmtClassName() << "," ;
                    
                }
                
                
            } else {
                continue;
                QualType q = st->getType(); //value type of expression
                //llvm::errs() << q.getAsString();
                if (st->isLValue()) {
                    rewriter->InsertTextAfter(st->getLocStart(), "/*LValue*/ ");
                    rewriter->InsertTextAfter(st->getLocEnd(), "");
                } else if (st->isXValue()) {
                    rewriter->InsertTextAfter(st->getLocStart(), "/*XValue*/ ");
                    rewriter->InsertTextAfter(st->getLocEnd(), "");
                }
            
            
            }
        }
        else {
        }
    }
   
}

inline SourceLocation getBeginningOfStatement(Stmt *s, Rewriter* rewriter) {
    return clang::Lexer::GetBeginningOfToken(s->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
}

/*
 Add the segfault handler to the main method (this doesn't work for libraries (since they don't have a main method)
 */
bool modify_main_function(Stmt *s, Rewriter* rewriter) {
    //Add the segfault handler to the start of the main function
    SourceLocation start_of_stmts = getBeginningOfStatement(s,rewriter);
    rewriter->InsertTextAfterToken(start_of_stmts, " SEGFAULTHANDLE ");
    return true;
}

bool MyRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f)
{
    
    if (f->hasBody())
    {
        
        // First check if we are in the cpp file passed to the compiler
        SourceLocation sl = f->getLocation();
        if (rewriter.getSourceMgr().getFileID(sl) !=
            rewriter.getSourceMgr().getMainFileID()) {
            return true; //we are not in the main cpp file
        }
        
        // get the body of this function so we can modify it
        Stmt *s = f->getBody();
        
        if (f->isMain()) return modify_main_function(s, &rewriter);
        
        // Get name of function
        DeclarationNameInfo dni = f->getNameInfo();
        std::string fname = f->getNameAsString();
        
        SourceRange sr = f->getSourceRange();
        
        
        
        if (contains_bad_statements(s)) return true; //check it doesn't contain bad statements
        
        std::string statements = rewriter.ConvertToString(s);
        QualType return_type = f->getResultType();
        std::string return_type_str = return_type.getAsString();
        
        
        
        SourceLocation end_of_func_name = clang::Lexer::getLocForEndOfToken(dni.getEndLoc(), 0, rewriter.getSourceMgr(), rewriter.getLangOpts());
        SourceLocation start_of_stmts =clang::Lexer::GetBeginningOfToken(s->getLocStart(), rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        std::string func_args_string = get_location_to_string(rewriter, &rewriter.getSourceMgr(), end_of_func_name, start_of_stmts);
        
        rewriter.setSourceMgr(rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        // Point to start of function name
        SourceLocation start_of_function_name_token = dni.getLoc();
        std::string new_function_name = fname+"_orig";
        int length_of_function_name = get_length_of_token_at_location(start_of_function_name_token, this->rewriter);
        
        std::string proper_func_name = get_location_to_string(rewriter, &rewriter.getSourceMgr(), sr.getBegin(), end_of_func_name);
        
        if(f->getQualifierLoc().hasQualifier()) {
            return true;
        std::string namespace_info = get_location_to_string(rewriter, &rewriter.getSourceMgr(), f->getQualifierLoc().getBeginLoc(), dni.getBeginLoc());
        
        }
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        
        std::string whole_func = get_location_to_string(rewriter, &rewriter.getSourceMgr(), start_of_stmts, END);
        
        /*
         Do all modifications after this
         */
        modify_statements(&rewriter,s);
        
        std::ostringstream debug_version_of_function;
        debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) ";
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << " else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db);  \n #endif \n";
        
        
        rewriter.InsertTextAfter(start_of_stmts, debug_version_of_function.str());
        rewriter.InsertTextAfter(END, "\n #if NO_INSTRUMENT == false \n } \n #endif \n");
        rewriter.InsertTextAfter(END, "}");
        
    }
    
    return true; // returning false aborts the traversal
}