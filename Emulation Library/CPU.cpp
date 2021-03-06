#include "CPU.h"
#include <cassert>


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


void CPU::Reset(const uint16_t offset, Memory& memory)
{
	this->PC = offset;
	this->SP = 0xFF;
	this->Flags.Reset();
	this->ResetRegisters();

	memory.Initialize();
}


void CPU::Reset(const uint16_t offset)
{
	this->PC = offset;
	this->SP = 0xFF;
	this->Flags.Reset();
	this->ResetRegisters();
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


uint8_t CPU::ReadByte(int32_t& cycles, const uint16_t address, const Memory& memory)
{
	uint8_t data = memory[address];
	--cycles;

	return data;
}


uint16_t CPU::ReadWord(int32_t& cycles, const uint16_t address, const Memory& memory)
{
	uint8_t lowByte = ReadByte(cycles, address, memory);
	uint8_t highByte = ReadByte(cycles, address + 1, memory);

	return lowByte | (highByte << 8);
}


/*  Write 1 byte to memory*/
void CPU::WriteByte(int32_t& cycles, const uint16_t address, Memory& memory, const uint8_t value)
{
	memory[address] = value;
	--cycles;
}


/*  Write 2 bytes to memory*/
void CPU::WriteWord(int32_t& cycles, const uint16_t address, Memory& memory, const uint16_t value)
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


void CPU::PushWordToStack(int32_t& cycles, Memory& memory, uint16_t value)
{
	this->WriteByte(cycles, StackPointerToAddress(), memory, value >> 8);
	--this->SP;
	this->WriteByte(cycles, StackPointerToAddress(), memory, value & 0xFF);
	--this->SP;
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
	--cycles;
	uint8_t value = this->ReadByte(cycles, this->StackPointerToAddress(), memory);
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

	bool crossedPageBoundary = //(absoluteAddress & 0xFF00) != (absoluteAddressX & 0xFF00);
		(absoluteAddress ^ absoluteAddressX) >> 8;

	if (alwaysConsumeCycle)
		--cycles;
	else if (crossedPageBoundary)
		--cycles;

	return absoluteAddressX;
}


/*  Addresing mode - Absolute with Y offset*/
uint16_t CPU::AddressAbsoluteY(int32_t& cycles, const Memory& memory, bool alwaysConsumeCycle = false)
{
	uint16_t absoluteAddress = this->FetchWord(cycles, memory);
	uint16_t absoluteAddressY = absoluteAddress + this->Y;

	bool crossedPageBoundary = //(absoluteAddress & 0xFF00) != (absoluteAddressY & 0xFF00);
		(absoluteAddress ^ absoluteAddressY) >> 8;

	if (alwaysConsumeCycle)
		--cycles;
	else if (crossedPageBoundary)
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

	bool crossedPageBoundary = //(effectiveAddress & 0xFF00) != (effectiveAddressY & 0xFF00);
		(effectiveAddress ^ effectiveAddressY) >> 8;

	if (alwaysConsumeCycle)
		--cycles;
	else if (crossedPageBoundary)
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
		this->SetNegativeAndZeroFlags(registry);
	};

	//And the A register with the value from memory address
	auto And = [&cycles, &memory, this](uint16_t address)
	{
		this->A &= this->ReadByte(cycles, address, memory);
		this->SetNegativeAndZeroFlags(this->A);
	};

	//Or the A register with the value from memory address
	auto Ora = [&cycles, &memory, this](uint16_t address)
	{
		this->A |= this->ReadByte(cycles, address, memory);
		this->SetNegativeAndZeroFlags(this->A);
	};

	//Eor the A register with the value from memory address
	auto Eor = [&cycles, &memory, this](uint16_t address)
	{
		this->A ^= this->ReadByte(cycles, address, memory);
		this->SetNegativeAndZeroFlags(this->A);
	};

	// Conditional branch
	auto BranchIf = [&cycles, &memory, this](bool test, bool expected)
	{
		uint8_t offset = this->FetchByte(cycles, memory);
		if (test == expected)
		{
			uint16_t oldPC = this->PC;
			this->PC += static_cast<int8_t>(offset);
			--cycles;

			const bool pageChanged = (this->PC >> 8) != (oldPC >> 8);
			if (pageChanged)
			{
				--cycles;
			}
		}
	};

	//Do add with carry given the operand
	auto ADC = [&cycles, &memory, this](uint8_t operand)
	{
		assert(Flags.D == false, "haven't handled decimal mode!");
		const bool areSignBitsTheSame = !((this->A ^ operand) & NegativeBit);
		uint16_t sum = this->A;
		sum += operand;
		sum += this->Flags.C;
		this->A = (sum & 0xFF);
		this->SetNegativeAndZeroFlags(A);
		Flags.C = sum > 0xFF;
		Flags.V = areSignBitsTheSame &&
			((this->A ^ operand) & NegativeBit);
	};

	//Do subtract with carry given the operand
	auto SBC = [&ADC](uint8_t operand)
	{
		ADC(~operand);
	};

	//Sets the processor status for CMP/CPX/CPY instructions
	auto CMP = [&cycles, &memory, this](uint8_t operand, const uint8_t value)
	{
		uint8_t temp = value - operand;

		this->Flags.C = value >= operand;
		this->Flags.Z = value == operand;
		this->Flags.N = (temp & NegativeBit) > 0;
	};


	//Shifts left 
	auto ASL = [&cycles, this](uint8_t operand) -> uint8_t
	{
		uint8_t result = operand << 1;
		Flags.C = (operand & NegativeBit) > 0;
		SetNegativeAndZeroFlags(result);
		--cycles;

		return result;
	};


	//Shifts right 
	auto LSR = [&cycles, this](uint8_t operand) -> uint8_t
	{
		uint8_t result = operand >> 1;
		Flags.C = (operand & CarryBit) > 0;
		SetNegativeAndZeroFlags(result);
		--cycles;

		return result;
	};


	//Shifts left 
	auto ROL = [&cycles, this](uint8_t operand) -> uint8_t
	{
		uint8_t newBitZero = Flags.C ? CarryBit : 0;
		Flags.C = (operand & NegativeBit) > 0;
		operand <<= 1;
		operand |= newBitZero;
		SetNegativeAndZeroFlags(operand);
		--cycles;

		return operand;
	};


	//Shifts right
	auto ROR = [&cycles, this](uint8_t operand) -> uint8_t
	{
		uint8_t newBitSeven = Flags.C ? NegativeBit : 0;
		Flags.C = (operand & CarryBit) > 0;
		operand >>= 1;
		operand |= newBitSeven;
		SetNegativeAndZeroFlags(operand);
		--cycles;

		return operand;
	};


	// Push processor status onto the stack
	// setting bits 4 & 5 on the stack
	auto PushPSToStack = [&cycles, &memory, this]()
	{
		uint8_t PSStack = this->PS | BreakBit | UnusedBit;
		PushByteOntoStack(cycles, memory, PSStack);
	};


	// Pop processor status from the stack
	// clearing bits 4 & 5
	auto PopPSFromStack = [&cycles, &memory, this]()
	{
		this->PS = PopByteFromStack(cycles, memory);
		this->Flags.B = false;
		this->Flags.Unused = false;
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
				this->SetNegativeAndZeroFlags(this->A);
			} break;
			case INS_LDX_IM:
			{
				this->X = this->FetchByte(cycles, memory);
				this->SetNegativeAndZeroFlags(this->X);
			} break;
			case INS_LDY_IM:
			{
				this->Y = this->FetchByte(cycles, memory);
				this->SetNegativeAndZeroFlags(this->Y);
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
			case INS_STX_ZPY:
			{
				uint8_t address = this->AddressZeroPageY(cycles, memory);
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
				this->PushWordToStack(cycles, memory, this->PC - 1);
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
				this->SetNegativeAndZeroFlags(this->X);
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
				this->SetNegativeAndZeroFlags(this->A);
				--cycles;
			} break;
			case INS_PHP:
			{
				PushPSToStack();
			} break;
			case INS_PLP:
			{
				PopPSFromStack();
				--cycles;
			} break;
			case INS_AND_IM:
			{
				this->A &= this->FetchByte(cycles, memory);
				this->SetNegativeAndZeroFlags(A);
			} break;
			case INS_ORA_IM:
			{
				this->A |= this->FetchByte(cycles, memory);
				this->SetNegativeAndZeroFlags(A);
			} break;
			case INS_EOR_IM:
			{
				this->A ^= this->FetchByte(cycles, memory);
				this->SetNegativeAndZeroFlags(A);
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
				this->Flags.N = (value & NegativeBit) != 0;
				this->Flags.V = (value & OverflowBit) != 0;
			} break;
			case INS_BIT_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				this->Flags.Z = !(this->A & value);
				this->Flags.N = (value & NegativeBit) != 0;
				this->Flags.V = (value & OverflowBit) != 0;
			} break;
			case INS_TAX:
			{
				this->X = this->A;
				--cycles;
				this->SetNegativeAndZeroFlags(this->X);
			} break;
			case INS_TAY:
			{
				this->Y = this->A;
				--cycles;
				this->SetNegativeAndZeroFlags(this->Y);
			} break;
			case INS_TXA:
			{
				this->A = this->X;
				--cycles;
				this->SetNegativeAndZeroFlags(this->A);
			} break;
			case INS_TYA:
			{
				this->A = this->Y;
				--cycles;
				this->SetNegativeAndZeroFlags(this->A);
			} break;
			case INS_INX:
			{
				++this->X;
				--cycles;
				this->SetNegativeAndZeroFlags(this->X);
			} break;
			case INS_INY:
			{
				++this->Y;
				--cycles;
				this->SetNegativeAndZeroFlags(this->Y);
			} break;
			case INS_DEX:
			{
				--this->X;
				--cycles;
				this->SetNegativeAndZeroFlags(this->X);
			} break;
			case INS_DEY:
			{
				--this->Y;
				--cycles;
				this->SetNegativeAndZeroFlags(this->Y);
			} break;
			case INS_DEC_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				--value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_DEC_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				--value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_DEC_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				--value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_DEC_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t value = this->ReadByte(cycles, address, memory);
				--value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_INC_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				++value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_INC_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				++value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_INC_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t value = this->ReadByte(cycles, address, memory);
				++value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_INC_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t value = this->ReadByte(cycles, address, memory);
				++value;
				--cycles;
				this->WriteByte(cycles, address, memory, value);
				this->SetNegativeAndZeroFlags(value);
			} break;
			case INS_BEQ:
			{
				BranchIf(this->Flags.Z, true);
			} break;
			case INS_BNE:
			{
				BranchIf(this->Flags.Z, false);
			} break;
			case INS_BCS:
			{
				BranchIf(this->Flags.C, true);
			} break;
			case INS_BCC:
			{
				BranchIf(this->Flags.C, false);
			} break;
			case INS_BMI:
			{
				BranchIf(this->Flags.N, true);
			} break;
			case INS_BPL:
			{
				BranchIf(this->Flags.N, false);
			} break;
			case INS_BVS:
			{
				BranchIf(this->Flags.V, true);
			} break;
			case INS_BVC:
			{
				BranchIf(this->Flags.V, false);
			} break;
			case INS_CLC:
			{
				this->Flags.C = false;
				--cycles;
			} break;
			case INS_SEC:
			{
				this->Flags.C = true;
				--cycles;
			} break;
			case INS_CLD:
			{
				this->Flags.D = false;
				--cycles;
			} break;
			case INS_SED:
			{
				this->Flags.D = true;
				--cycles;
			} break;
			case INS_CLI:
			{
				this->Flags.I = false;
				--cycles;
			} break;
			case INS_SEI:
			{
				this->Flags.I = true;
				--cycles;
			} break;
			case INS_CLV:
			{
				this->Flags.V = false;
				--cycles;
			} break;
			case INS_ADC:
			{
				uint8_t operand = this->FetchByte(cycles, memory);
				ADC(operand);
			} break;
			case INS_ADC_ZP:
			{
				uint16_t address = AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_ZPX:
			{
				uint16_t address = AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_ADC_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				ADC(operand);
			} break;
			case INS_SBC:
			{
				uint8_t operand = this->FetchByte(cycles, memory);
				SBC(operand);
			} break;
			case INS_SBC_ZP:
			{
				uint16_t address = AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_ZPX:
			{
				uint16_t address = AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_SBC_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				SBC(operand);
			} break;
			case INS_CMP:
			{
				uint8_t operand = this->FetchByte(cycles, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_ABSY:
			{
				uint16_t address = this->AddressAbsoluteY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_INDX:
			{
				uint16_t address = this->AddressIndirectX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CMP_INDY:
			{
				uint16_t address = this->AddressIndirectY(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->A);
			} break;
			case INS_CPX:
			{
				uint8_t operand = this->FetchByte(cycles, memory);
				CMP(operand, this->X);
			} break;
			case INS_CPY:
			{
				uint8_t operand = this->FetchByte(cycles, memory);
				CMP(operand, this->Y);
			} break;
			case INS_CPX_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->X);
			} break;
			case INS_CPY_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->Y);
			} break;
			case INS_CPX_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->X);
			} break;
			case INS_CPY_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				CMP(operand, this->Y);
			} break;
			case INS_ASL:
			{
				this->A = ASL(this->A);
			} break;
			case INS_ASL_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);
				
				this->WriteByte(cycles, address, memory, ASL(operand));
			} break;
			case INS_ASL_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ASL(operand));
			} break;
			case INS_ASL_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ASL(operand));
			} break;
			case INS_ASL_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ASL(operand));
			} break;
			case INS_LSR:
			{
				this->A = LSR(this->A);
			} break;
			case INS_LSR_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, LSR(operand));
			} break;
			case INS_LSR_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, LSR(operand));
			} break;
			case INS_LSR_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, LSR(operand));
			} break;
			case INS_LSR_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, LSR(operand));
			} break;
			case INS_ROL:
			{
				this->A = ROL(this->A);
			} break;
			case INS_ROL_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROL(operand));
			} break;
			case INS_ROL_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROL(operand));
			} break;
			case INS_ROL_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROL(operand));
			} break;
			case INS_ROL_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROL(operand));
			} break;
			case INS_ROR:
			{
				this->A = ROR(this->A);
			} break;
			case INS_ROR_ZP:
			{
				uint16_t address = this->AddressZeroPage(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROR(operand));
			} break;
			case INS_ROR_ZPX:
			{
				uint16_t address = this->AddressZeroPageX(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROR(operand));
			} break;
			case INS_ROR_ABS:
			{
				uint16_t address = this->AddressAbsolute(cycles, memory);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROR(operand));
			} break;
			case INS_ROR_ABSX:
			{
				uint16_t address = this->AddressAbsoluteX(cycles, memory, true);
				uint8_t operand = this->ReadByte(cycles, address, memory);

				this->WriteByte(cycles, address, memory, ROR(operand));
			} break;
			case INS_BRK:
			{
				constexpr uint16_t interruptVector = 0xFFFE;
				PushWordToStack(cycles, memory, this->PC + 1);
				PushPSToStack();
				this->Flags.I = true;
				this->PC = ReadWord(cycles, interruptVector, memory);
				this->Flags.B = true;
			} break;
			case INS_RTI:
			{
				PopPSFromStack();
				this->PC = PopWordFromStack(cycles, memory);
			} break;
			case INS_NOP:
			{
				--cycles;
			} break;
			default:
			{
				printf("Instruction %#05x not handled!\n", instruction);
				throw - 1;
			} break;
		}
	}

	const int32_t cyclesUsed = cyclesRequested - cycles;
	return cyclesUsed;
}


bool CPU::operator==(const CPU& other) const
{
	if (this->PC == other.PC && this->PS == other.PS 
		&& this->SP == other.SP && this->Flags == other.Flags)
	return false;
}


void CPU::SetNegativeAndZeroFlags(uint8_t value)
{
	Flags.Z = (value == 0);
	Flags.N = (value & 0b10000000) > 0;
}


uint16_t CPU::LoadProgram(const std::vector<uint8_t>& program, Memory& memory)
{
	uint16_t loadAddress = 0;
	if (program.size() > 2)
	{
		uint32_t index = 0;
		loadAddress = program[index++] + (program[index++] << 8);

		for (uint16_t i = loadAddress; i < loadAddress + program.size() - 2; i++)
		{
			memory[i] = program[index++];
		}
	}
	return loadAddress;
}


void CPU::PrintStatus() const
{
	printf("A: %d X: %d Y: %d\n", this->A, this->X, this->Y);
	printf("PC: %d SP: %d\n", this->PC, this->SP);
}
