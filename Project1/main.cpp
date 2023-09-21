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
	vector<int> immediateCodes = immediateValue(binaryInstruction);
	vector<string> format = instructionFormat(opCodes);

	string assembeledInstructions;
	for (int i = 0; i < opCodes.size(); i++) {
		int sourceIndex = sourceCodes[i];
		string rs = registerList[sourceIndex];
		int transferIndex = transferCodes[i];
		string rt = registerList[transferIndex];

		if (format[i] == "r") {
			int functionIndex = functionCodes[i];
			string command = registerInstructions[functionIndex];
			int destinationIndex = destinationCodes[i];
			string rd = registerList[destinationIndex];
			int shiftIndex = shiftCodes[i];
			string shiftAmount = to_string(shiftIndex);
			if (shiftIndex != 0) {
				assembeledInstructions = command + " " + rd + ", " + rt + ", " + shiftAmount;
			}
			else {
				 assembeledInstructions = command + " " + rd + ", " + rs + ", " + rt;
			}
		}
		else {
			int immediateIndex = immediateCodes[i];
			string immediateAmount = to_string(immediateIndex);

		}
		cout << assembeledInstructions << "\n";
	}

	return 0;  
}

vector<int> hexToDec(vector<string> input)
{
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		int temp = stoi(input[j], nullptr, 16);
		output.push_back(temp);
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
		int dec = stoi(input[j], nullptr, 2);
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
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> transferRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(11, 5);
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> destinationRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(16, 5);
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> shamt(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(21, 5);
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> funct(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(26, 6);
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> immediateValue(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(16, 16);
		int dec = stoi(temp, nullptr, 2);
		output.push_back(dec);
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
