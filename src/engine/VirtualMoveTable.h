#pragma once
#include <vector>
//#include <list>
#include "MoveTable.h"
//#include "VirtualEvaluation.h"
//#include "VirtualBoard.h"
//#include "fenUtility.h"

class VirtualMoveTable
{
    private:
    static const short directionShift[8]; //N, NE, E, SE, S, SW, W, NW
    static const short knightShift[8];
    static std::vector<int> pawnAttackList;
    static std::vector<int> kingVirtualAttackList;

    static void ReadSquare(int squareIndex, int &file, int &rank);

    public:
    //DIRECTIONS: N, NE, E, SE, S, SW, W, NW
    static short numSquaresToEdge[64][8]; //Number of squares to the edge from the i-th square in the j-th direction
    static std::vector<std::vector<short>> knightTargetSquares; //Indexes of squares a knight can jump to from the i-th square
    static std::vector<std::vector<short>> kingTargetSquares; //Indexes of squares a king can move to from the i-th square
    std::vector<Move> CurrentMoveList;
    std::vector<int> AttackList; //list of attacked squares; generated by GeneratePins(), which is called by Board::SwitchPlayer()
    std::vector<int> PinList; //list of squares with pieces pinned to their king 
    std::vector<int> PinDirectionList; //list of pin directions (pinned piece can move along the direction)
    std::vector<int> VirtualAttackList; //list of squares behind the king (if checked) which would be attacked but for the king OR squares theoretically attacked by the king, but protected (king cannot really move there)
    std::vector<int> DefenseList; //list of squares occupied by pieces that are defended (an enemy king cannot take them)
    bool IsChecked;
    short KnightCheckNum;
    int CheckingKnightSquare;
    std::vector<std::vector<int>> CheckSquares; //array of lists of squares along which the check is carried out (checking piece + squares where a check can be blocked)
    short enPassantSquare; //shows a square, where an en passant capture is possible (-1 if none; initally -1 or set by FEN)
    bool W_CanCastleKingside;
    bool B_CanCastleKingside;
    bool W_CanCastleQueenside;
    bool B_CanCastleQueenside;

    //Draw
    int consecutiveMoves; //if =100, any player can declare a draw; if =150, there is an automatic draw
    std::list<Position> occurredPositions;
    bool IsThreefoldRepetition;
    bool IsFiftymove;

    //pointers to the board
    unsigned short *activePlayer;
    int *squareState[64];

    //Fill numSquaresToEdge
    static void CalculateStartMoveData();
    std::vector<Move> GenerateMoves();
    void GenerateMoves(std::vector<Move> &moveList);
    void GenerateLongRangeMoves(int square, int pieceType, std::vector<Move> &moveList);
    void GenerateKnightMoves(int square, std::vector<Move> &moveList);
    void GeneratePawnMoves(int square, std::vector<Move> &moveList);
    void GenerateKingMoves(int square, std::vector<Move> &moveList);
    void GenerateAttacks();
    void CheckForPins(int startSquare, int targetSquare);
    void CheckForKnightChecks(int targetSquare, int knightSquare);
    void CheckForPawnChecks(int targetSquare);
    //void CheckState();// migrated to virtualevaluation
    void AddCurrentPosition();
    void AddCurrentPosition(std::vector<std::list<Position>::iterator> &iterList);
    void RemovePosition(std::list<Position>::iterator it);
    //void RemovePosition(int index);
    static int ReadPromotionPieceFromIndex(int index, int color);
    bool IsLegal(int startSquare, int targetSquare);
    bool IsEnPassant(int targetSquare);
    bool IsTwoSquareAdvance(int startSquare, int targetSquare);
    bool IsPinned(int square);
    bool IsPinned(int square, int &pinDir);
    bool IsAttacked(int square);
    bool IsVirtuallyAttacked(int square);
    bool IsCoveringCheck(int square);
    bool IsDefended(int targetSquare);
    bool IsCastling(int startSquare, int targetSquare, bool &IsKingside);
    bool IsCastling(int startSquare, int targetSquare, int color, bool &IsKingside);
    bool IsCastling(int startSquare, int targetSquare);
    bool IsSufficientMaterial();

    void LogMoveList();
};