#pragma once
#include "Framework.hpp"
#include "FileIO.hpp"

namespace Version1 {

	auto Test() {
		

		std::vector<byte> readData(FileIO::Read::File(LR"(data/Rozk³ad roku 22_23.pdf)"));
		FileIO::Write::File(LR"(data/aaa.pdf)", readData.size(), readData.data());

		return 0;
	}

}

// encoding

// decoding