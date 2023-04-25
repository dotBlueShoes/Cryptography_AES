#pragma once
#include "Framework.hpp"
#include "FileIO.hpp"

#include "AES.hpp"

#include <iostream>

template <size bufforLength>
block ByteToWString(
	OUT wchar buffor[bufforLength],
	IN const byte& value
) {
	swprintf(buffor, sizeof(buffor) / sizeof(*buffor), L"%d", value);
	wprintf(L"[%ls]\n", buffor);
}

template <size bufforLength>
block BytesToWString(
	OUT wchar buffor[bufforLength],
	IN const byte* values
) {
	int written = 0;

	written += swprintf(buffor + written, sizeof(buffor) / sizeof(*buffor), L"%d", values[0]);
	for (size i = 1; i < 32; ++i) {

		// SEPARATION
		buffor[written] = L',';
		buffor[written + 1] = L' ';
		written += 2;

		written += swprintf(buffor + written, sizeof(buffor) / sizeof(*buffor), L"%d", values[i]);
	}
}

namespace Tests {

	auto ReadWriteTest() {
		std::vector<byte> readData(FileIO::Read::File(LR"(data/Rozk³ad roku 22_23.pdf)"));
		FileIO::Write::File(LR"(data/aaa.pdf)", readData.size(), readData.data());

		return 0;
	}

	auto KeyExpensionTest() {

		const AES::BitKey256::Key256 initialKey {
			0x00, 0x01, 0x02, 0x03,
			0x04, 0x05, 0x06, 0x07,
			0x08, 0x09, 0x0a, 0x0b,
			0x0c, 0x0d, 0x0e, 0x0f,
			0x10, 0x11, 0x12, 0x13,
			0x14, 0x15, 0x16, 0x17,
			0x18, 0x19, 0x1a, 0x1b,
			0x1c, 0x1d, 0x1e, 0x1f
		};

		AES::BitKey256::Key256 expandedKey { 0 };

		AES::BitKey256::KeyExpansion(expandedKey, initialKey);

		{ // Byte to wchar string
			array<wchar, 16> buffor;
			ByteToWString<buffor.size()>(buffor.data(), 5);
			MessageBox(nullptr, buffor.data(), L"Log", MB_OK);
		}

		{ // Byte Array to wchar string
			// 96 - max per byte signs generated.
			// 64 - spaces and comas.
			array<wchar, 96 + 64> buffor;
			BytesToWString<buffor.size()>(buffor.data(), initialKey);
			wprintf(L"[%ls]\n", buffor.data());
			MessageBox(nullptr, buffor.data(), L"Log", MB_OK);
		}
		
	}

}

// encoding

// decoding