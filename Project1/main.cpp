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
			
			int long long opCodeIndex = stoll(opCodes[i], nullptr, 2);
			string operation = immediateInstructions[opCodeIndex];
			if (opCodes[i] == "001000" || "001001" || "001010" || "001011") {  //checks if the operation requires Sign Extension and has normal form
				string SignExtension = signExtend(immediateCodes[i]); 
				assembeledInstructions = operation + ", " + rt + ", " + rs + ", " + SignExtension;
			}
			else if (opCodes[i] == "100011" || "101011" || "100100" || "100101" || "110000" || "101000" || "101000" || "111000") { //checks if it is a load word or store word operation
				string SignExtension = signExtend(immediateCodes[i]);
				assembeledInstructions = operation + ", " + rt + ", " + SignExtension + "(" + rs + ")";
			}
			else if (opCodes[i] == "01111") {
				string SignExtension = signExtend(immediateCodes[i]);
				assembeledInstructions = operation + ", " + rt + ", " + SignExtension;
			}

		}
		cout << assembeledInstructions << "\n";
	}

	return 0;  
}

string signExtend(string immediateAmount) {
	string signExtension;
	if (immediateAmount.at(0) == '0') {
		string immediateValue = "0000000000000000" + immediateAmount; //Fills zeros
		//converts to decimal number
		long long int dec = stoll(immediateValue, nullptr, 2);
		//converts to a string for output
		signExtension = to_string(dec);
	}
	else {
		string immediateValue = "1111111111111111" + immediateAmount;  //Fills ones
		// Converts to a negative decimal number
		bitset<32> immediateBinary(immediateValue);
		immediateBinary = ~immediateBinary;
		immediateValue = immediateBinary.to_string();
		long long int dec = stoll(immediateValue, nullptr, 2);
		dec = -1 * (dec + 1);
		//converts to a string for output
		signExtension = to_string(dec);
	}
	return signExtension;
}

vector<int> hexToDec(vector<string> input)
{
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		int long long temp = stoll(input[j], nullptr, 16);
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
		int long long dec = stoll(input[j], nullptr, 2);
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
		int long long dec = stoll(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> transferRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(11, 5);
		int long long dec = stoll(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> destinationRegister(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(16, 5);
		int long long dec = stoll(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> shamt(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(21, 5);
		int long long dec = stoll(temp, nullptr, 2);
		output.push_back(dec);
	}
	return output;
}

vector<int> funct(vector<string> input) {
	vector<int> output;
	for (int j = 0; j < input.size(); j++) {
		string temp = input[j].substr(26, 6);
		int long long dec = stoll(temp, nullptr, 2);
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
