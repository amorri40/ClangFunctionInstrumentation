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
#include "clang/AST/Attr.h"
#include "clang/Sema/DeclSpec.h"
#include <iostream>
#include <sstream>

FunctionDecl * current_function;
extern bool c_file;
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

inline void insert_before_after(Stmt *st, Rewriter* rewriter, std::string before, std::string after, bool beforeEnd) {
    if (st->getLocStart().isInvalid()) return;
    if (st->getLocEnd().isInvalid()) return;
    SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
    SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
    
    /*
     Check the locations are valid and rewritable otherwise it will fail
     */
    
    if (end.isInvalid()) return;
    if (begining.isInvalid()) return;
    if (!rewriter->isRewritable(end)) return;
    if (!rewriter->isRewritable(begining)) return;
    
    bool b = rewriter->InsertTextBefore(begining, before);
    if (!b) {
        
        if (beforeEnd) {
        rewriter->InsertTextBefore(end, after);
        } else {
            rewriter->InsertTextAfter(end, after);
        }
    }
}

std::ostringstream additional_file_content;


inline std::string handle_type(QualType qt,Rewriter* rewriter, bool lvalue){
    
    bool pointer_type=false;
    //ExprValueKind evk = st->getValueKind();
    const Type* tp = qt.getTypePtr();
    
    if (tp->isPointerType()) {
        
        qt = tp->getPointeeType();
        tp = qt.getTypePtr();
        pointer_type=true;
        
        //return "";
    }
    
    if (tp->isPointerType()) {
        additional_file_content << "\n#define WasPointerToPointer_"<< tp->getTypeClassName() << " 0\n" ;
    return ""; //still a pointer so just return
    }
    
    if (tp->isVoidType()) return "Void";
    
    if (tp->isDependentType()) {
        //RecordDecl* rd = qt->getAsCXXRecordDecl();
        additional_file_content << "\n#define PrintDependentType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    if (tp->isVectorType()) {
        additional_file_content << "\n#define PrintVectorType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    /*if (tp->isAtomicType()) {
        additional_file_content << "\n#define PrintAtomicType_"<< tp->getTypeClassName() << " 0\n" ;
        return "Atomic";
    }*/
    if (tp->isClassType() || tp->isStructureType()) {
        if (c_file) return "";
        CXXRecordDecl* rd = tp->getAsCXXRecordDecl();
        
        NamedDecl* nd = rd->getUnderlyingDecl();
        if (rd->getAccess() == AS_private) return ""; //class isn't public
        if (rd->isAnonymousStructOrUnion()) return "";
        
        if (nd->getNameAsString() == "basic_string" && !pointer_type) {
        return "StringType";
        }
        
        if (rd->isTemplateDecl()) return "";
        if (isa<ClassTemplateSpecializationDecl>(rd)) return "";
        
        
        std::pair<  clang::CXXRecordDecl::conversion_iterator, clang::CXXRecordDecl::conversion_iterator > p = rd->getVisibleConversionFunctions();
        for (CXXRecordDecl::conversion_iterator it=p.first; it!=p.second; it++) {
            CXXConversionDecl* conver = cast<CXXConversionDecl>(*it);
            QualType q = conver->getResultType();
        }
        
        // loop through the methods
        for (CXXRecordDecl::method_iterator it=rd->method_begin(); it!=rd->method_end(); it++) {
            CXXMethodDecl* meth = cast<CXXMethodDecl>(*it);
            //additional_file_content << "\n// Meth:"+meth->getNameAsString()+"\n";
        }
        
        std::string getMember = ".";
        std::string refchar = "&";
        std::string log_prefix = "Norm";
        if (pointer_type) {
            getMember = "->"; refchar=" *"; log_prefix="Pointer";
            if (lvalue) {log_prefix+="L"; }//refchar+="&";}
        }
        
        
        if (!rd->field_empty()) {
            std::ostringstream log_method_body, whole_log_data;
            
            //if (current_function->getLinkageAndVisibility().getLinkage() == ExternalLinkage && pointer_type) return ""; //dllexport temp fix
            log_method_body << "{";
            if (pointer_type)
                log_method_body << "if (val==NULL) return val;";
            log_method_body <<" std::ostringstream v; v ";
            if (pointer_type) log_method_body << "<< \"Pointer to\" ";
            log_method_body << "<< \"{\"";
            // loop through the fields
            for (CXXRecordDecl::field_iterator it=rd->field_begin(); it!=rd->field_end(); it++) {
                FieldDecl* fie = cast<FieldDecl>(*it);
                
                if (fie->getAccess() == AS_private || fie->getAccess() == AS_protected) continue;
                if (fie->getAccess() == AS_public) {
                    if (fie->getType()->isIntegralOrEnumerationType())
                        log_method_body << " << \"'"<<fie->getNameAsString()<<"':\" << val"<< getMember <<fie->getNameAsString()<<" << \", \"";
                }
            }
            log_method_body << "<< \"}\"; inst_func_db.log_custom(line_num, start_loc, end_loc, typeid(val).name(),v.str()); return val;}\n";
            
            whole_log_data << "\n#ifndef PrintClassType_"<<log_prefix<<nd->getNameAsString()<<"\n ";
            
            if (!pointer_type) {
                
                whole_log_data << "\ntemplate <class T> T"<< refchar <<" log_builtin"<< log_prefix << nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, T"<< refchar <<" val) "<< log_method_body.str();
                
                whole_log_data << "\ntemplate <class T> const T"<< refchar <<" log_builtin"<< log_prefix << nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, const T"<< refchar <<" val) "<< log_method_body.str();
            }
            
            if (pointer_type) {
                
            whole_log_data << "\ntemplate <class T> T"<< refchar <<" log_builtin"<< log_prefix << nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, T"<< refchar <<" val) "<< log_method_body.str();
                
                whole_log_data << "\ntemplate <class T> const T"<< refchar <<" log_builtin"<< log_prefix << nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, const T"<< refchar <<" val) "<< log_method_body.str();
                
                
            }
            
            
            whole_log_data << "\n#define PrintClassType_"<<log_prefix<< nd->getNameAsString() << "(line,beg,end,arg) (stdlogger,log_builtin"<< log_prefix << nd->getNameAsString() << "(inst_func_db,line,beg,end,(arg)))\n";
            whole_log_data << "\n#endif\n";
            
            SourceLocation begining = clang::Lexer::GetBeginningOfToken(current_function->getOuterLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
            
            if (current_function->getUnderlyingDecl()->hasAttr<DLLExportAttr>()) return "";
            
            additional_file_content << whole_log_data.str();
            
            return "PrintClassType_"+log_prefix+nd->getNameAsString();
        }
        
        
        return "";
    }
    
    if (pointer_type) return ""; //only handle pointers to classes atm
    
    /*if (tp->isEnumeralType()) {
        //additional_file_content << "\n#define PrintEnumType_"<< tp->getTypeClassName() << " 0\n" ;
        return "EnumLog";
    }*/
    
    if (tp->isUnionType()) {
        additional_file_content << "\n#define PrintUnionType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";//return "EnumLog";
    }
    
    if (tp->isPlaceholderType()) {
        additional_file_content << "\n#define PrintPlaceholderType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    
    if (tp->isIntegralOrEnumerationType()) {
        if (qt->isFloatingType()) return "ALANG_LOG_DOUBLE";
        else if (tp->isBooleanType()) return "ALANG_LOG_BOOL";
        else if (tp->isCharType()) return "ALANG_LOG_CHAR";
        else if (tp->isIntegerType()) return "ALANG_LOG_INT";
        else return "ALANG_LOG_AS_INT";
        
        //return "IntegralOrEnumType";
    }
    
    if (tp->isAggregateType()) {
        additional_file_content << "\n#define PrintAggregateType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    
    
    if (tp->isRecordType()) {
         additional_file_content << "\n#define Print_"<< /*rd->getQualifiedNameAsString() <<*/ " 0\n" ;
        return "";
    }
    
    if (tp->isObjectType()) {
        additional_file_content << "\n#define PrintObjectType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    
    if (tp->isFundamentalType()) {
        additional_file_content << "\n#define PrintFundamentalType_"<< tp->getTypeClassName() << " 0\n" ;
        return "Fundamental";
    }
    
    //
    if (tp->isAtomicType() || tp->isEnumeralType()) {//|| tp->hasIntegerRepresentation()) {
        return "";//return "BuiltIn";
    }
    else {
        //additional_file_content << "\n#define Print"<< qt->getTypeClassName() << " 0\n" ;
    }
    return "";
}

inline std::string handle_type(Expr *st,Rewriter* rewriter){
    QualType qt = st->getType();
    
    if (st->isLValue()) {
        return handle_type(qt,rewriter,true);
        //return "";
    }
    //if (st->isLValue() && qt.getTypePtr()->isPointerType()) return "";
    return handle_type(qt,rewriter,false);
}

inline void wrap_with_macro(Expr *st, Rewriter* rewriter, std::string macro_name, bool beforeEnd, bool checktype) {
    SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
    SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
    
    int line = rewriter->getSourceMgr().getPresumedLineNumber(begining);
    int beg = rewriter->getSourceMgr().getPresumedColumnNumber(begining);
    int e = rewriter->getSourceMgr().getPresumedColumnNumber(end);
    
    
    std::string typest = handle_type(st,rewriter);
    if (typest ==  "Void") return;
    if (checktype) {
    if (typest != "") macro_name = typest;
    }
    
    std::ostringstream macro_call;
    macro_call << " " << macro_name << " ("<< line << ", ";
    macro_call << beg << ", " << e <<", ";
    macro_call << " (";
    
    insert_before_after(st,rewriter,macro_call.str(),")) ", beforeEnd);
}


void handle_call_argument(Expr* arg, Rewriter* rewriter) {
    if (arg == NULL) return;
    
    if (arg->isLValue()) {
        
        //insert_before_after(arg, rewriter, " CALL_LVALUE_ARG((", ")) ", true);
        if (!arg->getType()->isPointerType()) {
            wrap_with_macro(arg, rewriter, " CALL_LVALUE_ARG", true, true);
            return;
        }
        if (!arg->getType()->canDecayToPointerType()) return;
        wrap_with_macro(arg, rewriter, " CALL_LVALUE_ARG", true, true);
    }
    else {
        wrap_with_macro(arg, rewriter, " ARG_UNKNOWN", true, true);
    }
}

void modify_statements(Rewriter* rewriter, Stmt *s, FunctionDecl *f) {
    
    
    
    Stmt* statement_from_it = s;//*it;
        
        if (statement_from_it == NULL) return;
        if (s->getLocStart().isInvalid()) return;
        
        if (isa<CaseStmt>(*statement_from_it)) {
            CaseStmt *caseStatement = cast<CaseStmt>(statement_from_it);
            modify_statements(rewriter,caseStatement->getSubStmt(),f);
            return;
        }
        else if (isa<CXXDeleteExpr>(*statement_from_it) || isa<CXXNewExpr>(*statement_from_it) /*|| isa<CXXTemporaryObjectExpr>(*s)*/) return;
        else if (isa<CXXTemporaryObjectExpr>(*s)) {
            CXXTemporaryObjectExpr *temp = cast<CXXTemporaryObjectExpr>(statement_from_it);
            wrap_with_macro(temp, rewriter, "TemporaryObjectExpr", true,true);
            return;
        }
        else if (isa<ImplicitCastExpr>(*statement_from_it)) {
            ImplicitCastExpr *implicitcast = cast<ImplicitCastExpr>(statement_from_it);
            //wrap_with_macro(implicitcast, rewriter, implicitcast->getCastKindName(), true);
            std::ostringstream stringstr;
            
            if (!c_file)
            if (implicitcast->getSubExpr()->isCXX11ConstantExpr(f->getASTContext())) return; //static constant data can be optimised out
            
            if (implicitcast->getCastKind()== CK_LValueToRValue) {
                stringstr << " /*(" << implicitcast->getType().getAsString() << ")*/ " << implicitcast->getCastKindName();
                wrap_with_macro(implicitcast, rewriter, stringstr.str(), true,true);
                return;
            }
            else if (implicitcast->getCastKind() == CK_UserDefinedConversion) {
            stringstr << " (" << implicitcast->getType().getAsString() << ") " << implicitcast->getCastKindName();
            }
            else if (implicitcast->getCastKind() == CK_LValueBitCast) {
                wrap_with_macro(implicitcast, rewriter, "LValueBitCast", true,false);
                return;
            }
            else {
            stringstr << " /*(" << implicitcast->getType().getAsString() << ")*/ " << implicitcast->getCastKindName();
            }
            wrap_with_macro(implicitcast, rewriter, stringstr.str(), true,false);
            
            
        }
        else if (isa<IfStmt>(*statement_from_it)) {
            IfStmt *ifStatement = cast<IfStmt>(statement_from_it);
            //loop over children THEN wrap with the macro...
            for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
                modify_statements(rewriter,*it,f);
            }
            wrap_with_macro(ifStatement->getCond(), rewriter, " /*If*/ BOOLEXP", false,false); //wrap around whole expressiom
            return;
        }
        else if (isa<ParenExpr>(*statement_from_it)) {
            ParenExpr *paren = cast<ParenExpr>(statement_from_it);
            if (paren->isLValue())
                wrap_with_macro(paren, rewriter, " LVALUE_PAREN", true,true);
            else
                wrap_with_macro(paren, rewriter, " RVALUE_PAREN", true,true);
        }
         else if (isa<DeclStmt>(*statement_from_it)) {
            DeclStmt *declStatement = cast<DeclStmt>(statement_from_it);
             
             //declStatement->getDeclGroup().
            if (declStatement->isSingleDecl()) {
                Decl* d = declStatement->getSingleDecl();
                
                //d
                d->getDeclKindName();
                //rewriter->InsertTextAfter(d->getLocStart(), " /*DECL*/ ");
                //wrap_with_macro(, rewriter, "RHS", true,true);
            }
             
             return; //ignore decl children
        } else if (isa<CharacterLiteral>(*statement_from_it) || isa<IntegerLiteral>(*statement_from_it) || isa<StringLiteral>(*statement_from_it) || isa<CXXBoolLiteralExpr>(*statement_from_it) || isa<FloatingLiteral>(*statement_from_it)) {
             return;
        }
        else if (isa<ConditionalOperator>(*statement_from_it)) {
            ConditionalOperator *dre = cast<ConditionalOperator>(statement_from_it);
            return; //ignore conditional operator (ternary)
        }
        else if (isa<UnaryOperator>(*statement_from_it)) {
            UnaryOperator *dre = cast<UnaryOperator>(statement_from_it);
            //wrap_with_macro(dre->getSubExpr(), rewriter, " /*Unary*/", true,false); //should be true,false?
            if (dre->isArithmeticOp()) return;
            
            return;
        }
        else if (isa<BinaryOperator>(*statement_from_it)) {
            BinaryOperator *dre = cast<BinaryOperator>(statement_from_it);
            if (dre->getOpcodeStr() == ",") return; //ignore comma operator
            if (isa<CXXNewExpr>(dre->getRHS())) return; //don't handle initilisations
            if (!dre->isAssignmentOp()) {
                wrap_with_macro(dre->getLHS(), rewriter, " /*BinaryOp*/ LHS", true,true);//false);
            }
            else {
            //wrap whole assignment
                wrap_with_macro(dre, rewriter, " /*Assignment*/", true,true);
            }
            modify_statements(rewriter,dre->getRHS(),f);
            return;
        }
        else if (isa<CXXOperatorCallExpr>(*statement_from_it)) {
            CXXOperatorCallExpr *dre = cast<CXXOperatorCallExpr>(statement_from_it);
            
            return;
            for (int i=0; i<dre->getNumArgs(); i++) {
                Expr* arg = dre->getArg(i);
            if (isa<CXXNewExpr>(arg)) return;
            }
            
            
            
            for (int i=0; i<dre->getNumArgs(); i++) {
                Expr* arg = dre->getArg(i);
                if (arg == NULL) continue;
                modify_statements(rewriter,arg,f);
                
            }
            return;
            
        }
        else if (isa<DeclRefExpr>(*statement_from_it)) {
           DeclRefExpr *dre = cast<DeclRefExpr>(statement_from_it);
            if (dre->isRValue()) {
                wrap_with_macro(dre, rewriter, " RHS", true,true);
                
            } else if ( dre->isXValue()) {
               
                insert_before_after(dre,rewriter," /* x value declref! */ "," /* end x value*/ ", true);
                wrap_with_macro(dre, rewriter, " RHS", true,true);
            } else if ( dre->isLValue()) {
                std::ostringstream d;
                d << " /* LValue " << dre->getStmtClassName() << " " << dre->getDecl()->getNameAsString() << "*/ ";
                wrap_with_macro(dre, rewriter, " LHS", true,true);//false);
            }
            return;
        }
    
        else if (isa<CallExpr>(*statement_from_it) || isa<CXXMemberCallExpr>(*statement_from_it)) {
            CallExpr *dre = cast<CallExpr>(statement_from_it);
            if (!(dre->getCallReturnType()->isVoidType()))
            {
                if (dre->getCallReturnType()->isAtomicType())
                    wrap_with_macro(dre, rewriter, " CALL_R_ATOMIC", true,true); //should be true,false?
                else if (dre->isXValue())
                    wrap_with_macro(dre, rewriter, " CALLX", true,true); //should be true,false?
                else
                    wrap_with_macro(dre, rewriter, " CALLR", true,true); //should be true,false?
                
            }
            else
            insert_before_after(dre,rewriter," (CALL( ",")) ",true);
            //if (dre->getDirectCallee()->getNameAsString() == "free") return;
            
            
            for (int i=0; i<dre->getNumArgs(); i++) {
                Expr* arg = dre->getArg(i);
                
                handle_call_argument(arg,rewriter);
            }
            return;
        }
        else if (isa<ReturnStmt>(*statement_from_it)) {
            ReturnStmt *ret = cast<ReturnStmt>(statement_from_it);
            
            
            
            //
            //modify_statements(rewriter,ret->getRetValue()->IgnoreImplicit(),f);
            for(StmtIterator it = ret->child_begin(); it != ret->child_end(); ++it) {
                if (it == ret->child_begin()) continue;
                if (!isa<ImplicitCastExpr>(*it) && isa<Expr>(*it) )
                //modify_statements(rewriter,*it,f);
                handle_call_argument(cast<Expr>(*it),rewriter);
            }
            //ret->getRetValue()->getStmtClassName();
            //wrap_with_macro(cast<Expr>(*ret->child_begin()), rewriter, "RETURN_VAL", true);
            
            if (c_file) {
                
                if (ret->getLocEnd().isInvalid()) return;
                
                
                insert_before_after(ret, rewriter, " {alang_pop_ex(inst_func_db); ", "", true);
                SourceLocation end = clang::Lexer::getLocForEndOfToken(ret->IgnoreImplicit()->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
                if (rewriter->isRewritable(end) && end.isValid() )
                    rewriter->InsertTextAfterToken(end, "}");
                else
                    ret->dumpColor();
            }
            
            return;
        }
        else if (isa<MemberExpr>(*statement_from_it)) {
            //MemberExpr *dre = cast<MemberExpr>(statement_from_it);
            
            //insert_before_after(dre,rewriter," MEMBER_EXPR(( "," )) ",false);
            return;
            
        }
        else if (isa<ExprWithCleanups>(*statement_from_it)) {
            ExprWithCleanups *dre = cast<ExprWithCleanups>(statement_from_it);
            if (isa<CallExpr>(dre->getSubExpr()))
                wrap_with_macro(dre->getSubExpr(), rewriter, "ExprWithCleanupsCall", true,true); //should be true,true?
            else wrap_with_macro(dre, rewriter, dre->getStmtClassName(), true,true); //should be true,true?
            
        }
        /*else if (isa<ForStmt>(*statement_from_it)) {
            if (c_file) return; //skip for statements in c for now
        }*/ /*else if (isa<WhileStmt>(*statement_from_it)) {
            if (c_file) return; //skip while statements in c for now
        }*/
        else if (isa<Expr>(*statement_from_it)) {
            Expr *dre = cast<Expr>(statement_from_it);
            
        wrap_with_macro(dre, rewriter, dre->getStmtClassName(), true,true); //should be true,true?
        }
        else {
            //not an expression so its an unhandled statement
        }
    
    
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        modify_statements(rewriter,*it,f);
    }
   
}


/*
 Add the segfault handler to the main method (this doesn't work for libraries (since they don't have a main method)
 */
bool modify_main_function(Stmt *s, Rewriter* rewriter) {
    if (c_file) return true;
    //Add the segfault handler to the start of the main function
    SourceLocation start_of_stmts = getBeginningOfStatement(s,rewriter);
    rewriter->InsertTextAfterToken(start_of_stmts, " SEGFAULTHANDLE ");
    return true;
}


bool MyRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f)
{
    
    
    if (f->hasBody())
    {
        current_function = f;
        // First check if we are in the cpp file passed to the compiler
        SourceLocation sl = f->getLocation();
        if (rewriter.getSourceMgr().getFileID(sl) !=
            rewriter.getSourceMgr().getMainFileID()) {
            return true; //we are not in the main cpp file
        }
        
        if (f->isImplicit()) return true;
        if (f->isOverloadedOperator()) return true;
        if (f->isTrivial()) return true;
        if (isa<CXXDestructorDecl>(f)) return true;
        if (isa<CXXConstructorDecl>(f)) return true;
        if (f->isInlined()) return true;
        if (f->hasAttrs()) return true; //ATTR SEGFAULTS due to length
        
        
        
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
        
        SourceLocation start_of_func_name = clang::Lexer::GetBeginningOfToken(dni.getLocStart(), rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        std::string func_args_string = get_location_to_string(rewriter, &rewriter.getSourceMgr(), end_of_func_name, start_of_stmts);
        
        rewriter.setSourceMgr(rewriter.getSourceMgr(), rewriter.getLangOpts());
        
        // Point to start of function name
        SourceLocation start_of_function_name_token = dni.getLoc();
        std::string new_function_name = fname+"_orig";
        llvm::errs() << fname << "\n";
        //int length_of_function_name = get_length_of_token_at_location(start_of_function_name_token, this->rewriter);
        
        std::string proper_func_name = get_location_to_string(rewriter, &rewriter.getSourceMgr(), /*sr.getBegin()*/start_of_func_name, end_of_func_name);
        llvm::errs() << proper_func_name << "\n";
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        llvm::errs() << "END" << "\n";
        
        std::string whole_func = get_location_to_string(rewriter, &rewriter.getSourceMgr(), start_of_stmts, END);
        llvm::errs() << whole_func << "\n";
        
        /*
         Do all modifications after this
         */
        modify_statements(&rewriter,s,f);
        
        std::ostringstream params_to_log;
        params_to_log << "{";
        /* Log each argument to this function */
        for (int argument=0; argument< f->getNumParams(); argument++) {
             ParmVarDecl *pvd = f->getParamDecl(argument);
            if (pvd->isInvalidDecl()) continue;
            if (pvd->getNameAsString() == "") continue;
            
            
            int line = rewriter.getSourceMgr().getPresumedLineNumber(pvd->getLocStart());
            int beg = rewriter.getSourceMgr().getPresumedColumnNumber(pvd->getLocStart());
            int e = rewriter.getSourceMgr().getPresumedColumnNumber(pvd->getLocEnd());
            
            std::string log_param = handle_type(pvd->getType(), &rewriter,false);
            if (log_param == "" || log_param == "Void") {log_param = "LOGPARAMETER";}
            
            params_to_log << " "<<log_param<<"(" << line << "," << beg << "," << e << ",";
            params_to_log << pvd->getNameAsString() << "); ";
        }
        params_to_log << "} ";
        
        
        /*
         Now write final output
         */
        std::string proper_filename = rewriter.getSourceMgr().getFilename(f->getLocation());
        
        std::ostringstream debug_version_of_function;
        if (c_file) {
        //c version
            
            debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n static int alang_execution_number=0;\n static CStaticFunctionData* ali_function_db = NULL; if (ali_function_db == NULL) ali_function_db = new_CStaticFunctionData(\"" << proper_filename << "_" << fname << "\", __LINE__, __FILE__);  if (!ALI_GLOBAL_DEBUG || NO_INSTRUMENT || alang_execution_number > ALI_GLOBAL_MAX_EX || (ALI_EXE_PER_FRAME <= 0 && alang_execution_number > ALI_GLOBAL_MIN_EX) ) \n #endif \n";
            debug_version_of_function << " " << whole_func; //non modified version
            debug_version_of_function << "\n #if NO_INSTRUMENT == false \n else {void* inst_func_db = alang_push_ex(ali_function_db); alang_execution_number++; \n";
        } else {
        //cpp version
        debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(\"" << proper_filename << "_" << fname << "\", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) \n #endif \n";
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << "\n #if NO_INSTRUMENT == false \n else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); \n";
        }
        debug_version_of_function << params_to_log.str();
        
        rewriter.InsertTextAfter(start_of_stmts, debug_version_of_function.str());
        rewriter.InsertTextAfter(END, "\n alang_pop_ex(inst_func_db);}\n#endif \n");
        rewriter.InsertTextAfter(END, "}");
        
    }
    
    return true; // returning false aborts the traversal
}