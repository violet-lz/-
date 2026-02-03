#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include <functional>

class StackController {
public:
    StackController();
    ~StackController();

    void init(GameModel* gameModel, cocos2d::Node* viewContainer);
    void refreshView();

    void setOnDrawPileClickCallback(std::function<void()> callback);
    void setOnUndoCallback(std::function<void()> callback);

    // --- 万能牌点击回调 ---
    void setOnJokerClickCallback(std::function<void()> callback);

private:
    void onDrawPileClicked();
    void onJokerClicked(); // 内部处理点击动画

    GameModel* _gameModel;
    cocos2d::Node* _viewContainer;

    cocos2d::Sprite* _drawPileSprite;
    CardView* _topStackCardView;

    // --- 万能牌视图 ---
    CardView* _jokerPileView;

    std::function<void()> _onDrawPileClickCallback;
    std::function<void()> _onUndoCallback;
    std::function<void()> _onJokerClickCallback;
};

#endif // __STACK_CONTROLLER_H__
