#include "ArgumentsParser.h"
#include <numeric>

static void erase_duplicates(std::vector<std::string>& vec)
{
	//such method, because vector in most cases will be very small
	std::sort(vec.begin(), vec.end());
	vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

bool ArgumentsParser::parse_arguments(int argc, char* argv[])
{
	try
	{
		for (int count{ 1 }; count < argc - 1; count += 2)
		{
			std::cout << count/2 << ' ' << argv[count] << "=" << argv[count+1] << "\n";
		}
		
		auto result = parsed_options_.parse(argc, argv);
		if (result.arguments().empty() || result.count("help"))
		{
			return false;
		}

		std::vector<std::string> unmatched_args = result.unmatched();
		if (!unmatched_args.empty())
		{
			std::cout << "Recheck input args! Unmatched =" << std::accumulate(unmatched_args.begin(), unmatched_args.end(), std::string(" ")) << "\n";
			return false;
		}

		const bool file_parse_result = read_file_to_process(result);
		if(!file_parse_result)
		{
			return false;
		}

		const bool methods_parse_result = read_methods_to_process(result);
		if (!methods_parse_result)
		{
			return false;
		}

		bool words_to_count_parse_result = read_words_to_count(result);
		if (!methods_parse_result)
		{
			return false;
		}

		return true;
	}
	catch (cxxopts::OptionSpecException& option_spec_exc)
	{
		std::cout << option_spec_exc.what() << "\n";
		return false;
	}
	catch (cxxopts::OptionParseException& option_parse_exc)
	{
		std::cout << option_parse_exc.what() << "\n";
		return false;
	}
}

bool ArgumentsParser::read_file_to_process(cxxopts::ParseResult& parse_result)
{
	if (!parse_result.count("file"))
	{
		std::cout << "No file to process!\n";
		return false;
	}

	file_to_process_ = parse_result["file"].as<std::vector<std::string>>();
	erase_duplicates(file_to_process_);
	return true;
}

bool ArgumentsParser::read_methods_to_process(cxxopts::ParseResult& parse_result)
{
	if (!parse_result.count("method"))
	{
		std::cout << "No method to process!\n";
		return false;
	}

	std::vector<std::string> methods_names = parse_result["method"].as<std::vector<std::string>>();
	for (std::string method_name : methods_names)
	{
		ProcessMethod method = get_method_by_name(method_name);
		if (std::find(process_methods_.begin(), process_methods_.end(), method) != process_methods_.end())
		{
			continue;
		}

		process_methods_.push_back(method);
	}

	return true;
}

bool ArgumentsParser::read_words_to_count(cxxopts::ParseResult& parse_result)
{
	if (std::find(process_methods_.begin(), process_methods_.end(), ProcessMethod::words) == process_methods_.end())
	{
		return true;
	}

	words_to_count_ = parse_result["value"].as<std::vector<std::string>>();
	if (words_to_count_.empty())
	{
		std::cout << "Words to count have no values!\n";
		process_methods_.erase(std::remove(process_methods_.begin(), process_methods_.end(), ProcessMethod::words),
		                       process_methods_.end());
		return true;
	}

	if (process_methods_.empty())
	{
		std::cout << "To ONLY count words there have to be even one -v value!\n";
		return false;
	}

	erase_duplicates(words_to_count_);
	return true;
}

ArgumentsParser::ArgumentsParser()
{
	parsed_options_ = cxxopts::Options("Test program", "C++ console app for words count and checksum calculation! Have a nice day!");
	parsed_options_.add_options()
		("f,file", "Input file to process", cxxopts::value<std::vector<std::string>>())
		("m,method", "Operation while process (\"words\" - count words, \"checksum\" - calculate uint32 checksum of all words (CRC))", 
			cxxopts::value<std::vector<std::string>>())
		("v,value", "Word to count", cxxopts::value<std::vector<std::string>>())
		("h,help", "Print usage");
}

ArgumentsParser::~ArgumentsParser()
{
	delete(&parsed_options_);
}
