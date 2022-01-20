#ifndef BLACKMAN
#define BLACKMAN


#include "FFT_Q.h"

short int blackmanWin[FFTLEN] = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 
1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 
6, 6, 7, 7, 8, 9, 9, 10, 11, 12, 
13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 
22, 23, 25, 26, 27, 28, 30, 31, 32, 34, 
35, 36, 38, 39, 41, 42, 44, 46, 47, 49, 
51, 52, 54, 56, 58, 60, 61, 63, 65, 67, 
69, 71, 73, 75, 77, 80, 82, 84, 86, 89, 
91, 93, 96, 98, 100, 103, 105, 108, 110, 113, 
116, 118, 121, 124, 126, 129, 132, 135, 138, 141, 
144, 147, 150, 153, 156, 159, 162, 165, 168, 172, 
175, 178, 182, 185, 188, 192, 195, 199, 202, 206, 
210, 213, 217, 221, 224, 228, 232, 236, 240, 244, 
248, 252, 256, 260, 264, 268, 273, 277, 281, 285, 
290, 294, 299, 303, 308, 312, 317, 321, 326, 331, 
336, 340, 345, 350, 355, 360, 365, 370, 375, 380, 
385, 390, 396, 401, 406, 412, 417, 422, 428, 433, 
439, 444, 450, 456, 462, 467, 473, 479, 485, 491, 
497, 503, 509, 515, 521, 527, 534, 540, 546, 553, 
559, 566, 572, 579, 585, 592, 599, 605, 612, 619, 
626, 633, 640, 647, 654, 661, 668, 675, 683, 690, 
697, 705, 712, 720, 727, 735, 742, 750, 758, 766, 
774, 781, 789, 797, 805, 813, 822, 830, 838, 846, 
855, 863, 872, 880, 889, 897, 906, 915, 923, 932, 
941, 950, 959, 968, 977, 986, 995, 1004, 1014, 1023, 
1032, 1042, 1051, 1061, 1071, 1080, 1090, 1100, 1109, 1119, 
1129, 1139, 1149, 1159, 1170, 1180, 1190, 1200, 1211, 1221, 
1232, 1242, 1253, 1263, 1274, 1285, 1296, 1307, 1318, 1329, 
1340, 1351, 1362, 1373, 1384, 1396, 1407, 1419, 1430, 1442, 
1453, 1465, 1477, 1489, 1501, 1513, 1525, 1537, 1549, 1561, 
1573, 1585, 1598, 1610, 1623, 1635, 1648, 1660, 1673, 1686, 
1699, 1712, 1725, 1738, 1751, 1764, 1777, 1790, 1804, 1817, 
1831, 1844, 1858, 1871, 1885, 1899, 1913, 1926, 1940, 1954, 
1968, 1983, 1997, 2011, 2025, 2040, 2054, 2069, 2083, 2098, 
2113, 2127, 2142, 2157, 2172, 2187, 2202, 2217, 2233, 2248, 
2263, 2279, 2294, 2310, 2325, 2341, 2357, 2372, 2388, 2404, 
2420, 2436, 2452, 2468, 2485, 2501, 2517, 2534, 2550, 2567, 
2583, 2600, 2617, 2634, 2650, 2667, 2684, 2702, 2719, 2736, 
2753, 2770, 2788, 2805, 2823, 2840, 2858, 2876, 2894, 2911, 
2929, 2947, 2965, 2983, 3002, 3020, 3038, 3056, 3075, 3093, 
3112, 3131, 3149, 3168, 3187, 3206, 3225, 3244, 3263, 3282, 
3301, 3320, 3340, 3359, 3378, 3398, 3417, 3437, 3457, 3477, 
3496, 3516, 3536, 3556, 3576, 3596, 3617, 3637, 3657, 3678, 
3698, 3719, 3739, 3760, 3781, 3801, 3822, 3843, 3864, 3885, 
3906, 3927, 3948, 3970, 3991, 4012, 4034, 4055, 4077, 4099, 
4120, 4142, 4164, 4186, 4208, 4230, 4252, 4274, 4296, 4318, 
4340, 4363, 4385, 4408, 4430, 4453, 4475, 4498, 4521, 4544, 
4566, 4589, 4612, 4635, 4658, 4682, 4705, 4728, 4751, 4775, 
4798, 4822, 4845, 4869, 4893, 4916, 4940, 4964, 4988, 5012, 
5036, 5060, 5084, 5108, 5132, 5156, 5181, 5205, 5230, 5254, 
5279, 5303, 5328, 5352, 5377, 5402, 5427, 5452, 5477, 5502, 
5527, 5552, 5577, 5602, 5627, 5653, 5678, 5703, 5729, 5754, 
5780, 5805, 5831, 5857, 5882, 5908, 5934, 5960, 5986, 6011, 
6037, 6063, 6090, 6116, 6142, 6168, 6194, 6221, 6247, 6273, 
6300, 6326, 6353, 6379, 6406, 6432, 6459, 6486, 6513, 6539, 
6566, 6593, 6620, 6647, 6674, 6701, 6728, 6755, 6782, 6809, 
6836, 6864, 6891, 6918, 6946, 6973, 7000, 7028, 7055, 7083, 
7110, 7138, 7166, 7193, 7221, 7249, 7276, 7304, 7332, 7360, 
7388, 7415, 7443, 7471, 7499, 7527, 7555, 7583, 7611, 7640, 
7668, 7696, 7724, 7752, 7780, 7809, 7837, 7865, 7894, 7922, 
7950, 7979, 8007, 8035, 8064, 8092, 8121, 8149, 8178, 8206, 
8235, 8264, 8292, 8321, 8349, 8378, 8407, 8435, 8464, 8493, 
8522, 8550, 8579, 8608, 8637, 8665, 8694, 8723, 8752, 8781, 
8810, 8838, 8867, 8896, 8925, 8954, 8983, 9012, 9041, 9070, 
9098, 9127, 9156, 9185, 9214, 9243, 9272, 9301, 9330, 9359, 
9388, 9417, 9446, 9475, 9504, 9533, 9562, 9591, 9619, 9648, 
9677, 9706, 9735, 9764, 9793, 9822, 9851, 9880, 9909, 9938, 
9966, 9995, 10024, 10053, 10082, 10111, 10140, 10168, 10197, 10226, 
10255, 10284, 10312, 10341, 10370, 10399, 10427, 10456, 10485, 10513, 
10542, 10570, 10599, 10628, 10656, 10685, 10713, 10742, 10770, 10799, 
10827, 10856, 10884, 10912, 10941, 10969, 10997, 11026, 11054, 11082, 
11110, 11138, 11167, 11195, 11223, 11251, 11279, 11307, 11335, 11363, 
11391, 11418, 11446, 11474, 11502, 11530, 11557, 11585, 11613, 11640, 
11668, 11695, 11723, 11750, 11778, 11805, 11832, 11860, 11887, 11914, 
11941, 11968, 11996, 12023, 12050, 12077, 12103, 12130, 12157, 12184, 
12211, 12237, 12264, 12291, 12317, 12344, 12370, 12397, 12423, 12449, 
12475, 12502, 12528, 12554, 12580, 12606, 12632, 12658, 12684, 12709, 
12735, 12761, 12786, 12812, 12837, 12863, 12888, 12914, 12939, 12964, 
12989, 13014, 13039, 13064, 13089, 13114, 13139, 13163, 13188, 13213, 
13237, 13262, 13286, 13310, 13335, 13359, 13383, 13407, 13431, 13455, 
13479, 13503, 13526, 13550, 13574, 13597, 13620, 13644, 13667, 13690, 
13713, 13736, 13759, 13782, 13805, 13828, 13851, 13873, 13896, 13918, 
13941, 13963, 13985, 14007, 14029, 14051, 14073, 14095, 14117, 14138, 
14160, 14182, 14203, 14224, 14246, 14267, 14288, 14309, 14330, 14350, 
14371, 14392, 14412, 14433, 14453, 14474, 14494, 14514, 14534, 14554, 
14574, 14593, 14613, 14633, 14652, 14672, 14691, 14710, 14729, 14748, 
14767, 14786, 14805, 14823, 14842, 14860, 14879, 14897, 14915, 14933, 
14951, 14969, 14987, 15005, 15022, 15040, 15057, 15074, 15091, 15109, 
15126, 15142, 15159, 15176, 15192, 15209, 15225, 15242, 15258, 15274, 
15290, 15306, 15321, 15337, 15352, 15368, 15383, 15398, 15414, 15429, 
15443, 15458, 15473, 15487, 15502, 15516, 15530, 15545, 15559, 15573, 
15586, 15600, 15614, 15627, 15640, 15654, 15667, 15680, 15693, 15706, 
15718, 15731, 15743, 15756, 15768, 15780, 15792, 15804, 15816, 15827, 
15839, 15850, 15862, 15873, 15884, 15895, 15906, 15916, 15927, 15938, 
15948, 15958, 15968, 15978, 15988, 15998, 16008, 16017, 16027, 16036, 
16045, 16054, 16063, 16072, 16081, 16089, 16098, 16106, 16114, 16123, 
16131, 16138, 16146, 16154, 16161, 16169, 16176, 16183, 16190, 16197, 
16204, 16210, 16217, 16223, 16230, 16236, 16242, 16248, 16253, 16259, 
16265, 16270, 16275, 16280, 16286, 16290, 16295, 16300, 16304, 16309, 
16313, 16317, 16321, 16325, 16329, 16333, 16336, 16340, 16343, 16346, 
16349, 16352, 16355, 16357, 16360, 16362, 16365, 16367, 16369, 16371, 
16372, 16374, 16376, 16377, 16378, 16379, 16380, 16381, 16382, 16383, 
16383, 16384, 16384, 16384, 16384, 16384, 16384, 16383, 16383, 16382, 
16381, 16380, 16379, 16378, 16377, 16376, 16374, 16372, 16371, 16369, 
16367, 16365, 16362, 16360, 16357, 16355, 16352, 16349, 16346, 16343, 
16340, 16336, 16333, 16329, 16325, 16321, 16317, 16313, 16309, 16304, 
16300, 16295, 16290, 16286, 16280, 16275, 16270, 16265, 16259, 16253, 
16248, 16242, 16236, 16230, 16223, 16217, 16210, 16204, 16197, 16190, 
16183, 16176, 16169, 16161, 16154, 16146, 16138, 16131, 16123, 16114, 
16106, 16098, 16089, 16081, 16072, 16063, 16054, 16045, 16036, 16027, 
16017, 16008, 15998, 15988, 15978, 15968, 15958, 15948, 15938, 15927, 
15916, 15906, 15895, 15884, 15873, 15862, 15850, 15839, 15827, 15816, 
15804, 15792, 15780, 15768, 15756, 15743, 15731, 15718, 15706, 15693, 
15680, 15667, 15654, 15640, 15627, 15614, 15600, 15586, 15573, 15559, 
15545, 15530, 15516, 15502, 15487, 15473, 15458, 15443, 15429, 15414, 
15398, 15383, 15368, 15352, 15337, 15321, 15306, 15290, 15274, 15258, 
15242, 15225, 15209, 15192, 15176, 15159, 15142, 15126, 15109, 15091, 
15074, 15057, 15040, 15022, 15005, 14987, 14969, 14951, 14933, 14915, 
14897, 14879, 14860, 14842, 14823, 14805, 14786, 14767, 14748, 14729, 
14710, 14691, 14672, 14652, 14633, 14613, 14593, 14574, 14554, 14534, 
14514, 14494, 14474, 14453, 14433, 14412, 14392, 14371, 14350, 14330, 
14309, 14288, 14267, 14246, 14224, 14203, 14182, 14160, 14138, 14117, 
14095, 14073, 14051, 14029, 14007, 13985, 13963, 13941, 13918, 13896, 
13873, 13851, 13828, 13805, 13782, 13759, 13736, 13713, 13690, 13667, 
13644, 13620, 13597, 13574, 13550, 13526, 13503, 13479, 13455, 13431, 
13407, 13383, 13359, 13335, 13310, 13286, 13262, 13237, 13213, 13188, 
13163, 13139, 13114, 13089, 13064, 13039, 13014, 12989, 12964, 12939, 
12914, 12888, 12863, 12837, 12812, 12786, 12761, 12735, 12709, 12684, 
12658, 12632, 12606, 12580, 12554, 12528, 12502, 12475, 12449, 12423, 
12397, 12370, 12344, 12317, 12291, 12264, 12237, 12211, 12184, 12157, 
12130, 12103, 12077, 12050, 12023, 11996, 11968, 11941, 11914, 11887, 
11860, 11832, 11805, 11778, 11750, 11723, 11695, 11668, 11640, 11613, 
11585, 11557, 11530, 11502, 11474, 11446, 11418, 11391, 11363, 11335, 
11307, 11279, 11251, 11223, 11195, 11167, 11138, 11110, 11082, 11054, 
11026, 10997, 10969, 10941, 10912, 10884, 10856, 10827, 10799, 10770, 
10742, 10713, 10685, 10656, 10628, 10599, 10570, 10542, 10513, 10485, 
10456, 10427, 10399, 10370, 10341, 10312, 10284, 10255, 10226, 10197, 
10168, 10140, 10111, 10082, 10053, 10024, 9995, 9966, 9938, 9909, 
9880, 9851, 9822, 9793, 9764, 9735, 9706, 9677, 9648, 9619, 
9591, 9562, 9533, 9504, 9475, 9446, 9417, 9388, 9359, 9330, 
9301, 9272, 9243, 9214, 9185, 9156, 9127, 9098, 9070, 9041, 
9012, 8983, 8954, 8925, 8896, 8867, 8838, 8810, 8781, 8752, 
8723, 8694, 8665, 8637, 8608, 8579, 8550, 8522, 8493, 8464, 
8435, 8407, 8378, 8349, 8321, 8292, 8264, 8235, 8206, 8178, 
8149, 8121, 8092, 8064, 8035, 8007, 7979, 7950, 7922, 7894, 
7865, 7837, 7809, 7780, 7752, 7724, 7696, 7668, 7640, 7611, 
7583, 7555, 7527, 7499, 7471, 7443, 7415, 7388, 7360, 7332, 
7304, 7276, 7249, 7221, 7193, 7166, 7138, 7110, 7083, 7055, 
7028, 7000, 6973, 6946, 6918, 6891, 6864, 6836, 6809, 6782, 
6755, 6728, 6701, 6674, 6647, 6620, 6593, 6566, 6539, 6513, 
6486, 6459, 6432, 6406, 6379, 6353, 6326, 6300, 6273, 6247, 
6221, 6194, 6168, 6142, 6116, 6090, 6063, 6037, 6011, 5986, 
5960, 5934, 5908, 5882, 5857, 5831, 5805, 5780, 5754, 5729, 
5703, 5678, 5653, 5627, 5602, 5577, 5552, 5527, 5502, 5477, 
5452, 5427, 5402, 5377, 5352, 5328, 5303, 5279, 5254, 5230, 
5205, 5181, 5156, 5132, 5108, 5084, 5060, 5036, 5012, 4988, 
4964, 4940, 4916, 4893, 4869, 4845, 4822, 4798, 4775, 4751, 
4728, 4705, 4682, 4658, 4635, 4612, 4589, 4566, 4544, 4521, 
4498, 4475, 4453, 4430, 4408, 4385, 4363, 4340, 4318, 4296, 
4274, 4252, 4230, 4208, 4186, 4164, 4142, 4120, 4099, 4077, 
4055, 4034, 4012, 3991, 3970, 3948, 3927, 3906, 3885, 3864, 
3843, 3822, 3801, 3781, 3760, 3739, 3719, 3698, 3678, 3657, 
3637, 3617, 3596, 3576, 3556, 3536, 3516, 3496, 3477, 3457, 
3437, 3417, 3398, 3378, 3359, 3340, 3320, 3301, 3282, 3263, 
3244, 3225, 3206, 3187, 3168, 3149, 3131, 3112, 3093, 3075, 
3056, 3038, 3020, 3002, 2983, 2965, 2947, 2929, 2911, 2894, 
2876, 2858, 2840, 2823, 2805, 2788, 2770, 2753, 2736, 2719, 
2702, 2684, 2667, 2650, 2634, 2617, 2600, 2583, 2567, 2550, 
2534, 2517, 2501, 2485, 2468, 2452, 2436, 2420, 2404, 2388, 
2372, 2357, 2341, 2325, 2310, 2294, 2279, 2263, 2248, 2233, 
2217, 2202, 2187, 2172, 2157, 2142, 2127, 2113, 2098, 2083, 
2069, 2054, 2040, 2025, 2011, 1997, 1983, 1968, 1954, 1940, 
1926, 1913, 1899, 1885, 1871, 1858, 1844, 1831, 1817, 1804, 
1790, 1777, 1764, 1751, 1738, 1725, 1712, 1699, 1686, 1673, 
1660, 1648, 1635, 1623, 1610, 1598, 1585, 1573, 1561, 1549, 
1537, 1525, 1513, 1501, 1489, 1477, 1465, 1453, 1442, 1430, 
1419, 1407, 1396, 1384, 1373, 1362, 1351, 1340, 1329, 1318, 
1307, 1296, 1285, 1274, 1263, 1253, 1242, 1232, 1221, 1211, 
1200, 1190, 1180, 1170, 1159, 1149, 1139, 1129, 1119, 1109, 
1100, 1090, 1080, 1071, 1061, 1051, 1042, 1032, 1023, 1014, 
1004, 995, 986, 977, 968, 959, 950, 941, 932, 923, 
915, 906, 897, 889, 880, 872, 863, 855, 846, 838, 
830, 822, 813, 805, 797, 789, 781, 774, 766, 758, 
750, 742, 735, 727, 720, 712, 705, 697, 690, 683, 
675, 668, 661, 654, 647, 640, 633, 626, 619, 612, 
605, 599, 592, 585, 579, 572, 566, 559, 553, 546, 
540, 534, 527, 521, 515, 509, 503, 497, 491, 485, 
479, 473, 467, 462, 456, 450, 444, 439, 433, 428, 
422, 417, 412, 406, 401, 396, 390, 385, 380, 375, 
370, 365, 360, 355, 350, 345, 340, 336, 331, 326, 
321, 317, 312, 308, 303, 299, 294, 290, 285, 281, 
277, 273, 268, 264, 260, 256, 252, 248, 244, 240, 
236, 232, 228, 224, 221, 217, 213, 210, 206, 202, 
199, 195, 192, 188, 185, 182, 178, 175, 172, 168, 
165, 162, 159, 156, 153, 150, 147, 144, 141, 138, 
135, 132, 129, 126, 124, 121, 118, 116, 113, 110, 
108, 105, 103, 100, 98, 96, 93, 91, 89, 86, 
84, 82, 80, 77, 75, 73, 71, 69, 67, 65, 
63, 61, 60, 58, 56, 54, 52, 51, 49, 47, 
46, 44, 42, 41, 39, 38, 36, 35, 34, 32, 
31, 30, 28, 27, 26, 25, 23, 22, 21, 20, 
19, 18, 17, 16, 15, 14, 13, 13, 12, 11, 
10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 
4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 
1, 1, 0, 0, 0, 0, 0, 0 };

#endif