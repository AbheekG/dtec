#include "model.hh"

int Model::construct (const string &file_name) {
	auto tokens = lexer (file_name);

	// cout << "In model constrution tokens : \n\n";
	// for (auto &token : tokens) cout << token << endl;

	// Processed tokens
	int cur = 0;

	// Error check. States not found.
	if (tokens[cur] != "~STATES~") {
		cout << "Expecting ~STATES~. Didn't find :(\n";
		cout << "Found " << tokens[cur] << endl << endl;
		return 1;
	}

	cur++;
	/*
	States.
	*/
	cout << "Processing States.\n";
	n_states = 0;
	while (cur < tokens.size() && tokens[cur] != "~PARTITIONS~") {
		auto state = tokens[cur];
		state_to_int[state] = n_states;
		int_to_state.push_back (state);
		n_states++;
		cur++;
	}

	// Error check.
	if (cur == tokens.size()) {
		cout << "Expecting ~PARTITIONS~. Didn't find :(\n";
		return 2;
	} else {
		cur++;
	}

	/*
	Partitions.
	*/
	cout << "Processing Partitions.\n";
	n_partitions = 0;
	bool bracket = false;
	while (cur < tokens.size() && tokens[cur] != "~INITIALSTATE~") {
		auto token = tokens[cur];
		
		// Bracket checks
		if (bracket == false && token != "{") {
			cout << "Error while processing partitions. Expecting '{', not found.\n";
			return 201;
		}
		else if (bracket == true && token == "{" ) {
			cout << "Error while processing partitions. Not expecting '{', found.\n";
			return 202;
		}

		if (token == "{") {
			partitions.push_back(set<int>());
			bracket = true;
		}
		else if (token == "}") {
			n_partitions++;
			bracket = false;
		} else {
			if ( state_to_int.count (token) == 0 ) {
				cout << "Error while processing partitions. State " << token << " not found.\n";
				return 203;
			} else {
				partitions[n_partitions].insert(state_to_int[token]);
			}
		}
		cur++;
	}

	if (bracket) {
		cout << "Error while processing partitions. Expecting '}', not found.\n";
		return 204;
	}

	// Error check.
	if (cur == tokens.size()) {
		cout << "Expecting ~INITIAL STATE~. Didn't find :(\n";
		return 3;
	} else {
		cur++;
	}

	/*
	Initial State.
	*/
	cout << "Processing Initial State.\n";
	init_state = -1;
	if ( cur < tokens.size() && state_to_int.count (tokens[cur]) > 0 ) {
		init_state = state_to_int[tokens[cur]];
		cur++;
	} else {
		cout << "Didn't find any initial state. :(\n";
		return 301;
	}

	// Error check.
	if (cur == tokens.size() || tokens[cur] != "~ALPHABET~") {
		cout << "Expecting ~ALPHABET~. Didn't find :(\n";
		return 4;
	} else {
		cur++;
	}

	/*
	Alphabets.
	*/
	cout << "Processing Alphabets.\n";
	n_alphas = 0;
	while (cur < tokens.size() && tokens[cur] != "~TRANSITIONFUNCTION~") {
		auto alpha = tokens[cur];
		alpha_to_int[alpha] = n_alphas;
		int_to_alpha.push_back (alpha);
		n_alphas++;
		cur++;
	}

	// Error check.
	if (cur == tokens.size()) {
		cout << "Expecting ~TRANSITION FUNCTION~. Didn't find :(\n";
		return 5;
	} else {
		cur++;
	}

	/*
	Transition Function.
	*/
	cout << "Processing Transition Function.\n";
	TF.resize(n_states, vector<int>(n_alphas, -1));
	for (int i_states = 0; i_states < n_states; ++i_states) {

		// Getting domain state.
		if (cur >= tokens.size() || state_to_int.count (tokens[cur]) == 0) {
			cout << "Problem processing TF. State not found."<<tokens[cur]<<"\n";
			return 501;
		}

		auto &trans = TF[state_to_int[tokens[cur]]];

		// Getting transitions
		for (int i = 0; i < n_alphas; i++) {
			if ( cur+4 >= tokens.size() || tokens[cur+3] != ":" ||
				(tokens[cur+1] != "=" && tokens[cur+1] != "|")) {
				cout << "Problem processing TF. Insufficient/incorrect info.\n";
				cout << "For " << tokens[cur] << endl;
				return 502;
			} else if ( tokens[cur+2] != "*" && alpha_to_int.count (tokens[cur+2]) == 0) {
				cout << "Problem processing TF. Alpha not found.\n";
				return 503;
			} else if ( state_to_int.count (tokens[cur+4]) == 0) {
				cout << "Problem processing TF. Next state not found.\n";
				return 504;
			} else {
				// assert (tokens[cur+1] == "=" || tokens[cur+1] == "|");
				// assert (tokens[cur+3] == ":" );

				if ( tokens[cur+2] == "*" ) {
					for ( auto &tran : trans ) {
						if (tran == -1) 
							tran = state_to_int[tokens[cur+4]];
					}
					cur += 4;
					break;
				} else {
					trans[alpha_to_int[tokens[cur+2]]] = state_to_int[tokens[cur+4]];
				}
			}
			cur += 4;
		}
		cur++;
	}

	// Creating adjacency list
	vector<set<int> > TF_temp;
	TF_temp.resize(n_states);
	for (int i = 0; i < n_states; ++i) {
		for (int j = 0; j < n_alphas; ++j) {
			TF_temp[i].insert( TF[i][j] );
		}
	}

	TF_list.resize(n_states);
	for (int i = 0; i < n_states; ++i) {
		for (const auto &a : TF_temp[i]) {
			TF_list[i].push_back(a);
		}
	}
	TF_temp.clear();


	// Error check.
	if (cur == tokens.size() || tokens[cur] != "~ATOMICPROPOSITIONS~") {
		cout << "Expecting ~ATOMIC PROPOSITIONS~. Didn't find :(\n";
		return 6;
	} else {
		cur++;
	}

	/*
	Atomic Propositions.
	*/
	cout << "Processing Atomic Propositions.\n";
	n_labels = 0;
	while (cur < tokens.size() && tokens[cur] != "~LABELINGFUNCTION~") {
		auto label = tokens[cur];
		label_to_int[label] = n_labels;
		int_to_label.push_back (label);
		n_labels++;
		cur++;
	}

	// Error check.
	if (cur == tokens.size()) {
		cout << "Expecting ~LABELING FUNCTION~. Didn't find :(\n";
		return 7;
	} else {
		cur++;
	}
	
	/*
	Labels.
	*/
	cout << "Processing Labels.\n";
	LF.resize(n_states);
	for (auto &lf : LF) lf.resize(n_labels, false);
	while (cur < tokens.size()) {

		// Getting state
		auto state = tokens[cur];
		if (state_to_int.count (state) == 0) {
			cout << "Problem processing Labels. State not found.\n";
			return 701;
		}

		// Getting =
		cur++;
		if (cur >= tokens.size() || tokens[cur] != "=") {
			cout << "Problem processing Labels. Insufficient/incorrect info.\n";
			cout << "For " << tokens[cur-1] << endl;
			return 702;
		}

		// Getting labels
		cur++;
		while (cur < tokens.size() && label_to_int.count(tokens[cur]) != 0) {
			LF[state_to_int[state]][label_to_int[tokens[cur]]] = true;

			if (cur+1 < tokens.size() && tokens[cur+1] == "|")
				cur++;
			cur++;
		}
	}

	// print_model ();

	return 0;
}


void Model::print_model () {

	cout << "\n\nModel built.\n";

	// States
	cout << "\nNumber of states = " << n_states;
	assert (n_states == int_to_state.size());
	assert (state_to_int.size() == int_to_state.size());
	cout << "\nPrinting states and id\n";
	for (const auto &a : state_to_int) cout <<"( "<< a.first <<","<< a.second <<" ), ";
	cout << "\nPrinting id and states again\n";
	for (const auto &a : int_to_state) cout << a << ", ";

	// Partitions
	cout << "\n\nNumber of partitions / players = " << n_partitions;
	assert (n_partitions == partitions.size());
	cout << "\nPrinting player and states";
	for (const auto &a : partitions) {
		for (const auto &b : a) {
			cout << int_to_state[b] << ", ";
		}
		cout << endl;
	}

	// Initial State
	cout << "\nInitial state. = " << init_state;
	cout << " = " << int_to_state[init_state];
	cout << " = " << state_to_int[ int_to_state[init_state] ] << endl;

	// Alphabets
	cout << "\nNumber of alphabets = " << n_alphas;
	assert (n_alphas == int_to_alpha.size());
	assert (alpha_to_int.size() == int_to_alpha.size());
	cout << "\nPrinting alphas and id\n";
	for (const auto &a : alpha_to_int) cout <<"( "<< a.first <<","<< a.second <<" ), ";
	cout << "\nPrinting id and alphas again\n";
	for (const auto &a : int_to_alpha) cout << a << ", ";

	// Transition Function
	cout << "\n\nTransition function\n";
	for (int i = 0; i < n_states; ++i) {
		cout << int_to_state[i] << " --> ";
		for (int j = 0; j < n_alphas; ++j) {
			cout << int_to_alpha[j] << " : " << int_to_state[ TF[i][j] ] << " | ";
		}
		cout << endl;
	}
	cout << "\n\nTransition function List\n";
	for (int i = 0; i < n_states; ++i) {
		for (const auto a : TF_list[i]) {
			cout << a << " ";
			if (a > 6) break;
			cout << int_to_state[a] << " ";
		}
		cout << endl;
	}

	// Labels
	cout << "\nNumber of APs (labels) = " << n_labels;
	assert (n_labels == int_to_label.size());
	assert (label_to_int.size() == int_to_label.size());
	cout << "\nPrinting labels and id\n";
	for (const auto &a : label_to_int) cout <<"( "<< a.first <<","<< a.second <<" ), ";
	cout << "\nPrinting id and labels again\n";
	for (const auto &a : int_to_label) cout << a << ", ";

	// Labels
	cout << "\n\nLabels\n";
	assert (LF.size() == n_states);
	for (int i = 0; i < n_states; ++i) {
		cout << int_to_state[i] << " : ";
		assert (LF[i].size() == n_labels);
		for (int j = 0; j < n_labels; ++j) {
			if (LF[i][j]) {
				cout << int_to_label[j] << " | ";
			}
		}
		cout << endl;
	}
}


int Model::parse_specs (const string &file_name) {
	auto tokens = lexer (file_name);

	int cur = 0;
	if (tokens[cur] != "~REACHABILITY~") {
		cout << "Error parsing Specifications. Expecting ~REACHABILITY~\n";
		return 1;
	}

	reach_labels.resize(n_partitions);
	cur++;
	int i = 0;
	cout << "Processing Specifications.\n";
	bool bracket = false;
	while (cur < tokens.size()) {
		auto token = tokens[cur];

		int type = 1;
		if (token[0] == '~') {
			token = token.substr(1);
			type = 0;
		}
		
		// Bracket checks
		if (bracket == false && token != "{") {
			cout << "Error while processing specs. Expecting '{', not found.\n";
			return 101;
		}
		else if (bracket == true && token == "{" ) {
			cout << "Error while processing specs. Not expecting '{', found.\n";
			return 102;
		}

		if (token == "{") {
			bracket = true;
			reach_labels[i].resize(n_labels, -1);
		}
		else if (token == "}") {
			i++;
			bracket = false;
		} else {
			if ( label_to_int.count (token) == 0 ) {
				cout << "Error while processing specs. State " << token << " not found.\n";
				return 103;
			} else {
				reach_labels[i][label_to_int[token]] = type;
			}
		}
		cur++;
	}

	if (bracket) {
		cout << "Error while processing specs. Expecting '}', not found.\n";
		return 104;
	}

	// Processing label set
	reach_set.resize (n_partitions);
	for (int i = 0; i < n_partitions; ++i) {
		assert(reach_labels[i].size() == n_labels);
		
		// First inserting all states
		for (int j = 0; j < n_states; ++j) {
			bool ins = true;
			for (int k = 0; k < n_labels; ++k) {
				if (reach_labels[i][k] < 0) {
					continue;
				}

				if (int(LF[j][k]) != reach_labels[i][k]) {
					ins = false;
					break;
				}
			}
			if (ins) {
				reach_set[i].insert (j);
			}
		}

	}

	// print_specs ();

	return 0;
}

void Model::print_specs () {

	// for (const auto &rs : reach_labels) {
	// 	for (const auto &a : rs) {
	// 		cout << a << " ";
	// 	}
	// 	cout << endl;
	// }


	assert (reach_set.size() == n_partitions);

	for (const auto &rs : reach_set) {
		for (const auto &a : rs) {
			cout << int_to_state[a] << " ";
		}
		cout << endl;
	}

}
