#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "interpreter.hpp"
#include "tokenize.hpp"
#include "expression.hpp"
#include "interpreter_semantic_error.hpp"

using namespace std;

int cmdLineEval(string eval_string) {
	Interpreter interp;

	istringstream iss(eval_string);
	int EXIT = 0;
	bool ok = interp.parse(iss);
	if (!ok) {
		std::cerr << "Error: Failed to parse: " << eval_string << std::endl;
		EXIT = EXIT_FAILURE;
	}

	try {
		Expression result = interp.eval();
		cout << result << "\n";
	}
	catch (const InterpreterSemanticError& ex) {
		std::cerr << "Error: Failed to eval program: " << endl;
		EXIT = EXIT_FAILURE;
	}
	return EXIT;
}

int ReadEvalPrintLoop() {
	Interpreter interp;
	cout << "slisp> ";
	string input;
	while (getline(cin, input) && !cin.eof()) {

		istringstream iss(input);

		bool ok = interp.parse(iss);
		if (!ok) {
			std::cerr << "Error: Failed to parse: " << input << std::endl;
		}

		try {
			Expression result = interp.eval();
			cout << result << "\n";
			cout << "slisp> ";
		}
		catch (const InterpreterSemanticError& ex) {
			std::cerr << "Error: Failed to eval program: " << endl;
			//continue execution
			cout << "slisp> ";
		}

	}
	return EXIT_SUCCESS;
}

int executeFromFile(string fileName) {
	ifstream inSlpFile;
	inSlpFile.open(fileName);
	string line;
	string sd_string;

	int EXIT = 0;
	if (inSlpFile.is_open())
	{
		if (inSlpFile.peek() == EOF) {
			inSlpFile.close();
		}
		else {
			string conc;
			while (getline(inSlpFile, line))
			{
				size_t comment = line.find(";");
				line = line.substr(0, comment);
				conc += line;
			}

			Interpreter interp;

			istringstream iss(conc);

			bool ok = interp.parse(iss);

			if (!ok) {
				std::cerr << "Error: Failed to parse: " << conc << std::endl;
				EXIT = EXIT_FAILURE;
			}

			try {
				Expression result = interp.eval();
				cout << result << "\n";
				EXIT = EXIT_SUCCESS;
			}
			catch (const InterpreterSemanticError& ex) {
				
				std::cerr << "Error: Failed to eval program: " << endl;
				EXIT = EXIT_FAILURE;
			}


		}
		inSlpFile.close();
	}
	else {
		std::cerr << "Error: Failed to open file of program: " << endl;
		EXIT = EXIT_FAILURE;
	}
	return EXIT;
}

int main(int argc, char** argv)
{
	string fileName = "slisp.txt";

	int EXIT = 0;

	for (int i = 0; i < argc; ++i) {

		if (string(argv[i]) == "-e") {

			string eval_string = string(argv[i + 1]);
			i += 2;
			while (i < argc) {
				eval_string = eval_string + " " + string(argv[i]);
				i++;
			}

			EXIT = cmdLineEval(eval_string); //commandline evaluation

		}
		else if (string(argv[i]) == "./slisp" && argc == 1) {

			EXIT = ReadEvalPrintLoop(); //REPL evaluation
		}
		else if (string(argv[i]) != "-e" && string(argv[i]) != "./slisp") {
			fileName = string(argv[i]);

			EXIT = executeFromFile(fileName); //file evaluation
		}
		else if (string(argv[i]) != "./slisp") {
			EXIT = EXIT_FAILURE;
		}

	}
	return EXIT;
}
