#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/CardModel.h"
#include <vector>

// 定义操作类型
enum class UndoActionType {
    flipDrawPile,   // 翻备用牌
    matchCard       // 消除主牌
};

// 定义单步操作的数据快照
struct UndoStep {
    UndoActionType type;

    // 涉及的卡牌 (如果是消除，就是被消除的那张牌)
    CardModel* card;

    // 操作前的状态 (例如原本的位置、翻面状态等)
    // 简单起见，我们存储操作发生时导致状态变化的那些牌的指针
    // 对于 TriPeaks：
    // 1. 翻牌：DrawPile 顶弹出 -> Stack 顶压入。逆操作：Stack 顶弹出 -> DrawPile 顶压入。
    // 2. 消除：Playfield 移除 -> Stack 顶压入。逆操作：Stack 顶弹出 -> Playfield 放回 (恢复坐标, 恢复遮挡关系)。

    // 记录这次操作导致哪些牌从背面变成了正面 (以便回退时盖回去)
    std::vector<CardModel*> revealedCards;
};

class UndoManager {
public:
    UndoManager();
    ~UndoManager();

    void pushStep(const UndoStep& step);
    bool popStep(UndoStep& outStep); // 弹出一布，如果空返回false
    void clear();
    bool canUndo() const;

private:
    std::vector<UndoStep> _history;
};

#endif // __UNDO_MANAGER_H__
