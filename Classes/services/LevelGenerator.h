#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__

#include "models/GameModel.h"
#include <string>


// 职责：将关卡配置数据(JSON)转换为运行时游戏模型
// 规范：Services层，无状态服务,每局开始前调用一次。
class LevelGenerator {
public:
    // 读json，填数据到GameModel里
    static void generateGameModel(GameModel* model, const std::string& jsonContent);
};

#endif // __LEVEL_GENERATOR_H__
