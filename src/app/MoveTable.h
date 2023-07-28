#pragma once
#include <vector>
#include <list>

struct Move
{
    int startSquare;
    int targetSquare;

    Move(int start, int end)
    {
        startSquare = start;
        targetSquare = end;
    }
};

class MoveTable
{
    private:
    static const short directionShift[8]; //N, NE, E, SE, S, SW, W, NW
    static const short knightShift[8];
    static std::list<int> pawnAttackList;

    public:
    //DIRECTIONS: N, NE, E, SE, S, SW, W, NW
    static short numSquaresToEdge[64][8]; //Number of squares to the edge from the i-th square in the j-th direction
    static std::vector<std::vector<short>> knightTargetSquares; //Indexes of squares a knight can jump to from the i-th square
    static std::vector<std::vector<short>> kingTargetSquares; //Indexes of squares a king can move to from the i-th square
    static std::list<Move> CurrentMoveList;
    static std::list<int> AttackList; //list of attacked squares; generated by GeneratePins(), which is called by Board::SwitchPlayer()
    static std::list<int> PinList; //list of squares with pieces pinned to their king 
    static std::list<int> PinDirectionList; //list of pin directions (pinned piece can move along the direction)
    static short enPassantSquare; //shows a square, where an en passant capture is possible (-1 if none; initally -1 or set by FEN)
    static bool W_CanCastleKingside;
    static bool B_CanCastleKingside;
    static bool W_CanCastleQueenside;
    static bool B_CanCastleQueenside;

    //Fill numSquaresToEdge
    static void CalculateStartMoveData();
    static std::list<Move> GenerateMoves();
    static void GenerateMoves(std::list<Move> &moveList);
    static void GenerateLongRangeMoves(int square, int pieceType, std::list<Move> &moveList);
    static void GenerateKnightMoves(int square, std::list<Move> &moveList);
    static void GeneratePawnMoves(int square, std::list<Move> &moveList);
    static void GenerateKingMoves(int square, std::list<Move> &moveList);
    static void GenerateAttacks();
    static void CheckForPins(int startSquare, int targetSquare);
    static bool IsLegal(int startSquare, int targetSquare);
    static bool IsEnPassant(int targetSquare);
    static bool IsTwoSquareAdvance(int startSquare, int targetSquare);
    static bool IsPinned(int square);
    static bool IsPinned(int square, int &pinDir);
    static bool IsAttacked(int square);
};