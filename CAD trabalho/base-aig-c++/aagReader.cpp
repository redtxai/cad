#include "aagReader.h"

int getIntValue(string stringValue) {
	stringstream geek2(stringValue);
  int intValue = 0;
  geek2 >> intValue;
	return intValue;
}

string getStringValue(int intValue) {
	stringstream ss;
	ss << intValue;
	return ss.str();
}

AAGReader::AAGReader(string sourcePath) {
	filename = sourcePath;
	sourcePath += ".aag";
	source.open(sourcePath.c_str());
	debug.open("aagComentado.txt");
}

Aig* AAGReader::readFile() {
	//treating header
	source.getline(buf, 250, '\n');
	string s=buf;
	line.str(s);
	line >> word;

	if(strcmp("aag",word.c_str()) != 0) {
		debug << "the file is not an AAG file!";
		return NULL;
	}

	int nNodes, nInputs, nFFs, nOutputs, nAnds;
	line >> word;
	nNodes = atoi(word.c_str());
	line >> word;
	nInputs = atoi(word.c_str());
	line >> word;
	nFFs = atoi(word.c_str());
	line >> word;
	nOutputs = atoi(word.c_str());
	line >> word;
	nAnds = atoi(word.c_str());

	if (nNodes != nInputs + nFFs + nAnds) {
		debug << "Wrong file header";
		return NULL;
	}

	if (nFFs != 0) {
		debug << "FF not supported yet";
		return NULL;
	}

	debug << s << "\nThe file header is ok!\n\n";
	
	char alpha[] = "abcdefghijklmnopqrstuvwxyABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int alpha_index = 0;
	std::vector<char> alphabet( alpha, alpha+sizeof(alpha)-1 ) ;
	map<string, string> inputsOutputs;
	vector<string> outputs;

	//treating inputs
	for (int i = 0; i < nInputs; i++) {
		source.getline(buf, 250, '\n');
		s=buf;
		line.seekg(0);line.str(s);
		line >> word;

		inputsOutputs[word] = alpha[alpha_index];
		debug << "\n" << word << " = " << alpha[alpha_index];
		alpha_index++;
		//debug << "read the input[" << word << "] from the file\n";
		//debug << "   create in" << i << " and add it to an input list and the all nodes list\n";
	}

	//treating outputs
	debug << "\n";
	for (int i = 0; i < nOutputs; i++) {
		source.getline(buf, 250, '\n');
		s=buf;
		line.seekg(0);line.str(s);
		line >> word;

	  int x3 = getIntValue(word);
	  if (x3%2 != 0) {
	  	inputsOutputs[word] = "!";
		} else {
			inputsOutputs[word] = "";
		}
		outputs.push_back(word);
		debug << word << ": " << inputsOutputs[word] << "\n";
	}

	//connecting ands
	debug << "\n";
	string input1;
	string input2;
	string output;
	for (int i = 0; i < nAnds; i++) {
		source.getline(buf, 250, '\n');
		s=buf;
		line.seekg(0);line.str(s);
		line >> word;
		output = word;
		line >> word;
		input1 = word;
		line >> word;
		input2 = word;
		
		debug << "[" << output << "] [" << input1 << " " << input2 << "]\n";
		
		stringstream geek(input1);
	    int x = 0;
	    geek >> x;
	    
	    
	  	x = getIntValue(input1);
		if (x%2 != 0) {
			x--;
			string str2 = getStringValue(x);

	    input1 = "!" + inputsOutputs[str2];
		} else {
			input1 = inputsOutputs[input1];
		}

		int x2 = getIntValue(input2);
		if (x2%2 != 0) {
			x2--;
			string str22 = getStringValue(x2);

	    input2 = "!" + inputsOutputs[str22];
		} else {
			input2 = inputsOutputs[input2];
		}

		debug << "\n  " << input1;
		debug << "\n  " << input2;

		int intValue = getIntValue(output);
		intValue++;
		string outputString = getStringValue(intValue);
		if (inputsOutputs[outputString] == "!") {
			inputsOutputs[outputString] += "(" + input1 + "*" + input2 + ")";
			debug << "\n  " << outputString << ": " << inputsOutputs[outputString] << "\n";
		} else {
			inputsOutputs[output] = "(" + input1 + "*" + input2 + ")";
			debug << "\n  " << output << ": " << inputsOutputs[output] << "\n";
		}
		
		
		debug <<"=============================================================\n\n";
	}

	int i=0;
	debug << "\n";
	while(source) {
		source.getline(buf, 250, '\n');
		s=buf;
		line.seekg(0);line.str(s);
		line >> word;

		if(strcmp("c",word.substr(0).c_str()) == 0) {
			debug << "\nthe comments began. Ignore the file from here!\n";
			break;
		} else if(strcmp(word.substr(0).c_str(),"i") == 0) {
			debug << "i\n";
		} else if(strcmp(word.substr(0).c_str(),"o") == 0) {
			debug << "o\n";
		} else if(strcmp(word.substr(0).c_str(),"l") == 0) {
			debug << "l\n";
		}
	}

	string finalEquation = inputsOutputs[outputs[0]];;
	for (int i = 1; i < nOutputs; i++) {
		finalEquation += "+" + inputsOutputs[outputs[i]];
	}
	debug << "\n" << finalEquation << "\n";
	
	std::string equationOutputFileName = "equation-" + filename + ".txt";
	ofstream outputFile;
	outputFile.open(equationOutputFileName.c_str());

	outputFile << finalEquation;
	outputFile.close();
	return NULL;
}

int main(int argc, char *argv[])
{
	stringstream ss;
	string target;
	ss << argv[1];
	ss >> target;

    AAGReader reader(target);
    reader.readFile();

	stringstream ss2;
	string target2;
	ss2 << argv[2];
	ss2 >> target2;
	
    AAGReader reader2(target2);
    reader2.readFile();
	
	string bddCall = "start gerentebdd.exe ";
	bddCall += target;
	bddCall += " ";
	bddCall += target2;
	cout << bddCall.c_str();
	system(bddCall.c_str());
    return EXIT_SUCCESS;
}
