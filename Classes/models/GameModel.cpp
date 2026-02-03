#include "GameModel.h"

GameModel::GameModel()
{
}

GameModel::~GameModel()
{
    clear();
}

// --- PlayField ---
void GameModel::addCardToPlayField(CardModel* card) {
    _playFieldCards.push_back(card);
}

void GameModel::removeCardFromPlayField(CardModel* card) {
    auto it = std::find(_playFieldCards.begin(), _playFieldCards.end(), card);
    if (it != _playFieldCards.end()) {
        _playFieldCards.erase(it);
    }
}

// --- Stack ---
void GameModel::pushToStack(CardModel* card) {
    _stackCards.push_back(card);
}

CardModel* GameModel::getTopStackCard() {
    if (_stackCards.empty()) return nullptr;
    return _stackCards.back();
}

void GameModel::popFromStack() {
    if (!_stackCards.empty()) {
        _stackCards.pop_back();
    }
}

// --- Draw Pile ---
void GameModel::pushToDrawPile(CardModel* card) {
    _drawPileCards.push_back(card);
}

CardModel* GameModel::popFromDrawPile() {
    if (_drawPileCards.empty()) return nullptr;
    CardModel* card = _drawPileCards.back();
    _drawPileCards.pop_back();
    return card;
}

bool GameModel::isDrawPileEmpty() const {
    return _drawPileCards.empty();
}

void GameModel::pushBackToDrawPile(CardModel* card) {
    _drawPileCards.push_back(card);
}

// --- 万能牌实现 ---
void GameModel::pushToJokerPile(CardModel* card) {
    _jokerPileCards.push_back(card);
}

CardModel* GameModel::popFromJokerPile() {
    if (_jokerPileCards.empty()) return nullptr;
    CardModel* card = _jokerPileCards.back();
    _jokerPileCards.pop_back();
    return card;
}

bool GameModel::isJokerPileEmpty() const {
    return _jokerPileCards.empty();
}

int GameModel::getJokerPileSize() const {
    return (int)_jokerPileCards.size();
}

// --- Clear ---
void GameModel::clear()
{
    // 挨个删掉 new 出来的牌，防止内存泄漏
    for (auto card : _playFieldCards) delete card;
    _playFieldCards.clear();

    for (auto card : _stackCards) delete card;
    _stackCards.clear();

    for (auto card : _drawPileCards) delete card;
    _drawPileCards.clear();

    // 清理万能牌
    for (auto card : _jokerPileCards) delete card;
    _jokerPileCards.clear();
}
