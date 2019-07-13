/*
1小星星
2两只老虎
3隐形的翅膀
4童话镇
*/
int musicnum[][1000] = {{0, 1,500, 1,500, 5,500, 5,500, 6,500, 6,500, 5,500, -1,500,
     4,500, 4,500, 3,500, 3,500, 2,500, 2,500, 1,500, -1,500,
     5,500, 5,500, 4,500, 4,500, 3,500, 3,500, 2,500, -1,500,
     5,500, 5,500, 4,500, 4,500, 3,500, 3,500, 2,500, -1,500,
     1,500, 1,500, 5,500, 5,500, 6,500, 6,500, 5,500, -1,500,
     4,500, 4,500, 3,500, 3,500, 2,500, 2,500, 1,500, -1},
    {0, 1,400, 2,400, 3,400, 1,400, -1,400,
     1,400, 2,400, 3,400, 1,400, -1,400,
     3,400, 4,400, 5,400, -1,400,
     3,400, 4,400, 5,400, -1,400,
     5,400, 6,400, 5,400, 4,400, 3,400, 1,400, -1,400,
     5,400, 6,400, 5,400, 4,400, 3,400, 1,400, -1,400,
     2,400, 5,400, 1,400, -1,400,
     2,400, 5,400, 1,400, -1},
    {0, 1,350, 4,350, 6,350, -1,500,
     8,350, 6,350, -1,500,
     5,350, 4,350, 4,350, 4,350, -1,500,
     4,350, 2,350, 1,350, 1,350, -1,500,
     1,350, 4,350, 6,350, -1,500,
     8,350, 8,350, 8,350, -1,500,
     9,350, 8,350, -1,500,
     8,350, 5,350, 6,350, 5,350, -1,500,
     4,350, 5,350, 5,350, -1,500,
     9,350, 8,350, 6,350, -1,500,
     8,350, 8,350, 8,350, 9,350, 8,350, -1,500,
     6,350, 5,350, 4,350, 5,350, 2,350, -1,500,
     1,350, 2,350, 4,350, -1,500,
     6,350, 5,350, -1,500,
     6,350, 4,350, -1,500,
     6,350, 8,350, 11,350, -1,500,
     11,350, 10,350, -1,500,
     9,350, 8,350, 9,350, 11,350, -1,500,
     6,350, 5,350, 4,350, -1,500,
     4,350, 4,350, -1,500,
     4,350, 11,350, -1,500,
     9,350, 8,350, -1,500,
     6,350, 5,350, -1,500,
     4,350, 5,350, 5 - 1,350,
     6,350, 8,350, 11,350, -1,500,
     11,350, 10,350, -1,500,
     9,350, 8,350, 9,350, 11,350, -1,500,
     6,350, 5,350, 4,350, -1,500,
     4,350, 4,350, -1,500,
     4,350, 11,350, -1,500,
     9,350, 8,350, -1,500,
     6,350, 5,350, -1,500,
     5,350, 4,350, -1,500,
     4,350, -1},
    {0, 3, 250, 4, 250, 3, 750, 2, 2, -1, 500,
     3, 250, 4, 250, 3, 750, 2, 750, 1, 500, -2, 1000,
     3, 250, 4, 250, 3, 750, 2, 2, -1, 500,
     3, 250, 4, 250, 5, 1000, -1, 500,
     4, 500, 3, 1000, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 6, 250, 5, 1250, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 3, 250, -2, 1250, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 6, 250, 8, 1250, -1, 500,
     6, 250, 6, 250, 5, 250, 4, 250, 5, 250, 4, 250, 5, 500, 4, 250, 3, 1250, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 6, 250, 5, 1250, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 3, 250, -2, 1250, -1, 500,
     2, 250, 3, 250, 4, 250, 3, 250, 4, 250, 3, 250, 2, 500, 6, 250, 8, 1250, -1, 250,
     6, 250, 6, 250, 8, 250, 7, 250, 7, 250, 7, 250, 7, 500, 6, 250, 7, 250, 7, 500, 6, 750, -1, 250,
     6, 250, 6, 250, 9, 250, 8, 250, 9, 250, 8, 250, 7, 500, 9, 250, 8, 1250, -1, 250,
     6, 250, 6, 250, 6, 250, 7, 250, 7, 250, 7, 250, 6, 250, 7, 250, 6, 250, 5, 250, 7, 500, 6, 750, -1, 250,
     6, 250, 6, 250, 9, 250, 8, 250, 9, 250, 8, 250, 7, 500, 9, 250, 8, 1250, -1, 250,
     6, 250, 6, 250, 5, 500, 5, 500, 5, 500, 4, 500, 3, 1000, 6, 500, 5, 500,
     4, 500, 2, 500, 4, 500, 5, 500, 6, 500, 8, 500, 6, 500, 5, 500,
     4, 500, 2, 500, 2, 500, 1, 500, -2, 1000, -1, 500,
     4, 250, 3, 250, 2, 500, 1, 500, 2, 500, 4, 500, 4, 500, 8, 500, 6, 500, 6, 250, 5, 250,
     4, 500, 4, 500, 4, 500, 2, 500, 3, 1000, -1, 500,
     6, 250, 5, 250, 6, 500, 5, 500, 6, 500, 8, 500, 9, 500, 8, 500, 8, 250, 8, 500, 6, 250,
     9, 250, 8, 500, 7, 250, 8, 250, 7, 250, 8, 250, 7, 250, 6, 500, -1, 500,
     5, 250, 6, 250, 7, 500, 6, 500, 5, 500, 2, 250, 6, 500, 5, 250, 4, 500, 4, 250, 4, 250, 5, 250, 3, 250, 3, 500, 3, 250, 3, 250,
     3, 500, 2, 500, 3, 750, 6, 500, 5, 500, 4, 500, 2, 500, 4, 500, 5, 500, 6, 500, 8, 500, 6, 500, 5, 500, 4, 500,
     2, 500, 2, 500, 1, 500, -2, 1000, -1, 500,
     4, 250, 3, 250, 2, 500, 1, 500, 2, 500, 4, 500, 4, 500, 8, 500, 6, 500, 6, 250, 5, 250, -1, 250,
     4, 250, 5, 250, 4, 250, 5, 250, 4, 250, 2, 250, -1, 250,
     3, 1000, 6, 250, 5, 250, 6, 500, 5, 500, 6, 500, 8, 500, 9, 500, 8, 500, 8, 250, 8, 500, 6, 250, 9, 250, 8, 500, 7, 250, 8, 250, 7, 250, 8, 250, 7, 250, 6, 500, -1, 500,
     5, 250, 6, 250, 7, 500,
     6, 500, 5, 500, 2, 250, 6, 500, 5, 250, 4, 500, 4, 250, 4, 250, 5, 250, 3, 250, 3, 500, 3, 250, 3, 250, 3, 500, 2, 500, 3, 750, 1, 500, 3, 500, 2, 1000, 4, 500, 5, 500, 6, 1000, -1, 500,
     6, 250, 8, 250, 9, 250, 8, 250,
     6, 250, 8, 250, 8, 250, 6, 250, 5, 250, 8, 250, 6, 250, 8, 250, 6, 1000, -1, 500,
     7, 750, 6, 250, 5, 500,
     7, 500, 6, 500, 5, 500, 4, 1000, 3, 750, 2, 250, 3, 500, 4, 500, 3, 2}};