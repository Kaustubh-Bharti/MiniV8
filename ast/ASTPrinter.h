#pragma once

#include "AST.h"

void printAST(
    const Expression* expression,
    int indent = 0
);

void printAST(
    const IfStatement* statement,
    int indent = 0
);

void printAST(
    const WhileStatement* statement,
    int indent = 0
);

void printAST(
    const FunctionDeclaration* statement,
    int indent = 0
);