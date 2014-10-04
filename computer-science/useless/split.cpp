#include <iostream>
#include <vector>
#include <string>

#include <assert.h>

void split(const std::string& source, const std::string& delimeters, std::vector<std::string>* output) {
	output->clear();

	size_t read_till = 0;

	while(true) {
		size_t current_entry = source.find_first_of(delimeters, read_till);
		if(current_entry == std::string::npos) {
			break;
		}
		if(read_till != current_entry) {
			output->push_back(source.substr(read_till, current_entry - read_till));
		}

		read_till = current_entry + 1;
	}
	if(read_till != source.size()) {
		output->push_back(source.substr(read_till, source.size() - read_till));
	}
}

void assert_split(
		const std::string& source,
		const std::string& delimeters,
		const std::string& output0 = "",
		const std::string& output1 = "",
		const std::string& output2 = "") {
	size_t pieces_number = 3;
	if(output2 == "") {
		pieces_number--;
	}
	if(output1 == "") {
		pieces_number--;
	}
	if(output0 == "") {
		pieces_number--;
	}

	std::vector<std::string> output;
	split(source, delimeters, &output);

	assert(output.size() == pieces_number);

	if(pieces_number > 0) {
		assert(output[0] == output0);
	}
	if(pieces_number > 1) {
		assert(output[1] == output1);
	}
	if(pieces_number > 2) {
		assert(output[2] == output2);
	}
}

void test_split() {
	assert_split("", " ");
	assert_split(" ", " ");
	assert_split("       ", " ");

	assert_split("a", " ", "a");
	assert_split("aa ", " ", "aa");
	assert_split(" aaa", " ", "aaa");
	assert_split(" aaaa ", " ", "aaaa");

	assert_split("b   ", " ", "b");
	assert_split("   bb", " ", "bb");
	assert_split("   bbb   ", " ", "bbb");

	assert_split("a b", " ", "a", "b");
	assert_split(" a b", " ", "a", "b");
	assert_split("a b ", " ", "a", "b");
	assert_split(" a b ", " ", "a", "b");
	assert_split("   a   b   ", " ", "a", "b");

	assert_split("aaa bbb", " ", "aaa", "bbb");
	assert_split(" aaa bbb", " ", "aaa", "bbb");
	assert_split("aaa bbb ", " ", "aaa", "bbb");
	assert_split(" aaa bbb ", " ", "aaa", "bbb");
	assert_split("   aaa   bbb   ", " ", "aaa", "bbb");

	assert_split("a beta gamma", " \t", "a", "beta", "gamma");
	assert_split("	alpha	b	gamma	", " \t", "alpha", "b", "gamma");
	assert_split(" 	 alpha 	 beta 	 g 	 ", " \t", "alpha", "beta", "g");
	assert_split("	a beta	gamma", " \t", "a", "beta", "gamma");
	assert_split("alpha	beta	g", " \t", "alpha", "beta", "g");
}

int main() {
	test_split();
	return  0;
}

