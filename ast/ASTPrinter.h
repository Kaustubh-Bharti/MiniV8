#pragma once

#include "AST.h"

void printAST(
    const Expression* expression,
    int indent = 0
);

void printAST(
    const Statement* statement,
    int indent = 0
);