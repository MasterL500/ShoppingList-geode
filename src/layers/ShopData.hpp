const std::vector<std::vector<int>> ShopData = {
    //  REFERENCE
    //  - Hexadecimal code of the gamemode
    //  - Icon ID
    //  - Price in the shop
    //  - Shop ID
    //  - Page (for the mod)

    //  THE SHOP
    {0x1, 73, 1000, 1, 1},
    {0x1, 77, 1000, 1, 1},
    {0x1, 79, 500, 1, 1},
    {0x1, 86, 1000, 1, 1},
    {0x1, 102, 1000, 1, 1},
    {0x1, 107, 1000, 1, 1},
    {0x1, 109, 4000, 1, 1},
    {0x1, 113, 4000, 1, 1},
    {0x1, 140, 3000, 1, 1},
    {0x1, 162, 3000, 1, 1},
    {0x1, 191, 3000, 1, 1},
    {0x4, 27, 2000, 1, 1},
    {0x5, 19, 1500, 1, 1},
    {0x5, 20, 1000, 1, 1},
    {0x5, 40, 4000, 1, 1},
    {0x5, 57, 3000, 1, 1},
    {0x6, 23, 2500, 1, 1},
    {0x6, 25, 2000, 1, 1},
    {0x6, 40, 3500, 1, 1},
    {0x7, 21, 500, 1, 1},
    {0x7, 35, 4000, 1, 1},
    {0x7, 38, 5000, 1, 1},
    {0x8, 12, 3000, 1, 1},
    {0x8, 33, 3500, 1, 1},
    {0x9, 2, 2000, 1, 1},
    {0x9, 18, 4000, 1, 1},
    {0xD, 7, 5000, 1, 1},

    //  SCRATCH'S SHOP
    {0x1, 85, 2500, 2, 1},
    {0x1, 94, 3000, 2, 1},
    {0x1, 96, 2000, 2, 1},
    {0x1, 110, 5000, 2, 1},
    {0x1, 136, 5000, 2, 1},
    {0x1, 206, 2000, 2, 1},
    {0x1, 225, 3000, 2, 1},
    {0x1, 229, 5000, 2, 1},
    {0x4, 58, 4000, 2, 1},
    {0x4, 77, 4000, 2, 1},
    {0x5, 25, 2000, 2, 1},
    {0x5, 60, 2500, 2, 1},
    {0x5, 107, 3000, 2, 1},
    {0x6, 21, 4000, 2, 1},
    {0x6, 83, 3000, 2, 1},
    {0x6, 104, 2500, 2, 1},
    {0x7, 18, 2000, 2, 1},
    {0x7, 78, 2500, 2, 1},
    {0x8, 14, 3000, 2, 1},
    {0x8, 45, 3500, 2, 1},
    {0x9, 3, 3000, 2, 1},
    {0x9, 58, 3500, 2, 1},
    {0x9, 69, 5000, 2, 1},
    {0xd, 12, 6000, 2, 1},
    {0xd, 27, 6000, 2, 1},

    //  COMMUNITY SHOP
    {0x1, 117, 4000, 3, 1},
    {0x1, 118, 2000, 3, 1},
    {0x1, 119, 2000, 3, 1},
    {0x1, 120, 4000, 3, 1},
    {0x1, 121, 2000, 3, 1},
    {0x1, 122, 2000, 3, 1},
    {0x1, 123, 6000, 3, 1},
    {0x1, 124, 8000, 3, 1},
    {0x1, 125, 4000, 3, 1},
    {0x1, 126, 2000, 3, 1},
    {0x1, 127, 6000, 3, 1},
    {0x1, 128, 8000, 3, 1},
    {0x1, 129, 6000, 3, 1},
    {0x1, 130, 4000, 3, 1},
    {0x1, 131, 4000, 3, 1},
    {0x1, 132, 4000, 3, 1},
    {0x1, 133, 8000, 3, 1},
    {0x1, 134, 6000, 3, 1},
    {0x1, 135, 6000, 3, 1},
    {0x4, 36, 2000, 3, 1},
    {0x4, 37, 2000, 3, 1},
    {0x4, 38, 6000, 3, 1},
    {0x4, 39, 2000, 3, 1},
    {0x4, 40, 4000, 3, 1},
    {0x4, 41, 4000, 3, 1},
    {0x4, 42, 6000, 3, 1},
    {0x4, 43, 2000, 3, 1},
    {0x4, 44, 6000, 3, 1},
    {0x4, 45, 8000, 3, 1},
    {0x4, 46, 4000, 3, 1},
    {0x4, 47, 6000, 3, 1},
    {0x4, 48, 4000, 3, 1},
    {0x5, 30, 4000, 3, 1},
    {0x5, 31, 6000, 3, 1},
    {0x5, 32, 6000, 3, 1},
    {0x5, 33, 2000, 3, 1},
    {0x5, 34, 4000, 3, 1},
    {0x5, 35, 8000, 3, 1},
    {0x5, 36, 6000, 3, 1},
    {0x5, 37, 2000, 3, 1},

    {0x5, 38, 2000, 3, 2},
    {0x6, 29, 6000, 3, 2},
    {0x6, 30, 6000, 3, 2},
    {0x6, 31, 4000, 3, 2},
    {0x6, 32, 6000, 3, 2},
    {0x6, 33, 4000, 3, 2},
    {0x6, 34, 2000, 3, 2},
    {0x6, 35, 2000, 3, 2},
    {0x7, 24, 2000, 3, 2},
    {0x7, 25, 4000, 3, 2},
    {0x7, 26, 2000, 3, 2},
    {0x7, 27, 2000, 3, 2},
    {0x7, 28, 6000, 3, 2},
    {0x7, 29, 6000, 3, 2},
    {0x7, 30, 4000, 3, 2},
    {0x7, 31, 6000, 3, 2},
    {0x7, 32, 2000, 3, 2},
    {0x7, 33, 6000, 3, 2},
    {0x8, 18, 2000, 3, 2},
    {0x8, 19, 6000, 3, 2},
    {0x8, 20, 2000, 3, 2},
    {0x8, 21, 4000, 3, 2},
    {0x8, 22, 6000, 3, 2},
    {0x8, 23, 2000, 3, 2},
    {0x8, 24, 6000, 3, 2},
    {0x8, 25, 4000, 3, 2},
    {0x9, 11, 4000, 3, 2},
    {0x9, 12, 6000, 3, 2},
    {0x9, 13, 4000, 3, 2},
    {0x9, 14, 8000, 3, 2},
    {0x9, 15, 2000, 3, 2},
    {0x9, 16, 6000, 3, 2},

    //  THE MECHANIC
    {0x1, 168, 5000, 4, 1},
    {0x1, 185, 6000, 4, 1},
    {0x1, 190, 5000, 4, 1},
    {0x1, 230, 6000, 4, 1},
    {0x1, 253, 7000, 4, 1},
    {0x1, 358, 6000, 4, 1},
    {0x1, 360, 6000, 4, 1},
    {0x1, 480, 5000, 4, 1},
    {0x4, 80, 6000, 4, 1},
    {0x4, 111, 6000, 4, 1},
    {0x4, 126, 7000, 4, 1},
    {0x4, 144, 5000, 4, 1},
    {0x4, 164, 7000, 4, 1},
    {0x5, 49, 6000, 4, 1},
    {0x5, 54, 5000, 4, 1},
    {0x5, 64, 5000, 4, 1},
    {0x5, 65, 5000, 4, 1},
    {0x6, 86, 7000, 4, 1},
    {0x6, 95, 7000, 4, 1},
    {0x6, 107, 6000, 4, 1},
    {0x6, 132, 6000, 4, 1},
    {0x6, 136, 5000, 4, 1},
    {0x7, 68, 5000, 4, 1},
    {0x7, 77, 7000, 4, 1},
    {0x7, 85, 6000, 4, 1},
    {0x8, 52, 9000, 4, 1},
    {0x8, 68, 7000, 4, 1},
    {0x9, 50, 6000, 4, 1},
    {0x9, 56, 6000, 4, 1},
    {0xd, 6, 6000, 4, 1},

    //  DIAMOND SHOP
    {0x1, 181, 300, 5, 1},
    {0x1, 186, 500, 5, 1},
    {0x1, 196, 400, 5, 1},
    {0x1, 203, 600, 5, 1},
    {0x1, 250, 300, 5, 1},
    {0x1, 263, 600, 5, 1},
    {0x1, 290, 400, 5, 1},
    {0x1, 314, 400, 5, 1},
    {0x1, 421, 500, 5, 1},
    {0x1, 443, 600, 5, 1},
    {0x1, 450, 500, 5, 1},
    {0x1, 456, 400, 5, 1},
    {0x1, 463, 400, 5, 1},
    {0x1, 464, 500, 5, 1},
    {0x1, 465, 400, 5, 1},
    {0x1, 471, 400, 5, 1},
    {0x1, 474, 300, 5, 1},
    {0x1, 478, 400, 5, 1},
    {0x4, 62, 500, 5, 1},
    {0x4, 63, 300, 5, 1},
    {0x4, 66, 500, 5, 1},
    {0x4, 81, 600, 5, 1},
    {0x4, 87, 400, 5, 1},
    {0x4, 125, 500, 5, 1},
    {0x4, 129, 400, 5, 1},
    {0x4, 131, 500, 5, 1},
    {0x4, 168, 600, 5, 1},
    {0x5, 53, 600, 5, 1},
    {0x5, 55, 300, 5, 1},
    {0x5, 81, 500, 5, 1},
    {0x5, 98, 300, 5, 1},
    {0x5, 112, 500, 5, 1},
    {0x5, 113, 400, 5, 1},
    {0x5, 116, 500, 5, 1},
    {0x6, 45, 500, 5, 1},
    {0x6, 52, 500, 5, 1},
    {0x6, 78, 300, 5, 1},
    {0x6, 80, 400, 5, 1},
    {0x6, 100, 600, 5, 1},
    {0x6, 140, 600, 5, 1},

    {0x6, 141, 500, 5, 2},
    {0x6, 149, 600, 5, 2},
    {0x7, 56, 400, 5, 2},
    {0x7, 65, 300, 5, 2},
    {0x7, 75, 500, 5, 2},
    {0x7, 80, 300, 5, 2},
    {0x7, 82, 500, 5, 2},
    {0x7, 87, 400, 5, 2},
    {0x8, 34, 500, 5, 2},
    {0x8, 49, 500, 5, 2},
    {0x8, 51, 500, 5, 2},
    {0x8, 62, 300, 5, 2},
    {0x8, 63, 600, 5, 2},
    {0x9, 22, 400, 5, 2},
    {0x9, 29, 500, 5, 2},
    {0x9, 54, 300, 5, 2},
    {0x9, 60, 400, 5, 2},
    {0x9, 64, 500, 5, 2},
    {0x9, 65, 300, 5, 2},
    {0x9, 66, 500, 5, 2},
    {0xd, 10, 500, 5, 2},
    {0xd, 30, 500, 5, 2},
    {0xd, 40, 600, 5, 2},
    {0xd, 41, 400, 5, 2},
    {0xd, 42, 500, 5, 2},
    {0xe, 5, 500, 5, 2},
};

const std::vector<std::vector<int>> ShopDataExtras = {
    {0x2, 33, 1000, 1, 2},
    {0x2, 35, 1000, 1, 2},
    {0x2, 36, 1000, 1, 2},
    {0x2, 84, 1500, 1, 2},
    {0x2, 97, 2500, 1, 2},
    {0x2, 99, 1500, 1, 2},
    {0x3, 28, 1000, 1, 2},
    {0x3, 39, 1000, 1, 2},
    {0x3, 49, 2500, 1, 2},
    {0x3, 88, 2000, 1, 2},
    {0x3, 104, 2000, 1, 2},
    {0xa, 5, 4000, 1, 2},
    {0xa, 6, 4000, 1, 2},
    {0xb, 8, 7000, 1, 2},
    {0xb, 11, 7000, 1, 2},

    {0x2, 39, 1000, 2, 2},
    {0x2, 40, 1000, 2, 2},
    {0x2, 49, 1000, 2, 2},
    {0x2, 94, 1000, 2, 2},
    {0x3, 32, 1000, 2, 2},
    {0x3, 41, 1000, 2, 2},
    {0x3, 85, 1500, 2, 2},
    {0x3, 86, 1000, 2, 2},
    {0x3, 70, 1000, 2, 2},
    {0xa, 7, 4000, 2, 2},
    {0xb, 13, 10000, 2, 2},
    {0xb, 16, 15000, 2, 2},
    {0xc, 4, 1000, 2, 2},

    {0xc, 18, 15000, 4, 2},
    {0xc, 19, 15000, 4, 2},
    {0xc, 20, 15000, 4, 2},
    {0xf, 2, 20000, 4, 2},
    {0xf, 3, 20000, 4, 2},
    {0xf, 4, 20000, 4, 2},
    {0xf, 5, 20000, 4, 2},
    {0xf, 6, 20000, 4, 2},
    {0xb, 19, 15000, 4, 2},
    {0xc, 16, 40000, 4, 2},

    {0x2, 64, 100, 5, 2},
    {0x2, 71, 100, 5, 2},
    {0x2, 74, 100, 5, 2},
    {0x2, 77, 100, 5, 2},
    {0x2, 98, 100, 5, 2},
    {0x2, 100, 100, 5, 2},
    {0x3, 43, 100, 5, 2},
    {0x3, 59, 100, 5, 2},
    {0x3, 72, 100, 5, 2},
    {0x3, 77, 100, 5, 2},
    {0x3, 89, 100, 5, 2},
    {0x3, 106, 100, 5, 2},
    {0xb, 18, 1500, 5, 2},
    {0xc, 17, 2000, 5, 2},
};

const std::vector<std::vector<int>> PathData = {
    //  REFERENCE
    //  - UnlockType (GameStatsManager): Doesn't work for Icons >:v
    //  - IconType (GameManager)
    //  - Icon ID
    //  - Price in the shop
    //  - Page ID (For the mod)
    //  - Path ID

    {0x1, 0, 213, 25000, 6, 1},
    {0x9, 6, 49, 25000, 6, 2},
    {0x1, 0, 274, 25000, 6, 3},
    {0x1, 0, 188, 25000, 6, 4},
    {0x4, 1, 60, 25000, 6, 5},
    {0x1, 0, 227, 25000, 6, 6},
    {0x1, 0, 431, 25000, 6, 7},
    {0x1, 0, 217, 25000, 6, 8},
    {0x5, 2, 93, 25000, 6, 9},
    {0x1, 0, 172, 25000, 6, 10},
};