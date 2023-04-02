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

	// Block representing read / write bytes (IO) ( 16 * 16 = 256bits )
	using dataBlock = Matrix::Matrix4x4<uint16>; 

	// BitKey128
	// - 10 cykli powtórzeñ dla klucza 128 - bitowego
	namespace BitKey128 { 

		struct key128 {
			uint64 x;
			uint64 y;
		};

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

		struct key256 {
			uint64 x;
			uint64 y;
			uint64 z;
			uint64 w;
		};

	}

}