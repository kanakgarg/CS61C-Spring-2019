#ifndef CGEN_H
#define CGEN_H

#include "dast.h"

void GenerateCode(DAST* dast, char* filename);

void processDefault(DAST* dast,  char* startLabel, char* endLabel);

void processConstTrue(DAST* dast,  char* startLabel, char* endLabel);

void processConstFalse(DAST* dast,  char* startLabel, char* endLabel);

void processConstInt(DAST* dast,  char* startLabel, char* endLabel);

void processConstNull(DAST* dast,  char* startLabel, char* endLabel);

void processExprBinaryAdd(DAST* dast,
                          
                          char* startLabel,
                          char* endLabel);

void processExprBinarySub(DAST* dast,
                          
                          char* startLabel,
                          char* endLabel);

void processExprBinaryMul(DAST* dast,
                          
                          char* startLabel,
                          char* endLabel);

void processExprBinaryDiv(DAST* dast,
                          
                          char* startLabel,
                          char* endLabel);

void processExprBinaryEq(DAST* dast,
                         
                         char* startLabel,
                         char* endLabel);

void processExprBinaryNotEq(DAST* dast,
                            
                            char* startLabel,
                            char* endLabel);

void processExprBinaryGTEq(DAST* dast,
                           
                           char* startLabel,
                           char* endLabel);

void processExprBinaryGT(DAST* dast,
                         
                         char* startLabel,
                         char* endLabel);

void processExprBinaryLTEq(DAST* dast,
                           
                           char* startLabel,
                           char* endLabel);

void processExprBinaryLT(DAST* dast,
                         
                         char* startLabel,
                         char* endLabel);

void processExprBinaryBitAnd(DAST* dast,
                             
                             char* startLabel,
                             char* endLabel);

void processExprBinaryBitOr(DAST* dast,
                            
                            char* startLabel,
                            char* endLabel);

void processExprBinaryBitXor(DAST* dast,
                             
                             char* startLabel,
                             char* endLabel);

void processExprBinaryLogicAnd(DAST* dast,
                               
                               char* startLabel,
                               char* endLabel);

void processExprBinaryLogicOr(DAST* dast,
                              
                              char* startLabel,
                              char* endLabel);

void processIfElse(DAST* dast,  char* startLabel, char* endLabel);

void processFor(DAST* dast,  char* startLabel, char* endLabel);

void processVarDecl(DAST* dast,  char* startLabel, char* endLabel);

void processReturn(DAST* dast,  char* startLabel, char* endLabel);

void processExprAssign(DAST* dast,  char* startLabel, char* endLabel);

void dispatch(DAST* dast,  char* startLabel, char* endLabel);

void dispatchLeft(DAST* dast,  char* startLabel, char* endLabel);

void processLeftDot(DAST* dast,  char* startLabel, char* endLabel);

void processLeftAarow(DAST* dast,  char* startLabel, char* endLabel);

void processLeftID(DAST* dast,  char* startLabel, char* endLabel);

void processLeftPren(DAST* dast,  char* startLabel, char* endLabel);

void processLeftDeref(DAST* dast,  char* startLabel, char* endLabel);

#endif
