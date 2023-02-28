///////////////////////////////////////////////////////////////////////
// Tests for the commandline arguments parser
//
// The commandline arguments parser should be responsible for providing
// information about all settings that can be set for the various
// tools as commandline parameters, and provide default values for
// parameters that were not specified.
///////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include <utility/include/cmdargs_parser.h>

// No "actual testing", but demonstrating how to construct cmdargs_parser
TEST(cmdargs_parser, construction_empty_cmdline_args)
{
	const int argc = 1;
	const char* argv[argc] { "cantool" };	// There will always be a first parameter with the name of the executable
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_input_interface_with_default_name)
{
	const std::string input = "can";
	const std::string interface_name = "any";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--input", input.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, require_input_interface_type_after_keyword)
{
	const int argc = 2;
	const char* argv[argc] { "cantool", "--input" };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_FALSE(parser.valid());
}

// When no input interface is specified, use the default
TEST(cmdargs_parser, use_default_input_interface)
{
	const std::string input = "can";
	const std::string interface_name = "any";
	const int argc = 1;
	const char* argv[argc] { "cantool" };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_input_interface_type_and_name)
{
	const std::string input = "can";
	const std::string interface_name = "vcan0";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--input", input.c_str(), interface_name.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

// Use a different interface name (i.e. it cannot be a hardcoded constant, as would be the first TDD behaviour)
TEST(cmdargs_parser, specify_input_interface_type_and_name2)
{
	const std::string input = "can";
	const std::string interface_name = "can1";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--input", input.c_str(), interface_name.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_type).c_str(), input.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::input_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_output_interface_with_default_name)
{
	const std::string output = "can";
	const std::string interface_name = "any";
	const int argc = 3;
	const char* argv[argc] { "cantool", "--output", output.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, require_output_interface_type_after_keyword)
{
	const int argc = 2;
	const char* argv[argc] { "cantool", "--output" };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_FALSE(parser.valid());
}

// When no output interface is specified, use the default
TEST(cmdargs_parser, use_default_output_interface)
{
	const std::string output = "can";
	const std::string interface_name = "any";
	const int argc = 1;
	const char* argv[argc] { "cantool" };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

TEST(cmdargs_parser, specify_output_interface_type_and_name)
{
	const std::string output = "can";
	const std::string interface_name = "vcan0";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--output", output.c_str(), interface_name.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}

// Use a different interface name (i.e. it cannot be a hardcoded constant, as would be the first TDD behaviour)
TEST(cmdargs_parser, specify_output_interface_type_and_name2)
{
	const std::string output = "can";
	const std::string interface_name = "can1";
	const int argc = 4;
	const char* argv[argc] { "cantool", "--output", output.c_str(), interface_name.c_str() };
	utility::cmdargs_parser parser{ argc, argv };

	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_type).c_str(), output.c_str());
	EXPECT_STREQ(parser.get(utility::cmdargs_parser::values::output_interface_name).c_str(), interface_name.c_str());
	EXPECT_TRUE(parser.valid());
}
