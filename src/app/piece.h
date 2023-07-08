#pragma once

class Piece
{
    public:
    static const int none = 0;
    static const int king = 1;
    static const int pawn = 2;
    static const int knight = 3;
    static const int bishop = 4;
    static const int rook = 5;
    static const int queen = 6;

    static const int white = 8;
    static const int black = 16;

    static void ReadPiece(int num);
};