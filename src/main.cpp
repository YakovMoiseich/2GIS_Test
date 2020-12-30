#include <iostream>

#include "main.h"


#include <chrono>
#include <functional>
#include <map>

#include "ArgumentsParser/ArgumentsParser.h"
#include <codecvt>
#include <clocale>


int main(int argc, char* argv[])
{
	std::setlocale(0, "");
 	auto args_parser = new ArgumentsParser;
	bool const parse_result = args_parser->parse_arguments(argc, argv);
	if (!parse_result)
	{
		std::cout << args_parser->get_help();
		cout << "Enter to exit the program: ";
		cin.ignore().get();
		return 0;
	}

	std::vector<std::string> files_to_process = args_parser->get_files_to_process();
	std::vector<ProcessMethod> methods_to_process = args_parser->get_process_methods();
	std::vector<std::string> words_to_process = args_parser->get_words_to_count();

	for (std::string file_to_process : files_to_process)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		std::string process_report = FileProcessor::process_file(file_to_process, methods_to_process, words_to_process);
		auto t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		std::cout << main::readable_duration("Operation took: ", duration) << "\n";
		std::cout << process_report;
	}
	
	cout << "Enter to exit the program: ";
	cin.ignore().get();
	return 0;
}
