#ifndef __PLAY_FIELD_CONTROLLER_H__
#define __PLAY_FIELD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include <vector>
#include <functional>


// 职责：管理主牌区的逻辑和视图。
class PlayFieldController {
public:
    PlayFieldController();
    ~PlayFieldController();

    void init(GameModel* gameModel, cocos2d::Node* viewContainer);
    void createCardViews();

    // 设置“主牌被点击”的回调
    void setOnCardSelectCallback(std::function<void(CardView*)> callback);

    // 从视图中移除某张牌
    void removeCardView(CardView* view);

    // 检查所有牌的遮挡关系，并更新显示状态
    void updateCardStates();
private:
    void onCardClicked(CardView* cardView);

    GameModel* _gameModel;
    cocos2d::Node* _viewContainer;
    std::vector<CardView*> _cardViews;

    std::function<void(CardView*)> _onCardSelectCallback;
};

#endif // __PLAY_FIELD_CONTROLLER_H__
