///////////////////////////////////////////////////////////////////////
// Tests for the commandline arguments parser
//
// The commandline arguments parser should be responsible for providing
// information about all settings that can be set for the various
// tools as commandline parameters, and provide default values for
// parameters that were not specified.
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

enum class values
{
	input_interface,
};

class cmdargs_parser
{
	public:
		enum class values
		{
			input_interface,
			output_interface,
		};

	public:
		cmdargs_parser(int argc, const char** argv) {}

		std::string get(values parameter) { return "can"; }
};

// No "actual testing", but demonstrating how to construct cmdargs_parser
TEST(cmdargs_parser, construction_empty_cmdline_args)
{
	const int argc = 1;
	const char* argv[argc] { "cantool" };	// There will always be a first parameter with the name of the executable
	cmdargs_parser parser{ argc, argv };
}

TEST(cmdargs_parser, specify_input_interface)
{
	const std::string input = "can";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--input", input.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface).c_str(), input.c_str());
}

TEST(cmdargs_parser, specify_output_interface)
{
	const std::string output = "can";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--output", output.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface).c_str(), output.c_str());
}
