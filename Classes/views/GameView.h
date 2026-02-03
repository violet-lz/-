#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include <functional>

// 职责：游戏主场景，就是个大画布。 
// 场景：游戏开始时创建，所有东西都画在它上面。

class GameView : public cocos2d::Scene {
public:
    virtual bool init();

    // 拿主牌区的层
    cocos2d::Node* getPlayFieldLayer() const { return _playFieldLayer; }
    // 拿底部牌堆的层
    cocos2d::Node* getStackLayer() const { return _stackLayer; }

    /**
     * 显示结算弹窗
     * @param isWin 赢了还是输了
     * @param callback 点完按钮干啥
     */
    void showEndGamePopup(bool isWin, std::function<void()> callback);

    CREATE_FUNC(GameView);

private:
    cocos2d::Node* _playFieldLayer; // 放场上牌的层
    cocos2d::Node* _stackLayer;     // 放底下牌堆的层
    cocos2d::Node* _popupLayer;     // 弹窗专用层，盖在最上面
};

#endif // __GAME_VIEW_H__
