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

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

/*
 Check it doesn't contain bad statements which would make the debug code not work
 */
bool contains_bad_statements(Stmt *s) {
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        
        Stmt* statement_from_it = *it;
        
        if (statement_from_it == NULL) continue;
        if (!it->children().empty()) {if (contains_bad_statements(*it)) return true;}
        
        
        if (isa<LabelStmt>(*statement_from_it)) {
            //llvm::errs() << "Label_Statement!" << "\n\n\n";
            return true; //we don't want to debug functions with label statements
        } else {
            //llvm::errs() << "Not Label_Statement it is:" << it->getStmtClassName() << "\n\n\n";
            
        }
    }
    return false; //everything is good
}

void modify_statements(Rewriter* rewriter, Stmt *s) {
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        
        Stmt* statement_from_it = *it;
        
        if (statement_from_it == NULL) continue;
        if (!it->children().empty()) {modify_statements(rewriter,*it);}
        
        if (isa<CompoundAssignOperator>(*statement_from_it)) {
            //llvm::errs() << "Compund Assign!!" << "\n\n\n";
            
        } /*else if (isa<Decl>(*statement_from_it)) {
            llvm::errs() << "Decl!!" << "\n\n\n";
            Decl *decl = cast<Decl>(s);
            decl->getDeclKindName();
            
        }*/

        else if (isa<DeclStmt>(*statement_from_it)) {
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
        else if (isa<Expr>(*statement_from_it)) {
            
            
            
           // llvm::errs() << "Expression!!" << "\n\n\n";
            Expr *st = cast<Expr>(statement_from_it);
            
            
            
            if (isa<clang::BinaryOperator>(st)) {
                
                //llvm::errs() << "Binary operator" ;
                //rewriter->InsertTextAfter(st->getLocStart(), "/*BINOP*/ ");
                //rewriter->InsertTextAfter(st->getLocEnd(), "/*End of Binop*/");
                
                BinaryOperator* biOp = (BinaryOperator *) st;
                if (biOp->isAssignmentOp()) {
                    // get the lhs and rhs of the operator
                    Expr* lhs = biOp->getLHS();
                    
                    
                    
                    Expr* rhs = biOp->getRHS();
                    if (rhs->isLValue()) continue; //current doesn't support val = val = val
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
                    
                    rewriter->InsertTextAfter(lhs->getLocStart(), debug_version_of_lhs.str());
                    rewriter->InsertTextAfter(biOp->getOperatorLoc(), ")");
                    
                    /*
                     now do the right hand side
                    */
                    std::string rhs_name = rewriter->ConvertToString(lhs);
                    replaceAll(rhs_name,"\"","'");
                    
                    std::ostringstream debug_version_of_rhs;
                    
                    debug_version_of_rhs << "/*RHSValue*/ (inst_func_db.log_change_rhs(";
                    debug_version_of_rhs << "\"" << var_name << "\",__LINE__,";
                    //debug_version_of_rhs << rewriter->ConvertToString(rhs);
                    //debug_version_of_rhs << "),";
                    
                    
                    
                    rewriter->InsertTextAfter(rhs->getLocStart(), debug_version_of_rhs.str());
                    rewriter->InsertTextAfterToken(rhs->getLocEnd(), "))/*End of RHS*/");
                    
                    
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
            //it->PrintStats();
            //llvm::errs() << "else it is:" << it->getStmtClassName() << "\n\n\n";
            
        }
    }
   
}

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
        
        //if (f->isOverloadedOperator()) return true;
        //if ( f->getKind() == f->ClassScopeFunctionSpecialization) return true;
        
        //if (f->isVirtualAsWritten()) return true;
        //if (f->isCXXClassMember() || f->isCXXInstanceMember()) return true;
        
        
        // Get name of function
        DeclarationNameInfo dni = f->getNameInfo();
        
        //llvm::errs() << dni.getAsString() << "\n";
        // llvm::errs() << f->getQualifiedNameAsString()<< "\n";
       // DeclarationName dn = dni.getName();
       // std::string fname = dn.getAsString();
        std::string fname = f->getNameAsString();
        //llvm::errs() << "Function name:" << fname << "\n";
        
        
        //fprintf(stderr, "Visiting Function: %s %p\n", fname.c_str(), (void*)f);
        SourceRange sr = f->getSourceRange();
        Stmt *s = f->getBody();
        
        
        if (contains_bad_statements(s)) return true; //check it doesn't contain bad statements
        
        std::string statements = rewriter.ConvertToString(s);
        QualType return_type = f->getResultType();
        std::string return_type_str = return_type.getAsString();
        
        
        
        SourceLocation end_of_func_name = clang::Lexer::getLocForEndOfToken(dni.getEndLoc(), 0, rewriter.getSourceMgr(), rewriter.getLangOpts());
        SourceLocation start_of_stmts =clang::Lexer::GetBeginningOfToken(s->getLocStart(), rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        std::string func_args_string = get_location_to_string(rewriter, &rewriter.getSourceMgr(), end_of_func_name, start_of_stmts);
        
        //llvm::errs() << func_args_string << "\n";
        
        rewriter.setSourceMgr(rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        // Point to start of function name
        SourceLocation start_of_function_name_token = dni.getLoc();
        std::string new_function_name = fname+"_orig";
        int length_of_function_name = get_length_of_token_at_location(start_of_function_name_token, this->rewriter);
        
        
        
        //if(!f->isGlobal()) return true;
        //if(!f->isFileContext()) return true;
        
        
        std::string proper_func_name = get_location_to_string(rewriter, &rewriter.getSourceMgr(), sr.getBegin(), end_of_func_name);
        
        if(f->getQualifierLoc().hasQualifier()) {
            return true;
        std::string namespace_info = get_location_to_string(rewriter, &rewriter.getSourceMgr(), f->getQualifierLoc().getBeginLoc(), dni.getBeginLoc());
        
        
        //llvm::errs() << namespace_info << " namespace_info\n\n";
        } else {
            //replace_text_at_location(rewriter, start_of_function_name_token, length_of_function_name, new_function_name.c_str());
        }
        
     //std::string log_start = "{InstrumentFunctionDB inst_func_db(__FUNCTION__);\n";
        
        
        
        
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        
        std::string whole_func = get_location_to_string(rewriter, &rewriter.getSourceMgr(), start_of_stmts, END);
        
        /*
         Do all modifications after this
         */
        modify_statements(&rewriter,s);
        
        
        //std::string debug_func = "{ if (!ALI_GLOBAL_DEBUG) {";
        
        //size_t position_of_first_curly_bracket = statements.find_first_of('{');
        //statements.replace(position_of_first_curly_bracket, 1, debug_func);
        //replace_text_at_location(rewriter,position_of_first_curly_bracket,1,debug_func);
        
        
        //current idea: create inline version of original function which calls a function pointer, static int count;
        // issues:
        
        
        std::ostringstream debug_version_of_function;
        debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n if (!ALI_GLOBAL_DEBUG || NO_INSTRUMENT) ";
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << " else {static StaticFunctionData ali_function_db(__FUNCTION__, __LINE__, __FILE__); InstrumentFunctionDB inst_func_db(&ali_function_db);  \n #endif \n";
        /*
        debug_version_of_function << "\n " << proper_func_name << "_debug";
        //debug_version_of_function << " " << return_type_str.c_str() << " " << fname.data() << "_debug";
        debug_version_of_function << " " << func_args_string.c_str();
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << "\n " << proper_func_name << " " << func_args_string.c_str() << " { return "<< fname << "_debug();"<<" }";*/
        //now write the function pointer
        /*debug_version_of_function << " " << return_type_str.c_str();
        debug_version_of_function << " (* " << fname.data()<< ")";
        debug_version_of_function << " " << func_args_string.c_str();
        debug_version_of_function << " = &" << fname.data() << "_debug; \n";*/
        
        
        
        // PUT THIS BAXK IN!sprintf(fc, " %s debug_%s %s %s %s (* %s) %s = &debug_%s; \n", return_type_str.c_str(), fname.data(), func_args_string.c_str(),statements.c_str(), return_type_str.c_str(), fname.data(), func_args_string.c_str(),fname.data());
        //rewriter.InsertText(END, debug_version_of_function.str(), true, true);
        //statements.append(debug_version_of_function.str());
        rewriter.InsertTextAfter(start_of_stmts, debug_version_of_function.str());
        rewriter.InsertTextAfter(END, "\n #if NO_INSTRUMENT == false \n } \n #endif \n");
        rewriter.InsertTextAfter(END, "}");
        //rewriter.InsertText(END, debug_version_of_function.str(), true, true);
        //statements.replace(position_of_first_curly_bracket, 1, debug_version_of_function.str());
        
        //printf("End of: %s\n", "VisitFunctionDecl");
    }
    
    return true; // returning false aborts the traversal
}