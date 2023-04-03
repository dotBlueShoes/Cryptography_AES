#pragma once
#include "Framework.hpp"
#include "FileIO.hpp"

namespace Version1 {

	auto Test() {
		//byte* key = System.Text.Encoding.UTF8.GetBytes("ABCDEF0123456789");
		//byte* dataBlock = System.Text.Encoding.UTF8.GetBytes("Ala ma kota");

		//Ciphering(Ciphering(box, Klucz, 'E'), Klucz, 'D');

		std::vector<byte> readData(FileIO::Read::File(LR"(data/Rozk³ad roku 22_23.pdf)"));
		FileIO::Write::File(LR"(data/aaa.pdf)", readData.size(), readData.data());

		return 0;
	}

}

// encoding

// decoding