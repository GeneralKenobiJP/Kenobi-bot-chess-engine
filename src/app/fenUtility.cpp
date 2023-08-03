#include "fenUtility.h"
#include <ctype.h>
#include <iostream>
#include "board.h"
#include <MoveTable.h>

//const std::string FEN::startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
//const std::string FEN::startFEN = "rnbqkbnr/pppp1ppp/4p3/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const std::string FEN::startFEN = "8/1k6/3p4/p1p1p2p/P1PPP1pP/6P1/5K2/8 w - 0 1";
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
         std::cout << fenTxt << std::endl;
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
                std::cout << "finito" << std::endl;
                //fenTxt = fenTxt.substr(1);
                break;
            }
            //continue;
        }
        if(fenTxt[0]=='/')
            continue;
        currentSquare = currentRank * 8 + currentFile;
        std::cout << currentSquare << std::endl;
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

    //std::cout << fenTxt << std::endl;

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
        default:
            fenTxt = "  " + fenTxt;
            break;
        }
        fenTxt = fenTxt.substr(1);
    }
    fenTxt = fenTxt.substr(1);

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

    thisString = "";

    while (fenTxt[0] != ' ')
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

const std::string FEN::CutHalfmoves() const
{
    return FENtext.substr(0,FENtext.length()-3);
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