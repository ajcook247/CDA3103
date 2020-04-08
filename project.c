#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	switch (ALUControl)
	{
		// ADD
		case '0':							// NOTE: Can't treat '000' as a single character
			*ALUresult = A + B;
			break;

		// SUBTRACT
		case '1':
			*ALUresult = A - B;
			break;

		// SLT (SIGNED)
		case '2':
			if (A < B)
				ALUresult = 1;
			else
				ALUresult = 0;
			break;

		// SLT (UNSIGNED)
		case '3':
			if (A < B)
				ALUresult = 1;
			else
				ALUresult = 0;
			break;

		// AND
		case '4':
			*ALUresult = A & B;
			break;

		// OR
		case '5':
			*ALUresult = A | B;
			break;

		// SLL B BY 16 BITS
		case '6':
			*ALUresult = B << 16;
			break;

		// NOT A
		case '7':
			*ALUresult = ~A;
			break;

		default:
			break;
	}

	// Set the value of Zero depending on ALUresult
	if (*ALUresult == 0)
		*Zero = '1';
	else
		*Zero = '0';
}

/* instruction fetch */
/* 10 Points */

/*
1. Fetch the instruction addressed by PC from Mem and write it to instruction.
2. Return 1 if a halt condition occurs; otherwise, return 0.
*/
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if (PC % 4 != 0)
		return 1; // Word not aligned

	*instruction = Mem[PC >> 2];
	return 0;
}


/* instruction partition */
/* 10 Points */

/*
1. Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
2. Read line 41 to 47 of spimcore.c for more information.
*/
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */

/*
1. Decode the instruction using the opcode (op).
2. Assign the values of the control signals to the variables in the structure controls
(See spimcore.h file).
		The meanings of the values of the control signals:
		For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0 means
		that disabled, 2 means “don’t care”.
		For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the
		selected path of the multiplexer; 2 means “don’t care”.
		The following table shows the meaning of the values of ALUOp
*/
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */

// Assign the sign-extended value of offset to extended_value.
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */

/*
1. Apply ALU operations on data1, and data2 or extended_value (determined by ALUSrc).
2. The operation performed is based on ALUOp and funct.
3. Apply the function ALU(…).
4. Output the result to ALUresult.
5. Return 1 if a halt condition occurs; otherwise, return 0.
*/
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */

/*
1. Base on the value of MemWrite or MemRead to determine memory write
operation or memory read operation.
2. Read the content of the memory location addressed by ALUresult to memdata.
3. Write the value of data2 to the memory location addressed by ALUresult.
4. Return 1 if a halt condition occurs; otherwise, return 0.
*/
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */

// 1. Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3.
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */

// 1. Update the program counter (PC).
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}
