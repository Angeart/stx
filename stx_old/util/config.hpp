#pragma once
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "flexcode.hpp"

namespace stx
{
	namespace config_detail
	{
		using boost::filesystem::path;
		using tptree = boost::property_tree::basic_ptree<tstring,tstring>;

		class config
		{
			
		};
	}
}