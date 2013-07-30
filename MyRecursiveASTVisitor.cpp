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

inline SourceLocation getBeginningOfStatement(Stmt *s, Rewriter* rewriter) {
    
    return clang::Lexer::GetBeginningOfToken(s->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
}

inline void insert_before_after(Expr *st, Rewriter* rewriter, std::string before, std::string after) {
    if (st->getLocStart().isInvalid()) return;
    if (st->getLocEnd().isInvalid()) return;
    SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts()); //getBeginningOfStatement(statement_from_it,rewriter);
    SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
    if (end.isInvalid()) return;
    if (begining.isInvalid()) return;
    if (!rewriter->isRewritable(end)) return;
    if (!rewriter->isRewritable(begining)) return;
    
    bool b = rewriter->InsertTextBefore(begining, before);
    if (!b)
        bool a = rewriter->InsertTextBefore(end, after);
}

inline void insert_before_after_2(Stmt *st, Rewriter* rewriter, std::string before, std::string after) {
    SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts()); //getBeginningOfStatement(statement_from_it,rewriter);
    SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
    if (!rewriter->isRewritable(end)) return;
    if (!rewriter->isRewritable(begining)) return;
    bool b = rewriter->InsertTextBefore(begining, before);
    if (!b)
        rewriter->InsertTextAfter(end, after);
}

void modify_statements(Rewriter* rewriter, Stmt *s) {
    
    
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        
        Stmt* statement_from_it = *it;
        
        if (statement_from_it == NULL) continue;
        if (it->getLocStart().isInvalid()) continue;
        
        if (isa<CaseStmt>(*statement_from_it)) {
            CaseStmt *caseStatement = cast<CaseStmt>(statement_from_it);
            insert_before_after_2(caseStatement->getSubStmt(), rewriter, " /* start of case rhs */ ", " /* end of case rhs */ ");
            //caseStatement->get
            //caseStatement->getSubStmt()->
            modify_statements(rewriter,caseStatement->getSubStmt());
            continue;
        } else if (isa<SwitchCase>(*statement_from_it) || isa<SwitchCase>(*statement_from_it)) {
            SwitchCase *caseStatement = cast<SwitchCase>(statement_from_it);
            insert_before_after_2(caseStatement->getSubStmt(), rewriter, " /* start of switch */ ", " /* end of switch */ ");
            //caseStatement->get
            
            //modify_statements(rewriter,caseStatement->get);
            continue;
        }
         else if (isa<DeclStmt>(*statement_from_it)) {
            DeclStmt *declStatement = cast<DeclStmt>(statement_from_it);
            if (declStatement->isSingleDecl()) {
                Decl* d = declStatement->getSingleDecl();
                d->getDeclKindName();
                rewriter->InsertTextAfter(d->getLocStart(), " /*DECL*/ ");
            }
         } else if (isa<ReturnStmt>(*statement_from_it)) {
            ReturnStmt *declStatement = cast<ReturnStmt>(statement_from_it);
            //insert_before_after(declStatement->getRetValue(), rewriter, " LOGRETURN(( ", ")) ");
                
         } else if (isa<IntegerLiteral>(*statement_from_it) || isa<StringLiteral>(*statement_from_it)) {
          continue;
        } /*else if (isa<CompoundStmt>(*statement_from_it)) {
            // basically blocks of statements {}
            
            CompoundStmt *st = cast<CompoundStmt>(statement_from_it);
            rewriter->InsertTextAfter(st->getLocStart(), " /* Compound  ");
        }*/
        /*else if (isa<CompoundAssignOperator>(*statement_from_it)) {
        //ignore compound statements
        }*/
        else if (false) {//(isa<CallExpr>(*statement_from_it)) {
            
            CallExpr *st = cast<CallExpr>(statement_from_it);
            
            
            SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts()); //getBeginningOfStatement(statement_from_it,rewriter);
            SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
            if (end.isInvalid()) continue;
            if (begining.isInvalid()) continue;
            
            if (st->getDirectCallee()== NULL) continue;
            
            std::string function_name = st->getDirectCallee()->getNameAsString();
            
            /*for (int argument =0; argument < st->getNumArgs(); argument++) {
                Expr* arg = st->getArg(argument);
                int start_col = rewriter->getSourceMgr().getPresumedColumnNumber(arg->getLocStart());
                std::ostringstream argument_data;
                argument_data << "\"" << function_name << "\",__LINE__, " << start_col << ", ";
                argument_data << argument << ", ";
                insert_before_after(arg,rewriter,"(inst_func_db.log_argument(","))");
            }*/
            
            
            std::ostringstream debug_call;
            
            debug_call << " (inst_func_db.log_call(";
            //debug_call << "\"" << st->getDirectCallee()->getNameAsString() << "\",__LINE__), ";
            debug_call << "\"" <<  function_name << "\",__LINE__), ";
            if (st->isBuiltinCall())
            continue;
            //insert_before_after(st,rewriter,debug_call.str()," ) /*endcall ");
            
            if (!rewriter->isRewritable(end)) continue;
            if (!rewriter->isRewritable(begining)) continue;
            
            bool b = rewriter->InsertTextBefore(begining, debug_call.str());
            if (!b)
                bool a = rewriter->InsertTextBefore(end, " ) /*endcall*/ ");
            
            
            
        }
        else if (isa<BinaryOperator>(*statement_from_it)) {
            BinaryOperator *dre = cast<BinaryOperator>(statement_from_it);
            insert_before_after(dre->getLHS(), rewriter, " LHS(( ", ")) ");
            //insert_before_after(dre->getRHS(), rewriter, " RHS(( ", ")) ");
            //continue; //ignore children
        }
        else if (isa<BinaryConditionalOperator>(*statement_from_it)) {
            
            //continue;
        }
        else if (isa<CXXOperatorCallExpr>(*statement_from_it)) {
            CXXOperatorCallExpr *dre = cast<CXXOperatorCallExpr>(statement_from_it);
            for (int i=0; i<dre->getNumArgs(); i++) {
                Expr* arg = dre->getArg(i);
                //modify_statements(rewriter,dre->getArg(i));
                if (dre->getArg(i) == NULL) continue;
                if (dre->getArg(i)->isLValue())
                insert_before_after(dre->getArg(i), rewriter, " OPERATOR_LHS_ARG((", ")) ");
                else if (dre->getArg(i)->isRValue()) {
                    //if (dre->getArg(i)->
                    std::ostringstream comment;
                    comment << " /*" << arg->getType().getAsString() << "*/ OPERATOR_RHS_ARG((";
                    
                    //if (!arg->isOrdinaryOrBitFieldObject())
                insert_before_after(dre->getArg(i), rewriter, comment.str(), ")) ");
                }
            }
            
            //continue;
        }
        
        else if (isa<DeclRefExpr>(*statement_from_it)) {
           DeclRefExpr *dre = cast<DeclRefExpr>(statement_from_it);
            if (dre->isRValue()) {
//                std::string var_name = rewriter->ConvertToString(dre);
//                std::ostringstream debug_version_of_rhs;
//                int start_col = rewriter->getSourceMgr().getPresumedColumnNumber(dre->getLocStart());
//                int end_col = rewriter->getSourceMgr().getPresumedColumnNumber(dre->getLocEnd());
//                int line = rewriter->getSourceMgr().getPresumedLineNumber(dre->getLocStart());
//                
//                debug_version_of_rhs << " /*RHSValue*/ (inst_func_db.log_change_rhs(";
//                debug_version_of_rhs << "\"" << start_col << "_" << end_col << "_" << line << "_" << dre->getType().getAsString() << "_" << dre->getStmtClassName() << "\",__LINE__,";
//                
//            insert_before_after(dre,rewriter,debug_version_of_rhs.str(),"))");
                insert_before_after(dre, rewriter, " RHS(( ", ")) ");
                
            } else if ( dre->isXValue()) {
               
                insert_before_after(dre,rewriter," /* x value declref! */ "," /* end x value*/ ");
            } else if ( dre->isLValue()) {
                std::ostringstream d;
                d << " /* LValue " << dre->getStmtClassName() << " " << dre->getDecl()->getNameAsString() << "*/ ";
                
                //insert_before_after(dre,rewriter,d.str()," /* end L value*/ ");
                continue;
            }
        }
        else if (isa<CXXBindTemporaryExpr>(*statement_from_it)) {
            CXXBindTemporaryExpr *dre = cast<CXXBindTemporaryExpr>(statement_from_it);
            
            insert_before_after_2(dre,rewriter,"(/*CXXBindTemporaryExpr*/ "," /*End CXXBindTemporaryExpr*/)");
        }
        else if (isa<ImplicitCastExpr>(*statement_from_it)) {
            ImplicitCastExpr *dre = cast<ImplicitCastExpr>(statement_from_it);
            
            //insert_before_after_2(dre,rewriter,"(/*ImplicitCastExpr*/ "," /*ImplicitCastExpr*/)");
        }
        else if (isa<CXXMemberCallExpr>(*statement_from_it)) {
            CXXMemberCallExpr *dre = cast<CXXMemberCallExpr>(statement_from_it);
            
            insert_before_after_2(dre,rewriter," MEMBER_CALL(( "," )) ");
            for (int i=0; i<dre->getNumArgs(); i++) {
                //modify_statements(rewriter,dre->getArg(i));
                if (dre->getArg(i) == NULL) continue;
                insert_before_after(dre->getArg(i), rewriter, " CALL_ARG(( ", ")) ");
            }
            continue; // don't want to parse the memberExpr!!
        }
        else if (isa<MemberExpr>(*statement_from_it)) {
            MemberExpr *dre = cast<MemberExpr>(statement_from_it);
            
            insert_before_after_2(dre,rewriter," MEMBER_EXPR(( "," )) ");
        }
        else if (isa<CallExpr>(*statement_from_it)) {
            CallExpr *dre = cast<CallExpr>(statement_from_it);
            if (dre->isRValue())
                insert_before_after_2(dre,rewriter," CALLR(( "," )) ");
            else
            insert_before_after_2(dre,rewriter," CALL(( "," )) ");
            
            for (int i=0; i<dre->getNumArgs(); i++) {
                //modify_statements(rewriter,dre->getArg(i));
                if (dre->getArg(i) == NULL) continue;
                insert_before_after(dre->getArg(i), rewriter, " CALL_ARG((", ")) ");
            }
        }
        
        else if (false) {//else if (isa<Expr>(*statement_from_it)) {
            
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
                    debug_version_of_lhs << rewriter->ConvertToString(lhs);
                    debug_version_of_lhs << "),";
                    
                    if (lhs->getLocStart().isInvalid()) continue;
                    if (biOp->getOperatorLoc().isInvalid()) continue;
                    if (rhs->getLocStart().isInvalid()) continue;
                    if (biOp->getLocEnd().isInvalid()) continue;
                    
                    if (!rewriter->isRewritable(lhs->getLocStart()) || !rewriter->isRewritable(biOp->getOperatorLoc())) continue;
                    
                    rewriter->InsertTextAfter(lhs->getLocStart(), debug_version_of_lhs.str());
                    rewriter->InsertTextAfter(biOp->getOperatorLoc(), " ) ");
                    
                    
                    /*
                     now do the right hand side
                    */
                    //std::string rhs_name = rewriter->ConvertToString(lhs);
                    //replaceAll(rhs_name,"\"","'");
                    
                    std::ostringstream debug_version_of_rhs;
                    
                    debug_version_of_rhs << " /*RHSValue*/ (inst_func_db.log_change_rhs(";
                    debug_version_of_rhs << "\"" << var_name << "\",__LINE__,";
                    //debug_version_of_rhs << rewriter->ConvertToString(rhs);
                    //debug_version_of_rhs << "),";
                    
                    std::ostringstream class_name;
                    //class_name << "/*" << rhs->getStmtClassName() << "*/";
                    
                    //rewriter->InsertText(rhs->getExprLoc(), class_name.str());
                    
                    rewriter->InsertTextBefore(rhs->getLocStart(), debug_version_of_rhs.str());
                    rewriter->InsertTextAfterToken(biOp->getLocEnd(), "))/*End of RHS*/ ");
                    //llvm::errs() << rhs->getStmtClassName() << "," ;
                    
                }
                
                
            }
            //else {
            
            
            
            
            } //end if binary operator
        else {
            std::ostringstream class_name;
            class_name << " /*" << statement_from_it->getStmtClassName() << "*/ "; //should this not be statement_from_id
            std::string beg = class_name.str();
            class_name.str("");
            class_name << " /* END " << statement_from_it->getStmtClassName() << "*/ ";
            
            if (statement_from_it->getLocStart().isInvalid()) continue;
            //rewriter->InsertText(statement_from_it->getLocStart(), class_name.str());
            //insert_before_after_2(statement_from_it,rewriter,beg,class_name.str());
        
        }
        
        if (!it->children().empty()) {modify_statements(rewriter,*it); continue;}
        
    } //end of for loop
   
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
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        
        std::string whole_func = get_location_to_string(rewriter, &rewriter.getSourceMgr(), start_of_stmts, END);
        
        /*
         Do all modifications after this
         */
        modify_statements(&rewriter,s);
        
        std::ostringstream params_to_log;
        params_to_log << "{";
        /* Log each argument to this function */
        for (int argument=0; argument< f->getNumParams(); argument++) {
             ParmVarDecl *pvd = f->getParamDecl(argument);
            if (pvd->isInvalidDecl()) continue;
            if (pvd->getNameAsString() == "") continue;
            params_to_log << " LOGPARAMETER(";
            params_to_log << pvd->getNameAsString() << "); ";
        }
        params_to_log << "} ";
        
        
        /*
         Now write final output
         */
        
        std::ostringstream debug_version_of_function;
        debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT) \n #endif \n";
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << "\n #if NO_INSTRUMENT == false \n else {static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); \n";
        debug_version_of_function << params_to_log.str();
        
        
        rewriter.InsertTextAfter(start_of_stmts, debug_version_of_function.str());
        //rewriter.InsertTextAfter(END, "\n #if NO_INSTRUMENT == false \n } \n #endif \n");
        rewriter.InsertTextAfter(END, "\n}\n#endif \n");
        rewriter.InsertTextAfter(END, "}");
        
    }
    
    return true; // returning false aborts the traversal
}