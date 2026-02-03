#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

// 职责：一张牌的显示。根据模型数据画出具体的牌。
// 场景：游戏里每张可见的牌都是一个这玩意儿。
class CardView : public cocos2d::Node {
public:
    // 创建牌的视图
    static CardView* create(const CardModel* model);

    virtual bool init(const CardModel* model);

    // 设置点击回调
    void setOnClickCallback(std::function<void(CardView*)> callback);

    // 拿到这张牌的数据
    const CardModel* getModel() const { return _model; }

    // 刷新视图显示 (根据正面/背面状态切换图片) ---
    void refreshViewStatus();

    // 控制这张牌能不能点
    void setTouchEnabled(bool enabled);

protected:
    CardView();
    virtual ~CardView();
    // 触摸事件
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
private:
    const CardModel* _model;                             // 牌的数据
    std::function<void(CardView*)> _onClickCallback;     // 点了之后干啥
    cocos2d::EventListenerTouchOneByOne* _touchListener; // 点击监听器

    // 分开正面和背面，方便切换
    cocos2d::Node* _faceNode;     // 正面内容都放这
    cocos2d::Sprite* _backSprite; // 牌背面
};

#endif // __CARD_VIEW_H__
