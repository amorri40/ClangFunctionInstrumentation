//
//  MyASTConsumer.cpp
//  ClangInstrumentationPlugin
//
//  Created by Alasdair Morrison on 15/06/2013.
//  Copyright (c) 2013 Alasdair Morrison. All rights reserved.
//

#include "MyASTConsumer.h"
using namespace clang;

bool MyASTConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
    typedef DeclGroupRef::iterator iter;
    
    for (iter b = d.begin(), e = d.end(); b != e; ++b)
    {
        rv.TraverseDecl(*b);
    }
    
    return true; // keep going
}