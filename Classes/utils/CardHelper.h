#ifndef __CARD_HELPER_H__
#define __CARD_HELPER_H__

#include "configs/GameDefines.h"
#include <string>


// 职责：卡牌的工具类。根据牌的数据，生成对应的图片名。
//场景：创建卡牌视图的时候用。
class CardHelper {
public:
    // 拿牌的背景图
    static std::string getCardBgImageName();

    // 拿花色图
    static std::string getSuitImageName(CardSuitType suit);

    /**
     * 拿数字/字母图
     * @param face 点数
     * @param suit 花色，用来定颜色
     * @param isBig 大图还是小图
     */
    static std::string getNumberImageName(CardFaceType face, CardSuitType suit, bool isBig);

private:
    // 内部用，把点数转成"A","2"这种字符串
    static std::string getFaceString(CardFaceType face);
};

#endif // __CARD_HELPER_H__
