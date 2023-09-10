#include "fenUtility.h"
#include <ctype.h>
#include <iostream>
#include "board.h"
#include <MoveTable.h>

//const std::string FEN::startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0"; //INITIAL POSITION
//const std::string FEN::startFEN = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"; //POSITION 2
const std::string FEN::startFEN = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0"; //POSITION 3
//const std::string FEN::startFEN = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"; //POSITION 4
//const std::string FEN::startFEN = "r3k2r/Pppp1ppp/1b3nbN/nPB5/B1P1P3/q4N2/Pp1P2PP/R2Q1RK1 b kq - 0 1"; //POSITION 4 Move 2 after Bc5
//const std::string FEN::startFEN = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"; //POSITION 5
//const std::string FEN::startFEN = "1k6/8/8/8/8/8/8/Q3K2R w KQ - 0 1"; //Castling check
//const std::string FEN::startFEN = "rnbqkbnr/pppp1ppp/4p3/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//const std::string FEN::startFEN = "8/1k6/3p4/p1p1p2p/P1PPP1pP/6P1/5K2/8 w - 0 1";
//const std::string FEN::startFEN = "1k7/8/8/8/8/8/8/8 w - 0 1";

int FEN::ReadLetter(char letter) // lowercase
{
    switch (letter)
    {
    case 'r':
        return Piece::rook;
    case 'n':
        return Piece::knight;
    case 'b':
        return Piece::bishop;
    case 'q':
        return Piece::queen;
    case 'k':
        return Piece::king;
    case 'p':
        return Piece::pawn;
    }
    return Piece::none;
}

void FEN::ReadPosition() // non-static
{
    FEN::ReadPosition(FENtext);
}

void FEN::ReadPosition(std::string fenTxt) // static
{
    int currentFile = 0;
    int currentRank = 7; // 8 actually
    int currentSquare;

    // int debugNum = 0;

    while (fenTxt[0] != ' ')
    {
        //std::cout << fenTxt << std::endl;
        // debugNum++;

        // std::cout << "Debug: " << debugNum << std::endl;

        if (fenTxt[0] == '/')
        {
            currentFile = 0;
            currentRank -= 1;
            fenTxt = fenTxt.substr(1, fenTxt.length() - 1);
            continue;
        }
        if (isdigit(fenTxt[0]))
        {
            currentFile += (int)fenTxt[0]-48;
            fenTxt = fenTxt.substr(1, fenTxt.length() - 1);
            if(fenTxt[0] == ' ')
            {
                //std::cout << "finito" << std::endl;
                //fenTxt = fenTxt.substr(1);
                break;
            }
            //continue;
        }
        if(fenTxt[0]=='/')
            continue;
        currentSquare = currentRank * 8 + currentFile;
        //std::cout << currentSquare << std::endl;
        if (fenTxt[0] > 90) // checks if lowercase
        {
            Piece::SetPiece(ReadLetter(fenTxt[0]), Piece::black, currentSquare);
            fenTxt = fenTxt.substr(1, fenTxt.length() - 1);
            currentFile++;
            continue;
        }
        // uppercase here
        Piece::SetPiece(ReadLetter(fenTxt[0] + 32), Piece::white, currentSquare);
        fenTxt = fenTxt.substr(1, fenTxt.length() - 1);
        currentFile++;
    }

    if (fenTxt[1] == 'w')
    {
        Board::activePlayer = 1;
    }
    else //'b'
    {
        Board::activePlayer = 2;
    }

    fenTxt = fenTxt.substr(3);

    std::cout << fenTxt << std::endl;

    while (fenTxt[0] != ' ')
    {
        switch (fenTxt[0])
        {
        case 'K':
            MoveTable::W_CanCastleKingside = 1;
            break;
        case 'Q':
            MoveTable::W_CanCastleQueenside = 1;
            break;
        case 'k':
            MoveTable::B_CanCastleKingside = 1;
            break;
        case 'q':
            MoveTable::B_CanCastleQueenside = 1;
            break;
        case '-':
            break;
        default:
            fenTxt = "  " + fenTxt;
            break;
        }
        fenTxt = fenTxt.substr(1);
        std::cout << fenTxt << std::endl;
    }
    std::cout << fenTxt << std::endl;
    fenTxt = fenTxt.substr(1);
    std::cout << fenTxt << std::endl;

    std::string thisString = "";

    if (fenTxt[0] == '-')
    {
        MoveTable::enPassantSquare = -1;
        fenTxt = fenTxt.substr(2);
    }
    else
    {
        //fenTxt = fenTxt.substr(1);
        while (fenTxt[0] != ' ')
        {
            thisString += fenTxt[0];
            fenTxt = fenTxt.substr(1);
        }
        MoveTable::enPassantSquare = (short)std::stoi(thisString);
        fenTxt = fenTxt.substr(1);
    }

    std::cout << fenTxt << std::endl;

    thisString = "";

    while (fenTxt[0] != ' ' && !fenTxt.empty())
    {
        thisString += fenTxt[0];
        fenTxt = fenTxt.substr(1);
    }
    MoveTable::consecutiveMoves = std::stoi(thisString);
    //

    // std::cout << "done" << std::endl;
}

void FEN::GetPosition()
{
    std::string thisFen = "";
    int color;
    int type;
    int square = 56;
    int emptyNum;
    int colorChar;

    for (int rank = 7; rank >= 0; rank--)
    {
        emptyNum = 0;
        for (int file = 0; file <= 7; file++)
        {
            Piece::ReadPiece(Board::squareState[square], type, color);
            square++;

            colorChar = (color - 8) * 4;

            if (type == 0)
            {
                emptyNum++;
                continue;
            }

            if (emptyNum > 0)
            {
                thisFen += std::to_string(emptyNum);
                emptyNum = 0;
            }

            switch (type)
            {
            case Piece::pawn:
                thisFen += ('P' + colorChar);
                continue;
            case Piece::knight:
                thisFen += ('N' + colorChar);
                continue;
            case Piece::bishop:
                thisFen += ('B' + colorChar);
                continue;
            case Piece::rook:
                thisFen += ('R' + colorChar);
                continue;
            case Piece::queen:
                thisFen += ('Q' + colorChar);
                continue;
            case Piece::king:
                thisFen += ('K' + colorChar);
                continue;
            }
        }
        if (emptyNum > 0)
        {
            thisFen += std::to_string(emptyNum);
            emptyNum = 0;
        }
        if (rank > 0)
            thisFen += '/';
        square -= 16;
    }

    if (Board::activePlayer == 1)
        thisFen += " w ";
    else // 2
        thisFen += " b ";

    if (MoveTable::W_CanCastleKingside)
        thisFen += 'K';
    if (MoveTable::W_CanCastleQueenside)
        thisFen += 'Q';
    if (MoveTable::B_CanCastleKingside)
        thisFen += 'k';
    if (MoveTable::B_CanCastleQueenside)
        thisFen += 'q';

    if (thisFen[thisFen.length() - 1] != ' ')
        thisFen += ' ';

    if (MoveTable::enPassantSquare == -1)
        thisFen += "- ";
    else
    {
        thisFen += std::to_string(MoveTable::enPassantSquare);
        thisFen += ' ';
    }

    thisFen += std::to_string(MoveTable::consecutiveMoves);

    FENtext = thisFen;

    // fullmove counter is useless, so did not implement
}
void FEN::GetPosition(int squareState[64], short activePlayer, bool W_K, bool W_Q, bool B_K, bool B_Q, int enPassantSquare, int consecutiveMoves)
{
    std::string thisFen = "";
    int color;
    int type;
    int square = 56;
    int emptyNum;
    int colorChar;

    for (int rank = 7; rank >= 0; rank--)
    {
        emptyNum = 0;
        for (int file = 0; file <= 7; file++)
        {
            Piece::ReadPiece(squareState[square], type, color);
            square++;

            colorChar = (color - 8) * 4;

            if (type == 0)
            {
                emptyNum++;
                continue;
            }

            if (emptyNum > 0)
            {
                thisFen += std::to_string(emptyNum);
                emptyNum = 0;
            }

            switch (type)
            {
            case Piece::pawn:
                thisFen += ('P' + colorChar);
                continue;
            case Piece::knight:
                thisFen += ('N' + colorChar);
                continue;
            case Piece::bishop:
                thisFen += ('B' + colorChar);
                continue;
            case Piece::rook:
                thisFen += ('R' + colorChar);
                continue;
            case Piece::queen:
                thisFen += ('Q' + colorChar);
                continue;
            case Piece::king:
                thisFen += ('K' + colorChar);
                continue;
            }
        }
        if (emptyNum > 0)
        {
            thisFen += std::to_string(emptyNum);
            emptyNum = 0;
        }
        if (rank > 0)
            thisFen += '/';
        square -= 16;
    }

    if (activePlayer == 1)
        thisFen += " w ";
    else // 2
        thisFen += " b ";

    if (W_K)
        thisFen += 'K';
    if (W_Q)
        thisFen += 'Q';
    if (B_K)
        thisFen += 'k';
    if (B_Q)
        thisFen += 'q';

    if (thisFen[thisFen.length() - 1] != ' ')
        thisFen += ' ';

    if (enPassantSquare == -1)
        thisFen += "- ";
    else
    {
        thisFen += std::to_string(enPassantSquare);
        thisFen += ' ';
    }

    thisFen += std::to_string(consecutiveMoves);

    FENtext = thisFen;

    // fullmove counter is useless, so did not implement
}

void FEN::ReadContext(bool &W_K, bool &W_Q, bool &B_K, bool &B_Q, short &enPassantSquare, int &consecutiveMoves)
{
    //std::cout << "Been called, chief?" << std::endl;
    //std::cout << FENtext << std::endl;
    //std::cout << "I push my fingers" << std::endl;
    std::string fenTxt(FENtext);
    //std::cout << fenTxt << std::endl;
    int index = fenTxt.find("w");

    if(index == std::string::npos)
        index = fenTxt.rfind("b");

    //std::cout << "we are before the sus" << std::endl;
    //std::cout << index << std::endl;

    fenTxt = fenTxt.substr(index+2);

    //std::cout << "got past the sus" << std::endl;

    W_K = 0;
    W_Q = 0;
    B_K = 0;
    B_Q = 0;

    while (fenTxt[0] != ' ')
    {
        switch (fenTxt[0])
        {
        case 'K':
            W_K = 1;
            break;
        case 'Q':
            W_Q = 1;
            break;
        case 'k':
            B_K = 1;
            break;
        case 'q':
            B_Q = 1;
            break;
        default:
            fenTxt = "  " + fenTxt;
            break;
        }
        fenTxt = fenTxt.substr(1);
    }
    fenTxt = fenTxt.substr(1);

    //std::cout << "Woah, we're halfway there, oo-ooh livin' on a prayer" << std::endl;

    std::string thisString = "";

    if (fenTxt[0] == '-')
    {
        enPassantSquare = -1;
        fenTxt = fenTxt.substr(2);
    }
    else
    {
        //fenTxt = fenTxt.substr(1);
        while (fenTxt[0] != ' ')
        {
            thisString += fenTxt[0];
            fenTxt = fenTxt.substr(1);
        }
        enPassantSquare = (short)std::stoi(thisString);
        fenTxt = fenTxt.substr(1);
    }

    //std::cout << "Gotchya en-passanted" << std::endl;

    //std::cout << fenTxt <<std::endl;

    thisString = "";

    while (fenTxt[0] != ' ')
    {
        thisString += fenTxt[0];
        if(fenTxt.size()==1)
            break;
        fenTxt = fenTxt.substr(1);
    }

    //std::cout << "Almost done, eh?" << std::endl;

    consecutiveMoves = std::stoi(thisString);

    //std::cout << "It's done, chief" << std::endl;
}

const std::string FEN::CutHalfmoves() const
{
    int halfMoveLength = 0;
    int i = FENtext.size()-1;
    while(FENtext[i]!=' ')
    {
        halfMoveLength++;
        i--;
    }

    return FENtext.substr(0,FENtext.length()-2-halfMoveLength);
}

bool FEN::operator==(const FEN &f)
{
    //std::cout << CutHalfmoves() << std::endl;
    //std::cout << f.CutHalfmoves() << std::endl;
    if(CutHalfmoves() == f.CutHalfmoves())
    {
        //std::cout << "true" << std::endl;
        return true;
    }
    else
        return false;
}

int FEN::ReadConsecutiveMoves()
{
    std::string conMovesString;

    int halfMoveLength = 0;
    int i = FENtext.size()-1;
    while(FENtext[i]!=' ')
    {
        halfMoveLength++;
        i--;
    }

    conMovesString = FENtext.substr(FENtext.size()-1-halfMoveLength);

    return std::stoi(conMovesString);
}