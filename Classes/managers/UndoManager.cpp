#include "UndoManager.h"

UndoManager::UndoManager() {}
UndoManager::~UndoManager() { clear(); }

void UndoManager::pushStep(const UndoStep& step) {
    _history.push_back(step);
}

bool UndoManager::popStep(UndoStep& outStep) {
    if (_history.empty()) return false;
    outStep = _history.back();
    _history.pop_back();
    return true;
}

void UndoManager::clear() {
    _history.clear();
}

bool UndoManager::canUndo() const {
    return !_history.empty();
}
