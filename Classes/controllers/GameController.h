#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "managers/UndoManager.h"

/**
 * 类名：GameController
 * 职责：游戏的核心中枢（上帝类）。
 * 它负责协调数据模型(Model)、视图(View)以及子控制器(Sub-Controllers)之间的交互。
 * 所有的游戏全局逻辑（如：开始游戏、加载关卡、判断输赢、执行撤销）都由它来调度。
 *
 * 使用场景：通常在进入游戏场景时初始化，贯穿整个关卡流程。
 */
class GameController : public cocos2d::Ref {
public:
    // 获取单例对象。确保游戏中只有一个总控在运行。
    static GameController* getInstance();

    // 开始游戏流程。会初始化视图、绑定事件并加载第一关。
    void startGame();

    // 获取当前游戏的主场景对象，用于Director运行场景。
    cocos2d::Scene* getGameScene();

private:
    // 构造函数私有化，强制使用 getInstance()
    GameController();
    ~GameController();

    /**
     * 加载指定关卡的数据。
     * @param levelIndex 关卡索引（如1, 2...），根据这个数字去读取不同的配置。
     */
    void loadLevel(int levelIndex);

    /**
     * 检查游戏当前的胜负状态。
     * 每次操作（翻牌、消除）后都会调用，判断是否清空了盘面（赢）或无牌可走（输）。
     */
    void checkGameStatus();

    /**
     * 核心规则算法：判断两张牌是否可以消除。
     * @param cardA 点击的牌
     * @param cardB 牌堆顶的牌
     * @return 如果点数相邻（如2和3，K和A）或包含万能牌，返回 true。
     */
    bool isMatch(CardModel* cardA, CardModel* cardB);

    // 执行撤销操作。从 UndoManager 取出上一步状态并恢复模型和视图。
    void doUndo();

    static GameController* _instance;

    // --- 成员变量 ---
    GameModel* _gameModel;              // 游戏核心数据模型
    GameView* _gameView;                // 游戏主视图场景
    PlayFieldController* _playFieldController; // 负责主牌桌（金字塔区）的逻辑
    StackController* _stackController;         // 负责底部牌堆区的逻辑
    UndoManager* _undoManager;                 // 负责记录和回退操作步骤

    int _currentLevelIndex; // 当前进行到的关卡数
    int _jokerCount;        // 累积获得的万能牌数量（可能从上一关继承或购买）
};

#endif // __GAME_CONTROLLER_H__
