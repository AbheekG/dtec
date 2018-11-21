#include <fstream>
#include <string>

using std::string;

class Model {
	int a;
public:
	int construct (string);
	int parse_specs (string);
	int check ();
};