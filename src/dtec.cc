// R.S.
#include <iostream>
#include <string>

#include "model.hh"

using std::cout;
using std::string;

int help_message();
int process(const string &, const string &);

int main (int argc, char **argv) {
	if (argc == 2) {
		if (string(argv[1]) == "-h") {
			cout << "Helping.\n";
			help_message ();
		} else {
			cout << "Incorrect arguments.\n";
			help_message ();
		}
	} else if (argc == 5) {
		if (string(argv[1]) == "-m" && string(argv[3]) == "-s") {
			process (string(argv[2]), string(argv[4]));
		} else if (string(argv[1]) == "-s" && string(argv[3]) == "-m") {
			process (string(argv[4]), string(argv[2]));
		} else {
			cout << "Incorrect arguments.\n";
			help_message ();
		}
	} else {
		cout << "Incorrect arguments.\n";
		help_message ();
	}

	return 0;
}

int help_message () {
	cout << "Read README.md for help.\n";
	return 0;
}

int process (const string &model_file_name, const string &specs_file_name) {
	Model model;

	if ( model.construct (model_file_name) ) {
		help_message ();
		return 1;
	} else if ( model.parse_specs (specs_file_name) ) {
		help_message ();
		return 2;
	}

	model.check ();


	return 0;
}