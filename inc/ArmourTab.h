int8 ArmourTable[33][16] = {
/* AR 0  */ {   0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 1  */ {   4,  3,  3,  3,  3,  3,  3,  3,
                3,  3,  3,  3,  3,  3,  3,  3  },

/* AR 2  */ {   9,  8,  6,  6,  6,  6,  6,  6,
                6,  6,  6,  6,  6,  6,  6,  6  },

/* AR 3  */ {  14, 13, 12,  9,  9,  9,  9,  9,
                9,  9,  9,  9,  9,  9,  9,  9  },

/* AR 4  */ {  20, 18, 17, 16, 12, 12, 12, 12,
               12, 12, 12, 12, 12, 12, 12, 12  },

/* AR 5  */ {  26, 23, 22, 21, 20, 15, 15, 15,
               15, 15, 15, 15, 15, 15, 15, 15  },

/* AR 6  */ {  32, 29, 27, 26, 25, 24, 18, 18,
               18, 18, 18, 18, 18, 18, 18, 18  },

/* AR 7  */ {  37, 34, 32, 31, 30, 29, 28, 21,
               21, 21, 21, 21, 21, 21, 21, 21  },

/* AR 8  */ {  43, 40, 38, 36, 35, 34, 33, 32,
               24, 24, 24, 24, 24, 24, 24, 24  },

/* AR 9  */ {  49, 46, 43, 41, 40, 39, 38, 37,
               36, 27, 27, 27, 27, 27, 27, 27  },

/* AR 10 */ {  55, 51, 49, 47, 45, 44, 43, 42,
               41, 40, 30, 30, 30, 30, 30, 30  },

/* AR 11 */ {  65, 61, 57, 54, 52, 50, 49, 48,
               46, 45, 45, 31, 31, 31, 31, 31  },

/* AR 12 */ {  67, 63, 59, 57, 54, 53, 51, 50,
               48, 47, 46, 46, 32, 32, 32, 32  },

/* AR 13 */ {  69, 65, 61, 59, 56, 55, 53, 52,
               50, 49, 48, 48, 47, 33, 33, 33  },

/* AR 14 */ {  71, 67, 63, 61, 59, 57, 55, 54,
               52, 51, 50, 49, 49, 48, 34, 34  },

/* AR 15 */ {  73, 69, 65, 63, 61, 59, 57, 56,
               54, 53, 52, 51, 51, 50, 49, 35  },

/* AR 16 */ {  74, 70, 67, 65, 62, 61, 59, 58,
               56, 55, 54, 53, 52, 52, 51, 50  },

/* AR 17 */ {  76, 72, 69, 67, 64, 63, 61, 59,
               58, 57, 56, 55, 54, 53, 53, 52  },

/* AR 18 */ {  78, 74, 71, 68, 66, 64, 63, 61,
               60, 59, 58, 57, 56, 55, 55, 54  },

/* AR 19 */ {  79, 76, 73, 70, 68, 66, 65, 63,
               62, 61, 60, 59, 58, 57, 56, 56  },

/* AR 20 */ {  81, 78, 75, 72, 70, 68, 66, 65,
               64, 63, 61, 60, 60, 59, 58, 57  },

/* AR 21 */ {  83, 79, 76, 74, 72, 70, 68, 67,
               66, 64, 63, 62, 61, 61, 60, 59  },

/* AR 22 */ {  84, 81, 78, 76, 74, 72, 70, 69,
               67, 66, 65, 64, 63, 62, 61, 61  },

/* AR 23 */ {  86, 83, 80, 78, 75, 74, 72, 70,
               69, 68, 67, 66, 65, 64, 63, 62  },

/* AR 24 */ {  87, 84, 82, 79, 77, 75, 74, 72,
               71, 70, 69, 68, 67, 66, 65, 64  },

/* AR 25 */ {  89, 86, 83, 81, 79, 77, 75, 74,
               73, 71, 70, 69, 68, 67, 67, 66  },

/* AR 26 */ {  91, 88, 85, 83, 81, 79, 77, 76,
               74, 73, 72, 71, 70, 69, 68, 68  },

/* AR 27 */ {  92, 89, 87, 84, 82, 81, 79, 77,
               76, 75, 74, 73, 72, 71, 70, 69  },

/* AR 28 */ {  94, 91, 88, 86, 84, 82, 81, 79,
               78, 77, 75, 74, 73, 73, 72, 71  },

/* AR 29 */ {  95, 93, 90, 88, 86, 84, 82, 81,
               80, 78, 77, 76, 75, 74, 73, 73  },

/* AR 30 */ {  97, 94, 92, 89, 88, 86, 84, 83,
               81, 80, 79, 78, 77, 76, 75, 74  },

/* AR 31 */ {  98, 96, 93, 91, 89, 87, 86, 84,
               83, 82, 81, 79, 78, 78, 77, 76  },

/* AR 32 */ { 100, 97, 95, 93, 91, 89, 87, 86,
               85, 83, 82, 81, 80, 79, 78, 78  },

};


int8 AbsorbTable[17][16] = {
/* AR 0  */ {   0,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 1  */ {   2,  0,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 2  */ {   7,  3,  0,  0,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 3  */ {  12,  8,  5,  2,  0,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 4  */ {  17, 13, 10,  7,  3,  0,  0,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 5  */ {  22, 18, 15, 12,  8,  5,  2,  0,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 6  */ {  27, 23, 20, 17, 13, 10,  7,  3,
                0,  0,  0,  0,  0,  0,  0,  0  },

/* AR 7  */ {  32, 28, 25, 22, 18, 15, 12,  8,
                5,  2,  0,  0,  0,  0,  0,  0  },

/* AR 8  */ {  37, 33, 30, 27, 23, 20, 17, 13,
               10,  7,  3,  0,  0,  0,  0,  0  },

/* AR 9  */ {  42, 38, 35, 32, 28, 25, 22, 18,
               15, 12,  8,  5,  2,  0,  0,  0  },

/* AR 10 */ {  47, 43, 40, 37, 33, 30, 27, 23,
               20, 17, 13, 10,  7,  3,  0,  0  },

/* AR 11 */ {  52, 48, 45, 42, 38, 35, 32, 28,
               25, 22, 18, 15, 12,  8,  5,  2  },

/* AR 12 */ {  57, 53, 50, 47, 43, 40, 37, 33,
               30, 27, 23, 20, 17, 13, 10,  7  },

/* AR 13 */ {  62, 58, 55, 52, 48, 45, 42, 38,
               35, 32, 28, 25, 22, 18, 15, 12  },

/* AR 14 */ {  67, 63, 60, 57, 53, 50, 47, 43,
               40, 37, 33, 30, 27, 23, 20, 17  },

/* AR 15 */ {  72, 68, 65, 62, 58, 55, 52, 48,
               45, 42, 38, 35, 32, 28, 25, 22  },

/* AR 16 */ {  77, 73, 70, 67, 63, 60, 57, 53,
               50, 47, 43, 40, 37, 33, 30, 27  },

};