#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <string_view>

using namespace std;

class InvertedIndex {
public:
	struct Results {
		size_t count;
		size_t docid;
	};

	InvertedIndex() = default;
	explicit InvertedIndex(istream& document_input);
	const vector<Results>& Lookup(string_view word) const;

	const deque<string>& GetDocuments() const {
		return docs;
	}

private:
	map<string_view, vector<Results>> index;
	deque<string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input) : index(document_input) {}

	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
	InvertedIndex index;
};