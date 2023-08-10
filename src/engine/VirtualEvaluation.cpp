#include <VirtualEvaluation.h>
#include <piece.h>

VirtualBoard* VirtualEvaluation::board = nullptr;

void VirtualEvaluation::CountMaterial(int square, int &whiteVal, int &blackVal)
{
    int type;
    int color;

    Piece::ReadPiece(board->squareState[square], type, color);

    switch(type)
    {
        case Piece::none:
            break;
        case Piece::pawn:
            if(color == 8)
                whiteVal+=VirtualEvaluation::pawnValue;
            else
                blackVal+=VirtualEvaluation::pawnValue;
            break;
        case Piece::knight:
            if(color == 8)
                whiteVal+=VirtualEvaluation::knightValue;
            else
                blackVal+=VirtualEvaluation::knightValue;
            break;
        case Piece::bishop:
            if(color == 8)
                whiteVal+=VirtualEvaluation::bishopValue;
            else
                blackVal+=VirtualEvaluation::bishopValue;
            break;
        case Piece::rook:
            if(color == 8)
                whiteVal+=VirtualEvaluation::rookValue;
            else
                blackVal+=VirtualEvaluation::rookValue;
            break;
        case Piece::queen:
            if(color == 8)
                whiteVal+=VirtualEvaluation::queenValue;
            else
                blackVal+=VirtualEvaluation::queenValue;
            break;
    }
}

int VirtualEvaluation::Evaluate()
{
    int whiteValue=0;
    int blackValue=0;
    int value;

    for(int i=0;i<64;i++)
    {
        VirtualEvaluation::CountMaterial(i, whiteValue, blackValue);
    }

    value = whiteValue - blackValue;
    
    return value;
}

void VirtualEvaluation::EvaluateSides(int &whiteValue, int &blackValue, bool &IsTherePawn)
{
    whiteValue=0;
    blackValue=0;
    int value;

    for(int i=0;i<64;i++)
    {
        VirtualEvaluation::CountMaterial(i, whiteValue, blackValue);
        if(board->squareState[i]==Piece::pawn)
            IsTherePawn = true;
    }
}