#include <Evaluation.h>
#include <board.h>
#include <piece.h>

void Evaluation::CountMaterial(int square, int &whiteVal, int &blackVal)
{
    int type;
    int color;

    Piece::ReadPiece(Board::squareState[square], type, color);

    switch(type)
    {
        case Piece::none:
            break;
        case Piece::pawn:
            if(color == 8)
                whiteVal+=Evaluation::pawnValue;
            else
                blackVal+=Evaluation::pawnValue;
            break;
        case Piece::knight:
            if(color == 8)
                whiteVal+=Evaluation::knightValue;
            else
                blackVal+=Evaluation::knightValue;
            break;
        case Piece::bishop:
            if(color == 8)
                whiteVal+=Evaluation::bishopValue;
            else
                blackVal+=Evaluation::bishopValue;
            break;
        case Piece::rook:
            if(color == 8)
                whiteVal+=Evaluation::rookValue;
            else
                blackVal+=Evaluation::rookValue;
            break;
        case Piece::queen:
            if(color == 8)
                whiteVal+=Evaluation::queenValue;
            else
                blackVal+=Evaluation::queenValue;
            break;
    }
}

int Evaluation::Evaluate()
{
    int whiteValue;
    int blackValue;
    int value;

    for(int i=0;i<64;i++)
    {
        Evaluation::CountMaterial(i, whiteValue, blackValue);
    }

    value = whiteValue - blackValue;
    
    return value;
}