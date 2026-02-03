#include "GameController.h"
#include "services/LevelGenerator.h"

USING_NS_CC;

GameController* GameController::_instance = nullptr;

GameController* GameController::getInstance()
{
    if (!_instance) _instance = new GameController();
    return _instance;
}

GameController::GameController()
    : _gameModel(nullptr), _gameView(nullptr)
    , _playFieldController(nullptr), _stackController(nullptr)
    , _undoManager(nullptr)
    , _currentLevelIndex(1)
    , _jokerCount(0) // 初始0张
{
    _gameModel = new GameModel();
    _playFieldController = new PlayFieldController();
    _stackController = new StackController();
    _undoManager = new UndoManager();
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_playFieldController);
    CC_SAFE_DELETE(_stackController);
    CC_SAFE_DELETE(_undoManager);
}

void GameController::startGame()
{
    _gameView = GameView::create();
    loadLevel(_currentLevelIndex);

    // 1. 翻牌
    _stackController->setOnDrawPileClickCallback([this]() {
        if (_gameModel->isDrawPileEmpty()) return;
        CardModel* newCard = _gameModel->popFromDrawPile();

        // 正面
        newCard->setFaceUp(true);
        _gameModel->pushToStack(newCard);

        UndoStep step;
        step.type = UndoActionType::flipDrawPile;
        step.card = newCard;
        _undoManager->pushStep(step);

        _stackController->refreshView();
        checkGameStatus();
        });

    // 2. Joker
    _stackController->setOnJokerClickCallback([this]() {
        if (_gameModel->isJokerPileEmpty()) return;
        CardModel* joker = _gameModel->popFromJokerPile();
        joker->setFaceUp(true);
        _gameModel->pushToStack(joker);
        _undoManager->clear();
        _stackController->refreshView();
        checkGameStatus();
        });

    // 3. Undo
    _stackController->setOnUndoCallback([this]() { this->doUndo(); });

    // 4. 消除
    _playFieldController->setOnCardSelectCallback([this](CardView* clickedView) {
        CardModel* clickedCard = const_cast<CardModel*>(clickedView->getModel());
        if (!clickedCard->isFaceUp()) return;
        CardModel* topStackCard = _gameModel->getTopStackCard();

        if (isMatch(clickedCard, topStackCard)) {
            _gameModel->removeCardFromPlayField(clickedCard);
            _gameModel->pushToStack(clickedCard);
            _playFieldController->removeCardView(clickedView);

            UndoStep step;
            step.type = UndoActionType::matchCard;
            step.card = clickedCard;
            _undoManager->pushStep(step);

            _playFieldController->updateCardStates();
            _stackController->refreshView();
            checkGameStatus();
        }
        else {
            clickedView->runAction(Sequence::create(MoveBy::create(0.05f, Vec2(-10, 0)), MoveBy::create(0.05f, Vec2(20, 0)), MoveBy::create(0.05f, Vec2(-10, 0)), nullptr));
        }
        });
}

void GameController::loadLevel(int levelIndex)
{
    _gameModel->clear();
    _undoManager->clear();

    std::string jsonContent = "";
    if (levelIndex == 1) {
        // ---  JSON 坐标 ---
        jsonContent = R"({
            "Playfield": [
                { "CardFace": 12, "CardSuit": 0, "Position": {"x": 250, "y": 1000} },
                { "CardFace": 2,  "CardSuit": 0, "Position": {"x": 300, "y": 800} },
                { "CardFace": 2,  "CardSuit": 1, "Position": {"x": 350, "y": 600} },
                { "CardFace": 2,  "CardSuit": 0, "Position": {"x": 850, "y": 1000} },
                { "CardFace": 2,  "CardSuit": 0, "Position": {"x": 800, "y": 800} },
                { "CardFace": 1,  "CardSuit": 3, "Position": {"x": 750, "y": 600} }
            ],
            "Stack": [
                { "CardFace": 2, "CardSuit": 0, "Position": {"x": 0, "y": 0} },
                { "CardFace": 0, "CardSuit": 2, "Position": {"x": 0, "y": 0} },
                { "CardFace": 3, "CardSuit": 0, "Position": {"x": 0, "y": 0} }
            ]
        })";
    }
    else {
        // Level 2 第二关----未设置
        jsonContent = R"({"Playfield":[], "Stack":[]})";
    }

    if (!jsonContent.empty()) {
        LevelGenerator::generateGameModel(_gameModel, jsonContent);
    }

    // 强制底牌正面
    CardModel* topStack = _gameModel->getTopStackCard();
    if (topStack) topStack->setFaceUp(true);

    // 生成万能牌
    for (int i = 0; i < _jokerCount; ++i) {
        CardModel* joker = new CardModel();
        joker->init(CardFaceType::CFT_JOKER, CardSuitType::CST_JOKER);
        joker->setFaceUp(true);
        _gameModel->pushToJokerPile(joker);
    }

    _playFieldController->init(_gameModel, _gameView->getPlayFieldLayer());
    _playFieldController->createCardViews();

    _stackController->init(_gameModel, _gameView->getStackLayer());
    _stackController->refreshView();
}

void GameController::checkGameStatus()
{
    if (_gameModel->getPlayFieldCards().empty()) {
        _jokerCount = 0;
        _gameView->showEndGamePopup(true, [this]() {
            _currentLevelIndex++;
            this->loadLevel(_currentLevelIndex);
            });
        return;
    }

    if (_gameModel->isDrawPileEmpty()) {
        if (!_gameModel->isJokerPileEmpty()) return;

        CardModel* topStack = _gameModel->getTopStackCard();
        bool canMove = false;
        for (auto card : _gameModel->getPlayFieldCards()) {
            if (card->isFaceUp() && isMatch(card, topStack)) {
                canMove = true;
                break;
            }
        }

        if (!canMove) {
            _jokerCount++;
            _gameView->showEndGamePopup(false, [this]() {
                this->loadLevel(_currentLevelIndex);
                });
        }
    }
}

void GameController::doUndo()
{
    if (!_undoManager->canUndo()) return;
    UndoStep step;
    _undoManager->popStep(step);

    if (step.type == UndoActionType::flipDrawPile) {
        CardModel* card = _gameModel->getTopStackCard();
        if (card == step.card) {
            _gameModel->popFromStack();
            _gameModel->pushBackToDrawPile(card);
        }
    }
    else if (step.type == UndoActionType::matchCard) {
        CardModel* card = _gameModel->getTopStackCard();
        if (card == step.card) {
            _gameModel->popFromStack();
            _gameModel->addCardToPlayField(card);
        }
    }

    _playFieldController->createCardViews();
    _stackController->refreshView();
}

bool GameController::isMatch(CardModel* cardA, CardModel* cardB)
{
    if (!cardA || !cardB) return false;
    if (cardA->isJoker() || cardB->isJoker()) return true;
    int f1 = (int)cardA->getFace();
    int f2 = (int)cardB->getFace();
    int diff = std::abs(f1 - f2);
    return (diff == 1 || diff == 12);
}

cocos2d::Scene* GameController::getGameScene() { return _gameView; }

