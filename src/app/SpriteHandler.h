#pragma once
#include <SFML/Graphics.hpp>
#include "MoveTable.h"
#include <vector>

class SpriteHandler //this class does not handle boardSprite, which is handled by main.cpp instead
{
    private:
    static const int moveDotRadius = 15;
    static const sf::Color moveDotColor;
    static const sf::Color promotionMenuColor;
    static std::vector<sf::CircleShape> debugShapes;
    static const int FONT_SIZE_A=26;
    static sf::Font FontA;
    static sf::Text CurrentEvaluationText;

    public:
    static sf::IntRect pieceTextureRect[12];
    static std::vector<sf::Sprite> pieceSprite;
    static std::vector<sf::CircleShape> dots;
    static sf::Sprite promotionPieceSprite[8];
    static sf::RectangleShape promotionMenu;
    static int pieceNum;
    static bool IsPromotion;

    static void LoadPieceSprites(std::vector<sf::Sprite> &Sprite, sf::Texture &texture); //here we set pieceTextureRect and texture file for pieces
    static void SelectPieceTexture(int spriteIndex, int targetPieceType, int targetPieceColor); //selects texture rect according to the piece
    static void SetupBoard(sf::Sprite pieceSprite[],int width, int height); //obsolete
    static void DrawPieces(sf::RenderWindow &window); //draws each piece
    static void DrawPromotionMenu(sf::RenderWindow &window);
    static void DrawDot(int square);
    static void DrawMoveDots(int square, std::list<Move> moveList); //draws dots at the center of each square, where a move can be made (after piece selection)
    static void RemoveMoveDots();
    static void DrawDots(sf::RenderWindow &window);
    static void ShowPromotionMenu(int square);
    static void InitializePromotionMenu();
    static void HidePromotionMenu(int type, int color);
    static void DrawDebug(std::list<int> feature, sf::Color color, int offset);
    static void DrawDebugShapes(sf::RenderWindow &window);
    static void ClearDebug();
    static void DisplayCurrentEvaluation();
    static void DrawTexts(sf::RenderWindow &window);
    static void LoadFonts(int posX, int posY, std::string dir);
};