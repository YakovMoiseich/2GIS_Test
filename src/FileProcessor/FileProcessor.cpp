#include <map>

#include "FileProcessor.h"
#include <algorithm>
#include <sstream>
#include <locale>
#include <functional>
#include <codecvt>

bool FileProcessor::compare_char(char& c1, char& c2)
{
	if (c1 == c2)
	{
		return true;
	}
	if (std::toupper(c1) == std::toupper(c2))
	{
		return true;
	}

	return false;
}

bool FileProcessor::case_insensitive_compare(std::string& str1, std::string& str2)
{
	return ((str1.size() == str2.size())
		&& std::equal(str1.begin(), str1.end(), str2.begin(), &compare_char));
}

size_t FileProcessor::calculate_file_size(std::string& file_path)
{
	std::streampos begin, end;
	std::ifstream file(file_path, std::ios::binary);
	if (!file.is_open()) return -1;
	
	begin = file.tellg();
	file.seekg(0, std::ios::end);
	end = file.tellg();
	file.close();
	
	return end - begin;
}

std::map<std::string, int> FileProcessor::count_words(std::ifstream& file, std::vector<std::string>& words_to_count)
{
	std::map<std::string, int> found_words = {};
	for (const std::string& word_to_count : words_to_count)
	{
		found_words[word_to_count] = 0;
	}
	
	std::string word;
	while (file >> word)
	{
		split_and_count(word, words_to_count,found_words);
	}

	return found_words;
}

void FileProcessor::split_and_count(std::string& word, std::vector<std::string>& words_to_count, std::map<std::string, int>& found_words)
{
	std::vector<std::string> splitted_words = split_by_alphanumeric_chars(word);
	for (std::string word_to_count : words_to_count)
	{
		for (std::string splitted_word : splitted_words)
		{
			bool is_equal = compare_words(splitted_word, word_to_count);
			found_words[word_to_count] = is_equal ? ++found_words[word_to_count] : found_words[word_to_count];
		}
	}
}

bool FileProcessor::compare_words(std::string& word_from_file, std::string& word_to_find)
{
	return case_insensitive_compare(word_from_file, word_to_find);
}

int32_t FileProcessor::count_check_sum(std::string& file_path)
{
	std::ifstream file;
	file.open(file_path);
	if (!file.is_open()) return -1;

	uint32_t check_sum = 0;
	std::string word;
	while (file >> word)
	{
		auto cstr = word.c_str();
		uint32_t crc = CRC::Calculate(cstr, sizeof(cstr), CRC::CRC_32());
		check_sum += crc;
	}

	return check_sum;
}

uint32_t FileProcessor::count_check_sum_with_table(std::ifstream& file)
{
	CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());
	uint32_t check_sum = 0;
	std::string word;
	while (file >> word)
	{
		auto cstr = word.c_str();
		uint32_t crc = CRC::Calculate(cstr, sizeof(cstr), table);
		check_sum += crc;
	}

	return check_sum;
}

uint32_t FileProcessor::count_CRC(const void* word, CRC::Table<std::uint32_t, 32> table)
{
	return CRC::Calculate(word, sizeof(word), table);
}

std::vector<std::string> FileProcessor::split_by_alphanumeric_chars(std::string& str)
{
	std::replace_if(str.begin(), str.end(), [](unsigned char ch){ return std::ispunct(ch); }, ' ');
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(str);
	while (std::getline(tokenStream, token, ' '))
	{
		token.erase(std::remove_if(token.begin(), token.end(), [](unsigned char ch){ return std::isspace(ch); }), token.end());
		if (token.empty())
		{
			continue;
		}

		tokens.push_back(token);
	}

	return tokens;
}

std::string FileProcessor::process_file(std::string& file_path, std::vector<ProcessMethod>& methods, std::vector<std::string>& words_to_count)
{
	std::string process_report = "Process file \"" + file_path + "\"\n";
	std::ifstream file;
	file.open(file_path);
	
	if (!file.is_open())
	{
		process_report += "Can't open file to process!";
		return process_report;
	}

	bool const no_correct_methods = methods.empty() || (methods.size() == 1 && methods[0] == ProcessMethod::unknown);
	if (methods.empty())
	{
		process_report += "No correct methods for processing!";
		return process_report;
	}

	const bool calculate_checksum_only = methods.size() == 1 && methods[0] == ProcessMethod::checksum;
	if (calculate_checksum_only)
	{
		uint32_t file_checksum = count_check_sum_with_table(file);
		process_report += "Checksum:\n";
		process_report += "    " + std::to_string(file_checksum) + "\n";
		return process_report;
	}

	const bool count_words_only = methods.size() == 1 && methods[0] == ProcessMethod::words;
	if (count_words_only)
	{
		process_report += "Words:\n";
		std::map<std::string, int> file_words = count_words(file, words_to_count);
		for (std::pair<std::string, int> file_word : file_words)
		{
			process_report += "    " + file_word.first + " found = " + std::to_string(file_word.second) + " times\n";
		}
		
		return process_report;
	}

	uint32_t checksum = 0;
	CRC::Table<std::uint32_t, 32> table(CRC::CRC_32());
	std::map<std::string, int> found_words = {};
	for (const std::string& word_to_count : words_to_count)
	{
		found_words[word_to_count] = 0;
	}

	int word_num = 0;
	std::string word;
	while (file >> word)
	{
		++word_num;		
		checksum += count_CRC(word.c_str(), table);
		split_and_count(word, words_to_count, found_words);
	}

	process_report += "Checksum:\n";
	process_report += "    " + std::to_string(checksum) + "\n";
	process_report += "Words:\n";
	for (std::pair<std::string, int> found_word : found_words)
	{
		process_report += "    " + found_word.first + " found = " + std::to_string(found_word.second) + " times\n";
	}
	
	return process_report;
}



