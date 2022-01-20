#ifndef HAMMING
#define HAMMING


#include "FFT_Q.h"

short int hammingWin[FFTLEN] = { 1311, 1311, 1311, 1311, 1311, 1312, 1312, 1312, 1313, 1314, 
1314, 1315, 1316, 1317, 1318, 1319, 1320, 1321, 1322, 1324, 
1325, 1326, 1328, 1329, 1331, 1333, 1335, 1337, 1339, 1341, 
1343, 1345, 1347, 1349, 1352, 1354, 1357, 1359, 1362, 1365, 
1367, 1370, 1373, 1376, 1379, 1383, 1386, 1389, 1392, 1396, 
1399, 1403, 1407, 1410, 1414, 1418, 1422, 1426, 1430, 1434, 
1438, 1442, 1447, 1451, 1456, 1460, 1465, 1470, 1474, 1479, 
1484, 1489, 1494, 1499, 1504, 1510, 1515, 1520, 1526, 1531, 
1537, 1542, 1548, 1554, 1560, 1566, 1572, 1578, 1584, 1590, 
1596, 1603, 1609, 1616, 1622, 1629, 1636, 1642, 1649, 1656, 
1663, 1670, 1677, 1684, 1691, 1699, 1706, 1714, 1721, 1729, 
1736, 1744, 1752, 1760, 1767, 1775, 1783, 1792, 1800, 1808, 
1816, 1825, 1833, 1842, 1850, 1859, 1867, 1876, 1885, 1894, 
1903, 1912, 1921, 1930, 1939, 1949, 1958, 1967, 1977, 1986, 
1996, 2006, 2015, 2025, 2035, 2045, 2055, 2065, 2075, 2085, 
2096, 2106, 2116, 2127, 2137, 2148, 2158, 2169, 2180, 2191, 
2201, 2212, 2223, 2235, 2246, 2257, 2268, 2279, 2291, 2302, 
2314, 2325, 2337, 2349, 2360, 2372, 2384, 2396, 2408, 2420, 
2432, 2444, 2456, 2469, 2481, 2494, 2506, 2519, 2531, 2544, 
2556, 2569, 2582, 2595, 2608, 2621, 2634, 2647, 2660, 2674, 
2687, 2700, 2714, 2727, 2741, 2754, 2768, 2782, 2795, 2809, 
2823, 2837, 2851, 2865, 2879, 2893, 2907, 2922, 2936, 2950, 
2965, 2979, 2994, 3008, 3023, 3038, 3053, 3067, 3082, 3097, 
3112, 3127, 3142, 3157, 3173, 3188, 3203, 3219, 3234, 3249, 
3265, 3280, 3296, 3312, 3327, 3343, 3359, 3375, 3391, 3407, 
3423, 3439, 3455, 3471, 3488, 3504, 3520, 3537, 3553, 3570, 
3586, 3603, 3619, 3636, 3653, 3669, 3686, 3703, 3720, 3737, 
3754, 3771, 3788, 3806, 3823, 3840, 3857, 3875, 3892, 3910, 
3927, 3945, 3962, 3980, 3997, 4015, 4033, 4051, 4069, 4087, 
4105, 4123, 4141, 4159, 4177, 4195, 4213, 4231, 4250, 4268, 
4287, 4305, 4323, 4342, 4361, 4379, 4398, 4416, 4435, 4454, 
4473, 4492, 4511, 4530, 4549, 4568, 4587, 4606, 4625, 4644, 
4663, 4682, 4702, 4721, 4741, 4760, 4779, 4799, 4818, 4838, 
4858, 4877, 4897, 4917, 4936, 4956, 4976, 4996, 5016, 5036, 
5056, 5076, 5096, 5116, 5136, 5156, 5176, 5197, 5217, 5237, 
5257, 5278, 5298, 5319, 5339, 5359, 5380, 5401, 5421, 5442, 
5462, 5483, 5504, 5525, 5545, 5566, 5587, 5608, 5629, 5650, 
5671, 5692, 5713, 5734, 5755, 5776, 5797, 5818, 5839, 5861, 
5882, 5903, 5925, 5946, 5967, 5989, 6010, 6031, 6053, 6074, 
6096, 6118, 6139, 6161, 6182, 6204, 6226, 6247, 6269, 6291, 
6313, 6334, 6356, 6378, 6400, 6422, 6444, 6466, 6488, 6510, 
6532, 6554, 6576, 6598, 6620, 6642, 6664, 6686, 6708, 6731, 
6753, 6775, 6797, 6820, 6842, 6864, 6886, 6909, 6931, 6954, 
6976, 6998, 7021, 7043, 7066, 7088, 7111, 7133, 7156, 7178, 
7201, 7224, 7246, 7269, 7291, 7314, 7337, 7359, 7382, 7405, 
7427, 7450, 7473, 7496, 7518, 7541, 7564, 7587, 7610, 7632, 
7655, 7678, 7701, 7724, 7747, 7770, 7792, 7815, 7838, 7861, 
7884, 7907, 7930, 7953, 7976, 7999, 8022, 8045, 8068, 8091, 
8114, 8137, 8160, 8183, 8206, 8229, 8252, 8275, 8298, 8322, 
8345, 8368, 8391, 8414, 8437, 8460, 8483, 8506, 8529, 8552, 
8576, 8599, 8622, 8645, 8668, 8691, 8714, 8737, 8761, 8784, 
8807, 8830, 8853, 8876, 8899, 8923, 8946, 8969, 8992, 9015, 
9038, 9061, 9084, 9108, 9131, 9154, 9177, 9200, 9223, 9246, 
9269, 9292, 9316, 9339, 9362, 9385, 9408, 9431, 9454, 9477, 
9500, 9523, 9546, 9569, 9592, 9615, 9638, 9661, 9684, 9707, 
9730, 9753, 9776, 9799, 9822, 9845, 9868, 9891, 9914, 9937, 
9959, 9982, 10005, 10028, 10051, 10074, 10097, 10119, 10142, 10165, 
10188, 10210, 10233, 10256, 10279, 10301, 10324, 10347, 10369, 10392, 
10415, 10437, 10460, 10483, 10505, 10528, 10550, 10573, 10595, 10618, 
10640, 10663, 10685, 10708, 10730, 10752, 10775, 10797, 10819, 10842, 
10864, 10886, 10909, 10931, 10953, 10975, 10997, 11020, 11042, 11064, 
11086, 11108, 11130, 11152, 11174, 11196, 11218, 11240, 11262, 11284, 
11306, 11328, 11349, 11371, 11393, 11415, 11437, 11458, 11480, 11502, 
11523, 11545, 11566, 11588, 11610, 11631, 11653, 11674, 11695, 11717, 
11738, 11760, 11781, 11802, 11823, 11845, 11866, 11887, 11908, 11929, 
11950, 11971, 11993, 12014, 12035, 12055, 12076, 12097, 12118, 12139, 
12160, 12181, 12201, 12222, 12243, 12263, 12284, 12304, 12325, 12345, 
12366, 12386, 12407, 12427, 12448, 12468, 12488, 12508, 12529, 12549, 
12569, 12589, 12609, 12629, 12649, 12669, 12689, 12709, 12729, 12748, 
12768, 12788, 12808, 12827, 12847, 12867, 12886, 12906, 12925, 12945, 
12964, 12983, 13003, 13022, 13041, 13060, 13079, 13099, 13118, 13137, 
13156, 13175, 13194, 13212, 13231, 13250, 13269, 13288, 13306, 13325, 
13343, 13362, 13381, 13399, 13417, 13436, 13454, 13472, 13491, 13509, 
13527, 13545, 13563, 13581, 13599, 13617, 13635, 13653, 13671, 13688, 
13706, 13724, 13741, 13759, 13776, 13794, 13811, 13829, 13846, 13863, 
13881, 13898, 13915, 13932, 13949, 13966, 13983, 14000, 14017, 14034, 
14050, 14067, 14084, 14100, 14117, 14133, 14150, 14166, 14183, 14199, 
14215, 14231, 14248, 14264, 14280, 14296, 14312, 14328, 14344, 14359, 
14375, 14391, 14406, 14422, 14438, 14453, 14469, 14484, 14499, 14515, 
14530, 14545, 14560, 14575, 14590, 14605, 14620, 14635, 14650, 14664, 
14679, 14694, 14708, 14723, 14737, 14752, 14766, 14780, 14794, 14809, 
14823, 14837, 14851, 14865, 14879, 14893, 14906, 14920, 14934, 14947, 
14961, 14974, 14988, 15001, 15015, 15028, 15041, 15054, 15067, 15080, 
15093, 15106, 15119, 15132, 15145, 15157, 15170, 15182, 15195, 15207, 
15220, 15232, 15244, 15257, 15269, 15281, 15293, 15305, 15317, 15329, 
15340, 15352, 15364, 15375, 15387, 15398, 15410, 15421, 15432, 15444, 
15455, 15466, 15477, 15488, 15499, 15510, 15520, 15531, 15542, 15552, 
15563, 15573, 15584, 15594, 15604, 15615, 15625, 15635, 15645, 15655, 
15665, 15674, 15684, 15694, 15704, 15713, 15723, 15732, 15741, 15751, 
15760, 15769, 15778, 15787, 15796, 15805, 15814, 15823, 15832, 15840, 
15849, 15857, 15866, 15874, 15883, 15891, 15899, 15907, 15915, 15923, 
15931, 15939, 15947, 15955, 15962, 15970, 15977, 15985, 15992, 16000, 
16007, 16014, 16021, 16028, 16035, 16042, 16049, 16056, 16063, 16069, 
16076, 16082, 16089, 16095, 16101, 16108, 16114, 16120, 16126, 16132, 
16138, 16144, 16149, 16155, 16161, 16166, 16172, 16177, 16183, 16188, 
16193, 16198, 16203, 16208, 16213, 16218, 16223, 16228, 16232, 16237, 
16241, 16246, 16250, 16254, 16259, 16263, 16267, 16271, 16275, 16279, 
16283, 16286, 16290, 16294, 16297, 16301, 16304, 16307, 16311, 16314, 
16317, 16320, 16323, 16326, 16329, 16331, 16334, 16337, 16339, 16342, 
16344, 16347, 16349, 16351, 16353, 16355, 16357, 16359, 16361, 16363, 
16364, 16366, 16368, 16369, 16371, 16372, 16373, 16374, 16375, 16377, 
16378, 16378, 16379, 16380, 16381, 16381, 16382, 16383, 16383, 16383, 
16384, 16384, 16384, 16384, 16384, 16384, 16384, 16384, 16383, 16383, 
16383, 16382, 16381, 16381, 16380, 16379, 16378, 16378, 16377, 16375, 
16374, 16373, 16372, 16371, 16369, 16368, 16366, 16364, 16363, 16361, 
16359, 16357, 16355, 16353, 16351, 16349, 16347, 16344, 16342, 16339, 
16337, 16334, 16331, 16329, 16326, 16323, 16320, 16317, 16314, 16311, 
16307, 16304, 16301, 16297, 16294, 16290, 16286, 16283, 16279, 16275, 
16271, 16267, 16263, 16259, 16254, 16250, 16246, 16241, 16237, 16232, 
16228, 16223, 16218, 16213, 16208, 16203, 16198, 16193, 16188, 16183, 
16177, 16172, 16166, 16161, 16155, 16149, 16144, 16138, 16132, 16126, 
16120, 16114, 16108, 16101, 16095, 16089, 16082, 16076, 16069, 16063, 
16056, 16049, 16042, 16035, 16028, 16021, 16014, 16007, 16000, 15992, 
15985, 15977, 15970, 15962, 15955, 15947, 15939, 15931, 15923, 15915, 
15907, 15899, 15891, 15883, 15874, 15866, 15857, 15849, 15840, 15832, 
15823, 15814, 15805, 15796, 15787, 15778, 15769, 15760, 15751, 15741, 
15732, 15723, 15713, 15704, 15694, 15684, 15674, 15665, 15655, 15645, 
15635, 15625, 15615, 15604, 15594, 15584, 15573, 15563, 15552, 15542, 
15531, 15520, 15510, 15499, 15488, 15477, 15466, 15455, 15444, 15432, 
15421, 15410, 15398, 15387, 15375, 15364, 15352, 15340, 15329, 15317, 
15305, 15293, 15281, 15269, 15257, 15244, 15232, 15220, 15207, 15195, 
15182, 15170, 15157, 15145, 15132, 15119, 15106, 15093, 15080, 15067, 
15054, 15041, 15028, 15015, 15001, 14988, 14974, 14961, 14947, 14934, 
14920, 14906, 14893, 14879, 14865, 14851, 14837, 14823, 14809, 14794, 
14780, 14766, 14752, 14737, 14723, 14708, 14694, 14679, 14664, 14650, 
14635, 14620, 14605, 14590, 14575, 14560, 14545, 14530, 14515, 14499, 
14484, 14469, 14453, 14438, 14422, 14406, 14391, 14375, 14359, 14344, 
14328, 14312, 14296, 14280, 14264, 14248, 14231, 14215, 14199, 14183, 
14166, 14150, 14133, 14117, 14100, 14084, 14067, 14050, 14034, 14017, 
14000, 13983, 13966, 13949, 13932, 13915, 13898, 13881, 13863, 13846, 
13829, 13811, 13794, 13776, 13759, 13741, 13724, 13706, 13688, 13671, 
13653, 13635, 13617, 13599, 13581, 13563, 13545, 13527, 13509, 13491, 
13472, 13454, 13436, 13417, 13399, 13381, 13362, 13343, 13325, 13306, 
13288, 13269, 13250, 13231, 13212, 13194, 13175, 13156, 13137, 13118, 
13099, 13079, 13060, 13041, 13022, 13003, 12983, 12964, 12945, 12925, 
12906, 12886, 12867, 12847, 12827, 12808, 12788, 12768, 12748, 12729, 
12709, 12689, 12669, 12649, 12629, 12609, 12589, 12569, 12549, 12529, 
12508, 12488, 12468, 12448, 12427, 12407, 12386, 12366, 12345, 12325, 
12304, 12284, 12263, 12243, 12222, 12201, 12181, 12160, 12139, 12118, 
12097, 12076, 12055, 12035, 12014, 11993, 11971, 11950, 11929, 11908, 
11887, 11866, 11845, 11823, 11802, 11781, 11760, 11738, 11717, 11695, 
11674, 11653, 11631, 11610, 11588, 11566, 11545, 11523, 11502, 11480, 
11458, 11437, 11415, 11393, 11371, 11349, 11328, 11306, 11284, 11262, 
11240, 11218, 11196, 11174, 11152, 11130, 11108, 11086, 11064, 11042, 
11020, 10997, 10975, 10953, 10931, 10909, 10886, 10864, 10842, 10819, 
10797, 10775, 10752, 10730, 10708, 10685, 10663, 10640, 10618, 10595, 
10573, 10550, 10528, 10505, 10483, 10460, 10437, 10415, 10392, 10369, 
10347, 10324, 10301, 10279, 10256, 10233, 10210, 10188, 10165, 10142, 
10119, 10097, 10074, 10051, 10028, 10005, 9982, 9959, 9937, 9914, 
9891, 9868, 9845, 9822, 9799, 9776, 9753, 9730, 9707, 9684, 
9661, 9638, 9615, 9592, 9569, 9546, 9523, 9500, 9477, 9454, 
9431, 9408, 9385, 9362, 9339, 9316, 9292, 9269, 9246, 9223, 
9200, 9177, 9154, 9131, 9108, 9084, 9061, 9038, 9015, 8992, 
8969, 8946, 8923, 8899, 8876, 8853, 8830, 8807, 8784, 8761, 
8737, 8714, 8691, 8668, 8645, 8622, 8599, 8576, 8552, 8529, 
8506, 8483, 8460, 8437, 8414, 8391, 8368, 8345, 8322, 8298, 
8275, 8252, 8229, 8206, 8183, 8160, 8137, 8114, 8091, 8068, 
8045, 8022, 7999, 7976, 7953, 7930, 7907, 7884, 7861, 7838, 
7815, 7792, 7770, 7747, 7724, 7701, 7678, 7655, 7632, 7610, 
7587, 7564, 7541, 7518, 7496, 7473, 7450, 7427, 7405, 7382, 
7359, 7337, 7314, 7291, 7269, 7246, 7224, 7201, 7178, 7156, 
7133, 7111, 7088, 7066, 7043, 7021, 6998, 6976, 6954, 6931, 
6909, 6886, 6864, 6842, 6820, 6797, 6775, 6753, 6731, 6708, 
6686, 6664, 6642, 6620, 6598, 6576, 6554, 6532, 6510, 6488, 
6466, 6444, 6422, 6400, 6378, 6356, 6334, 6313, 6291, 6269, 
6247, 6226, 6204, 6182, 6161, 6139, 6118, 6096, 6074, 6053, 
6031, 6010, 5989, 5967, 5946, 5925, 5903, 5882, 5861, 5839, 
5818, 5797, 5776, 5755, 5734, 5713, 5692, 5671, 5650, 5629, 
5608, 5587, 5566, 5545, 5525, 5504, 5483, 5462, 5442, 5421, 
5401, 5380, 5359, 5339, 5319, 5298, 5278, 5257, 5237, 5217, 
5197, 5176, 5156, 5136, 5116, 5096, 5076, 5056, 5036, 5016, 
4996, 4976, 4956, 4936, 4917, 4897, 4877, 4858, 4838, 4818, 
4799, 4779, 4760, 4741, 4721, 4702, 4682, 4663, 4644, 4625, 
4606, 4587, 4568, 4549, 4530, 4511, 4492, 4473, 4454, 4435, 
4416, 4398, 4379, 4361, 4342, 4323, 4305, 4287, 4268, 4250, 
4231, 4213, 4195, 4177, 4159, 4141, 4123, 4105, 4087, 4069, 
4051, 4033, 4015, 3997, 3980, 3962, 3945, 3927, 3910, 3892, 
3875, 3857, 3840, 3823, 3806, 3788, 3771, 3754, 3737, 3720, 
3703, 3686, 3669, 3653, 3636, 3619, 3603, 3586, 3570, 3553, 
3537, 3520, 3504, 3488, 3471, 3455, 3439, 3423, 3407, 3391, 
3375, 3359, 3343, 3327, 3312, 3296, 3280, 3265, 3249, 3234, 
3219, 3203, 3188, 3173, 3157, 3142, 3127, 3112, 3097, 3082, 
3067, 3053, 3038, 3023, 3008, 2994, 2979, 2965, 2950, 2936, 
2922, 2907, 2893, 2879, 2865, 2851, 2837, 2823, 2809, 2795, 
2782, 2768, 2754, 2741, 2727, 2714, 2700, 2687, 2674, 2660, 
2647, 2634, 2621, 2608, 2595, 2582, 2569, 2556, 2544, 2531, 
2519, 2506, 2494, 2481, 2469, 2456, 2444, 2432, 2420, 2408, 
2396, 2384, 2372, 2360, 2349, 2337, 2325, 2314, 2302, 2291, 
2279, 2268, 2257, 2246, 2235, 2223, 2212, 2201, 2191, 2180, 
2169, 2158, 2148, 2137, 2127, 2116, 2106, 2096, 2085, 2075, 
2065, 2055, 2045, 2035, 2025, 2015, 2006, 1996, 1986, 1977, 
1967, 1958, 1949, 1939, 1930, 1921, 1912, 1903, 1894, 1885, 
1876, 1867, 1859, 1850, 1842, 1833, 1825, 1816, 1808, 1800, 
1792, 1783, 1775, 1767, 1760, 1752, 1744, 1736, 1729, 1721, 
1714, 1706, 1699, 1691, 1684, 1677, 1670, 1663, 1656, 1649, 
1642, 1636, 1629, 1622, 1616, 1609, 1603, 1596, 1590, 1584, 
1578, 1572, 1566, 1560, 1554, 1548, 1542, 1537, 1531, 1526, 
1520, 1515, 1510, 1504, 1499, 1494, 1489, 1484, 1479, 1474, 
1470, 1465, 1460, 1456, 1451, 1447, 1442, 1438, 1434, 1430, 
1426, 1422, 1418, 1414, 1410, 1407, 1403, 1399, 1396, 1392, 
1389, 1386, 1383, 1379, 1376, 1373, 1370, 1367, 1365, 1362, 
1359, 1357, 1354, 1352, 1349, 1347, 1345, 1343, 1341, 1339, 
1337, 1335, 1333, 1331, 1329, 1328, 1326, 1325, 1324, 1322, 
1321, 1320, 1319, 1318, 1317, 1316, 1315, 1314, 1314, 1313, 
1312, 1312, 1312, 1311, 1311, 1311, 1311, 1311 };

#endif