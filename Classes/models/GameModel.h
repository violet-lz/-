#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <vector>
#include <algorithm>
#include "CardModel.h"



// 职责：游戏的核心数据。所有牌堆都在这儿管着。
// 场景：整个游戏逻辑都围绕这个类的数据转。
class GameModel {
public:
    GameModel();
    ~GameModel();

    // --- 场上的牌 ---
    void addCardToPlayField(CardModel* card);    // 往场上加牌
    void removeCardFromPlayField(CardModel* card); // 从场上拿走牌
    const std::vector<CardModel*>& getPlayFieldCards() const { return _playFieldCards; }

    // --- 底牌堆 ---
    void pushToStack(CardModel* card);        // 往底牌堆里压牌
    CardModel* getTopStackCard();             // 拿最上面的底牌
    void popFromStack();                      // 撤销时用，把顶上那张牌拿掉

    // --- 摸牌堆 ---
    void pushToDrawPile(CardModel* card);     // 往摸牌堆里塞牌
    CardModel* popFromDrawPile();             // 从摸牌堆抽一张
    bool isDrawPileEmpty() const;             // 摸牌堆空了没
    void pushBackToDrawPile(CardModel* card); // 撤销时用，把牌塞回摸牌堆

    // --- 万能牌堆 ---
    void pushToJokerPile(CardModel* card);    // 加个万能牌
    CardModel* popFromJokerPile();            // 用一个万能牌
    bool isJokerPileEmpty() const;            // 还有万能牌吗
    int getJokerPileSize() const;             // 还剩几个万能牌

    // 清空所有牌，重开一局用
    void clear();

private:
    std::vector<CardModel*> _playFieldCards; // 场上的牌
    std::vector<CardModel*> _stackCards;     // 底部的牌堆
    std::vector<CardModel*> _drawPileCards;  // 摸牌堆
    std::vector<CardModel*> _jokerPileCards; // 万能牌堆
};

#endif // __GAME_MODEL_H__
