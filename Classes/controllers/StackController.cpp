#include "StackController.h"
#include "utils/CardHelper.h"

USING_NS_CC;

StackController::StackController()
    : _gameModel(nullptr), _viewContainer(nullptr)
    , _drawPileSprite(nullptr), _topStackCardView(nullptr)
    , _jokerPileView(nullptr)
{
}

StackController::~StackController() {}

void StackController::init(GameModel* gameModel, cocos2d::Node* viewContainer)
{
    _gameModel = gameModel;
    _viewContainer = viewContainer;
}

// 刷新底部区域的所有视图。
// 这是一个的UI函数，负责重新绘制：备用牌堆、当前底牌、万能牌堆、撤销按钮。
void StackController::refreshView()
{
    if (!_viewContainer || !_gameModel) return;

    // 暴力刷新：清空重绘。虽然性能不是最优，但逻辑最简单不易出错。
    _viewContainer->removeAllChildren();
    // 重置指针，防止野指针访问
    _topStackCardView = nullptr;
    _drawPileSprite = nullptr;
    _jokerPileView = nullptr;

    // --- UI布局参数配置 ---
    float zoneCenterY = 290.0f; // 底部区域的垂直中心线
    float cardScale = 1.5f;

    // 坐标定义 
    float drawPileX = 300.0f; // 备用牌 
    float stackCardX = drawPileX + 200.0f;
    float jokerX = stackCardX + 300.0f;

    // --- 1. 绘制备用牌堆 ---
    // 如果牌堆里还有牌，就显示一个牌背
    if (!_gameModel->isDrawPileEmpty()) {
        std::string bgName = CardHelper::getCardBgImageName();
        _drawPileSprite = Sprite::create(bgName);
        if (_drawPileSprite) {
            _drawPileSprite->setScale(cardScale);
            _drawPileSprite->setPosition(drawPileX, zoneCenterY);
            _viewContainer->addChild(_drawPileSprite);

            auto listener = EventListenerTouchOneByOne::create();
            listener->setSwallowTouches(true);
            listener->onTouchBegan = [this](Touch* t, Event* e) {
                Vec2 p = _drawPileSprite->getParent()->convertToNodeSpace(t->getLocation());
                return _drawPileSprite->getBoundingBox().containsPoint(p);
                };
            listener->onTouchEnded = [this](Touch*, Event*) { this->onDrawPileClicked(); };
            _viewContainer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _drawPileSprite);
        }
    }

    // --- 2. 绘制当前底牌---
    // 这是玩家用来配对的那张目标牌，需要显示出来
    CardModel* topCard = _gameModel->getTopStackCard();
    if (topCard) {
        topCard->setFaceUp(true);

        _topStackCardView = CardView::create(topCard);
        _topStackCardView->setScale(cardScale);
        _topStackCardView->setPosition(stackCardX, 140.0f);
        _viewContainer->addChild(_topStackCardView);
    }

    // --- 3. 绘制万能牌堆---
    // 无论有没有万能牌，都先创建一个显示的底座
    CardModel* displayJoker = new CardModel();
    displayJoker->init(CardFaceType::CFT_JOKER, CardSuitType::CST_JOKER);
    displayJoker->setFaceUp(true);

    _jokerPileView = CardView::create(displayJoker);
    _jokerPileView->setScale(cardScale);
    _jokerPileView->setPosition(jokerX, 140.0f); // X=700
    _viewContainer->addChild(_jokerPileView);

    // 显示剩余数量的小标签
    int count = _gameModel->getJokerPileSize();

    // 万能牌数量标签
    auto countLabel = Label::createWithSystemFont("x" + std::to_string(count), "Arial", 50);
    countLabel->setPosition(70, -15);//设置万能牌下方的数量标签
    countLabel->setColor(Color3B::YELLOW);
    countLabel->enableOutline(Color4B::BLACK, 2);
    _jokerPileView->addChild(countLabel);

    // 如果数量为0，变灰且不可点击
    if (count > 0) {
        _jokerPileView->setOpacity(255);
        _jokerPileView->setOnClickCallback([this](CardView*) { this->onJokerClicked(); });
    }
    else {
        _jokerPileView->setColor(Color3B::GRAY);
        _jokerPileView->setOpacity(128); // 0张时变灰
    }

    // --- 4. 绘制撤销按钮 (Undo) ---
    // 创建一个橙色的色块作为按钮
    auto undoLabel = Label::createWithSystemFont("UNDO", "Arial", 32);
    auto undoBg = LayerColor::create(Color4B(200, 100, 50, 255), 100, 60); // 橙色按钮
    undoBg->ignoreAnchorPointForPosition(false);//锚点生效，节点的位置会基于锚点来计算
    undoBg->setAnchorPoint(Vec2(0.5, 0.5));//将锚点设置在节点中心

    // X=100 (最左侧), Y=290 (垂直居中)
    undoBg->setPosition(100, zoneCenterY);
    undoLabel->setPosition(undoBg->getContentSize() / 2);
    undoBg->addChild(undoLabel);

    _viewContainer->addChild(undoBg);

    auto undoListener = EventListenerTouchOneByOne::create();
    undoListener->setSwallowTouches(true);
    undoListener->onTouchBegan = [undoBg](Touch* t, Event*) {
        Vec2 p = undoBg->getParent()->convertToNodeSpace(t->getLocation());
        return undoBg->getBoundingBox().containsPoint(p);
        };
    undoListener->onTouchEnded = [this](Touch*, Event*) {
        if (_onUndoCallback) _onUndoCallback();
        };
    _viewContainer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(undoListener, undoBg);
}

// 回调
void StackController::setOnDrawPileClickCallback(std::function<void()> callback) { _onDrawPileClickCallback = callback; }
void StackController::setOnUndoCallback(std::function<void()> callback) { _onUndoCallback = callback; }
void StackController::setOnJokerClickCallback(std::function<void()> callback) { _onJokerClickCallback = callback; }
void StackController::onDrawPileClicked() { if (_drawPileSprite) _drawPileSprite->runAction(Sequence::create(ScaleTo::create(0.05f, 1.6f), ScaleTo::create(0.05f, 1.5f), nullptr)); if (_onDrawPileClickCallback) _onDrawPileClickCallback(); }
void StackController::onJokerClicked() { if (_jokerPileView) _jokerPileView->runAction(Sequence::create(ScaleTo::create(0.05f, 1.6f), ScaleTo::create(0.05f, 1.5f), nullptr)); if (_onJokerClickCallback) _onJokerClickCallback(); }
