#pragma once
#include "Framework.hpp"
#include "Matrix.hpp"

// About:
//  Wszystkie rundy sk�adaj� si� z kilku krok�w, z kt�rych 
//   ka�dy roz�o�ony jest na cztery podobne (lecz nie identyczne) etapy. 
//  Jeden z tych etap�w jest zale�ny od szyfrowania samego klucza.
//  Wykonuj�c operacje w odwrotnej kolejno�ci u�ywaj�c tego samego klucza, 
//   mo�na przekszta�ci� szyfrogram z powrotem w tekst jawny.

// 1. Rozszerzenie klucza � z g��wnego klucza algorytmu �tworzy si� 
//	   kolejne klucze. AES wymaga osobnego klucza 128-bitowego dla ka�dej 
//	   rundy, plus jeden dodatkowy.
// 2. Runda wst�pna
//		1. Dodawanie klucza rundy � ka�dy bajt macierzy stanu jest 
//			mieszany z blokiem rundy za pomoc� operatora bitowego XOR.
// 3. Rundy
//		1. Zamiana Bajt�w � nieliniowa zamiana, podczas kt�rej 
//			ka�dy bajt jest zamieniany innym.
//		2. Zamiana Wierszy � etap transpozycji, podczas kt�rego 
//			trzy ostatnie wiersze macierzy stanu s� cyklicznie 
//			zmieniane okre�lon� ilo�� razy.
//		3. Mieszanie Kolumn � Operacja odnosi si� do kolumn macierzy. 
//			Polega na ��czeniu czterech bajt�w w ka�dej kolumnie.
//		4. Dodaj klucz rundy
// 4. Final Round ( Brak Operacji Mieszania Kolumn )
//		1. Zamiana Bajt�w
//		2. Zamiana Wierszy
//		3. Dodaj klucz rundy.

// 11 keys ( 11 * 4 = 44 words )
// 13 kyes ( 13 * 4 = 52 words )
// 15 keys ( 15 * 4 = 60 words )

namespace AES {

	// Block representing read / write bytes (IO) ( 16 * 16 = 256bits )
	using dataBlock = Matrix::Matrix4x4<uint16>; 

	// BitKey128
	// - 10 cykli powt�rze� dla klucza 128 - bitowego
	namespace BitKey128 { 

		struct key128 {
			uint64 x;
			uint64 y;
		};

	}

	// BitKey192
	// 12 cykli powt�rze� dla klucza 192 - bitowego
	namespace BitKey192 {

		struct key192 {
			uint64 x;
			uint64 y;
			uint64 z;
		};

	}

	// BitKey256
	// 14 cykli powt�rze� dla klucza 256 - bitowego
	namespace BitKey256 {

		struct key256 {
			uint64 x;
			uint64 y;
			uint64 z;
			uint64 w;
		};

	}

}