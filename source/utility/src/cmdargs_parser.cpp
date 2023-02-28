///////////////////////////////////////////////////////////////////////
// Commandline arguments parser implementation
//
// The commandline arguments parser should be responsible for providing
// information about all settings that can be set for the various
// tools as commandline parameters, and provide default values for
// parameters that were not specified.
///////////////////////////////////////////////////////////////////////
#include <utility/include/cmdargs_parser.h>

// Constructor - parses the commandline arguments
utility::cmdargs_parser::cmdargs_parser(int argc, const char** argv) :
	_valid(false),
	_values()
{
	auto isOK = true;

	for(int i = 0; i < argc; i++)
	{
		// Parse input interface specification
		if(std::string(argv[i]).compare("--input") == 0)
		{
			if(i+1 >= argc)		// Require the interface type to be specified
				isOK = false;
			else if(i+2 < argc)	// The interface name is optional - parse if supplied
				_values[values::input_interface_name] = argv[i+2];
			i += 2;
		}
		// Parse output interface specification
		else if(std::string(argv[i]).compare("--output") == 0)
		{
			if(i+1 >= argc)		// Require the interface type to be specified
				isOK = false;
			else if(i+2 < argc)	// The interface name is optional - parse if supplied
				_values[values::output_interface_name] = argv[i+2];
			i += 2;
		}
	}

	_valid = isOK;
}

// Retrieve a specified argument
std::string utility::cmdargs_parser::get(values parameter)
{
	// Lookup the parameter and return it if found
	auto v = _values.find(parameter);
	if(v != _values.end())
		return v->second;

	// Otherwise return a default value
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

// Returns a status indicating whether valid commandline arguments were specified
bool utility::cmdargs_parser::valid() const
{
	return _valid;
}
