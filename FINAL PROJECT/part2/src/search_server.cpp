#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"

#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <iostream>

InvertedIndex::InvertedIndex(istream& document_input) {
	for (string document; getline(document_input, document); ) {
		docs.push_back(move(document));
		size_t docid = docs.size() - 1;

		for (string_view word : SplitIntoWordsView(docs.back())) {
			auto& word_docs = index[word];

			if (word_docs.empty() || word_docs.back().docid != docid) {
				word_docs.push_back({ 1, docid });
			}
			else {
				word_docs.back().count++;
			}
		}
	}
}

const vector<InvertedIndex::Results>& InvertedIndex::Lookup(string_view word) const {
	static const vector<Results> empty_result;

	if (auto it = index.find(word); it != index.end()) {
		return it->second;
	}

	return empty_result;
}

void UpdateDocumentBaseAsync(istream& document_input, Synchronized<InvertedIndex>& index) {
	InvertedIndex new_index(document_input);
	swap(index.GetAccess().ref_to_value, new_index);
}

void AddQueriesStreamAsync(istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index_handle) {
	vector<size_t> docid_count;
	vector<int64_t> docids;

	for (string current_query; getline(query_input, current_query); ) {
		{
			auto access = index_handle.GetAccess();
			const size_t docs_count = access.ref_to_value.GetDocuments().size();
			docid_count.assign(docs_count, 0);
			docids.resize(docs_count);

			auto& index = access.ref_to_value;

			for (string_view word : SplitIntoWordsView(current_query)) {
				for (const auto& result : index.Lookup(word)) {
					docid_count[result.docid] += result.count;
				}
			}
		}

		iota(docids.begin(), docids.end(), 0);
		partial_sort(
			docids.begin(),
			Head(docids, 5).end(),
			docids.end(),
			[&docid_count](int64_t lhs, int64_t rhs) {
				return make_pair(docid_count[lhs], -lhs) > make_pair(docid_count[rhs], -rhs);
			}
		);

		search_results_output << current_query << ':';
		for (size_t docid : Head(docids, 5)) {
			const size_t hitcount = docid_count[docid];
			if (hitcount == 0) break;

			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << hitcount << '}';
		}
		search_results_output << '\n';
	}
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	futures.push_back(async(UpdateDocumentBaseAsync, ref(document_input), ref(index)));
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output) {
	futures.push_back(
		async(AddQueriesStreamAsync, ref(query_input), ref(search_results_output), ref(index))
	);
}