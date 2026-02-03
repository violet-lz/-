#include "LevelGenerator.h"
#include "cocos2d.h"  
#include "json/document.h"

USING_NS_CC; 

void LevelGenerator::generateGameModel(GameModel* model, const std::string& jsonContent)
{
    model->clear();

    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());// 解析json字符串

    // 1. 解析场上的牌
    if (doc.HasParseError()) {
        CCLOG("JSON Parse Error!"); // 引入头文件后，CCLOG 也能正常使用了
        return;
    }

    // 1. 解析 Playfield (主牌区)
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& arr = doc["Playfield"];
        int uidCounter = 1000;

        for (rapidjson::SizeType i = 0; i < arr.Size(); i++) {
            const auto& item = arr[i];

            int faceInt = item["CardFace"].GetInt();
            int suitInt = item["CardSuit"].GetInt();

            // 读取坐标
            float x = 0, y = 0;
            if (item.HasMember("Position")) {
                x = item["Position"]["x"].GetFloat();
                y = item["Position"]["y"].GetFloat();
            }

            CardModel* card = new CardModel();
            card->init((CardFaceType)faceInt, (CardSuitType)suitInt);
            card->setPosition(x, y); // 存入 JSON 里的坐标
            card->setUid(uidCounter++);

            model->addCardToPlayField(card);
        }
    }

    // 2. 解析 Stack (底牌/备用牌)
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& arr = doc["Stack"];

        for (rapidjson::SizeType i = 0; i < arr.Size(); i++) {
            const auto& item = arr[i];
            int faceInt = item["CardFace"].GetInt();
            int suitInt = item["CardSuit"].GetInt();

            CardModel* card = new CardModel();
            card->init((CardFaceType)faceInt, (CardSuitType)suitInt);

            if (i == 0) {
                // 第1个作为初始底牌
                model->pushToStack(card);
            }
            else {
                // 剩下的作为备用牌堆
                model->pushToDrawPile(card);
            }
        }
    }
}
