#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <bitset>
using namespace std;



//Creates vector indexes for the registers
vector<string> registerList = { "$zero","$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra" };

//Intializes vector indexes for I-type and R-Type instructions
vector<string> immediateInstructions = {
	"R-Type",    "NULL",        "NULL",        "NULL",        "beq",        "bne",        "NULL",    "NULL",
	"addi",        "addiu",    "slti",        "sltiu",    "andi",        "ori",        "NULL",    "lui",
	"NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"NULL",        "NULL",        "NULL",        "lw",        "lbu",        "lhu",        "NULL",    "NULL",
	"sb",        "sh",        "NULL",        "sw",        "NULL",        "NULL",        "NULL",    "NULL",
	"ll",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"sc"
};

vector<string> registerInstructions{
	"sll",        "NULL",        "srl",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",        "NULL",    "NULL",
	"add",        "addu",        "sub",        "subu",        "and",        "or",        "NULL",    "nor",
	"NULL",        "NULL",        "slt",        "sltu"
};


vector<int> hexToDec(vector<string> input);
vector<string> decToBin(vector<int> input);
vector<int> binToDec(vector<int> input);

vector<string> opcode(vector<string> input);
vector<int> sourceRegister(vector<string> input);
vector<int> transferRegister(vector<string> input);
vector<int> destinationRegister(vector<string> input);
vector<int> shamt(vector<string> input);
vector<int> funct(vector<string> input);
vector<string> immediateValue(vector<string> input);

vector<string> instructionFormat(vector<string> input);
string signExtend(string immediateAmount);