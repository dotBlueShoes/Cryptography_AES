#pragma once
#include "Framework.hpp"

namespace FileIO {

	namespace Read {

		auto File(
			IN	const wchar* const filePath
		) {
			std::ifstream inputFile(filePath, std::ios::binary);

			// READ
			std::vector<byte> buffer(std::istreambuf_iterator<char>(inputFile), {});
			inputFile.close();

			return buffer;
		}

	}

	namespace Write {

		auto File(
			IN	const wchar* const filePath,
			IN	const size bufferCount,
			IN	const byte* const buffer
		) {
			std::ofstream outputFile(filePath, std::ios::binary);

			for (size i = 0; i < bufferCount; ++i) {
				outputFile << buffer[i];
			}

			outputFile.close();
		}

	}

}