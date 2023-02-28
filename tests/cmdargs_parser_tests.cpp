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
			input_interface_type,
			output_interface_type,
			input_interface_name,
			output_interface_name,
		};

	public:
		cmdargs_parser(int argc, const char** argv) : _valid(false), _values()
		{
			auto isOK = true;
			for(int i = 0; i < argc; i++)
			{
				if(std::string(argv[i]).compare("--input") == 0)
				{
					if(i+1 >= argc) isOK = false;
					else if(i+2 < argc) _values[values::input_interface_name] = argv[i+2];
					i += 2;
				}
				else if(std::string(argv[i]).compare("--output") == 0)
				{
					if(i+1 >= argc) isOK = false;
					else if(i+2 < argc) _values[values::output_interface_name] = argv[i+2];
					i += 2;
				}
			}
			_valid = isOK;
		}

		std::string get(values parameter)
		{
			// Lookup the parameter and return it if found
			auto v = _values.find(parameter);
			if(v != _values.end())
				return v->second;

			switch(parameter)
			{
				case values::input_interface_name:
				case values::output_interface_name:
					return "any";
				default:
					break;
			}

			return "can";
		}
		bool valid() const { return _valid; }

	private:
		bool _valid;
		std::map<values,std::string> _values;
};

// No "actual testing", but demonstrating how to construct cmdargs_parser
TEST(cmdargs_parser, construction_empty_cmdline_args)
{
	const int argc = 1;
	const char* argv[argc] { "cantool" };	// There will always be a first parameter with the name of the executable
	cmdargs_parser parser{ argc, argv };

	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_input_interface_with_default_name)
{
	const std::string input = "can";
	const std::string interface_name = "any";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--input", input.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, require_input_interface_type_after_keyword)
{
	const int argc = 2;
	const char* argv[argc] { "cantool", "--input" };
	cmdargs_parser parser{ argc, argv };

	EXPECT_FALSE(parser.valid());
}

// When no input interface is specified, use the default
TEST(cmdargs_parser, use_default_input_interface)
{
	const std::string input = "can";
	const std::string interface_name = "any";
	const int argc = 1;
	const char* argv[argc] { "cantool" };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_input_interface_type_and_name)
{
	const std::string input = "can";
	const std::string interface_name = "vcan0";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--input", input.c_str(), interface_name.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

// Use a different interface name (i.e. it cannot be a hardcoded constant, as would be the first TDD behaviour)
TEST(cmdargs_parser, specify_input_interface_type_and_name2)
{
	const std::string input = "can";
	const std::string interface_name = "can1";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--input", input.c_str(), interface_name.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_output_interface_with_default_name)
{
	const std::string output = "can";
	const std::string interface_name = "any";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--output", output.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, require_output_interface_type_after_keyword)
{
	const int argc = 2;
	const char* argv[argc] { "cantool", "--output" };
	cmdargs_parser parser{ argc, argv };

	EXPECT_FALSE(parser.valid());
}

// When no output interface is specified, use the default
TEST(cmdargs_parser, use_default_output_interface)
{
	const std::string output = "can";
	const std::string interface_name = "any";
	const int argc = 1;
	const char* argv[argc] { "cantool" };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_output_interface_type_and_name)
{
	const std::string output = "can";
	const std::string interface_name = "vcan0";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--output", output.c_str(), interface_name.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

// Use a different interface name (i.e. it cannot be a hardcoded constant, as would be the first TDD behaviour)
TEST(cmdargs_parser, specify_output_interface_type_and_name2)
{
	const std::string output = "can";
	const std::string interface_name = "can1";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--output", output.c_str(), interface_name.c_str() };
	cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}
