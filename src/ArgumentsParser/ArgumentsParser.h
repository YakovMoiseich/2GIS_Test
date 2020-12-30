#pragma once
#include <string>
#include <vector>

#include "../../include/cxxopts.hpp"
#include "../FileProcessor/FileProcessor.h"
#include <codecvt>

class ArgumentsParser
{
public:
	bool parse_arguments(int argc, char* argv[]);
	std::vector<std::string> get_words_to_count() const
	{
		return words_to_count_;
	}
	std::vector<std::u16string> get_u16_words_to_count() const
	{
		return u16_words_to_count_;
	}
	std::vector<ProcessMethod> get_process_methods() const
	{
		return process_methods_;
	}
	std::vector<std::string> get_files_to_process() const
	{
		return file_to_process_;
	}
	std::string get_help() const
	{
		return parsed_options_.help();
	}

	ArgumentsParser();
	~ArgumentsParser();
	
private:
	cxxopts::Options parsed_options_ = cxxopts::Options("Undefined program", "Undefined help");
	std::vector<std::string> file_to_process_;
	std::vector<std::string> words_to_count_;
	std::vector<std::u16string> u16_words_to_count_;
	std::vector<ProcessMethod> process_methods_;

	bool read_file_to_process(cxxopts::ParseResult& parse_result);
	bool read_methods_to_process(cxxopts::ParseResult& parse_result);
	bool read_words_to_count(cxxopts::ParseResult& parse_result);
};

