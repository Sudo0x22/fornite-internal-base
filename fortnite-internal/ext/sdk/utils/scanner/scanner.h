#pragma once
#include"../spoofer/spoofer.hpp"
#include<vector>
#include<memory>

namespace memory
{
	size_t StrLen(const char* pattern)
	{
		return SpoofReturn(strlen, pattern);
	}
	unsigned long _cdecl Strtoul(const char* s, char** ss, int index)
	{
		return SpoofReturn(strtoul, s, ss, index);
	}
}

class Scanner
{
public:
	inline std::uint8_t* PatternScanner(LPVOID RawData, LPCSTR Sig)
	{
		static auto pattern_to_byte = [](LPCSTR Pattern)
		{
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(Pattern);
			auto end = const_cast<char*>(Pattern) + memory::StrLen(Pattern);
			
			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(memory::Strtoul(current, &current, 16));
				}
			}
			return bytes;

		};

		auto dos_header = (PIMAGE_DOS_HEADER)RawData;
		auto nt_headers = (PIMAGE_NT_HEADERS)((std::uint8_t*)RawData + dos_header->e_lfanew);

		auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte(Sig);
		auto scan_bytes = reinterpret_cast<std::uint8_t*>(RawData);

		auto size = pattern_bytes.size();
		auto data = pattern_bytes.data();

		for (auto i = 0ul; i < size_of_image - size; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < size; ++j)
			{
				if (scan_bytes[i + j] != data[j] && data[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scan_bytes[i];
			}
		}
		return nullptr;
	}
};