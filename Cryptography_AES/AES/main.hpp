#pragma once
#include "../Framework.hpp"
#include "../FileIO.hpp"
#include "AES.hpp"

#include <iostream>

#define LOG_TYPE L"Log"
#define LOG_128 L"-128-"
#define LOG_192 L"-192-"
#define LOG_256 L"-256-"
#define LOG_NOCODED L"NOCODED"
#define LOG_ENCODED L"ENCODED"
#define LOG_DECODED L"DECODED"

template <size bufforLength>
block Int64ToWString(
	OUT wchar buffor[bufforLength],
	IN const int64& value
) {
	swprintf(buffor, bufforLength, L"%lld", value);
}

template <size bufforLength>
block ByteToWString(
	OUT wchar buffor[bufforLength],
	IN const byte& value
) {
	swprintf(buffor, sizeof(buffor) / sizeof(*buffor), L"%d", value);
}

template <size bufforLength>
block BytesToWString(
	OUT wchar buffor[bufforLength],
	IN const byte* values,
	IN const size& valuesCount
) {
	int written = 0;

	written += swprintf(buffor + written, sizeof(buffor) / sizeof(*buffor), L"%d", values[0]);
	for (size i = 1; i < valuesCount; ++i) {

		// SEPARATION
		buffor[written] = L',';
		buffor[written + 1] = L' ';
		written += 2;

		written += swprintf(buffor + written, sizeof(buffor) / sizeof(*buffor), L"%d", values[i]);
	}
}

namespace Tests {

	auto Test128(
		IN const AES::Key128& key,
		IN const AES::Block& nocoded
	) {

		AES::BlockWcharBuffor buffor;
		AES::Block encoded, decoded;

		uint8* expandedKey = AES::Initialize(key.size());
		AES::ExpendKey<AES::Key128>(expandedKey, key);

		{
			BytesToWString<buffor.size()>(buffor.data(), nocoded.data(), nocoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_128 LOG_NOCODED, MB_OK);
		}

		AES::Encode<AES::Key128>(encoded, expandedKey, nocoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), encoded.data(), encoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_128 LOG_ENCODED, MB_OK);
		}

		AES::Decode<AES::Key128>(decoded, expandedKey, encoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), decoded.data(), decoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_128 LOG_DECODED, MB_OK);
		}

		free(expandedKey);
	}

	auto Test192(
		IN const AES::Key192& key,
		IN const AES::Block& nocoded
	) {

		AES::BlockWcharBuffor buffor;
		AES::Block encoded, decoded;

		uint8* expandedKey = AES::Initialize(key.size());
		AES::ExpendKey<AES::Key192>(expandedKey, key);

		{
			BytesToWString<buffor.size()>(buffor.data(), nocoded.data(), nocoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_192 LOG_NOCODED, MB_OK);
		}

		AES::Encode<AES::Key192>(encoded, expandedKey, nocoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), encoded.data(), encoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_192 LOG_ENCODED, MB_OK);
		}

		AES::Decode<AES::Key192>(decoded, expandedKey, encoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), decoded.data(), decoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_192 LOG_DECODED, MB_OK);
		}

		free(expandedKey);
	}

	auto Test256(
		IN const AES::Key256& key,
		IN const AES::Block& nocoded
	) {

		AES::BlockWcharBuffor buffor;
		AES::Block encoded, decoded;

		uint8* expandedKey = AES::Initialize(key.size());
		AES::ExpendKey<AES::Key256>(expandedKey, key);

		{
			BytesToWString<buffor.size()>(buffor.data(), nocoded.data(), nocoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_256 LOG_NOCODED, MB_OK);
		}

		AES::Encode<AES::Key256>(encoded, expandedKey, nocoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), encoded.data(), encoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_256 LOG_ENCODED, MB_OK);
		}

		AES::Decode<AES::Key256>(decoded, expandedKey, encoded);

		{
			BytesToWString<buffor.size()>(buffor.data(), decoded.data(), decoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_256 LOG_DECODED, MB_OK);
		}

		free(expandedKey);
	}

	auto ReadWriteTest() {
		//const wchar* const readFilePath = LR"(data/Rozk³ad roku 22_23.pdf)";
		//const wchar* const writFilePath = LR"(data/aaa.pdf)";
		const wchar* const readFilePath = LR"(data/1.txt)";
		const wchar* const writFilePath = LR"(data/2.txt)";

		std::vector<byte> readData(FileIO::Read::File(readFilePath));
		FileIO::Write::File(writFilePath, readData.size(), readData.data());

		return 0;
	}

	auto WcharByteTest(
		//IN const wchar& wValue,
		//IN const byte& bValue
	) {
		
		const byte bValue1 = 0b00001000, bValue2 = 0b00000100;
		const wchar wValue1 = 0b0000'1000'0000'0100;
		const wchar wValue2 = 0b0000'0100'0000'1000;
		wchar wValue = 0;

		wValue = bValue1;
		wValue <<= 8;
		wValue += bValue2;

		if (wValue == wValue1) {
			MessageBox(nullptr, L"1", LOG_TYPE, MB_OK);
		} else if (wValue == wValue2) {
			MessageBox(nullptr, L"2", LOG_TYPE, MB_OK);
		} else {
			MessageBox(nullptr, L"3", LOG_TYPE, MB_OK);
		}
		
	}

}

namespace AES {

	template <class KeyType>
	auto ReadEncodeWrite(
		OUT  uint8& wordsLeftCount,
		OUT  wchar* strEncoded,
		IN const wchar* const strNocoded,
		IN const size strNocodedCount,
		IN const KeyType& key
	) {
		// Likely uses the result of the division - single DIV instruction.
		const uint64 blocksCount = strNocodedCount / 16;
		wordsLeftCount = (strNocodedCount % 16) / 2;

		uint8 count(0);

		AES::Block nocoded, encoded;

		uint8* expandedKey = AES::Initialize(key.size());
		AES::ExpendKey<KeyType>(expandedKey, key);

		// For each Block
		for (size i = 0; i < blocksCount; ++i) {

			// Copy read data to Block form
			count = 0;
			for (uint8 j = 0; j < 8 /* wchars... /2 */; ++j) {
				nocoded[count] = (uint8)(strNocoded[(i * 8) + j] << 8); // high
				nocoded[count + 1] = (uint8)(strNocoded[(i * 8) + j]);  // low
				count += 2;
			}

			AES::Encode<KeyType>(encoded, expandedKey, nocoded);

			// Write encoded Block
			count = 0;
			for (size j = 0; j < 8; ++j) {
				wchar temp = encoded[count];
				temp <<= 8;
				temp += encoded[count + 1];

				strEncoded[(i * 8) + j] = temp;
				count += 2;
			}

		}

		// Last Block
		if (wordsLeftCount != 0) {
			// Initialize the whole block with 0'es so we don't do that with for-loop later.
			AES::Block lastBlock { 0 };

			count = 0;
			for (uint8 i = 0; i < wordsLeftCount; ++i) {
				lastBlock[count] = (uint8)(strNocoded[(strNocodedCount - (wordsLeftCount * 2)) / 2 + i] << 8); // high
				lastBlock[count + 1] = (uint8)(strNocoded[(strNocodedCount - (wordsLeftCount * 2)) / 2 + i]);  // low
				count += 2;
			}
		
			// Last block with 0'es
			AES::Encode<KeyType>(encoded, expandedKey, lastBlock);
		
			// Write encoded lastBlock
			count = 0;
			for (size i = 0; i < 8; ++i) {
				wchar temp = encoded[count];
				temp <<= 8;
				temp += encoded[count + 1];
		
				strEncoded[(blocksCount * 8) + i] = temp;
				count += 2;
			}
		}

		free(expandedKey);

	}

	template <class KeyType>
	auto ReadDecodeWrite(
		OUT  wchar* strDecoded,
		IN const wchar* const strEncoded,
		IN const size& strEncodedCount,
		IN const KeyType& key,
		IN const uint8& wordsLeftCount = 0
	) {
		const uint64 blocksCount = strEncodedCount / 16;

		AES::Block encoded, decoded;

		uint8* expandedKey = AES::Initialize(key.size());
		AES::ExpendKey<KeyType>(expandedKey, key);

		if (wordsLeftCount == 0) {

			// For each Block
			for (size i = 0; i < blocksCount; ++i) {
				// Copy read data to Block form
				uint8 count = 0;
				for (uint8 j = 0; j < 8; ++j) {
					encoded[count] = (uint8)(strEncoded[(i * 8) + j] >> 8); // high
					encoded[count + 1] = (uint8)(strEncoded[(i * 8) + j]);  // low
					count += 2;
				}

				AES::Decode<KeyType>(decoded, expandedKey, encoded);

				// Write decoded Block
				count = 0;
				for (size j = 0; j < 8; ++j) {
					wchar temp = decoded[count];
					temp <<= 8;
					temp += decoded[count + 1];

					strDecoded[(i * 8) + j] = temp;
					count += 2;
				}
			}

		} else {

			// For each Block (exclude last one)
			for (size i = 0; i < blocksCount - 1; ++i) {

				// Copy read data to Block form
				uint8 count = 0;
				for (uint8 j = 0; j < 8; ++j) {
					encoded[count] = (uint8)(strEncoded[(i * 8) + j] >> 8); // high
					encoded[count + 1] = (uint8)(strEncoded[(i * 8) + j]);  // low
					count += 2;
				}

				AES::Decode<KeyType>(decoded, expandedKey, encoded);

				// Write decoded Block
				count = 0;
				for (size j = 0; j < decoded.size() / 2; ++j) {
					wchar temp = decoded[count];
					temp <<= 8;
					temp += decoded[count + 1];

					strDecoded[(i * 8) + j] = temp;
					count += 2;
				}

			}

			{ // Last block
				const size lastBlockPosition = blocksCount - 1;

				// Copy read data to Block form
				uint8 count = 0;
				for (uint8 j = 0; j < 8; ++j) {
					encoded[count] = (byte)(strEncoded[(lastBlockPosition * 8) + j] >> 8);	// high
					encoded[count + 1] = (byte)(strEncoded[(lastBlockPosition * 8) + j]);	// low
					count += 2;
				}

				AES::Decode<KeyType>(decoded, expandedKey, encoded);

				// Write decoded Block
				count = 0;
				for (size j = 0; j < wordsLeftCount; ++j) {
					wchar temp = decoded[count];
					temp <<= 8;
					temp += decoded[count + 1];

					strDecoded[(lastBlockPosition * 8) + j] = temp;
					count += 2;
				}
			}

		}

		free(expandedKey);
	}

	template <class KeyType>
	auto ReadEncodeWrite(
		OUT  uint8& bytesLeftCount,
		IN const wchar* const nocodedFilePath,
		IN const wchar* const encodedFilePath,
		IN const KeyType& key
	) {
		std::vector<byte> readData(FileIO::Read::File(nocodedFilePath));

		// Likely uses the result of the division - single DIV instruction.
		const uint64 blocksCount = (uint64)readData.size() / 16;
		bytesLeftCount = readData.size() % 16;

		if (bytesLeftCount) { // There is a reminder we need to take care of.

			{
				AES::Block nocoded, encoded;
				std::ofstream outputFile(encodedFilePath, std::ios::binary);

				uint8* expandedKey = AES::Initialize(key.size());
				AES::ExpendKey<KeyType>(expandedKey, key);

				// For each Block
				for (size i = 0; i < blocksCount; ++i) {

					// Copy read data to Block form
					for (uint8 j = 0; j < 16; ++j) {
						nocoded[j] = readData[(i * 16) + j];
					}

					AES::Encode<KeyType>(encoded, expandedKey, nocoded);

					// Write encoded Block
					for (size j = 0; j < encoded.size(); ++j) {
						outputFile << encoded[j];
					}

				}

				{
					AES::Block lastBlock { 0 };

					for (uint8 i = 0; i < bytesLeftCount; ++i) {
						lastBlock[i] = readData[readData.size() - bytesLeftCount + i];
					}

					AES::Encode<KeyType>(encoded, expandedKey, lastBlock);

					// Write encoded lastBlock
					for (size i = 0; i < encoded.size(); ++i) {
						outputFile << encoded[i];
					}
				}

				outputFile.close();
				free(expandedKey);
			}
		} else { // leftBytesCount == 0;
			// Enumerate though all blocks
		}
	}

	template <class KeyType>
	auto ReadDecodeWrite(
		IN const wchar* const encodedFilePath,
		IN const wchar* const decodedFilePath,
		IN const KeyType& key,
		IN const uint8 bytesLeftCount = 0
	) {
		std::vector<byte> readData(FileIO::Read::File(encodedFilePath));

		// Likely uses the result of the division - single DIV instruction.
		const uint64 blocksCount = (uint64)readData.size() / 16;
		const uint64 leftBytesCount = readData.size() % 16;

		//{
		//	array<wchar, 10> buffor;
		//	Int64ToWString<buffor.size()>(buffor.data(), blocksCount);
		//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
		//	Int64ToWString<buffor.size()>(buffor.data(), leftBytesCount);
		//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
		//}

		{
			AES::Block encoded, decoded;
			std::ofstream outputFile(decodedFilePath, std::ios::binary);

			uint8* expandedKey = AES::Initialize(key.size());
			AES::ExpendKey<KeyType>(expandedKey, key);

			// ! NOPE
			//outputFile << "\xEF\xBB\xBF";

			// For each Block (exclude last one)
			for (size i = 0; i < blocksCount - 1; ++i) {

				// Copy read data to Block form
				for (uint8 j = 0; j < 16; ++j) {
					encoded[j] = readData[(i * 16) + j];
				}

				AES::Decode<KeyType>(decoded, expandedKey, encoded);

				// Write decoded Block
				for (size j = 0; j < decoded.size(); ++j) {
					outputFile << decoded[j];
				}

			}

			

			{ // Last block
				const size lastBlockPosition = blocksCount - 1;

				//{
				//	array<wchar, 10> buffor;
				//	Int64ToWString<buffor.size()>(buffor.data(), readData.size());
				//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
				//	Int64ToWString<buffor.size()>(buffor.data(), (lastBlockPosition * 16) + 15);
				//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
				//}
			
				// Copy read data to Block form
				for (uint8 j = 0; j < 16; ++j) {
					encoded[j] = readData[(lastBlockPosition * 16) + j];
				}
			
				AES::Decode<KeyType>(decoded, expandedKey, encoded);
			
				// Copy read data to Block form
				//for (uint8 j = 0; j < bytesLeftCount; ++j) {
				//	encoded[j] = readData[(lastBlockPosition * 16) + j];
				//}

				// Write decoded Block
				for (size j = 0; j < bytesLeftCount; ++j) {
					outputFile << decoded[j];
				}
			}
			outputFile.close();
			free(expandedKey);
		}
	}

}