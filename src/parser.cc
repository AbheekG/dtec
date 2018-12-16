#include "model.hh"

void vector_push_back (vector<string> &first, vector<string> second) {
	for (auto& s : second) {
		first.push_back(s);
	}
}

vector<string> clean_dotdot (vector<string> lines) {

	// cout << "In clean_dotdot : " << line << endl;
	
	vector<string> tokens;
	for (auto &line : lines) {
		auto i = line.find("..");
		if (i == string::npos) {
			tokens.push_back (line);
		} else {
			auto j = line.find("[");
			auto left = line.substr (0, j);
			int num1 = stoi (line.substr(j+1, i-j-1));
			int num2 = stoi ( line.substr( i+2, line.find("]") -i-2 ) );
			for ( ; num1 <= num2 ; num1++) {
				tokens.push_back ( left + "[" + std::to_string (num1) + "]" );
			}
		}
	}

	// for (auto &token : tokens) cout << "In clean_dotdot token : " << token << endl;

	// TODO: add some more checks for invalid

	return tokens;
}

vector<string> clean_special (string line) {

	const static vector<string> Special = {"{", "}", "=", ":", "|", "*"};
	
	vector<string> tokens;
	// tokens.push_back(line); return tokens;
	while (line.size() > 0) {
		auto i = string::npos;
		for (auto s : Special ) i = std::min (i, line.find(s));
		
		if (i == string::npos) {
			tokens.push_back (line);
			break;
		} else {
			if (i > 0) tokens.push_back (line.substr(0, i));
			tokens.push_back (line.substr(i, 1));
			line = line.substr(i+1);
		}
	}

	return tokens;
}

vector<string> clean_commas (string line) {

	// cout << "In clean_commas : " << line << endl;
	
	vector<string> tokens;
	while (line.size() > 0) {
		auto i = line.find(",");
		vector_push_back ( tokens, clean_dotdot( clean_special ( line.substr(0, i) ) ) );
		if (i == string::npos) {
			break;
		} else {
			line = line.substr(i+1);
		}
	}

	return tokens;
}

string clean_spaces (string line) {

	string new_line;
	
	for (auto &l : line) {
		if (l != ' ' && l != '\t') {
			new_line.push_back (l);
		}
	}
	return new_line;
}

vector<string> clean_line (string line) {

	auto new_line = clean_spaces (line);
	// cout << new_line << endl;
	
	if (new_line.size() == 0) {
		return vector<string>(0);
	}
	return clean_commas (new_line);
}

vector<string> Model::lexer (const string &file_name) {

	vector<string> tokens;
	string line;
	ifstream file;
	file.open(file_name.c_str());

	if (file) {
		cout << "\nFile " << file_name << " opened.\n";
		while (getline( file, line )) {
			line = line.substr( 0, line.find("#") );
			// if (line.find("\t") != string::npos) cout << line << " has TAB.\n";
			// cout << line.size() << " $ " << line << endl;
			vector_push_back (tokens, clean_line (line));
			// for (auto &token : tokens) cout << token << endl;
			// cout << "Parsed a line\n";
		}
		file.close();
	} else {
		cout << "\nFile " << file_name << " couldn't be opened.\n";
	}

	// TODO: Token keyword match

	return tokens;
	
}