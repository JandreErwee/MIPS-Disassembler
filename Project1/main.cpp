#include "main.h"



int main() {
	//opens user specified file
	string fileLocation;
	cout << "What file would you like to dissasemble? ";
	cin >> fileLocation;
	ifstream inputFile(fileLocation);

	//Checks if input file is valid
	if (inputFile.is_open()) {
		cout << "Reading File..." << "\n";
	}
	else {
		cout << "ERROR: File could not be found";
		return -1;
	}
	string line;
	vector<string> hexInstruction;
	
	//copies machine code line by line into a vector
	while (!inputFile.eof()) {
		getline(inputFile, line);
		hexInstruction.push_back(line);

	}
	//converts hexadecimal instructions into decimal and binary
	vector<int> decimalInstruction = hexToDec(hexInstruction);
	vector<string> binaryInstruction = decToBin(decimalInstruction);


	//seperates all of the insructions into their components
	vector<string> opCodes = opcode(binaryInstruction);
	vector<int> sourceCodes = sourceRegister(binaryInstruction);
	vector<int> transferCodes = transferRegister(binaryInstruction);
	vector<int> destinationCodes = destinationRegister(binaryInstruction);
	vector<int> shiftCodes = shamt(binaryInstruction);
	vector<int> functionCodes = funct(binaryInstruction);
	vector<string> immediateCodes = immediateValue(binaryInstruction);
	vector<string> format = instructionFormat(opCodes);

	//checks for any error before compiling
	for (int i = 0; i < opCodes.size(); i++) {
		bitset<6> bits(opCodes[i]);
		int opCodeIndex = bits.to_ulong();
		string operation = immediateInstructions[opCodeIndex];
		if ((operation == "NULL") || (hexInstruction[i].length() != 8)) {
			cout << "ERROR: Cannot disassemble 0x" << hexInstruction[i] << " at line " << i << "\n";
			return -1;
		}
	}

	string labels[100];
	vector<int> labelFlag;
	for (int i = 0; i < opCodes.size(); i++) {
		if ((opCodes[i] == "000100") || (opCodes[i] == "000101")) {
			string SignExtension = signExtend(immediateCodes[i]);
			int offset =stoll(SignExtension, nullptr, 10);
			//cout << "Current Index: " << i << " Offset: " << SignExtension << "\n";
			int labelCheck = i + 1 + offset;
			//cout << "Label created at " << labelCheck << "\n";
			labelFlag.push_back(labelCheck);
			offset = offset * 4;
			int address = 4 * (i + 1) + offset;
			labels[labelCheck] = to_string(address);
		}
	}
	labelFlag.push_back(-1);

	//creates and output file with a name that matches the input file
	string outfile = fileLocation.substr(0, fileLocation.length()-4);
	ofstream outputFile(outfile + ".s");



	string assembeledInstructions;
	for (int i = 0; i < opCodes.size(); i++) {
		int sourceIndex = sourceCodes[i];
		string rs = registerList[sourceIndex];
		int transferIndex = transferCodes[i];
		string rt = registerList[transferIndex];

		if (find(labelFlag.begin(), labelFlag.end(), i) != labelFlag.end()) {
			int jumpAddress = stoll(labels[i],nullptr,10);
			cout << "Addr_" << hex << setfill('0') << setw(4) << jumpAddress << ": " << "\n";
			outputFile << "Addr_" << hex << setfill('0') << setw(4) << jumpAddress << ": " << "\n";

		} 
			if (format[i] == "r") {
			int functionIndex = functionCodes[i];
			string command = registerInstructions[functionIndex];
			int destinationIndex = destinationCodes[i];
			string rd = registerList[destinationIndex];
			int shiftIndex = shiftCodes[i];
			string shiftAmount = to_string(shiftIndex);
			if (shiftIndex != 0) {
				assembeledInstructions = "\t" + command + "\t" + rd + ", " + rt + ", " + shiftAmount;
				cout << assembeledInstructions << "\n";
				outputFile << assembeledInstructions << "\n";

			}
			else {
				assembeledInstructions = "\t" + command + "\t" + rd + ", " + rs + ", " + rt;
				cout << assembeledInstructions << "\n";
				outputFile << assembeledInstructions << "\n";

			}
		}
		else {
			bitset<6> bits(opCodes[i]);
			int opCodeIndex = bits.to_ulong();
			string operation = immediateInstructions[opCodeIndex];
			if ((opCodes[i] == "001000") || (opCodes[i] == "001001") || (opCodes[i] == "001010") || (opCodes[i] == "001011") || (opCodes[i] == "001100") || (opCodes[i] == "001101")) {  //checks if the operation requires Sign Extension and has normal form
				string SignExtension = signExtend(immediateCodes[i]); 
				assembeledInstructions = "\t" + operation + "\t" + rt + ", " + rs + ", " + SignExtension;
				cout << assembeledInstructions << "\n";
				outputFile << assembeledInstructions << "\n";
			}
			else if ((opCodes[i] == "100011") || (opCodes[i] == "101011") || (opCodes[i] == "100100") || (opCodes[i] == "100101") || (opCodes[i] == "110000") || (opCodes[i] == "101000") || (opCodes[i] == "101000") || (opCodes[i] == "111000")) { //checks if it is a load word or store word operation
				string SignExtension = signExtend(immediateCodes[i]);
				assembeledInstructions = "\t" + operation + "\t" + rt + ", " + SignExtension + "(" + rs + ")";
				cout << assembeledInstructions << "\n";
				outputFile << assembeledInstructions << "\n";
			}
			else if (opCodes[i] == "01111") {
				string SignExtension = signExtend(immediateCodes[i]);
				assembeledInstructions = "\t" + operation + "\t" + rt + ", " + SignExtension;
				cout << assembeledInstructions << "\n";
				outputFile << assembeledInstructions << "\n";
			}
			else if ((opCodes[i] == "000100") || (opCodes[i] == "000101")) {
				string SignExtension = signExtend(immediateCodes[i]);
				int offset = stoll(SignExtension, nullptr, 10);
				offset = offset * 4;
				int address = 4 * (i + 1) + offset;
				cout << "\t" << operation << "\t" << rs << ", " << rt << ", " << "Addr_" << hex << setfill('0') << setw(4) << address << "\n";
				outputFile << "\t" << operation << "\t" << rs << ", " << rt << ", " << "Addr_" << hex << setfill('0') << setw(4) << address << "\n";

			}

		}
	}
	cout << "Above Code was stored in file " << outfile << ".s";
	return 0;  
}

string signExtend(string input) {
	string signExtension;
	bitset<16> preExt(input);
	bitset<16> mask("1000000000000000");
	bitset<16> msb = preExt & mask;
	if (msb == mask) {
		input = "1111111111111111" + input;
		bitset<32> bits(input);
		bits = ~bits;
		int dec = bits.to_ulong();
		dec = -1 * (dec + 1);
		signExtension = to_string(dec);
		//cout << "Negative: " << dec << "\n";
	}
	else {
		input = "0000000000000000" + input;
		bitset<32> bits(input);
		int dec = bits.to_ulong();
	    signExtension = to_string(dec);
		//cout << "Postivive: " << dec << "\n";
	}
	return signExtension;
}

vector<int> hexToDec(vector<string> input)
{
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		unsigned long int dec = stoul(input[j], nullptr, 16);
		output.push_back(dec);
	}
	return output;
}

vector<string> decToBin(vector<int> input) {
	vector<string> output;
	for (int j = 0; j < input.size(); j++) {
		bitset<32> temp(input[j]);
		string temp1 = temp.to_string();
		output.push_back(temp1);
	}
	return output;
}

vector<int> binToDec(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		bitset<32> binHolder(input[j]);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<string> opcode(vector<string> input) {
	vector<string> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(0, 6);
		output.push_back(temp);
	}
	return output;
}

vector<int> sourceRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(6, 5);
		bitset<5> binHolder(temp);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<int> transferRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(11, 5);
		bitset<5> binHolder(temp);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<int> destinationRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(16, 5);
		bitset<5> binHolder(temp);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<int> shamt(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(21, 5);
		bitset<5> binHolder(temp);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<int> funct(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(26, 6);
		bitset<6> binHolder(temp);
		int dec = binHolder.to_ulong();
		output.push_back(dec);
	}
	return output;
}

vector<string> immediateValue(vector<string> input) {
	vector<string> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(16, 16);
		output.push_back(temp);
	}
	return output;
}

vector<string> instructionFormat(vector<string> input) {
	vector<string> output;
	for (int j = 0; j < input.size(); j++) {
		if (input[j] == "000000") {
			output.push_back("r");
			//cout << "Instruction type of line " << j << " is " << output[j] << "\n";
		}
		else {
			output.push_back("i");
			//cout << "Instruction type of line " << j << " is " << output[j] << "\n";
		}
	}
	return output;
}
