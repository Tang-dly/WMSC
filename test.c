#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// {"美人鱼", "船锚", "占卜球", "海怪", "钥匙", "宝箱", "钩子", "藏宝图", "刀", "炮弹"};
int bitMask = 0;
int score = 0;
int flag = 1;
int generateRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

handleM()
{

}

handleC()
{

}

handleZ()
{

}

handleH()
{

}

handleY()
{

}

handleB()
{
    
}
handleG()
{

}

handleT()
{

}

handleD()
{
    
}

handleP()
{
    
}
void getCard()
{
    int characterId = generateRandomInRange(0, 9);
    int newScore = generateRandomInRange(2, 7);
    switch (characterId)
    {
    case 0:
        printf("美人鱼%d\n", newScore);
        handleM();
        break;
    
    default:
        break;
    }
    if((bitMask >> characterId) & 1 == 0) {
        bitMask |= 1 << characterId;
    } else {
        printf("boom!\n");
        flag = 0;
        return;
    }
}
int main()
{
    int flag = 0;
    while(1) {
        getCard(score);
        if(flag == 0) {
            printf("您的最终得分为：%d\n", score);
            break;
        } else {
            printf("是否继续抽卡？[1/0]\n");
            scanf("%d", &flag);
            if (flag == 0)
            {
                printf("您的最终得分为：%d\n", score);
                break;
            }            
        }

    }
}
