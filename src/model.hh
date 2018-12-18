#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <cassert>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::set;
using std::map;
using std::queue;

class Model {

	// States
	int n_states;
	map <string, int> state_to_int;
	vector<string> int_to_state;

	// Partitions
	int n_partitions;
	vector< set<int> > partitions;

	// Initial State
	int init_state;

	// Alphabets
	int n_alphas;
	map <string, int> alpha_to_int;
	vector<string> int_to_alpha;

	// Transition Function
	vector<vector<int> > TF;
	vector<vector<int> > TF_list;	// Adjacency list

	// Atomic Propositions
	int n_labels;
	map <string, int> label_to_int;
	vector<string> int_to_label;

	// Labels
	// TODO: maybe storing differently helpful, like using set
	vector<vector<bool> > LF;

	// Reachability Specs.
	vector<vector<short> > reach_labels;
	vector<set<int> > reach_set;	// For each player, set of states (T_i).

	// Private Functions
	// For lexicographic analysis
	vector<string> lexer (const string &);
	void print_model ();
	void print_specs ();

	// For checking.
	set<int> intersect(const set<int>&, const set<int>&);
	bool bfs(const set<int>&, const set<int>&, vector<int>&);
	bool dfs(const set<int>&, const set<int>&, vector<int>&, vector<bool>&, int);
public:
	int construct (const string &);
	int parse_specs (const string &);
	int check ();
};