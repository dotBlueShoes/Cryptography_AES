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

		uint8* expandedKey = aes_init(key.size());
		aes_key_expansion(key.data(), expandedKey);

		{
			BytesToWString<buffor.size()>(buffor.data(), nocoded.data(), nocoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_128 LOG_NOCODED, MB_OK);
		}

		aes_cipher(nocoded.data(), encoded.data(), expandedKey);

		{
			BytesToWString<buffor.size()>(buffor.data(), encoded.data(), encoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_128 LOG_ENCODED, MB_OK);
		}

		aes_inv_cipher(encoded.data(), decoded.data(), expandedKey);

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

		uint8* expandedKey = aes_init(key.size());
		aes_key_expansion(key.data(), expandedKey);

		{
			BytesToWString<buffor.size()>(buffor.data(), nocoded.data(), nocoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_192 LOG_NOCODED, MB_OK);
		}

		aes_cipher(nocoded.data(), encoded.data(), expandedKey);

		{
			BytesToWString<buffor.size()>(buffor.data(), encoded.data(), encoded.size());
			MessageBox(nullptr, buffor.data(), LOG_TYPE LOG_192 LOG_ENCODED, MB_OK);
		}

		aes_inv_cipher(encoded.data(), decoded.data(), expandedKey);

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

		uint8* expandedKey = aes_init(key.size());
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

}

namespace AES {

	template <class KeyType>
	auto ReadEncodeWrite(
		OUT  uint8& bytesLeftCount,
		OUT  wchar* strEncoded,
		IN const wchar* const strNocoded,
		IN const size strNocodedCount,
		IN const KeyType& key
	) {
		// Likely uses the result of the division - single DIV instruction.
		const uint64 blocksCount = strNocodedCount / 16;
		bytesLeftCount = strNocodedCount % 16;

		//strEncoded = new wchar[(blocksCount * 16) + bytesLeftCount];

		//if (blocksCount == 0) {
		//	MessageBox(nullptr, L"Hello there !", LOG_TYPE, MB_OK);
		//} else {
			if (bytesLeftCount) { // There is a reminder we need to take care of.

				// Initialize the whole block with 0'es so we don't do that with for-loop later.
				AES::Block lastBlock { 0 };

				for (uint8 i = 0; i < bytesLeftCount; ++i) {
					lastBlock[i] = strNocoded[strNocodedCount - bytesLeftCount + i];
				}

				{
					AES::Block nocoded, encoded;
					//std::ofstream outputFile(encodedFilePath, std::ios::binary);

					uint8* expandedKey = aes_init(key.size());
					AES::ExpendKey<KeyType>(expandedKey, key);

					// For each Block
					for (size i = 0; i < blocksCount; ++i) {

						// Copy read data to Block form
						for (uint8 j = 0; j < 16; ++j) {
							nocoded[j] = strNocoded[(i * 16) + j];
						}

						AES::Encode<KeyType>(encoded, expandedKey, nocoded);

						// Write encoded Block
						for (size j = 0; j < encoded.size(); ++j) {
							strEncoded[(blocksCount * 16) + j] = encoded[j];
						}

					}

					// Last block with 0'es
					AES::Encode<KeyType>(encoded, expandedKey, lastBlock);

					// Write encoded lastBlock
					for (size i = 0; i < encoded.size(); ++i) {
						strEncoded[i] = encoded[i];
					}

					//for (size i = 0; i < bytesLeftCount; ++i) {
					//	strEncoded[i] = encoded[i];
					//}

					free(expandedKey);
				}
			//} else { // leftBytesCount == 0;
			//	
			//	// Enumerate though all blocks
			//}

		}
	}

	template <class KeyType>
	auto ReadDecodeWrite(
		OUT  wchar* strDecoded,
		IN const wchar* const strEncoded,
		IN const size strEncodedCount,
		IN const KeyType& key,
		IN const uint8 bytesLeftCount = 0
	) {
		// Likely uses the result of the division - single DIV instruction.
		const uint64 blocksCount = strEncodedCount / 16;
		const uint64 leftBytesCount = strEncodedCount % 16;

		{

			uint8* expandedKey = aes_init(key.size());
			AES::ExpendKey<KeyType>(expandedKey, key);

			//if (blocksCount != 0) {

				AES::Block encoded, decoded;

				// For each Block (exclude last one)
				for (size i = 0; i < blocksCount - 1; ++i) {

					// Copy read data to Block form
					for (uint8 j = 0; j < 16; ++j) {
						encoded[j] = strEncoded[(i * 16) + j];
					}

					AES::Decode<KeyType>(decoded, expandedKey, encoded);

					// Write decoded Block
					for (size j = 0; j < decoded.size(); ++j) {
						strDecoded[(i * 16) + j] = decoded[j];
					}

				}

				{ // Last block
					const size lastBlockPosition = blocksCount - 1;

					// Copy read data to Block form
					for (uint8 j = 0; j < 16; ++j) {
						encoded[j] = strEncoded[(lastBlockPosition * 16) + j];
					}

					AES::Decode<KeyType>(decoded, expandedKey, encoded);

					// Write decoded Block
					for (size j = 0; j < bytesLeftCount; ++j) {
						strDecoded[(lastBlockPosition * 16) + j] = decoded[j];
					}
				}

			//} else {
			//
			//	AES::Block encoded { 0 }, decoded;
			//
			//	// Copy read data to Block form
			//	for (uint8 i = 0; i < strEncodedCount; ++i) {
			//		encoded[i] = strEncoded[i];
			//	}
			//
			//	AES::Decode<KeyType>(decoded, expandedKey, encoded);
			//
			//	// Output decoded Block
			//	for (size i = 0; i < strEncodedCount; ++i) {
			//		strDecoded[i] = decoded[i];
			//	}
			//
			//}

			free(expandedKey);
		}
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

		//{
		//	array<wchar, 10> buffor;
		//	Int64ToWString<buffor.size()>(buffor.data(), blocksCount);
		//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
		//	Int64ToWString<buffor.size()>(buffor.data(), bytesLeftCount);
		//	MessageBox(nullptr, buffor.data(), LOG_TYPE, MB_OK);
		//}

		if (bytesLeftCount) { // There is a reminder we need to take care of.
			AES::Block lastBlock { 0 };
			for (uint8 i = 0; i < bytesLeftCount; ++i) {
				lastBlock[i] = readData[readData.size() - bytesLeftCount + i];
			}

			{
				AES::Block nocoded, encoded;
				std::ofstream outputFile(encodedFilePath, std::ios::binary);

				uint8* expandedKey = aes_init(key.size());
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

				// Last block with 0'es
				AES::Encode<KeyType>(encoded, expandedKey, lastBlock);
				
				// Write encoded lastBlock
				for (size i = 0; i < encoded.size(); ++i) {
					outputFile << encoded[i];
				}

				//for (uint8 i = 0; i < bytesLeftCount; ++i) {
				//	outputFile << lastBlock[i];
				//}

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

			uint8* expandedKey = aes_init(key.size());
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