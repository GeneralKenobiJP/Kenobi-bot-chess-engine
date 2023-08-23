#pragma once
#include <vector>
#include <list>
#include "fenUtility.h"

struct Move
{
    int startSquare;
    int targetSquare;

    int promotionPiece;

    Move()
    {
        startSquare = 0;
        targetSquare = 0;
        promotionPiece = 0;
    }

    Move(int start, int end)
    {
        startSquare = start;
        targetSquare = end;
        promotionPiece = 0;
    }
    Move(int start, int end, int promotion)
    {
        startSquare = start;
        targetSquare = end;
        promotionPiece = promotion;
    }

    void LogMove();
};

struct Position
{
    FEN fen;
    int occurrenceNum;

    Position(FEN thisFen, int num)
    {
        fen = thisFen;
        occurrenceNum = num;
    }
};

class MoveTable
{
    private:
    static const short directionShift[8]; //N, NE, E, SE, S, SW, W, NW
    static const short knightShift[8];
    static std::vector<int> pawnAttackList;
    static std::vector<int> kingVirtualAttackList;

    public:
    //DIRECTIONS: N, NE, E, SE, S, SW, W, NW
    static short numSquaresToEdge[64][8]; //Number of squares to the edge from the i-th square in the j-th direction
    static std::vector<std::vector<short>> knightTargetSquares; //Indexes of squares a knight can jump to from the i-th square
    static std::vector<std::vector<short>> kingTargetSquares; //Indexes of squares a king can move to from the i-th square
    static std::vector<Move> CurrentMoveList;
    static std::vector<int> AttackList; //list of attacked squares; generated by GeneratePins(), which is called by Board::SwitchPlayer()
    static std::vector<int> PinList; //list of squares with pieces pinned to their king 
    static std::vector<int> PinDirectionList; //list of pin directions (pinned piece can move along the direction)
    static std::vector<int> VirtualAttackList; //list of squares behind the king (if checked) which would be attacked but for the king OR squares theoretically attacked by the king, but protected (king cannot really move there)
    static std::vector<int> DefenseList; //list of squares occupied by pieces that are defended (an enemy king cannot take them)
    static bool IsChecked;
    static short KnightCheckNum;
    static int CheckingKnightSquare;
    static std::vector<std::vector<int>> CheckSquares; //array of lists of squares along which the check is carried out (checking piece + squares where a check can be blocked)
    static short enPassantSquare; //shows a square, where an en passant capture is possible (-1 if none; initally -1 or set by FEN)
    static bool W_CanCastleKingside;
    static bool B_CanCastleKingside;
    static bool W_CanCastleQueenside;
    static bool B_CanCastleQueenside;

    //Draw
    static int consecutiveMoves; //if =100, any player can declare a draw; if =150, there is an automatic draw
    static std::vector<Position> occurredPositions;
    static bool IsThreefoldRepetition;
    static bool IsFiftymove;

    //Fill numSquaresToEdge
    static void CalculateStartMoveData();
    static std::vector<Move> GenerateMoves();
    static void GenerateMoves(std::vector<Move> &moveList);
    static void GenerateLongRangeMoves(int square, int pieceType, std::vector<Move> &moveList);
    static void GenerateKnightMoves(int square, std::vector<Move> &moveList);
    static void GeneratePawnMoves(int square, std::vector<Move> &moveList);
    static void GenerateKingMoves(int square, std::vector<Move> &moveList);
    static void GenerateAttacks();
    static void CheckForPins(int startSquare, int targetSquare);
    static void CheckForKnightChecks(int targetSquare, int knightSquare);
    static void CheckForPawnChecks(int targetSquare);
    static void CheckState();
    static void AddCurrentPosition();
    static bool IsLegal(int startSquare, int targetSquare);
    static bool IsEnPassant(int targetSquare);
    static bool IsTwoSquareAdvance(int startSquare, int targetSquare);
    static bool IsPinned(int square);
    static bool IsPinned(int square, int &pinDir);
    static bool IsAttacked(int square);
    static bool IsVirtuallyAttacked(int square);
    static bool IsCoveringCheck(int square);
    static bool IsDefended(int targetSquare);
    static bool IsCastling(int startSquare, int targetSquare, bool &IsKingside);
    static bool IsSufficientMaterial();
};