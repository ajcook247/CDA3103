#include "spimcore.h"

/* FAILING:
		afaa0000	# sw $10, 0($29)        	Mem[sp+0] = r10	
		016a602a	# slt $12, $11, $10    		if(r11<r10) then r12 = 1	$t4 = 0		
*/

// Joshua Colston
// Noy Mizrachi
// Adam Cook

/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	switch (ALUControl)
	{
		// ADD
		case '0':
			*ALUresult = A + B;
			break;

		// SUBTRACT
		case '1':
			*ALUresult = A - B;
			break;

		// SLT (SIGNED)
		case '2':
			if (A < B)
				*ALUresult = 1;
			else
				*ALUresult = 0;
			break;

		// SLT (UNSIGNED)
		case '3':
			if (A < B)
				*ALUresult = 1;
			else
				*ALUresult = 0;
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
	if ((PC % 4) != 0)
		return 1; // Word not aligned

	if (PC < 0x0000 || PC > 0xffff)
		return 1; // out of bounds

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
	/*
		op,	// instruction [31-26]
		r1,	// instruction [25-21]
		r2,	// instruction [20-16]
		r3,	// instruction [15-11]
		funct,	// instruction [5-0]
		offset,	// instruction [15-0]
		jsec;	// instruction [25-0]
	*/

	// BitMaskOP represents first 6 bits, & with *op to keep
	/*
	int opMask 			= 0b11111100000000000000000000000000;
	int r1Mask 			= 0b00000011111000000000000000000000;
	int r2Mask 			= 0b00000000000111110000000000000000;
	int r3Mask 			= 0b00000000000000001111100000000000;
	int functMask 		= 0b11111100000000000000000000011111;
	int offsetMask 		= 0b11111100000000001111111111111111;
	int jsecMask 		= 0b00000011111111111111111111111111;

	*op = instruction & opMask;
	*r1 = instruction & r1Mask;
	*r2 = instruction & r2Mask;
	*r3 = instruction & r3Mask;
	*funct = instruction & functMask;
	*offset = instruction & offsetMask;
	*jsec = instruction & jsecMask;
	*/
	
	*op = (instruction & 0xfc000000) >> 26;
	*r1 = (instruction & 0x03e00000) >> 21;
	*r2 = (instruction & 0x001f0000) >> 16;
	*r3 = (instruction & 0x0000f800) >> 11;
	*funct = instruction & 0x0000003f;
	*offset = instruction & 0x0000ffff;
	*jsec = instruction & 0x03ffffff;
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
	/*
	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemtoReg;
	char ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;
	*/

	switch (op)
	{

		// 0x0 - r-type
		// 0x8 - addi
		// 0x23 - lw
		// 0x2b - sw
		// 0x0f - lui
		// 0x4 - branch
		// 0xa - slti
		// oxb - sltiu
		// 0x2 - jump

		// r-type
		case 0:
			controls->RegDst = '1';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '7';
			controls->MemWrite = '0';
			controls->ALUSrc = '0';
			controls->RegWrite = '1';
			break;

		// addi
		case 8:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '0';
			controls->MemWrite = '0';
			controls->ALUSrc = '1';
			controls->RegWrite = '1';
			break;

		// lw
		case 35:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '1';
			controls->MemtoReg = '1';
			controls->ALUOp = '0';
			controls->MemWrite = '0';
			controls->ALUSrc = '1';
			controls->RegWrite = '1';
			break;

		// sw
		case 43:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '0';
			controls->MemWrite = '1';
			controls->ALUSrc = '1';
			controls->RegWrite = '2';
			break;

		// lui
		case 15:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '6';
			controls->MemWrite = '0';
			controls->ALUSrc = '1';
			controls->RegWrite = '1';
			break;

		// branch
		case 4:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '1';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '1';
			controls->MemWrite = '0';
			controls->ALUSrc = '0';
			controls->RegWrite = '1';
			break;

		// slti
		case 10:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '2';
			controls->MemWrite = '0';
			controls->ALUSrc = '1';
			controls->RegWrite = '1';
			break;

		// sltiu
		case 11:
			controls->RegDst = '0';
			controls->Jump = '0';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '3';
			controls->MemWrite = '0';
			controls->ALUSrc = '1';
			controls->RegWrite = '1';
			break;

		// jump
		case 2:
			controls->RegDst = '0';
			controls->Jump = '1';
			controls->Branch = '0';
			controls->MemRead = '0';
			controls->MemtoReg = '0';
			controls->ALUOp = '2';
			controls->MemWrite = '0';
			controls->ALUSrc = '2';
			controls->RegWrite = '2';
			break;

		default:
			return 1;

	}
	
	return 0;
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
	// negative = fill 1, positive = fill 0
	unsigned extends = 0xffff0000;
	unsigned negative = offset >> 16;

	if (negative == 1)
		*extended_value = offset | extends;
	else
		*extended_value = offset & 0x0000ffff;
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
	if (ALUOp == '7')
	{
		switch (funct)
		{
			// add
			case 32:
				ALUOp = '0';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
			
			// subtract
			case 34:
				ALUOp = '1';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
				
			// slt
			case 42:
				ALUOp = '2';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
			
			// slti
			case 43:
				ALUOp = '3';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
			
			// and
			case 36:
				ALUOp = '4';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
			
			// or			
			case 37:
				ALUOp = '5';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
				
			// sll
			case 6:
				ALUOp = '6';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
				
			// not
			case 39:
				ALUOp = '7';
				ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;
				
			default:
				return 1;
		}
	}
	else
	{
		switch (ALUOp)
		{
			case '0': // add
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '1': // subtract
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '2': // slt (signed)
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '3': // slt (unsigned)
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '4': // and
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '5': // or
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			case '6': // sll
				if (ALUSrc == '1')
					ALU(data1, extended_value, ALUOp, ALUresult, Zero);
				else
					ALU(data1, data2, ALUOp, ALUresult, Zero);
				break;

			default:
				return 1;
		}
	}
	return 0;
}

/* Read / Write Memory */
/* 10 Points */

/*
1. Base on the value of MemWrite or MemRead to determine memory write
operation or memory read operation.
2. Read the content of the memory location addressed by ALUresult to memdata. (LW)
3. Write the value of data2 to the memory location addressed by ALUresult. (SW)
4. Return 1 if a halt condition occurs; otherwise, return 0.
*/
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if (MemWrite == '1' && MemRead == '1')
		return 1;
	
	if ((MemWrite == '1' || MemRead == '1'))
	{
		if (ALUresult % 4 != 0)
			return 1;
		if (ALUresult < 0x000 || ALUresult > 0xffff)
			return 1;
	}

	if (MemWrite == '1' && MemRead == '0') // sw
		Mem[ALUresult >> 2] = data2;

	if (MemWrite == '0' && MemRead == '1') // lw
		*memdata = Mem[ALUresult >> 2];

	return 0;
}


/* Write Register */
/* 10 Points */

// 1. Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3.
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	// R: r3
	// I: r2

	if (RegWrite == '1' && MemtoReg == '1')
		Reg[r2] = memdata;
	else if (MemtoReg == '0' && RegDst == '1')
		Reg[r3] = ALUresult;
	else
		Reg[r2] = ALUresult;
}

/* PC update */
/* 10 Points */

// 1. Update the program counter (PC).
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	if (Branch == '0' && Jump == '0')
		*PC = *PC + 4;
	else if (Branch == '1' && Jump == '0')
		*PC = (extended_value << 2) + (*PC + 4);
	else if (Branch == '0' && Jump == '1')
	{
		unsigned shift = jsec << 2;
		unsigned bits = (*PC + 4) | 0xF0000000;
		*PC = bits | shift;
	}
}
