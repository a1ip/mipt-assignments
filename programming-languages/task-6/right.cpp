#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

struct Generation {
	string word;
	string chain;

	Generation(const string& word, const string& chain) : word(word), chain(chain) { }
};

class Producer {
public:
	Producer() { }

	void AddProduction(char left, const string& right) {
		productions_[left].push_back(right);
	}

	void Produce(const Generation& input, vector<Generation>& output) {
		UpdateNonterminals_();

		const string& word = input.word;

		size_t entry = word.find_last_of(nonterminals_);
		if(entry != string::npos) {
			for(size_t i = 0; i < productions_[word[entry]].size(); i++) {
				output.push_back(Derive_(input, entry, productions_[word[entry]][i]));
			}
		}
	}

	void Produce(const vector<Generation>& input, vector<Generation>& output) {
		for(size_t i = 0; i < input.size(); i++) {
			Produce(input[i], output);
		}
	}

private:
	void UpdateNonterminals_() {
		nonterminals_.clear();
		nonterminals_.reserve(productions_.size());

		map<char, vector<string> >::const_iterator itr;
		for(itr = productions_.begin(); itr != productions_.end(); ++itr) {
			nonterminals_.push_back(itr->first);
		}
	}

	string IntToString_(int number) const {
		stringstream converter;
		string result;

		converter << number;
		converter >> result;

		return result;
	}

	Generation Derive_(const Generation& generation, size_t entry, const string& production) const {
		Generation result = generation;

		result.word.replace(entry, 1, production);
		result.chain.append(IntToString_(entry));
		result.chain.append(production);

		return result;
	} 

	map<char, vector<string> > productions_;
	string nonterminals_;
};

class GenerationComparator {
public:
	bool operator () (const Generation& left, const Generation& right) const {
		/*if(left.word.size() < right.word.size()) {
			return true;
		}*/
		return left.word < right.word;
	}
};

const size_t max_depth = 20;

int main() {
	cout << "Staring generation..." << endl;

	Producer producer;
	producer.AddProduction('s', "is");
	producer.AddProduction('s', "ifes");
	producer.AddProduction('s', "o");
	producer.AddProduction('f', "ifef");
	producer.AddProduction('f', "o");

	vector<vector<Generation> > generations;
	generations.resize(max_depth + 1);

	generations[0].push_back(Generation("s", ""));

	for(size_t i = 0; i < max_depth; i++) {
		producer.Produce(generations[i], generations[i + 1]);
	}

	vector<Generation> final;

	for(size_t i = 0; i < generations.size(); i++) {
		for(size_t j = 0; j < generations[i].size(); j++) {
			final.push_back(generations[i][j]);
		}
	}

	cout << "Starting sorting..." << endl;

	sort(final.begin(), final.end(), GenerationComparator());

	cout << "Starting searching..." << endl;

	for(size_t i = 0; i < final.size() - 1; i++) {
		if(final[i].word == final[i + 1].word) {
			cout << "<" << final[i].word << "> <" << final[i].chain << ">" << endl;
			cout << "<" << final[i + 1].word << "> <" << final[i + 1].chain << ">" << endl;
		}
	}

	cout << "Finished!" << endl;

	return 0;
}

