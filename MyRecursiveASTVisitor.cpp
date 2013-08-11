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

FunctionDecl * current_function;
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

inline void insert_before_after(Expr *st, Rewriter* rewriter, std::string before, std::string after, bool beforeEnd) {
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
inline std::string handle_type(Expr *st,Rewriter* rewriter){
    QualType qt = st->getType();
    //ExprValueKind evk = st->getValueKind();
    const Type* tp = qt.getTypePtr();
    if (tp->isVoidType()) return "Void";
    if (tp->isPointerType()) {
        //tp=(tp->getPointeeCXXRecordDecl()->getTypeForDecl());
        qt = tp->getPointeeType();
        tp = qt.getTypePtr();
        return "";
    }
    if (tp->hasPointerRepresentation()) {
        //RecordDecl* rd = qt->getAsCXXRecordDecl();
        
        additional_file_content << "\n#define PrintPointerRepr_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    if (tp->isDependentType()) {
        //RecordDecl* rd = qt->getAsCXXRecordDecl();
        additional_file_content << "\n#define PrintDependentType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    if (tp->isVectorType()) {
        additional_file_content << "\n#define PrintVectorType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    if (tp->isAtomicType()) {//tp->isScalarType()) {
        //ScalarTypeKind stk = tp->getScalarTypeKind();
        //Type::STK_
        
        additional_file_content << "\n#define PrintAtomicType_"<< tp->getTypeClassName() << " 0\n" ;
        return "Atomic";
    }
    if (tp->isClassType() || tp->isStructureType()) {
        CXXRecordDecl* rd = tp->getAsCXXRecordDecl();
        //rd->getTagKind()
        NamedDecl* nd = rd->getUnderlyingDecl();
        
        
        if (nd->getNameAsString() == "basic_string") {
        return "StringType";
        }
        
        if (rd->isDependentType()) return ""; //don't handle template classes yet
        if (rd->isDependentContext()) return "";
        if (rd->isTemplateDecl()) return "";
        if (isa<ClassTemplateSpecializationDecl>(rd)) return "";
        
        
        std::pair<  clang::CXXRecordDecl::conversion_iterator, clang::CXXRecordDecl::conversion_iterator > p = rd->getVisibleConversionFunctions();
        for (CXXRecordDecl::conversion_iterator it=p.first; it!=p.second; it++) {
            CXXConversionDecl* conver = cast<CXXConversionDecl>(*it);
            //conver->getConversionType()
            QualType q = conver->getResultType();
            
            //if (q.getTypePtr()->isAnyCharacterType()) {
            //    additional_file_content << "\n// Conver:"+conver->getNameAsString()+"\n";
            //}
        }
        
        // loop through the methods
        for (CXXRecordDecl::method_iterator it=rd->method_begin(); it!=rd->method_end(); it++) {
            CXXMethodDecl* meth = cast<CXXMethodDecl>(*it);
            //additional_file_content << "\n// Meth:"+meth->getNameAsString()+"\n";
        }
        
        if (!rd->field_empty()) {
            std::ostringstream log_method_body, whole_log_data;
            
            
            
            log_method_body <<"{ std::ostringstream v; v";
            // loop through the fields
            for (CXXRecordDecl::field_iterator it=rd->field_begin(); it!=rd->field_end(); it++) {
                FieldDecl* fie = cast<FieldDecl>(*it);
                
                if (fie->getVisibility()== DefaultVisibility) {
                    
                    if (fie->getType()->isIntegralOrEnumerationType())
                        log_method_body << " << val."+fie->getNameAsString();
                    /*
                     T& log_builtin(int line_num, int start_loc, int end_loc, T& val) {
                     std::ostringstream v;
                     v << val;
                     ali_clang_add_to_map(typeid(T).name(),v.str()) return val;
                     }
                     */
                }
            }
            log_method_body << "; inst_func_db.log_custom(line_num, start_loc, end_loc, typeid(val).name(),v.str()); return val;}\n";
            
            
            whole_log_data << "#ifndef PrintClassType_"<<nd->getNameAsString()<<"\n";
            whole_log_data << "__attribute__((weak)) std::ostream & operator<< (std::ostream &out, "<<nd->getQualifiedNameAsString()<<" const &t) {return out;}";
            whole_log_data << "\ntemplate <class T> T& log_builtin"<< nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, T& val) "<< log_method_body.str();
            whole_log_data << "\ntemplate <class T> const T& log_builtin"<< nd->getNameAsString() << "(ali_clang_plugin_runtime::InstrumentFunctionDB& inst_func_db, int line_num, int start_loc, int end_loc, const T& val) "<< log_method_body.str();
            whole_log_data << "#define PrintClassType_"<< nd->getNameAsString() << "(line,beg,end,arg) (stdlogger,log_builtin"<< nd->getNameAsString() << "(inst_func_db,line,beg,end,(arg)))\n";
            whole_log_data << "\n#endif\n";
            SourceLocation begining = clang::Lexer::GetBeginningOfToken(current_function->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
            bool b = rewriter->InsertTextBefore(begining, whole_log_data.str());
            
            return "PrintClassType_"+ nd->getNameAsString();
        }
        
        
        return "";
    }
    if (tp->isEnumeralType()) {
        additional_file_content << "\n#define PrintEnumType_"<< tp->getTypeClassName() << " 0\n" ;
        return "EnumLog";
    }
    
    if (tp->isStructureType()) {
        const RecordType* rt = tp->getAsStructureType();
        RecordDecl* rd = rt->getDecl();
        
        
        additional_file_content << "\n#define PrintStructType_"<< rd->getNameAsString() << " 0\n" ;
        return "";//return "EnumLog";
    }
    
    if (tp->isUnionType()) {
        additional_file_content << "\n#define PrintUnionType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";//return "EnumLog";
    }
    if (tp->isReferenceType()) {
        additional_file_content << "\n#define PrintReferenceType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";//return "EnumLog";
    }
    if (tp->isPlaceholderType()) {
        additional_file_content << "\n#define PrintPlaceholderType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";
    }
    
    /*if (tp->isArithmeticType()) {
        additional_file_content << "\n#define PrintArithmeticType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";//return "EnumLog";
    }*/
    
    if (tp->isIntegralOrEnumerationType()) {
        
        //additional_file_content << "\n#define PrintIntegralOrEnumType_"<< tp->getTypeClassName() << " 0\n" ;
        return "IntegralOrEnumType";//return "EnumLog";
    }
    
    if (tp->isAggregateType()) {
        additional_file_content << "\n#define PrintAggregateType_"<< tp->getTypeClassName() << " 0\n" ;
        return "";//return "EnumLog";
    }
    
    
    if (tp->isRecordType()) {
        //const RecordType* struc = tp->getAsStructureType();
        //RecordDecl* rd = struc->getDecl();
        //TagDecl* td = rd->getDefinition();
        
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

inline void wrap_with_macro(Expr *st, Rewriter* rewriter, std::string macro_name, bool beforeEnd, bool checktype) {
    SourceLocation begining = clang::Lexer::GetBeginningOfToken(st->getLocStart(), rewriter->getSourceMgr(), rewriter->getLangOpts());
    SourceLocation end = clang::Lexer::getLocForEndOfToken(st->getLocEnd(),0, rewriter->getSourceMgr(), rewriter->getLangOpts());
    
    int line = rewriter->getSourceMgr().getPresumedLineNumber(begining);
    int beg = rewriter->getSourceMgr().getPresumedColumnNumber(begining);
    int e = rewriter->getSourceMgr().getPresumedColumnNumber(end);
    
    if (checktype) {
    std::string typest = handle_type(st,rewriter);
    if (typest ==  "Void") return;
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
    
    if (arg->isLValue())
        insert_before_after(arg, rewriter, " CALL_LVALUE_ARG((", ")) ", true);
    else if (arg->getType()->hasIntegerRepresentation()) {
        insert_before_after(arg, rewriter, " CALL_ARG((", ")) ", true);
    } else if (arg->getType()->isBuiltinType()) {
        insert_before_after(arg, rewriter, " CALL_ARG((", ")) ", true);
    } else if (arg->getType()->isPointerType()) {
        if (arg->getType().isCanonical())
          insert_before_after(arg, rewriter, " CALL_ARG((", ")) ", true);
        else
            insert_before_after(arg, rewriter, " ARG_UNKNOWN((", ")) ", true);
    }
    else if (arg->getType()->hasPointerRepresentation()) {
        insert_before_after(arg, rewriter, " CALL_ARG(( ", ")) ", true);
    } else {
        insert_before_after(arg, rewriter, " ARG_UNKNOWN((", ")) ", true);
    }
}

void modify_statements(Rewriter* rewriter, Stmt *s, FunctionDecl *f) {
    
    current_function = f;
    
    Stmt* statement_from_it = s;//*it;
        
        if (statement_from_it == NULL) return;
        if (s->getLocStart().isInvalid()) return;
        
        if (isa<CaseStmt>(*statement_from_it)) {
            CaseStmt *caseStatement = cast<CaseStmt>(statement_from_it);
            modify_statements(rewriter,caseStatement->getSubStmt(),f);
            return;
        }
        else if (isa<CXXDeleteExpr>(*statement_from_it) || isa<CXXNewExpr>(*statement_from_it) || isa<CXXTemporaryObjectExpr>(*s)) return;
        else if (isa<ImplicitCastExpr>(*statement_from_it)) {
            ImplicitCastExpr *implicitcast = cast<ImplicitCastExpr>(statement_from_it);
            //wrap_with_macro(implicitcast, rewriter, implicitcast->getCastKindName(), true);
            std::ostringstream stringstr;
            
            if (implicitcast->getSubExpr()->isCXX11ConstantExpr(f->getASTContext())) return; //static constant data can be optimised out
            
            if (implicitcast->getCastKind()== CK_LValueToRValue) {
                stringstr << " /*(" << implicitcast->getType().getAsString() << ")*/ " << implicitcast->getCastKindName();
            }
            else {
            stringstr << " /*(" << implicitcast->getType().getAsString() << ")*/ " << implicitcast->getCastKindName();
            }
            wrap_with_macro(implicitcast, rewriter, stringstr.str(), true,false);
            
            
        }
        else if (isa<IfStmt>(*statement_from_it)) {
            IfStmt *ifStatement = cast<IfStmt>(statement_from_it);
            //wrap_with_macro(ifStatement->getCond(), rewriter, " /*If*/ BOOLEXP", true);
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
                //wrap_with_macro(d->, rewriter, "RHS", true);
            }
             return; //ignore decl children
        } else if (isa<CharacterLiteral>(*statement_from_it) || isa<IntegerLiteral>(*statement_from_it) || isa<StringLiteral>(*statement_from_it) || isa<CXXBoolLiteralExpr>(*statement_from_it)) {
             return;
        }
        else if (isa<UnaryOperator>(*statement_from_it)) {
            UnaryOperator *dre = cast<UnaryOperator>(statement_from_it);
           // wrap_with_macro(dre->getSubExpr(), rewriter, " /*Unary*/", false);
           // return;
        }
        else if (isa<BinaryOperator>(*statement_from_it)) {
            BinaryOperator *dre = cast<BinaryOperator>(statement_from_it);
            if (dre->getOpcodeStr() == ",") return; //ignore comma operator
            if (isa<CXXNewExpr>(dre->getRHS())) return; //don't handle initilisations
            if (!dre->isAssignmentOp())
                 wrap_with_macro(dre->getLHS(), rewriter, " /*BinaryOp*/ LHS", true,false);
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
                /*if (isa<IntegerLiteral>(arg) || isa<StringLiteral>(arg) || isa<ImplicitCastExpr>(arg)) {
                    return;
                }
                
                
                if (arg->isLValue())
                {
                    if (dre->getOperator() == clang::OO_Equal) continue;
                    if (dre->getOperator() == clang::OO_LessLess) continue;
                   
                }
                
                //else if (arg->isRValue()) {
                //insert_before_after(dre,rewriter,arg->getStmtClassName(),arg->getStmtClassName(), true);
                   
                    if (arg->getType().isCanonical())
                        wrap_with_macro(arg, rewriter, " OPERATOR_RHS_ARG_CANONICAL", true);
                    else
                        wrap_with_macro(arg, rewriter, " OPERATOR_RHS_ARG_NOTCANONICAL", true);
                    */
               // }
            }
            return;
            
        }
        else if (isa<DeclRefExpr>(*statement_from_it)) {
           DeclRefExpr *dre = cast<DeclRefExpr>(statement_from_it);
            if (dre->isRValue()) {
                wrap_with_macro(dre, rewriter, " RHS", true,true);
                
            } else if ( dre->isXValue()) {
               
                insert_before_after(dre,rewriter," /* x value declref! */ "," /* end x value*/ ", true);
            } else if ( dre->isLValue()) {
                std::ostringstream d;
                d << " /* LValue " << dre->getStmtClassName() << " " << dre->getDecl()->getNameAsString() << "*/ ";
                
            }
            return;
        }
//        else if (isa<CXXMemberCallExpr>(*statement_from_it)) {
//            CXXMemberCallExpr *dre = cast<CXXMemberCallExpr>(statement_from_it);
//            
//            insert_before_after(dre,rewriter," MEMBER_CALL(( "," )) ",false);
//            for (int i=0; i<dre->getNumArgs(); i++) {
//                Expr* arg = dre->getArg(i);
//                handle_call_argument(arg,rewriter);
//            }
//            return; // don't want to parse the memberExpr!!
//        }
    
        else if (isa<CallExpr>(*statement_from_it) || isa<CXXMemberCallExpr>(*statement_from_it)) {
            CallExpr *dre = cast<CallExpr>(statement_from_it);
            if (!(dre->getCallReturnType()->isVoidType()) && !dre->isBuiltinCall())
            {
                if (dre->getCallReturnType()->isAtomicType())
                    wrap_with_macro(dre, rewriter, " CALL_R_ATOMIC", false,true);
                else if (dre->isXValue())
                    wrap_with_macro(dre, rewriter, " CALLX", false,true);
                else
                    wrap_with_macro(dre, rewriter, " CALLR", false,true);
                
            }
            else
            insert_before_after(dre,rewriter," (CALL( ",")) ",true);
            
            for (int i=0; i<dre->getNumArgs(); i++) {
                Expr* arg = dre->getArg(i);
                handle_call_argument(arg,rewriter);
            }
            return;
        }
        else if (isa<ReturnStmt>(*statement_from_it)) {
            //ReturnStmt *ret = cast<ReturnStmt>(statement_from_it);
            //ret->getRetValue()->getStmtClassName();
            //wrap_with_macro(cast<Expr>(*ret->child_begin()), rewriter, "RETURN_VAL", true);
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
                wrap_with_macro(dre->getSubExpr(), rewriter, "ExprWithCleanupsCall", false,true);
            else wrap_with_macro(dre, rewriter, dre->getStmtClassName(), false,true);
            
        }
        else if (isa<Expr>(*statement_from_it)) {
            Expr *dre = cast<Expr>(statement_from_it);
            
        wrap_with_macro(dre, rewriter, dre->getStmtClassName(), false,true);
        }
        else {
//            std::ostringstream class_name;
//            class_name << " /*" << statement_from_it->getStmtClassName() << "*/ "; //should this not be statement_from_id
//            std::string beg = class_name.str();
//            class_name.str("");
//            class_name << " /* END " << statement_from_it->getStmtClassName() << "*/ ";
//            
//            if (statement_from_it->getLocStart().isInvalid()) return;
        
        }
    
    
    for(StmtIterator it = s->child_begin(); it != s->child_end(); ++it) {
        modify_statements(rewriter,*it,f);
    }
   
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


bool MyRecursiveASTVisitor::VisitRecordDecl(clang::RecordDecl *rd) {//::VisitTypeDecl(clang::TypeDecl *td){
    //rd->dumpColor();
    /*if (rd->isAnonymousStructOrUnion()) return true;
    if (rd->isFromASTFile()) return true;
    if (rd->isHidden()) return true;
    if (rd->isImplicit()) return true;
    
    //if (!rd->isReferenced()) return true;
    //if (!rd->isUsed()) return true;
    if (rd->isCompleteDefinition() || rd->isBeingDefined()) {
        if (rd->isFileContext()) {
            additional_file_content << "\n#define Print"<< rd->getNameAsString() << " 0\n" ;
        } else {
            additional_file_content << "\n#define NotFilePrint_"<< rd->getNameAsString() << "_" << rd->getKindName() << "(arg) arg\n" ;
        }
    }
    
    */
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
        
        if (f->isImplicit()) return true;
        if (f->isOverloadedOperator()) return true;
        if (f->isTrivial()) return true;
        if (isa<CXXDestructorDecl>(f)) return true;
        if (isa<CXXConstructorDecl>(f)) return true;
        if (f->isInlined()) return true;
        
        
        
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
            
            params_to_log << " LOGPARAMETER(" << line << "," << beg << "," << e << ",";
            params_to_log << pvd->getNameAsString() << "); ";
        }
        params_to_log << "} ";
        
        
        /*
         Now write final output
         */
        std::string proper_filename = rewriter.getSourceMgr().getFilename(f->getLocation());
        
        std::ostringstream debug_version_of_function;
        debug_version_of_function << "{ \n #if NO_INSTRUMENT == false \n static ali_clang_plugin_runtime::StaticFunctionData ali_function_db(\"" << proper_filename << "_" << fname << "\", __LINE__, __FILE__);  if (!ali_clang_plugin_runtime::ALI_GLOBAL_DEBUG || NO_INSTRUMENT || ali_function_db.execution_number > ali_clang_plugin_runtime::ALI_GLOBAL_MAX_EX) \n #endif \n";
        debug_version_of_function << " " << whole_func;
        debug_version_of_function << "\n #if NO_INSTRUMENT == false \n else {ali_clang_plugin_runtime::InstrumentFunctionDB inst_func_db(&ali_function_db); \n";
        debug_version_of_function << params_to_log.str();
        
        
        rewriter.InsertTextAfter(start_of_stmts, debug_version_of_function.str());
        //rewriter.InsertTextAfter(END, "\n #if NO_INSTRUMENT == false \n } \n #endif \n");
        rewriter.InsertTextAfter(END, "\n}\n#endif \n");
        rewriter.InsertTextAfter(END, "}");
        
    }
    
    return true; // returning false aborts the traversal
}