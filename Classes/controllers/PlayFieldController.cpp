#include "PlayFieldController.h"

USING_NS_CC;

PlayFieldController::PlayFieldController()
    : _gameModel(nullptr)
    , _viewContainer(nullptr)
    , _onCardSelectCallback(nullptr)
{
}

PlayFieldController::~PlayFieldController()
{
    _cardViews.clear();
}

void PlayFieldController::init(GameModel* gameModel, cocos2d::Node* viewContainer)
{
    _gameModel = gameModel;
    _viewContainer = viewContainer;
}


// 初始化所有卡牌视图。
// 创建Sprite，负责计算牌的位置和遮挡关系。
void PlayFieldController::createCardViews()
{
    if (!_gameModel || !_viewContainer) return;

    // 先把桌面上旧的牌全清掉，防止重叠
    _viewContainer->removeAllChildren();
    _cardViews.clear();

    const auto& cards = _gameModel->getPlayFieldCards();
    float cardScale = 1.5f;

    // 全局偏移量，用来整体微调所有牌在屏幕上的位置
    float globalOffsetX = -100.0f;
    float globalOffsetY = 200.0f;

    // zOrder 决定渲染顺序，后面的牌会盖住前面的牌
    // 这里假设 JSON 数据里的顺序就是堆叠顺序（底部的先画）
    int zOrder = 0;

    for (auto cardModel : cards) {
        auto cardView = CardView::create(cardModel);
        cardView->setScale(cardScale);

        // 计算最终屏幕坐标：原始JSON坐标 + 全局偏移
        float finalX = cardModel->getX() + globalOffsetX;
        float finalY = cardModel->getY() + globalOffsetY;

        cardView->setPosition(finalX, finalY);

        // 绑定点击事件，使用 std::bind 转发给本类的 onCardClicked 处理
        cardView->setOnClickCallback(std::bind(&PlayFieldController::onCardClicked, this, std::placeholders::_1));

        _viewContainer->addChild(cardView, zOrder);
        _cardViews.push_back(cardView);

        zOrder++;
    }

    // 牌放好后，必须立刻计算一次遮挡关系，否则被压住的牌可能也是正面朝上的
    updateCardStates();
}

void PlayFieldController::onCardClicked(CardView* cardView)
{
    if (!cardView) return;

    // 点击动画 (ScaleBy 是相对缩放，所以这里不需要改，它会自动基于1.5倍放大)
    cardView->setTouchEnabled(false);
    cardView->runAction(Sequence::create(
        ScaleBy::create(0.1f, 1.2f),
        ScaleBy::create(0.1f, 1.0f / 1.2f),
        CallFunc::create([cardView]() {
            cardView->setTouchEnabled(true);
            }),
        nullptr
    ));

    if (_onCardSelectCallback) {
        _onCardSelectCallback(cardView);
    }
}

// 核心遮挡算法：更新所有牌的状态（正面 / 背面 / 可点击）。
// 遍历所有牌，看是否被“层级更高”且“矩形相交”的牌盖住了。
void PlayFieldController::updateCardStates()
{
    for (auto viewA : _cardViews) {
        CardModel* modelA = const_cast<CardModel*>(viewA->getModel());
        bool isCovered = false; // 默认没被遮挡

        Rect rectA = viewA->getBoundingBox();

        for (auto viewB : _cardViews) {
            if (viewA == viewB) continue; // 自己不遮挡自己

            // 规则：只有 ZOrder 比自己大的牌才可能遮挡自己
            // (通常 ZOrder 大的在上面)
            if (viewB->getLocalZOrder() > viewA->getLocalZOrder()) {
                Rect rectB = viewB->getBoundingBox();

                // 物理碰撞检测：如果两个矩形有重叠，A就是被B遮挡了
                if (rectA.intersectsRect(rectB)) {
                    isCovered = true;
                    break; // 只要被一张牌挡住，就是被挡住了，不用看其他的
                }
            }
        }

        // 没被遮挡设为正面(FaceUp)，被遮挡设为背面
        modelA->setFaceUp(!isCovered);
        // 通知 View 层刷新贴图（显示花色还是显示牌背）
        viewA->refreshViewStatus();
    }
}

void PlayFieldController::setOnCardSelectCallback(std::function<void(CardView*)> callback)
{
    _onCardSelectCallback = callback;
}

void PlayFieldController::removeCardView(CardView* view)
{
    if (!view) return;

    auto it = std::find(_cardViews.begin(), _cardViews.end(), view);
    if (it != _cardViews.end()) {
        _cardViews.erase(it);
    }

    view->removeFromParent();
}
