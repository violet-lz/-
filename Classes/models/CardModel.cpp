#include "CardModel.h"

CardModel::CardModel()
    : _face(CardFaceType::CFT_NONE)
    , _suit(CardSuitType::CST_NONE)
    , _uid(-1)
    , _x(0.0f)
    , _y(0.0f)
    , _isFaceUp(false) // 默认为牌是背面
{
}

CardModel::~CardModel()
{
}

void CardModel::init(CardFaceType face, CardSuitType suit)
{
    _face = face;
    _suit = suit;
}
