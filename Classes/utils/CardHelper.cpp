#include "CardHelper.h"

std::string CardHelper::getCardBgImageName()
{
    return "card_general.png";
}

std::string CardHelper::getSuitImageName(CardSuitType suit)
{
    switch (suit) {
    case CardSuitType::CST_CLUBS:    return "suits/club.png";
    case CardSuitType::CST_DIAMONDS: return "suits/diamond.png";
    case CardSuitType::CST_HEARTS:   return "suits/heart.png";
    case CardSuitType::CST_SPADES:   return "suits/spade.png";
    default: return "";
    }
}

std::string CardHelper::getNumberImageName(CardFaceType face, CardSuitType suit, bool isBig)
{
    // 1. 拼尺寸前缀
    std::string sizeStr = isBig ? "big_" : "small_";

    // 2. 拼颜色前缀
    std::string colorStr = "black_";
    if (suit == CardSuitType::CST_DIAMONDS || suit == CardSuitType::CST_HEARTS) {
        colorStr = "red_";
    }

    // 3. 拼点数
    std::string faceStr = getFaceString(face);

    // 拼最终路径: number/small_red_A.png
    return "number/" + sizeStr + colorStr + faceStr + ".png";
}


std::string CardHelper::getFaceString(CardFaceType face)
{
    switch (face) {
    case CardFaceType::CFT_ACE:   return "A";
    case CardFaceType::CFT_TWO:   return "2";
    case CardFaceType::CFT_THREE: return "3";
    case CardFaceType::CFT_FOUR:  return "4";
    case CardFaceType::CFT_FIVE:  return "5";
    case CardFaceType::CFT_SIX:   return "6";
    case CardFaceType::CFT_SEVEN: return "7";
    case CardFaceType::CFT_EIGHT: return "8";
    case CardFaceType::CFT_NINE:  return "9";
    case CardFaceType::CFT_TEN:   return "10";
    case CardFaceType::CFT_JACK:  return "J";
    case CardFaceType::CFT_QUEEN: return "Q";
    case CardFaceType::CFT_KING:  return "K";
    default: return "";
    }
}
