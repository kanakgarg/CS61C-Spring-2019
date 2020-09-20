#include "instructions.h"
#include <stdio.h>
#include <string.h>

const char* regNames[] = {"x0", "fp", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t0", "t1", "t2", "t3", "t4", "t5", "sp", "ra", "gp", "a0", "a1"};

void emitLABEL(char* label) {
	printf("%s:\n", label);
}

void emitCOMMENT(char* comment) {
	printf("# %s\n", comment);
}

void emitADDI(enum reg rd, enum reg rs, int imm) {
	printf("addi %s %s %d\n", regNames[rd], regNames[rs], imm);
}

void emitADD(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("add %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitSUB(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("sub %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitMUL(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("mul %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitDIV(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("div %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitOR(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("or %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitXOR(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("xor %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitXORI(enum reg rd, enum reg rs1, int imm) {
	printf("xori %s %s %d\n", regNames[rd], regNames[rs1], imm);
}

void emitAND(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("and %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitSW(enum reg rs1, int imm, enum reg rs2) {
	printf("sw %s %d(%s)\n", regNames[rs1], imm, regNames[rs2]);
}

void emitLW(enum reg rd, int imm, enum reg rs) {
	printf("lw %s %d(%s)\n", regNames[rd], imm, regNames[rs]);
}

void emitSB(enum reg rs1, int imm, enum reg rs2) {
	printf("sb %s %d(%s)\n", regNames[rs1], imm, regNames[rs2]);
}

void emitSEQZ(enum reg rd, enum reg rs) {
	printf("seqz %s %s\n", regNames[rd], regNames[rs]);
}

void emitSNEZ(enum reg rd, enum reg rs) {
	printf("snez %s %s\n", regNames[rd], regNames[rs]);
}

void emitSLT(enum reg rd, enum reg rs1, enum reg rs2) {
	printf("slt %s %s %s\n", regNames[rd], regNames[rs1], regNames[rs2]);
}

void emitSLTIU(enum reg rd, enum reg rs1, int imm) {
  printf("sltiu %s %s %d\n", regNames[rd], regNames[rs1], imm);
}

void emitLA(enum reg rd, char* label) {
	printf("la %s %s\n", regNames[rd], label);
}

void emitMV(enum reg rd, enum reg rs) {
	printf("mv %s %s\n", regNames[rd], regNames[rs]);
}

void emitBEQ(enum reg rs1, enum reg rs2, char* label) {
	printf("beq %s %s %s\n", regNames[rs1], regNames[rs2], label);
}

void emitBEQZ(enum reg rs, char* label) {
	printf("beqz %s %s\n", regNames[rs], label);
}

void emitBNEZ(enum reg rs, char* label) {
	printf("bnez %s %s\n", regNames[rs], label);
}

void emitJAL(enum reg rs, char* label) {
	printf("jal %s %s\n", regNames[rs], label);
}

void emitJ(char* label) {
	printf("j %s\n", label);
}

void emitJR(enum reg rs) {
	printf("jr %s\n", regNames[rs]);
}

void emitECALL () {
  printf ("ecall\n");
}

void emitTEXT () {
  printf (".text\n");
}

void emitDATA () {
  printf (".data\n");
}

void emitGLOBLMAIN () {
  printf (".globl main\n");
}

void emitWORDVALUE (int64_t value) {
  printf (".word %ld\n", value);
}

void emitWORDLABEL (char *label) {
  printf (".word %s\n", label);
}

void emitSTRING (char *string, size_t size) {
  size_t end = strlen (string);
  for (int i = 0; i < end; i++) {
    printf (".byte %d\n", string[i]);
  }
  for (int i = end; i < size; i++) {
    printf (".byte 0\n");
  }
}
