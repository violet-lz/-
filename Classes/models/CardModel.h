#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "configs/GameDefines.h"


 // 职责：扑克牌的数据本体。存花色、点数、坐标这些。
 // 场景：游戏里每张牌都是一个这玩意儿。
class CardModel {
public:
    CardModel();
    ~CardModel();

    // 初始化牌的点数花色
    void init(CardFaceType face, CardSuitType suit);

    CardFaceType getFace() const { return _face; }
    CardSuitType getSuit() const { return _suit; }

    // 设置json里的坐标
    void setPosition(float x, float y) { _x = x; _y = y; }
    float getX() const { return _x; }
    float getY() const { return _y; }

    // 设个唯一id，好找
    void setUid(int id) { _uid = id; }
    int getUid() const { return _uid; }

    // 设置牌是正面还是背面
    void setFaceUp(bool isFaceUp) { _isFaceUp = isFaceUp; }
    bool isFaceUp() const { return _isFaceUp; }

    // 是不是万能牌
    bool isJoker() const { return _face == CardFaceType::CFT_JOKER; }

private:
    CardFaceType _face; // 点数
    CardSuitType _suit; // 花色
    int _uid;           // 唯一id
    float _x;           // 关卡数据里的x坐标
    float _y;           // 关卡数据里的y坐标
    bool _isFaceUp;     // 正面还是背面
};

#endif // __CARD_MODEL_H__
