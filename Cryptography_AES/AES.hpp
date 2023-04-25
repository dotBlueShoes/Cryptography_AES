#pragma once
#include "Framework.hpp"
#include "Matrix.hpp"

// About:
//  Wszystkie rundy sk³adaj¹ siê z kilku kroków, z których 
//   ka¿dy roz³o¿ony jest na cztery podobne (lecz nie identyczne) etapy. 
//  Jeden z tych etapów jest zale¿ny od szyfrowania samego klucza.
//  Wykonuj¹c operacje w odwrotnej kolejnoœci u¿ywaj¹c tego samego klucza, 
//   mo¿na przekszta³ciæ szyfrogram z powrotem w tekst jawny.

// 1. Rozszerzenie klucza – z g³ównego klucza algorytmu „tworzy siê” 
//	   kolejne klucze. AES wymaga osobnego klucza 128-bitowego dla ka¿dej 
//	   rundy, plus jeden dodatkowy.
// 2. Runda wstêpna
//		1. Dodawanie klucza rundy – ka¿dy bajt macierzy stanu jest 
//			mieszany z blokiem rundy za pomoc¹ operatora bitowego XOR.
// 3. Rundy
//		1. Zamiana Bajtów – nieliniowa zamiana, podczas której 
//			ka¿dy bajt jest zamieniany innym.
//		2. Zamiana Wierszy – etap transpozycji, podczas którego 
//			trzy ostatnie wiersze macierzy stanu s¹ cyklicznie 
//			zmieniane okreœlon¹ iloœæ razy.
//		3. Mieszanie Kolumn – Operacja odnosi siê do kolumn macierzy. 
//			Polega na ³¹czeniu czterech bajtów w ka¿dej kolumnie.
//		4. Dodaj klucz rundy
// 4. Final Round ( Brak Operacji Mieszania Kolumn )
//		1. Zamiana Bajtów
//		2. Zamiana Wierszy
//		3. Dodaj klucz rundy.

// 11 keys ( 11 * 4 = 44 words )
// 13 kyes ( 13 * 4 = 52 words )
// 15 keys ( 15 * 4 = 60 words )

namespace AES {

	// Block representing read / write bytes (IO) ( 16 * 8 = 128bits )
	using dataBlock = Matrix::Matrix4x4<uint8>; 

	// BitKey128
	// - 10 cykli powtórzeñ dla klucza 128 - bitowego
	namespace BitKey128 { 

		struct key128 {
			uint64 x;
			uint64 y;
		};

		block SubBytes(
		
		) {

		}

		block ShiftRows(
		
		) {

		}

		block MixColumns(
		
		) {

		}

		block AddRoundKey(
		
		) {

		}

	}

	// BitKey192
	// 12 cykli powtórzeñ dla klucza 192 - bitowego
	namespace BitKey192 {

		struct key192 {
			uint64 x;
			uint64 y;
			uint64 z;
		};

	}

	// BitKey256
	// 14 cykli powtórzeñ dla klucza 256 - bitowego
	namespace BitKey256 {

		///*
		// * The cipher Key.
		// */
		//int K;

		/*
		 * Number of columns (32-bit words) comprising the State. For this
		 * standard, Nb = 4.
		 */
		const int Nb = 4;

		/*
		 * Number of 32-bit words comprising the Cipher Key. For this
		 * standard, Nk = 4, 6, or 8.
		 */
		const int Nk = 8;

		/*
		 * Number of rounds, which is a function of  Nk  and  Nb (which is
		 * fixed). For this standard, Nr = 10, 12, or 14.
		 */
		int Nr = 14;

		using Key256 = uint8[Nk * Nb];

		//key256& expandedKey; // w

		block KeyExpansion(
			OUT Key256& expandedKey,
			IN const Key256& key
		) {
			uint8 lengh = Nb * (Nr + 1); // 4 * 15 = 60.
			uint8 tmp[Nb];

			// Firstly we copy from key to expendedKey
			for (size i = 0; i < Nk; i++) {
				expandedKey[Nb * i + 0] = key[Nb * i + 0];
				expandedKey[Nb * i + 1] = key[Nb * i + 1];
				expandedKey[Nb * i + 2] = key[Nb * i + 2];
				expandedKey[Nb * i + 3] = key[Nb * i + 3];
			}

			// Here come's the rounds.
			for (size i = Nk; i < lengh; i++) {
				tmp[0] = expandedKey[Nb * (i - 1) + 0];
				tmp[1] = expandedKey[Nb * (i - 1) + 1];
				tmp[2] = expandedKey[Nb * (i - 1) + 2];
				tmp[3] = expandedKey[Nb * (i - 1) + 3];

			//	if (i % Nk == 0) { // first round
			//		//rot_word(tmp);
			//		//sub_word(tmp);
			//		//coef_add(tmp, Rcon(i / Nk), tmp);
			//	} else if (Nk > 6 && i % Nk == 4) { // key dependant code ...
			//		//sub_word(tmp);
			//	}
			//
			//	expandedKey[4 * i + 0] = expandedKey[4 * (i - Nk) + 0] ^ tmp[0];
			//	expandedKey[4 * i + 1] = expandedKey[4 * (i - Nk) + 1] ^ tmp[1];
			//	expandedKey[4 * i + 2] = expandedKey[4 * (i - Nk) + 2] ^ tmp[2];
			//	expandedKey[4 * i + 3] = expandedKey[4 * (i - Nk) + 3] ^ tmp[3];
			}

		}

	}

}