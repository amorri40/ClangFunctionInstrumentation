//
//  MyRecursiveASTVisitor.cpp
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include "MyRecursiveASTVisitor.h"

bool MyRecursiveASTVisitor::VisitFunctionDecl(FunctionDecl *f)
{
    fprintf(stderr, "visiting Function: %s %p\n", std::string(f->getName()).c_str(), (void*)f);
    f->dumpColor();
    //DeclarationName new_name =
    //f->setDeclName (DeclarationName N)
    //f->getLocation();
    
    if (f->hasBody())
    {
        printf("%s\n", "Function has a body!");
        SourceRange sr = f->getSourceRange();
        Stmt *s = f->getBody();
        
        
        
        // Get name of function
        DeclarationNameInfo dni = f->getNameInfo();
        DeclarationName dn = dni.getName();
        std::string fname = dn.getAsString();
        llvm::errs() << "Function name:" << fname << "\n";
        
        // Point to start of function declaration
        SourceLocation ST = sr.getBegin();
        
        // Add comment
        char fc[256];
        //sprintf(fc, "// Begin function %s returning %s\n", fname.data(), ret.data());
        Rewrite.InsertText(ST, fc, true, true);
        
        if (f->isMain())
            llvm::errs() << "Found main()\n";
        
        SourceLocation END = s->getLocEnd().getLocWithOffset(1);
        sprintf(fc, "\n// End function %s\n", fname.data());
        Rewrite.InsertText(END, fc, true, true);
    }
    printf("End of: %s\n", "VisitFunctionDecl");
    return false;//true; // returning false aborts the traversal
}