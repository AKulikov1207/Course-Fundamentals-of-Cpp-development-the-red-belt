#include "test_runner.h"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <deque>

using namespace std;

class Translator {
public:
	Translator() = default;

	void Add(string_view source, string_view target) {
		const string_view source_view = GetClonedView(source);
		const string_view target_view = GetClonedView(target);
		forward_lang[source_view] = target_view;
		backward_lang[target_view] = source_view;
	}

	string_view TranslateForward(string_view source) const {
		return Translate(forward_lang, source);
	}

	string_view TranslateBackward(string_view target) const {
		return Translate(backward_lang, target);
	}

private:
	string_view GetClonedView(string_view str) {
		for (const auto* map_ptr : { &forward_lang, &backward_lang }) {
			const auto it = map_ptr->find(str);
			if (it != map_ptr->end()) {
				return it->first;
			}
		}
		return data.emplace_back(str);
	}

	static string_view Translate(const map<string_view, string_view>& dict, string_view str) {
		if (const auto it = dict.find(str); it != dict.end()) {
			return it->second;
		} else {
			return {};
		}
	}

	map<string_view, string_view> forward_lang;
	map<string_view, string_view> backward_lang;
	deque<string> data;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
