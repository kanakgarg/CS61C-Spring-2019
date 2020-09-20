#ifndef INST_H
#define INST_H

#include <stdint.h>
#include <stddef.h>

enum reg {
	x0, FP, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, T0, T1, T2, T3, T4, T5, SP, RA, GP, A0, A1
};

void emitLABEL(char* label);

void emitCOMMENT(char* comment);

void emitADDI(enum reg rd, enum reg rs, int imm);

void emitADD(enum reg rd, enum reg rs1, enum reg rs2);

void emitSUB(enum reg rd, enum reg rs1, enum reg rs2);

void emitMUL(enum reg rd, enum reg rs1, enum reg rs2);

void emitDIV(enum reg rd, enum reg rs1, enum reg rs2);

void emitOR(enum reg rd, enum reg rs1, enum reg rs2);

void emitXOR(enum reg rd, enum reg rs1, enum reg rs2);

void emitXORI(enum reg rd, enum reg rs1, int imm);

void emitAND(enum reg rd, enum reg rs1, enum reg rs2);

void emitSW(enum reg rs1, int imm, enum reg rs2);

void emitLW(enum reg rd, int imm, enum reg rs);

void emitSB(enum reg rs1, int imm, enum reg rs2);

void emitSEQZ(enum reg rd, enum reg rs);

void emitSNEZ(enum reg rd, enum reg rs);

void emitSLT(enum reg rd, enum reg rs1, enum reg rs2);

void emitSLTIU(enum reg rd, enum reg rs1, int imm);

void emitLA(enum reg rd, char* label);

void emitMV(enum reg rd, enum reg rs);

void emitJAL(enum reg rd, char* label);

void emitBEQ(enum reg rs1, enum reg rs2, char* label);

void emitBEQZ(enum reg rs, char* label);

void emitBNEZ(enum reg rs, char* label);

void emitJAL(enum reg rs, char* label);

void emitJR(enum reg rs);

void emitJ(char* label);

void emitECALL ();

void emitTEXT ();

void emitDATA ();

void emitGLOBLMAIN ();

void emitWORDVALUE (int64_t value);

void emitWORDLABEL (char *label);

void emitSTRING (char *string, size_t size);

#endif
