#include "GameView.h"

USING_NS_CC;

bool GameView::init()
{
    if (!Scene::init()) return false;

    // 1.紫色背景放底下
    auto stackBg = LayerColor::create(Color4B(145, 30, 140, 255), 1080, 580);
    this->addChild(stackBg, 0);

    // 2. 上面土黄色背景
    auto playfieldBg = LayerColor::create(Color4B(170, 120, 70, 255), 1080, 1500);
    playfieldBg->setPosition(0, 580);
    this->addChild(playfieldBg, 0);

    // 3. 牌都放这两个层里，方便管理
    _playFieldLayer = Node::create();
    this->addChild(_playFieldLayer, 1);

    _stackLayer = Node::create();
    this->addChild(_stackLayer, 2);

    // 4. 弹窗层，z值设高点，保证在最上面
    _popupLayer = Node::create();
    this->addChild(_popupLayer, 100);

    return true;
}

void GameView::showEndGamePopup(bool isWin, std::function<void()> callback)
{
    _popupLayer->removeAllChildren(); // 确保清理旧弹窗

    // 半透明黑背景，挡住后面的
    auto mask = LayerColor::create(Color4B(0, 0, 0, 180), 2080, 2080); // 足够大覆盖宽屏
    mask->setPosition(-500, -500); // 偏移一点覆盖全屏
    _popupLayer->addChild(mask);

    // 把点击吞掉，防止点到后面的牌
    auto maskListener = EventListenerTouchOneByOne::create();
    maskListener->setSwallowTouches(true);
    maskListener->onTouchBegan = [](Touch*, Event*) { return true; };
    _popupLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(maskListener, mask);

    // 弹窗内容都放这容器里，方便整体移动
    auto container = Node::create();
    container->setPosition(540, 960); // 屏幕中间
    _popupLayer->addChild(container);

    // 大标题，胜利或失败
    std::string titleStr = isWin ? "VICTORY!" : "FAILED";
    auto titleLabel = Label::createWithSystemFont(titleStr, "Arial", 120);
    titleLabel->setPosition(0, 200);
    titleLabel->setColor(isWin ? Color3B::YELLOW : Color3B::RED);
    titleLabel->enableOutline(Color4B::BLACK, 4);
    container->addChild(titleLabel);

    // 按钮背景
    auto btnBg = LayerColor::create(isWin ? Color4B(0, 150, 0, 255) : Color4B(200, 50, 50, 255), 600, 160);
    btnBg->ignoreAnchorPointForPosition(false);
    btnBg->setAnchorPoint(Vec2(0.5, 0.5));
    btnBg->setPosition(0, -100);
    container->addChild(btnBg);

    // 按钮文字
    std::string btnStr = isWin ? "Next Level" : "Try Again";
    auto btnLabel = Label::createWithSystemFont(btnStr, "Arial", 80);
    btnLabel->setPosition(btnBg->getContentSize() / 2);
    btnBg->addChild(btnLabel);

    // 按钮点击事件
    auto btnListener = EventListenerTouchOneByOne::create();
    btnListener->setSwallowTouches(true);
    btnListener->onTouchBegan = [btnBg](Touch* t, Event*) {
        Vec2 p = btnBg->getParent()->convertToNodeSpace(t->getLocation());
        return btnBg->getBoundingBox().containsPoint(p);
        };
    btnListener->onTouchEnded = [this, callback](Touch*, Event*) {
        // 先移除弹窗，再执行逻辑
        this->_popupLayer->removeAllChildren();
        if (callback) callback();
        };
    _popupLayer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(btnListener, btnBg);
}
