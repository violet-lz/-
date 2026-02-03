#ifndef __GAME_DEFINES_H__
#define __GAME_DEFINES_H__

// 扑克牌的花色定义
enum class CardSuitType {
    CST_CLUBS = 0,      // 梅花 (对应资源或JSON中的0)
    CST_DIAMONDS = 1,   // 方块
    CST_HEARTS = 2,     // 红桃
    CST_SPADES = 3,     // 黑桃
    CST_NONE = -1,      // 无效值，用于初始化或错误检查
    CST_JOKER = 99      // 万能卡花色，特殊处理
};

// 扑克牌的点数定义
// 用于计算两张牌是否数值相邻（如 A和2，K和A）
enum class CardFaceType {
    CFT_ACE = 0,        // A (JSON: 0)
    CFT_TWO = 1,
    CFT_THREE = 2,
    CFT_FOUR = 3,
    CFT_FIVE = 4,
    CFT_SIX = 5,
    CFT_SEVEN = 6,
    CFT_EIGHT = 7,
    CFT_NINE = 8,
    CFT_TEN = 9,
    CFT_JACK = 10,
    CFT_QUEEN = 11,
    CFT_KING = 12,      // K (JSON: 12)
    CFT_NONE = -1,      // 无效值
    CFT_JOKER = 99      // 万能牌点数，可匹配任何牌
};

#endif // __GAME_DEFINES_H__
