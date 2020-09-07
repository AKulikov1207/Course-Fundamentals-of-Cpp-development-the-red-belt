#include <string>
#include <iterator>
#include <iostream>
#include <list>

#include "test_runner.h"

using namespace std;

class Editor {
public:
	Editor() : pos{ 0 }, text{ list<char>() }, buffer{ list<char>() }, position{ text.begin() } {}

	void Left() {						// сдвинуть курсор влево
		if (pos != 0) {
			--pos;
			--position;
		}
	} 

	void Right() {						// сдвинуть курсор вправо
		if (pos != text.size()) {
			++pos;
			++position;
		}
	}

	void Insert(char token) {			// вставить символ token
		text.insert(position, token);
		++pos;
	}

	void Cut(size_t tokens = 1) {		// вырезать не более tokens символов, начиная с текущей позиции курсора
		buffer.clear();
		size_t tmp = pos;
		auto f_it = position;

		for (auto it = position; tmp != text.size() && tmp++ != pos + tokens; ++it, ++position)
			buffer.push_back(*it);

		text.erase(f_it, position);
	}

	void Copy(size_t tokens = 1) {	    // cкопировать не более tokens символов, начиная с текущей позиции курсора
		buffer.clear();
		size_t tmp = pos;

		for (auto it = position; tmp != text.size() && tmp++ != pos + tokens; ++it)
			buffer.push_back(*it);
	}

	void Paste() {						// вставить содержимое буфера в текущую позицию курсора
		for (const char& c : buffer) {
			text.insert(position, c);
			++pos;
		}
	}

	string GetText() const {			// получить текущее содержимое текстового редактора
		string tmp = string();

		for (const char& c : text)
			tmp.push_back(c);
		return tmp;
	}
private:
	size_t pos;
	list<char> text, buffer;
	list<char>::iterator position;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}