#pragma once

/* Copyright (c) 2013, Takashi Michikawa */

#include <vector>
#include <utility>
#include <cmath>
#include <omp.h>

#define MC_VOLUME_PADDING 10

typedef std::vector<std::vector<std::vector<float> > > ScalarVolume;
inline ScalarVolume initScalarVolume( size_t gridsize, float value = 0 ){
	gridsize += (MC_VOLUME_PADDING * 2); // add padding
	return std::vector<std::vector<std::vector<float> > >( gridsize, 
		std::vector<std::vector<float> >(gridsize, std::vector<float>( gridsize, value )) );
}
inline ScalarVolume initScalarVolume( size_t x, size_t y, size_t z, float value = 0, bool addPadding = false ){
        if(addPadding)
        {
            x += (MC_VOLUME_PADDING * 2); // add padding
            y += (MC_VOLUME_PADDING * 2); // add padding
            z += (MC_VOLUME_PADDING * 2); // add padding
        }

        return std::vector<std::vector<std::vector<float> > >( z,
                std::vector<std::vector<float> >(y, std::vector<float>( x, value )) );
}
inline ScalarVolume addPaddingToVolume( const ScalarVolume & fromVolume, float value = 0 ){
	size_t gridsize = fromVolume.size();
	ScalarVolume volume = initScalarVolume( gridsize, value );
	#pragma omp parallel for
	for(int x = 0; x < gridsize; x++)
		for(int y = 0; y < gridsize; y++)
			for(int z = 0; z < gridsize; z++)
				volume[x+MC_VOLUME_PADDING][y+MC_VOLUME_PADDING][z+MC_VOLUME_PADDING] = fromVolume[x][y][z];
	return volume;
}

int mc_edtable[] = {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7
};

int mc_colidx[] = {
        0, 0, 3, 6, 12, 15, 21, 33, 42, 45,
        57, 63, 72, 78, 87, 96, 102, 105, 111, 123,
        132, 144, 153, 168, 180, 198, 213, 228, 240, 255,
        267, 279, 288, 291, 303, 309, 318, 336, 351, 366,
        378, 390, 405, 414, 426, 441, 453, 465, 474, 480,
        489, 498, 504, 519, 531, 543, 552, 567, 579, 591,
        600, 612, 621, 630, 636, 639, 651, 669, 684, 690,
        699, 714, 726, 738, 753, 768, 780, 789, 801, 813,
        822, 828, 837, 852, 864, 873, 879, 891, 900, 915,
        927, 939, 948, 960, 969, 978, 984, 996, 1011, 1026,
        1038, 1053, 1065, 1077, 1086, 1101, 1113, 1125, 1134, 1146,
        1155, 1164, 1170, 1179, 1191, 1203, 1212, 1224, 1233, 1242,
        1248, 1260, 1269, 1278, 1284, 1293, 1299, 1305, 1308, 1311,
        1329, 1341, 1356, 1368, 1383, 1398, 1410, 1416, 1431, 1440,
        1452, 1461, 1473, 1485, 1494, 1506, 1521, 1536, 1548, 1563,
        1575, 1587, 1596, 1611, 1623, 1635, 1644, 1656, 1665, 1674,
        1680, 1686, 1701, 1710, 1722, 1737, 1749, 1761, 1770, 1779,
        1791, 1797, 1806, 1818, 1827, 1836, 1842, 1851, 1863, 1875,
        1884, 1896, 1905, 1914, 1920, 1932, 1941, 1950, 1956, 1965,
        1971, 1977, 1980, 1986, 2001, 2016, 2028, 2037, 2049, 2061,
        2070, 2079, 2091, 2103, 2112, 2118, 2127, 2136, 2142, 2151,
        2163, 2175, 2184, 2196, 2205, 2214, 2220, 2232, 2241, 2250,
        2256, 2265, 2271, 2277, 2280, 2289, 2301, 2313, 2322, 2334,
        2343, 2352, 2358, 2370, 2379, 2388, 2394, 2403, 2409, 2415,
        2418, 2424, 2433, 2442, 2448, 2457, 2463, 2469, 2472, 2481,
        2487, 2493, 2496, 2502, 2505, 2508, 2508
};
int mc_idxtable[] = {
        /*0(00000000)*/
        /*1(10000000)*/ 0, 3, 8,
        /*2(01000000)*/ 1, 0, 9,
        /*3(11000000)*/ 1, 8, 9, 1, 3, 8,
        /*4(00100000)*/ 2, 10, 3,
        /*5(10100000)*/ 0, 10, 8, 0, 2, 10,
        /*6(01100000)*/ 1, 10, 9, 1, 2, 10, 0, 9, 3, 3, 9, 10,
        /*7(11100000)*/ 8, 9, 10, 1, 10, 9, 1, 2, 10,
        /*8(00010000)*/ 2, 1, 11,
        /*9(10010000)*/ 2, 8, 11, 2, 3, 8, 1, 11, 0, 0, 11, 8,
        /*10(01010000)*/ 2, 9, 11, 2, 0, 9,
        /*11(11010000)*/ 9, 11, 8, 2, 8, 11, 2, 3, 8,
        /*12(00110000)*/ 11, 3, 1, 11, 10, 3,
        /*13(10110000)*/ 10, 8, 11, 0, 11, 8, 0, 1, 11,
        /*14(01110000)*/ 11, 10, 9, 3, 9, 10, 3, 0, 9,
        /*15(11110000)*/ 8, 11, 10, 8, 9, 11,
        /*16(00001000)*/ 8, 7, 4,
        /*17(10001000)*/ 3, 4, 0, 3, 7, 4,
        /*18(01001000)*/ 4, 1, 7, 4, 9, 1, 8, 7, 0, 0, 7, 1,
        /*19(11001000)*/ 3, 7, 1, 4, 1, 7, 4, 9, 1,
        /*20(00101000)*/ 10, 4, 2, 10, 7, 4, 3, 2, 8, 8, 2, 4,
        /*21(10101000)*/ 0, 2, 4, 10, 4, 2, 10, 7, 4,
        /*22(01101000)*/ 0, 8, 3, 2, 10, 7, 1, 2, 7, 1, 7, 4, 1, 4, 9,
        /*23(11101000)*/ 2, 10, 7, 1, 2, 7, 1, 7, 4, 1, 4, 9,
        /*24(00011000)*/ 1, 11, 4, 2, 7, 11, 1, 8, 2, 1, 4, 8, 11, 7, 4, 2, 8, 7,
        /*25(10011000)*/ 2, 3, 7, 2, 7, 11, 4, 11, 7, 1, 11, 4, 1, 4, 0,
        /*26(01011000)*/ 4, 9, 11, 4, 11, 7, 2, 8, 7, 0, 8, 2, 2, 7, 11,
        /*27(11011000)*/ 11, 2, 3, 11, 3, 7, 11, 7, 4, 4, 9, 11,
        /*28(00111000)*/ 1, 8, 3, 1, 4, 8, 1, 11, 4, 7, 4, 11, 7, 11, 10,
        /*29(10111000)*/ 7, 11, 10, 4, 11, 7, 4, 1, 11, 1, 4, 0,
        /*30(01111000)*/ 7, 11, 10, 4, 11, 7, 4, 9, 11, 0, 8, 3,
        /*31(11111000)*/ 7, 11, 10, 4, 11, 7, 4, 9, 11,
        /*32(00000100)*/ 4, 5, 9,
        /*33(10000100)*/ 9, 3, 5, 9, 0, 3, 4, 5, 8, 8, 5, 3,
        /*34(01000100)*/ 5, 0, 4, 5, 1, 0,
        /*35(11000100)*/ 1, 3, 5, 8, 5, 3, 8, 4, 5,
        /*36(00100100)*/ 3, 2, 9, 10, 5, 2, 3, 4, 10, 3, 9, 4, 2, 5, 9, 10, 4, 5,
        /*37(10100100)*/ 9, 0, 2, 9, 2, 5, 10, 5, 2, 4, 5, 10, 4, 10, 8,
        /*38(01100100)*/ 3, 0, 4, 3, 4, 10, 5, 10, 4, 2, 10, 5, 2, 5, 1,
        /*39(11100100)*/ 10, 1, 2, 10, 5, 1, 10, 4, 5, 4, 10, 8,
        /*40(00010100)*/ 5, 2, 4, 5, 11, 2, 9, 4, 1, 1, 4, 2,
        /*41(10010100)*/ 1, 9, 0, 3, 8, 4, 2, 3, 4, 2, 4, 5, 2, 5, 11,
        /*42(01010100)*/ 0, 4, 2, 5, 2, 4, 5, 11, 2,
        /*43(11010100)*/ 3, 8, 4, 2, 3, 4, 2, 4, 5, 2, 5, 11,
        /*44(00110100)*/ 5, 11, 10, 5, 10, 4, 3, 9, 4, 1, 9, 3, 3, 4, 10,
        /*45(10110100)*/ 4, 10, 8, 5, 10, 4, 5, 11, 10, 1, 9, 0,
        /*46(01110100)*/ 10, 3, 0, 10, 0, 4, 10, 4, 5, 5, 11, 10,
        /*47(11110100)*/ 4, 10, 8, 5, 10, 4, 5, 11, 10,
        /*48(00001100)*/ 7, 9, 8, 7, 5, 9,
        /*49(10001100)*/ 7, 5, 3, 9, 3, 5, 9, 0, 3,
        /*50(01001100)*/ 5, 1, 7, 0, 7, 1, 0, 8, 7,
        /*51(11001100)*/ 7, 1, 3, 7, 5, 1,
        /*52(00101100)*/ 10, 7, 5, 10, 5, 2, 9, 2, 5, 3, 2, 9, 3, 9, 8,
        /*53(10101100)*/ 2, 10, 7, 2, 7, 5, 2, 5, 9, 9, 0, 2,
        /*54(01101100)*/ 2, 5, 1, 10, 5, 2, 10, 7, 5, 8, 3, 0,
        /*55(11101100)*/ 2, 5, 1, 10, 5, 2, 10, 7, 5,
        /*56(00011100)*/ 1, 9, 8, 1, 8, 2, 7, 2, 8, 11, 2, 7, 11, 7, 5,
        /*57(10011100)*/ 11, 7, 5, 2, 7, 11, 2, 3, 7, 0, 1, 9,
        /*58(01011100)*/ 2, 5, 11, 2, 7, 5, 2, 8, 7, 8, 2, 0,
        /*59(11011100)*/ 11, 7, 5, 2, 7, 11, 2, 3, 7,
        /*60(00111100)*/ 7, 11, 10, 7, 5, 11, 1, 9, 8, 1, 8, 3,
        /*61(10111100)*/ 7, 11, 10, 7, 5, 11, 1, 9, 0,
        /*62(01111100)*/ 5, 11, 10, 5, 10, 7, 3, 0, 8,
        /*63(11111100)*/ 7, 11, 10, 7, 5, 11,
        /*64(00000010)*/ 6, 7, 10,
        /*65(10000010)*/ 3, 6, 0, 3, 10, 6, 8, 0, 7, 7, 0, 6,
        /*66(01000010)*/ 7, 10, 0, 6, 1, 10, 7, 9, 6, 7, 0, 9, 10, 1, 0, 6, 9, 1,
        /*67(11000010)*/ 7, 8, 9, 7, 9, 6, 1, 6, 9, 10, 6, 1, 10, 1, 3,
        /*68(00100010)*/ 2, 7, 3, 2, 6, 7,
        /*69(10100010)*/ 2, 6, 0, 7, 0, 6, 7, 8, 0,
        /*70(01100010)*/ 1, 2, 6, 1, 6, 9, 7, 9, 6, 0, 9, 7, 0, 7, 3,
        /*71(11100010)*/ 9, 1, 2, 9, 2, 6, 9, 6, 7, 7, 8, 9,
        /*72(00010010)*/ 11, 7, 1, 11, 6, 7, 2, 1, 10, 10, 1, 7,
        /*73(10010010)*/ 2, 3, 10, 6, 7, 8, 11, 6, 8, 11, 8, 0, 11, 0, 1,
        /*74(01010010)*/ 10, 2, 0, 10, 0, 7, 9, 7, 0, 6, 7, 9, 6, 9, 11,
        /*75(11010010)*/ 6, 9, 11, 7, 9, 6, 7, 8, 9, 3, 10, 2,
        /*76(00110010)*/ 3, 1, 7, 11, 7, 1, 11, 6, 7,
        /*77(10110010)*/ 6, 7, 8, 11, 6, 8, 11, 8, 0, 11, 0, 1,
        /*78(01110010)*/ 9, 3, 0, 9, 7, 3, 9, 6, 7, 6, 9, 11,
        /*79(11110010)*/ 6, 9, 11, 7, 9, 6, 7, 8, 9,
        /*80(00001010)*/ 8, 6, 4, 8, 10, 6,
        /*81(10001010)*/ 4, 0, 6, 3, 6, 0, 3, 10, 6,
        /*82(01001010)*/ 0, 8, 10, 0, 10, 1, 6, 1, 10, 9, 1, 6, 9, 6, 4,
        /*83(11001010)*/ 10, 1, 3, 6, 1, 10, 6, 9, 1, 9, 6, 4,
        /*84(00101010)*/ 6, 4, 2, 8, 2, 4, 8, 3, 2,
        /*85(10101010)*/ 4, 2, 6, 4, 0, 2,
        /*86(01101010)*/ 9, 6, 4, 1, 6, 9, 1, 2, 6, 3, 0, 8,
        /*87(11101010)*/ 9, 6, 4, 1, 6, 9, 1, 2, 6,
        /*88(00011010)*/ 11, 6, 4, 11, 4, 1, 8, 1, 4, 2, 1, 8, 2, 8, 10,
        /*89(10011010)*/ 1, 4, 0, 11, 4, 1, 11, 6, 4, 10, 2, 3,
        /*90(01011010)*/ 6, 9, 11, 6, 4, 9, 0, 8, 10, 0, 10, 2,
        /*91(11011010)*/ 9, 6, 4, 9, 11, 6, 10, 2, 3,
        /*92(00111010)*/ 4, 8, 3, 4, 3, 1, 4, 1, 11, 11, 6, 4,
        /*93(10111010)*/ 1, 4, 0, 11, 4, 1, 11, 6, 4,
        /*94(01111010)*/ 6, 9, 11, 6, 4, 9, 0, 8, 3,
        /*95(11111010)*/ 6, 9, 11, 6, 4, 9,
        /*96(00000110)*/ 5, 9, 10, 5, 10, 6, 4, 7, 9, 7, 10, 9,
        /*97(10000110)*/ 8, 4, 7, 10, 6, 5, 3, 10, 5, 3, 5, 9, 3, 9, 0,
        /*98(01000110)*/ 6, 5, 1, 6, 1, 10, 0, 10, 1, 7, 10, 0, 7, 0, 4,
        /*99(11000110)*/ 10, 1, 3, 6, 1, 10, 6, 5, 1, 4, 7, 8,
        /*100(00100110)*/ 4, 7, 3, 4, 3, 9, 2, 9, 3, 5, 9, 2, 5, 2, 6,
        /*101(10100110)*/ 5, 2, 6, 9, 2, 5, 9, 0, 2, 8, 4, 7,
        /*102(01100110)*/ 5, 2, 6, 5, 1, 2, 3, 0, 4, 3, 4, 7,
        /*103(11100110)*/ 2, 5, 1, 2, 6, 5, 4, 7, 8,
        /*104(00010110)*/ 5, 11, 6, 7, 10, 2, 4, 7, 2, 4, 2, 1, 4, 1, 9,
        /*105(10010110)*/ 2, 3, 10, 8, 4, 7, 5, 11, 6, 0, 1, 9,
        /*106(01010110)*/ 7, 0, 4, 10, 0, 7, 10, 2, 0, 11, 6, 5,
        /*107(11010110)*/ 11, 6, 5, 7, 8, 4, 2, 3, 10,
        /*108(00110110)*/ 9, 3, 1, 4, 3, 9, 4, 7, 3, 6, 5, 11,
        /*109(10110110)*/ 8, 4, 7, 5, 11, 6, 0, 1, 9,
        /*110(01110110)*/ 0, 7, 3, 0, 4, 7, 6, 5, 11,
        /*111(11110110)*/ 11, 6, 5, 7, 8, 4,
        /*112(00001110)*/ 8, 10, 9, 5, 9, 10, 5, 10, 6,
        /*113(10001110)*/ 10, 6, 5, 3, 10, 5, 3, 5, 9, 3, 9, 0,
        /*114(01001110)*/ 10, 6, 5, 10, 5, 1, 10, 1, 0, 0, 8, 10,
        /*115(11001110)*/ 10, 1, 3, 6, 1, 10, 6, 5, 1,
        /*116(00101110)*/ 5, 2, 6, 9, 2, 5, 9, 3, 2, 3, 9, 8,
        /*117(10101110)*/ 5, 2, 6, 9, 2, 5, 9, 0, 2,
        /*118(01101110)*/ 5, 2, 6, 5, 1, 2, 3, 0, 8,
        /*119(11101110)*/ 5, 2, 6, 5, 1, 2,
        /*120(00011110)*/ 2, 8, 10, 1, 8, 2, 1, 9, 8, 5, 11, 6,
        /*121(10011110)*/ 10, 2, 3, 1, 9, 0, 6, 5, 11,
        /*122(01011110)*/ 8, 2, 0, 8, 10, 2, 11, 6, 5,
        /*123(11011110)*/ 5, 11, 6, 2, 3, 10,
        /*124(00111110)*/ 3, 9, 8, 3, 1, 9, 5, 11, 6,
        /*125(10111110)*/ 6, 5, 11, 9, 0, 1,
        /*126(01111110)*/ 6, 5, 11, 0, 8, 3,
        /*127(11111110)*/ 5, 11, 6,
        /*128(00000001)*/ 5, 6, 11,
        /*129(10000001)*/ 11, 5, 0, 6, 8, 5, 11, 3, 6, 11, 0, 3, 5, 8, 0, 6, 3, 8,
        /*130(01000001)*/ 11, 0, 6, 11, 1, 0, 5, 6, 9, 9, 6, 0,
        /*131(11000001)*/ 11, 1, 3, 11, 3, 6, 8, 6, 3, 5, 6, 8, 5, 8, 9,
        /*132(00100001)*/ 6, 3, 5, 6, 10, 3, 11, 5, 2, 2, 5, 3,
        /*133(10100001)*/ 6, 10, 8, 6, 8, 5, 0, 5, 8, 11, 5, 0, 11, 0, 2,
        /*134(01100001)*/ 2, 11, 1, 0, 9, 5, 3, 0, 5, 3, 5, 6, 3, 6, 10,
        /*135(11100001)*/ 5, 8, 9, 6, 8, 5, 6, 10, 8, 2, 11, 1,
        /*136(00010001)*/ 6, 1, 5, 6, 2, 1,
        /*137(10010001)*/ 0, 1, 5, 0, 5, 8, 6, 8, 5, 3, 8, 6, 3, 6, 2,
        /*138(01010001)*/ 2, 0, 6, 9, 6, 0, 9, 5, 6,
        /*139(11010001)*/ 5, 8, 9, 6, 8, 5, 6, 3, 8, 3, 6, 2,
        /*140(00110001)*/ 1, 5, 3, 6, 3, 5, 6, 10, 3,
        /*141(10110001)*/ 5, 6, 10, 5, 10, 8, 5, 8, 0, 0, 1, 5,
        /*142(01110001)*/ 0, 9, 5, 3, 0, 5, 3, 5, 6, 3, 6, 10,
        /*143(11110001)*/ 5, 8, 9, 6, 8, 5, 6, 10, 8,
        /*144(00001001)*/ 7, 11, 8, 7, 6, 11, 4, 8, 5, 5, 8, 11,
        /*145(10001001)*/ 6, 3, 7, 6, 11, 3, 0, 3, 11, 5, 0, 11, 5, 4, 0,
        /*146(01001001)*/ 4, 9, 5, 6, 11, 1, 7, 6, 1, 7, 1, 0, 7, 0, 8,
        /*147(11001001)*/ 6, 3, 7, 11, 3, 6, 11, 1, 3, 9, 5, 4,
        /*148(00101001)*/ 6, 10, 7, 4, 8, 3, 5, 4, 3, 5, 3, 2, 5, 2, 11,
        /*149(10101001)*/ 11, 0, 2, 5, 0, 11, 5, 4, 0, 7, 6, 10,
        /*150(01101001)*/ 10, 7, 6, 4, 9, 5, 1, 2, 11, 8, 3, 0,
        /*151(11101001)*/ 2, 11, 1, 5, 4, 9, 10, 7, 6,
        /*152(00011001)*/ 7, 6, 2, 7, 2, 8, 1, 8, 2, 4, 8, 1, 4, 1, 5,
        /*153(10011001)*/ 3, 6, 2, 3, 7, 6, 5, 4, 0, 5, 0, 1,
        /*154(01011001)*/ 8, 2, 0, 7, 2, 8, 7, 6, 2, 5, 4, 9,
        /*155(11011001)*/ 3, 6, 2, 3, 7, 6, 5, 4, 9,
        /*156(00111001)*/ 4, 1, 5, 8, 1, 4, 8, 3, 1, 10, 7, 6,
        /*157(10111001)*/ 1, 4, 0, 1, 5, 4, 7, 6, 10,
        /*158(01111001)*/ 5, 4, 9, 8, 3, 0, 6, 10, 7,
        /*159(11111001)*/ 10, 7, 6, 4, 9, 5,
        /*160(00000101)*/ 4, 11, 9, 4, 6, 11,
        /*161(10000101)*/ 8, 4, 6, 8, 6, 3, 11, 0, 3, 9, 0, 11, 11, 3, 6,
        /*162(01000101)*/ 4, 6, 0, 11, 0, 6, 11, 1, 0,
        /*163(11000101)*/ 6, 11, 1, 6, 1, 3, 6, 3, 8, 8, 4, 6,
        /*164(00100101)*/ 2, 11, 9, 2, 9, 3, 4, 10, 3, 6, 10, 4, 4, 3, 9,
        /*165(10100101)*/ 4, 10, 8, 4, 6, 10, 2, 11, 9, 2, 9, 0,
        /*166(01100101)*/ 10, 4, 6, 3, 4, 10, 3, 0, 4, 1, 2, 11,
        /*167(11100101)*/ 4, 10, 8, 4, 6, 10, 2, 11, 1,
        /*168(00010101)*/ 6, 2, 4, 1, 4, 2, 1, 9, 4,
        /*169(10010101)*/ 3, 6, 2, 8, 6, 3, 8, 4, 6, 9, 0, 1,
        /*170(01010101)*/ 0, 6, 2, 0, 4, 6,
        /*171(11010101)*/ 3, 6, 2, 8, 6, 3, 8, 4, 6,
        /*172(00110101)*/ 3, 6, 10, 3, 4, 6, 3, 9, 4, 9, 3, 1,
        /*173(10110101)*/ 6, 10, 8, 6, 8, 4, 0, 1, 9,
        /*174(01110101)*/ 10, 4, 6, 3, 4, 10, 3, 0, 4,
        /*175(11110101)*/ 4, 10, 8, 4, 6, 10,
        /*176(00001101)*/ 9, 8, 11, 7, 11, 8, 7, 6, 11,
        /*177(10001101)*/ 11, 7, 6, 11, 3, 7, 11, 0, 3, 0, 11, 9,
        /*178(01001101)*/ 6, 11, 1, 7, 6, 1, 7, 1, 0, 7, 0, 8,
        /*179(11001101)*/ 6, 3, 7, 11, 3, 6, 11, 1, 3,
        /*180(00101101)*/ 3, 9, 8, 2, 9, 3, 2, 11, 9, 6, 10, 7,
        /*181(10101101)*/ 9, 0, 2, 9, 2, 11, 10, 7, 6,
        /*182(01101101)*/ 6, 10, 7, 3, 0, 8, 11, 1, 2,
        /*183(11101101)*/ 1, 2, 11, 10, 7, 6,
        /*184(00011101)*/ 8, 7, 6, 8, 6, 2, 8, 2, 1, 1, 9, 8,
        /*185(10011101)*/ 7, 6, 2, 7, 2, 3, 1, 9, 0,
        /*186(01011101)*/ 8, 2, 0, 7, 2, 8, 7, 6, 2,
        /*187(11011101)*/ 3, 6, 2, 3, 7, 6,
        /*188(00111101)*/ 1, 9, 8, 1, 8, 3, 7, 6, 10,
        /*189(10111101)*/ 7, 6, 10, 1, 9, 0,
        /*190(01111101)*/ 6, 10, 7, 3, 0, 8,
        /*191(11111101)*/ 6, 10, 7,
        /*192(00000011)*/ 5, 10, 11, 5, 7, 10,
        /*193(10000011)*/ 3, 10, 11, 3, 11, 0, 5, 0, 11, 8, 0, 5, 8, 5, 7,
        /*194(01000011)*/ 9, 5, 7, 9, 7, 0, 10, 1, 0, 11, 1, 10, 10, 0, 7,
        /*195(11000011)*/ 8, 5, 7, 8, 9, 5, 11, 1, 3, 11, 3, 10,
        /*196(00100011)*/ 7, 3, 5, 2, 5, 3, 2, 11, 5,
        /*197(10100011)*/ 8, 5, 7, 0, 5, 8, 0, 11, 5, 11, 0, 2,
        /*198(01100011)*/ 0, 7, 3, 9, 7, 0, 9, 5, 7, 11, 1, 2,
        /*199(11100011)*/ 8, 5, 7, 8, 9, 5, 11, 1, 2,
        /*200(00010011)*/ 5, 7, 1, 10, 1, 7, 10, 2, 1,
        /*201(10010011)*/ 8, 5, 7, 0, 5, 8, 0, 1, 5, 2, 3, 10,
        /*202(01010011)*/ 7, 10, 2, 7, 2, 0, 7, 0, 9, 9, 5, 7,
        /*203(11010011)*/ 5, 8, 9, 5, 7, 8, 3, 10, 2,
        /*204(00110011)*/ 3, 5, 7, 3, 1, 5,
        /*205(10110011)*/ 8, 5, 7, 0, 5, 8, 0, 1, 5,
        /*206(01110011)*/ 0, 7, 3, 9, 7, 0, 9, 5, 7,
        /*207(11110011)*/ 8, 5, 7, 8, 9, 5,
        /*208(00001011)*/ 10, 11, 8, 5, 8, 11, 5, 4, 8,
        /*209(10001011)*/ 0, 3, 10, 0, 10, 11, 0, 11, 5, 5, 4, 0,
        /*210(01001011)*/ 1, 10, 11, 0, 10, 1, 0, 8, 10, 4, 9, 5,
        /*211(11001011)*/ 10, 1, 3, 10, 11, 1, 9, 5, 4,
        /*212(00101011)*/ 4, 8, 3, 5, 4, 3, 5, 3, 2, 5, 2, 11,
        /*213(10101011)*/ 11, 0, 2, 5, 0, 11, 5, 4, 0,
        /*214(01101011)*/ 4, 9, 5, 1, 2, 11, 8, 3, 0,
        /*215(11101011)*/ 2, 11, 1, 5, 4, 9,
        /*216(00011011)*/ 8, 5, 4, 8, 1, 5, 8, 2, 1, 2, 8, 10,
        /*217(10011011)*/ 4, 1, 5, 4, 0, 1, 2, 3, 10,
        /*218(01011011)*/ 2, 8, 10, 2, 0, 8, 4, 9, 5,
        /*219(11011011)*/ 4, 9, 5, 2, 3, 10,
        /*220(00111011)*/ 4, 1, 5, 8, 1, 4, 8, 3, 1,
        /*221(10111011)*/ 4, 1, 5, 4, 0, 1,
        /*222(01111011)*/ 5, 4, 9, 8, 3, 0,
        /*223(11111011)*/ 9, 5, 4,
        /*224(00000111)*/ 11, 9, 10, 4, 10, 9, 4, 7, 10,
        /*225(10000111)*/ 3, 10, 11, 0, 3, 11, 0, 11, 9, 4, 7, 8,
        /*226(01000111)*/ 1, 10, 11, 0, 10, 1, 0, 7, 10, 7, 0, 4,
        /*227(11000111)*/ 1, 10, 11, 1, 3, 10, 7, 8, 4,
        /*228(00100111)*/ 9, 4, 7, 9, 7, 3, 9, 3, 2, 2, 11, 9,
        /*229(10100111)*/ 11, 0, 2, 11, 9, 0, 8, 4, 7,
        /*230(01100111)*/ 4, 7, 3, 4, 3, 0, 2, 11, 1,
        /*231(11100111)*/ 11, 1, 2, 8, 4, 7,
        /*232(00010111)*/ 7, 10, 2, 4, 7, 2, 4, 2, 1, 4, 1, 9,
        /*233(10010111)*/ 9, 0, 1, 3, 10, 2, 4, 7, 8,
        /*234(01010111)*/ 7, 0, 4, 10, 0, 7, 10, 2, 0,
        /*235(11010111)*/ 4, 7, 8, 10, 2, 3,
        /*236(00110111)*/ 9, 3, 1, 4, 3, 9, 4, 7, 3,
        /*237(10110111)*/ 7, 8, 4, 0, 1, 9,
        /*238(01110111)*/ 0, 7, 3, 0, 4, 7,
        /*239(11110111)*/ 7, 8, 4,
        /*240(00001111)*/ 9, 10, 11, 9, 8, 10,
        /*241(10001111)*/ 3, 10, 11, 0, 3, 11, 0, 11, 9,
        /*242(01001111)*/ 1, 10, 11, 0, 10, 1, 0, 8, 10,
        /*243(11001111)*/ 1, 10, 11, 1, 3, 10,
        /*244(00101111)*/ 3, 9, 8, 2, 9, 3, 2, 11, 9,
        /*245(10101111)*/ 11, 0, 2, 11, 9, 0,
        /*246(01101111)*/ 11, 1, 2, 3, 0, 8,
        /*247(11101111)*/ 1, 2, 11,
        /*248(00011111)*/ 2, 8, 10, 1, 8, 2, 1, 9, 8,
        /*249(10011111)*/ 10, 2, 3, 1, 9, 0,
        /*250(01011111)*/ 2, 8, 10, 2, 0, 8,
        /*251(11011111)*/ 3, 10, 2,
        /*252(00111111)*/ 3, 9, 8, 3, 1, 9,
        /*253(10111111)*/ 1, 9, 0,
        /*254(01111111)*/ 0, 8, 3
        /*255(11111111)*/
};

struct Point3f {
	float x;
	float y;
	float z;
};

int polygonize( std::vector< std::pair<Point3f, double> >& cell, const double isovalue, std::vector<Point3f>& pnts, const double iso_eps = 1.0e-6) {
	unsigned char tableid = 0x00;
    for( int i = 0 ; i < 8 ; ++i ) {
		if ( std::fabs( cell[i].second - isovalue ) < iso_eps ) cell[i].second = isovalue + iso_eps;
		if ( isovalue <=  cell[i].second ) tableid += ( 0x01 <<i );
	}
	if ( tableid == 0x00 || tableid == 0xFF ) return 0;
	Point3f ep[12];
	for( int i = 0 ; i < 12 ; i++ ) {
		const int& id0 = mc_edtable[ 2 * i + 0];
		const int& id1 = mc_edtable[ 2 * i + 1];
		
		const double& v0 = cell[id0].second;
		const double& v1 = cell[id1].second;
		if( std::fabs( v0 - v1 ) <  1.0e-10 ) continue;
		
		const double t = ( isovalue - v0 ) * 1.0 / ( v1 - v0 );
		const Point3f& p0 = cell[id0].first;
		const Point3f& p1 = cell[id1].first;
		
		ep[i].x = static_cast<float>(( 1.0 - t ) * p0.x + t * p1.x);
		ep[i].y = static_cast<float>(( 1.0 - t ) * p0.y + t * p1.y);
		ep[i].z = static_cast<float>(( 1.0 - t ) * p0.z + t * p1.z);
	}
	
	int numTriangles = 0;
	for( int i =  mc_colidx[tableid] ; i < mc_colidx[tableid+1] ; i += 3 ) {
		++numTriangles;
		pnts.push_back( ep[ mc_idxtable[i  ] ] );
		pnts.push_back( ep[ mc_idxtable[i+1] ] );
		pnts.push_back( ep[ mc_idxtable[i+2] ] );
	}
	return numTriangles;
}

inline std::vector< std::vector<Point3f> > march( const ScalarVolume & volume, double isovalue = 0.0, bool isPadded = false )
{
        int sz = (int)volume.size();
        int sy = (int)volume.front().size();
        int sx = (int)volume.front().front().size();

	std::vector< std::vector<Point3f> > allTriangles;

	std::vector< std::vector< std::vector<Point3f> > > trianglesThread( omp_get_max_threads() );

	#pragma omp parallel for
	for( int z = 0 ; z < sz - 1 ; ++z ) {
		for( int y = 0 ; y < sy - 1 ; ++y ) {
			for( int x = 0 ; x < sx - 1 ; ++x ) {
				std::vector<std::pair<Point3f, double> > cell;
				for( int dz = 0 ; dz < 2; ++dz ) {
					for( int dy = 0 ; dy < 2 ; ++dy ) {
						for( int dx = 0 ; dx < 2 ; ++dx ) {
							Point3f p;
							p.x  = x + dx;
							p.y  = y + dy;
							p.z  = z + dz;
							float value = volume[p.z][p.y][p.x];
							cell.push_back ( std::make_pair( p, value) );
						}
					}
				}				

				std::vector<Point3f> pnts;
				polygonize(cell, isovalue, pnts ) ;

				// Remove padding
                                if(isPadded)
                                {
                                    for(size_t i = 0; i < pnts.size(); i++){
                                            pnts[i].x -= MC_VOLUME_PADDING;
                                            pnts[i].y -= MC_VOLUME_PADDING;
                                            pnts[i].z -= MC_VOLUME_PADDING;
                                    }
                                }

				for(size_t i = 0; i < pnts.size() / 3; i++)
				{
					std::vector<Point3f> triangle;
					triangle.push_back( pnts[i * 3 + 2] );
					triangle.push_back( pnts[i * 3 + 1] );
					triangle.push_back( pnts[i * 3 + 0] );

					trianglesThread[ omp_get_thread_num() ].push_back( triangle );
				}
			}
		}
	}

	// Combine
	for(auto tris : trianglesThread)
		for(auto t : tris)
			allTriangles.push_back(t);

	return allTriangles;
}