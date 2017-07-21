#pragma once
#include "../config.hpp"
#include <filesystem>
#include <fstream>
#include <vector>
#include "../util/flexcode.hpp"

namespace stx
{
	namespace filesystem
	{
		using namespace std::tr2::sys;

		path executable_path();
		path executable_dir();

		struct line
		{
			line() = delete;
			explicit line(unsigned int num)
				: number(num)
				, ifs(nullptr)
			{};

			friend line& operator >> (line& l, stx::tstring& str);
			friend line& operator >> (stx::tifstream& ifs, line& line);
			friend std::vector<line>& operator >> (stx::tifstream& ifs, std::vector<line>& lines);
			friend std::vector<line>& operator >> (std::vector<line>& lines, std::vector<stx::tstring>& str);

		public:
			unsigned int number;
		private:
			stx::tifstream* ifs;
		};

	}
}