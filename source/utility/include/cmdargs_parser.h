///////////////////////////////////////////////////////////////////////
// Commandline arguments parser
//
// The commandline arguments parser should be responsible for providing
// information about all settings that can be set for the various
// tools as commandline parameters, and provide default values for
// parameters that were not specified.
///////////////////////////////////////////////////////////////////////
#pragma once

#include <map>
#include <string>

namespace utility
{
	class cmdargs_parser
	{
		public:
			// Parameters than can be specified as commandline arguments and be retrieved from cmdargs_parser
			enum class values
			{
				input_interface_type,
				output_interface_type,
				input_interface_name,
				output_interface_name,
			};

		public:
			// Constructors / destructor
			cmdargs_parser(int argc, const char** argv);
			~cmdargs_parser() = default;

			// Do not allow copying
			cmdargs_parser(const cmdargs_parser&) = delete;
			cmdargs_parser& operator=(const cmdargs_parser&) = delete;

			// Public interface
			std::string get(values parameter);
			bool valid() const;

		private:
			// Data members
			bool _valid;
			std::map<values,std::string> _values;
	};
}
