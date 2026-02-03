#include "CardView.h"
#include "utils/CardHelper.h"
#include <cmath>

USING_NS_CC;

CardView::CardView()
    : _model(nullptr), _onClickCallback(nullptr), _touchListener(nullptr)
    , _faceNode(nullptr), _backSprite(nullptr)
{
}

CardView::~CardView()
{
    if (_touchListener) _eventDispatcher->removeEventListener(_touchListener);
}

CardView* CardView::create(const CardModel* model)
{
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel* model)
{
    if (!Node::init()) return false;
    _model = model;

    Size cardSize = Size(150, 200); // 牌的基础大小，后面会缩放

    // 1. 创建牌背面，默认看不见
    std::string bgName = CardHelper::getCardBgImageName(); // 这里暂时用通用背景当牌背，最好有专门的 card_back.png
    _backSprite = Sprite::create(bgName);
    if (!_backSprite) {// 万一没图，画个蓝色方块
        _backSprite = Sprite::create();
        _backSprite->setTextureRect(Rect(0, 0, 150, 200));
        _backSprite->setColor(Color3B(50, 50, 150)); // 蓝色牌背
    }
    else {
        _backSprite->setColor(Color3B(100, 100, 255)); // 染成蓝色模拟牌背
    }
    _backSprite->setPosition(cardSize / 2);
    this->addChild(_backSprite);

    // 2. 创建正面节点容器
    _faceNode = Node::create();
    _faceNode->setContentSize(cardSize);
    _faceNode->setAnchorPoint(Vec2(0, 0));
    this->addChild(_faceNode);

    // 牌的底图
    auto bg = Sprite::create(bgName);
    if (bg) { bg->setPosition(cardSize / 2); _faceNode->addChild(bg); }

    // ---判断是否是万能牌 ---
    if (_model->isJoker()) {
        // 使用 DrawNode 画一个金色的五角星 (彻底解决字符不显示问题)
        auto starNode = DrawNode::create();
        // 星星的中心点
        Vec2 center(cardSize.width / 2, cardSize.height / 2 + 20);

        // 计算五角星的10个顶点
        Vec2 verts[10];
        float r_outer = 45.0f; // 外半径
        float r_inner = 18.0f; // 内半径

        for (int i = 0; i < 10; i++) {
            // 角度：从上方(-90度)开始，每步36度
            float angle = CC_DEGREES_TO_RADIANS(i * 36 - 90);
            float r = (i % 2 == 0) ? r_outer : r_inner;
            verts[i] = center + Vec2(cos(angle) * r, sin(angle) * r);
        }
        // 绘制多边形：填充金色，黑色描边
        starNode->drawPolygon(verts, 10, Color4F(1.0f, 0.84f, 0.0f, 1.0f), 2, Color4F::BLACK);

        _faceNode->addChild(starNode);

        // 绘制英文单词 "JOKER"
        auto textLabel = Label::createWithSystemFont("JOKER", "Arial", 30); // 字号30
        textLabel->setPosition(cardSize.width / 2, cardSize.height / 2 - 50); // 在星星下方
        textLabel->setColor(Color3B::BLACK);
        textLabel->enableBold(); // 加粗
        _faceNode->addChild(textLabel);

        // 角标
        auto smallJ = Label::createWithSystemFont("J", "Arial", 30);
        smallJ->setPosition(20, cardSize.height - 20);
        smallJ->setColor(Color3B::RED);
        _faceNode->addChild(smallJ);
    }
    else {
    //       普通卡牌样式
    CardFaceType face = _model->getFace();
    CardSuitType suit = _model->getSuit();

    // 左上角数字
    std::string smallNumPath = CardHelper::getNumberImageName(face, suit, false);
    auto smallNumber = Sprite::create(smallNumPath);
    if (smallNumber) {
        smallNumber->setAnchorPoint(Vec2(0.5f, 1.0f));
        smallNumber->setPosition(25, cardSize.height - 15);
        _faceNode->addChild(smallNumber);
    }

    /// 左上角花色
    std::string suitPath = CardHelper::getSuitImageName(suit);
    auto smallSuit = Sprite::create(suitPath);
    // 如果没有数字图(防止空指针)，就不加小花色
    if (smallSuit && smallNumber) {
        smallSuit->setAnchorPoint(Vec2(0.5f, 1.0f));
        smallSuit->setPosition(smallNumber->getPositionX(),
            smallNumber->getPositionY() - smallNumber->getContentSize().height - 5);
        smallSuit->setScale(0.4f);
        _faceNode->addChild(smallSuit);
    }

    // 中间大花色
    auto centerSuit = Sprite::create(suitPath);
    if (centerSuit) {
        centerSuit->setPosition(cardSize.width / 2, cardSize.height / 2);
        centerSuit->setScale(1.5f);
        _faceNode->addChild(centerSuit);
    }
    }

    // 设置自己的大小，方便触摸检测
    this->setContentSize(cardSize);
    if (bg) this->setContentSize(bg->getContentSize());

    // 4. 初始化触摸
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

    // 5. 刷新一下初始状态
    refreshViewStatus();

    return true;
}

void CardView::refreshViewStatus()
{
    bool isFaceUp = _model->isFaceUp();

    if (isFaceUp) {
        _faceNode->setVisible(true);
        _backSprite->setVisible(false);
        setTouchEnabled(true); // 正面可点
    }
    else {
        _faceNode->setVisible(false);
        _backSprite->setVisible(true);
        setTouchEnabled(false); // 背面不可点
    }
}

void CardView::setTouchEnabled(bool enabled)
{
    if (_touchListener) _touchListener->setEnabled(enabled);
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    if (!_faceNode->isVisible()) return false; // 背面不响应

    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size s = this->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    return rect.containsPoint(locationInNode);
}

void CardView::onTouchEnded(Touch* touch, Event* event)
{
    if (_onClickCallback) _onClickCallback(this);
}

void CardView::setOnClickCallback(std::function<void(CardView*)> callback)
{
    _onClickCallback = callback;
}
