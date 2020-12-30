#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <cwctype>

#include "../../include/CRC.h"

enum class ProcessMethod { unknown, checksum, words };
static const std::unordered_map<std::string, ProcessMethod> names_methods = { {"unknown", ProcessMethod::unknown}, {"checksum", ProcessMethod::checksum}, {"words", ProcessMethod::words} };
static ProcessMethod get_method_by_name(std::string& method_name)
{
	auto it = names_methods.find(method_name);
	if (it != names_methods.end()) {
		return it->second;
	}
	
	return ProcessMethod::unknown;
}

class FileProcessor
{
	public:
	bool static compare_char(char& c1, char& c2);
	bool static case_insensitive_compare(std::string& str1, std::string& str2);

	static size_t calculate_file_size(std::string& file_path);
	static std::map<std::string, int> count_words(std::ifstream& file, std::vector<std::string>& words_to_count);
	static void split_and_count(std::string& word, std::vector<std::string>& words_to_count, std::map<std::string, int>& found_words);
	static bool compare_words(std::string& word_from_compare, std::string& word_to_find);
	static bool u32_compare_words(std::u32string& word_from_file, std::u32string& word_to_find);
	static int32_t count_check_sum(std::string& file_path);
	static uint32_t count_check_sum_with_table(std::ifstream& file);
	static uint32_t count_CRC(const void* word, CRC::Table<std::uint32_t, 32> table);
	static std::vector<std::string> split_by_alphanumeric_chars(std::string& str);

	static std::string process_file(std::string& file_path, std::vector<ProcessMethod>& methods, std::vector<std::string>& words_to_count);
};
