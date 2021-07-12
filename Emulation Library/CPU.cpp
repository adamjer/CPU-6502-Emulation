#include "CPU.h"

void CPU::ResetRegisters()
{
	this->A = this->X = this->Y = 0;
}

void CPU::Reset(Memory& memory)
{
	this->PC = 0xFFFC;
	this->SP = 0xFF;
	this->Flags.Reset();
	this->ResetRegisters();

	memory.Initialize();
}

void CPU::Reset(const uint16_t& offset, Memory& memory)
{
	this->PC = offset;
	this->SP = 0xFF;
	this->Flags.Reset();
	this->ResetRegisters();

	memory.Initialize();
}

uint8_t CPU::FetchByte(int32_t& cycles, const Memory& memory)
{
	uint8_t data = memory[PC++];
	--cycles;

	return data;
}

uint16_t CPU::FetchWord(int32_t& cycles, const Memory& memory)
{
	// CPU 6502 is little endian
	// first byte is more signifant one (first in WORD)
	uint16_t data = memory[PC++];
	data |= (memory[PC++] << 8);
	cycles -= 2;

	return data;
}

uint8_t CPU::ReadByte(int32_t& cycles, const uint16_t& address, const Memory& memory)
{
	uint8_t data = memory[address];
	--cycles;

	return data;
}

uint16_t CPU::ReadWord(int32_t& cycles, const uint16_t& address, const Memory& memory)
{
	uint8_t lowByte = ReadByte(cycles, address, memory);
	uint8_t highByte = ReadByte(cycles, address + 1, memory);

	return lowByte | (highByte << 8);
}

/*  Write 1 byte to memory*/
void CPU::WriteByte(int32_t& cycles, const uint16_t& address, Memory& memory, const uint8_t value)
{
	memory[address] = value;
	--cycles;
}

/*  Write 2 bytes to memory*/
void CPU::WriteWord(int32_t& cycles, const uint16_t& address, Memory& memory, const uint16_t value)
{
	memory[address] = value & 0xFF;
	memory[address + 1] = (value >> 8);
	cycles -= 2;
}

/*  Return the stack pointer as a full 16-bit address in the 1st page*/
uint16_t CPU::StackPointerToAddress() const
{
	return 0x100 | this->SP;
}

/*  Push the PC-1 onto the stack*/
void CPU::PushProgramCounterToStack(int32_t& cycles, Memory& memory)
{
	this->WriteWord(cycles, this->StackPointerToAddress() - 1, memory, this->PC - 1);
	this->SP -= 2;
}

void CPU::PushByteOntoStack(int32_t& cycles, Memory& memory, uint8_t value)
{
	uint16_t address = this->StackPointerToAddress();
	memory[address] = value;
	--cycles;
	--this->SP;
	--cycles;
}

uint16_t CPU::PopWordFromStack(int32_t& cycles, const Memory& memory)
{
	this->SP += 2;
	uint16_t value = ReadWord(cycles, this->StackPointerToAddress() - 1, memory);
	--cycles;
	return value;
}

uint8_t CPU::PopByteFromStack(int32_t& cycles, const Memory& memory)
{
	++this->SP;
	uint8_t value = this->ReadByte(cycles, this->StackPointerToAddress(), memory);
	cycles -= 2;
	return value;
}

/*  Addresing mode - Zero page*/
uint16_t CPU::AddressZeroPage(int32_t& cycles, const Memory& memory)
{
	uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
	return zeroPageAddress;
}

/*  Addresing mode - Zero page with X offset*/
uint16_t CPU::AddressZeroPageX(int32_t& cycles, const Memory& memory)
{
	uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
	zeroPageAddress += this->X;
	--cycles;
	return zeroPageAddress;
}

/*  Addresing mode - Zero page with Y offset*/
uint16_t CPU::AddressZeroPageY(int32_t& cycles, const Memory& memory)
{
	uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
	zeroPageAddress += this->Y;
	--cycles;
	return zeroPageAddress;
}

/*  Addresing mode - Absolute*/
uint16_t CPU::AddressAbsolute(int32_t& cycles, const Memory& memory)
{
	uint16_t absoluteAddress = this->FetchWord(cycles, memory);
	return absoluteAddress;
}

/*  Addresing mode - Absolute with X offset*/
uint16_t CPU::AddressAbsoluteX(int32_t& cycles, const Memory& memory, bool alwaysConsumeCycle = false)
{
	uint16_t absoluteAddress = this->FetchWord(cycles, memory);
	uint16_t absoluteAddressX = absoluteAddress + this->X;

	if (alwaysConsumeCycle)
		--cycles;
	else if (absoluteAddressX - absoluteAddress >= 0xFF)
		--cycles;

	return absoluteAddressX;
}

/*  Addresing mode - Absolute with Y offset*/
uint16_t CPU::AddressAbsoluteY(int32_t& cycles, const Memory& memory, bool alwaysConsumeCycle = false)
{
	uint16_t absoluteAddress = this->FetchWord(cycles, memory);
	uint16_t absoluteAddressY = absoluteAddress + this->Y;
	if (alwaysConsumeCycle)
		--cycles;
	else if (absoluteAddressY - absoluteAddress >= 0xFF)
		--cycles;

	return absoluteAddressY;
}

/*  Addresing mode - Indirect X | Indexed Indirect X*/
uint16_t CPU::AddressIndirectX(int32_t& cycles, const Memory& memory)
{
	uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
	zeroPageAddress += this->X;
	--cycles;
	uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);

	return effectiveAddress;
}

/*  Addresing mode - Indirect Y | Indexed Indirect Y*/
uint16_t CPU::AddressIndirectY(int32_t& cycles, const Memory& memory, bool alwaysConsumeCycle = false)
{
	uint8_t zeroPageAddress = this->FetchByte(cycles, memory);
	uint16_t effectiveAddress = this->ReadWord(cycles, zeroPageAddress, memory);
	uint16_t effectiveAddressY = effectiveAddress + this->Y;
	if (alwaysConsumeCycle)
		--cycles;
	else if (effectiveAddressY - effectiveAddress >= 0xFF)
		--cycles;

	return effectiveAddressY;
}

//return the number of cycles that were used
int32_t CPU::Execute(int32_t cycles, Memory& memory)
{
	//Load a register with the value from memory address
	auto LoadRegister = [&cycles, &memory, this](uint16_t address, uint8_t& registry)
	{
		registry = this->ReadByte(cycles, address, memory);
		this->LoadRegisterSetStatus(registry);
	};

	//And the A register with the value from memory address
	auto And = [&cycles, &memory, this](uint16_t address)
	{
		this->A &= this->ReadByte(cycles, address, memory);
		this->LoadRegisterSetStatus(this->A);
	};

	//Or the A register with the value from memory address
	auto Ora = [&cycles, &memory, this](uint16_t address)
	{
		this->A |= this->ReadByte(cycles, address, memory);
		this->LoadRegisterSetStatus(this->A);
	};

	//Eor the A register with the value from memory address
	auto Eor = [&cycles, &memory, this](uint16_t address)
	{
		this->A ^= this->ReadByte(cycles, address, memory);
		this->LoadRegisterSetStatus(this->A);
	};

	const uint32_t cyclesRequested = cycles;
	while (cycles > 0)
	{
		uint8_t instruction = this->FetchByte(cycles, memory);

		switch (instruction)
		{
			case INS_LDA_IM:
			{
				this->A = this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(this->A);
			} break;
			case INS_LDX_IM:
			{
				this->X = this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(this->X);
			} break;
			case INS_LDY_IM:
			{
				this->Y = this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(this->Y);
			} break;
			case INS_LDA_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDX_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				LoadRegister(address, this->X);
			} break;
			case INS_LDY_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				LoadRegister(address, this->Y);
			} break;
			case INS_LDA_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDX_ZPY:
			{
				uint16_t address = this->AddressZeroPageY(cycles, memory);
				LoadRegister(address, this->X);
			} break;
			case INS_LDY_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				LoadRegister(address, this->Y);
			} break;
			case INS_LDA_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDX_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				LoadRegister(address, this->X);
			} break;
			case INS_LDY_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				LoadRegister(address, this->Y);
			} break;
			case INS_LDA_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDY_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				LoadRegister(address, this->Y);
			} break;
			case INS_LDA_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDX_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				LoadRegister(address, this->X);
			} break;
			case INS_LDA_INDX:
			{
				uint16_t address = AddressIndirectX(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_LDA_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				LoadRegister(address, this->A);
			} break;
			case INS_STA_ZP:
			{
				uint8_t address = this->AddressZeroPage(cycles, memory);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STX_ZP:
			{
				uint8_t address = this->AddressZeroPage(cycles, memory);
				this->WriteByte(cycles, address, memory, this->X);
				// StatusFlags not affected
			} break;
			case INS_STY_ZP:
			{
				uint8_t address = this->AddressZeroPage(cycles, memory);
				this->WriteByte(cycles, address, memory, this->Y);
				// StatusFlags not affected
			} break;
			case INS_STA_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STX_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				this->WriteByte(cycles, address, memory, this->X);
				// StatusFlags not affected
			} break;
			case INS_STY_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				this->WriteByte(cycles, address, memory, this->Y);
				// StatusFlags not affected
			} break;
			case INS_STA_ZPX:
			{
				uint8_t address = this->AddressZeroPageX(cycles, memory);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STY_ZPX:
			{
				uint8_t address = this->AddressZeroPageX(cycles, memory);
				this->WriteByte(cycles, address, memory, this->Y);
				// StatusFlags not affected
			} break;
			case INS_STA_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STA_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory, true);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STA_INDX:
			{
				uint16_t address = AddressIndirectX(cycles, memory);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_STA_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory, true);
				this->WriteByte(cycles, address, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_JSR:
			{
				uint16_t subroutineAddress = this->FetchWord(cycles, memory);
				this->PushProgramCounterToStack(cycles, memory);
				this->PC = subroutineAddress;
				--cycles;
				// StatusFlags not affected
			} break;
			case INS_RTS:
			{
				uint16_t returnAddress = this->PopWordFromStack(cycles, memory);
				this->PC = returnAddress + 1;
				cycles -= 2;
				// StatusFlags not affected
			} break;
			case INS_JMP_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				this->PC = address;
				// StatusFlags not affected
			} break;
			case INS_JMP_IND:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				address = this->ReadWord(cycles, address, memory);
				this->PC = address;
				// StatusFlags not affected
			} break;
			case INS_TSX:
			{
				this->X = this->SP;
				this->LoadRegisterSetStatus(this->X);
				--cycles;
			} break;
			case INS_TXS:
			{
				this->SP = this->X;
				--cycles;
				// StatusFlags not affected
			} break;
			case INS_PHA:
			{
				this->PushByteOntoStack(cycles, memory, this->A);
				// StatusFlags not affected
			} break;
			case INS_PLA:
			{
				this->A = this->PopByteFromStack(cycles, memory);
				this->LoadRegisterSetStatus(this->A);
			} break;
			case INS_PHP:
			{
				this->PushByteOntoStack(cycles, memory, this->PS);
				// StatusFlags not affected
			} break;
			case INS_PLP:
			{
				this->PS = this->PopByteFromStack(cycles, memory);
				// StatusFlags not affected
			} break;
			case INS_AND_IM:
			{
				this->A &= this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(A);
			} break;
			case INS_ORA_IM:
			{
				this->A |= this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(A);
			} break;
			case INS_EOR_IM:
			{
				this->A ^= this->FetchByte(cycles, memory);
				this->LoadRegisterSetStatus(A);
			} break;
			case INS_AND_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				And(address);
			} break;
			case INS_ORA_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				And(address);
			} break;
			case INS_ORA_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				And(address);
			} break;
			case INS_ORA_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				And(address);
			} break;
			case INS_ORA_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				And(address);
			} break;
			case INS_ORA_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				And(address);
			} break;
			case INS_ORA_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				Eor(address);
			} break;
			case INS_AND_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				And(address);
			} break;
			case INS_ORA_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				Ora(address);
			} break;
			case INS_EOR_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				Eor(address);
			} break;
			case INS_BIT_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				this->Flags.Z = !(this->A & value);
				this->PS |= (value & 0b11000000);
			} break;
			case INS_BIT_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				this->Flags.Z = !(this->A & value);
				this->PS |= (value & 0b11000000);
			} break;
			default:
			{
				printf("Instruction not handled!\n");
				throw - 1;
			} break;
		}
	}

	const int32_t cyclesUsed = cyclesRequested - cycles;
	return cyclesUsed;
}

/**  Sets the correct Process status after a load register instruction
*    LDA, LDX, LDY
*    @reg A,X or Y Register*/
void CPU::LoadRegisterSetStatus(uint8_t reg)
{
	Flags.Z = (reg == 0);
	Flags.N = (reg & 0b10000000) > 0;
}

