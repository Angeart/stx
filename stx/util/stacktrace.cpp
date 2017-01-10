#include "stacktrace.hpp"
#include <boost/predef/compiler.h>
#include "../filesystem/filesystem.hpp"

#if BOOST_COMP_GNUC_EMULATED
#include <execinfo.h>
#endif
#if BOOST_COMP_MSVC_EMULATED
#include <iomanip>
#include <windows.h>
#include <dbghelp.h>
#include <array>
#endif

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <allocators>
#include <sstream>


namespace stx
{
	namespace debug
	{
#if _M_IX86 && BOOST_COMP_MSVC_EMULATED
		using long_x = unsigned long;
#else
		using long_x = unsigned long long;
#endif
		stx::tstring backtrace(unsigned int trace_size)
		{

			std::stringstream ss;
			stx::tstring result_str;

#if BOOST_COMP_GNUC_EMULATED
			using debug_result = std::vector<stx::tstring>;

			std::allocator<void*> alloc;
			auto trace = alloc.allocate(trace_size);

			auto size = backtrace(trace, trace_size);

			auto symbols = backtrace_symbols(trace, size);

			debug_result result(symbols, symbols + size);

			auto count = result.size();
			for (auto& r : result)
			{
				count--;
				ss <<
					__func__ <<
					" : backtrace [" <<
					count <<
					"] " <<
					r.c_str() <<
					std::endl;
			}

			free(symbols);

			alloc.destroy(trace);

#endif
#if BOOST_COMP_MSVC_EMULATED
			static struct sym_t {
				HANDLE proc;
				sym_t() {
					proc = GetCurrentProcess();
					SymSetOptions(
						SYMOPT_DEFERRED_LOADS | // シンボルを参照する必要があるときまで読み込まない
						SYMOPT_LOAD_LINES |     // 行番号情報を読み込む
						SYMOPT_UNDNAME          // すべてのシンボルを装飾されていない形式で表します
					);
					if (!SymInitialize(proc, 0, true)) {
						throw std::exception("error : SymInitialize");
					}
					// cout << "<SymInitialize>" << endl;
				}
				~sym_t() {
					SymCleanup(proc);
					// cout << "<SymCleanup>" << endl;
				}
			} s_sym;

			std::array<void*, 8> addr;
			int count = RtlCaptureStackBackTrace(0, addr.size(), &addr[0], 0);
			ss << "---- BEGIN BACKTRACE ----" << std::endl;
			for (int Li = 1; Li < count; ++Li) {
				auto p = reinterpret_cast<uintptr_t>(addr[Li]);

				IMAGEHLP_MODULE module;
				::memset(&module, 0, sizeof(module));
				module.SizeOfStruct = sizeof(module);
				if (!SymGetModuleInfo(s_sym.proc, p, &module)) {
					throw std::exception("error : SymGetModuleInfo");
				}

				char buffer[sizeof(IMAGEHLP_SYMBOL) + MAX_PATH];
				::memset(buffer, 0, sizeof(buffer));
				auto symbol = reinterpret_cast<IMAGEHLP_SYMBOL*>(buffer);
				symbol->SizeOfStruct = sizeof(*symbol);
				symbol->MaxNameLength = MAX_PATH;

				long_x disp = 0;
				if (!SymGetSymFromAddr(s_sym.proc, p, &disp, symbol)) {
					throw std::exception("error : SymGetSymFromAddr");
				}
				if (!strcmp(symbol->Name, "__tmainCRTStartup")) {
					break;
				}

				stx::tstring text = "?";
				IMAGEHLP_LINE line;
				::memset(&line, 0, sizeof(line));
				line.SizeOfStruct = sizeof(line);
				DWORD disp2 = 0;
				if (!SymGetLineFromAddr(s_sym.proc, p, &disp2, &line)) {
					line.FileName = "?";
					line.LineNumber = 0;
					text = "?";
				}
				else {
					std::ifstream ifs(line.FileName, std::ios::in);
					stx::filesystem::line l(line.LineNumber);
					ifs >> l >> text;
				}

				ss << Li
					<< " : 0x" << std::hex << std::setw(sizeof(uintptr_t) * 2) << std::setfill('0') << p
					<< " : " << module.ModuleName
					<< " : " << symbol->Name
					<< " : " << line.FileName << "(" << std::dec << line.LineNumber << ")"
					<< " : " << text.c_str()
					<< std::endl;
			}
			ss << "---- END BACKTRACE ----" << std::endl << std::endl;
#endif
			ss >> result_str;

			return result_str;
		}
	}
}