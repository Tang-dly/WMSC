#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define CARD_CNT 5
// {"美人鱼", "船锚", "占卜球", "海怪", "钥匙", "宝箱", "钩子", "藏宝图", "刀", "炮弹"};
int bitMask = 0;
int flag = 1;    //1为正常， 0为不抽了或者牌库空了， -1为爆炸
int playerId = 1;  //当前玩家id

typedef struct Bag {
    int characterId;
    int score;
    struct Bag *next;
} bag_t;


//玩家战利品
bag_t *player1 = NULL;
bag_t *player2 = NULL;

//总牌库
bag_t *store = NULL;

//弃牌堆
bag_t *discard = NULL;

//手牌库
bag_t *hand = NULL;

void handleCard(int characterId, int newScore, int printFlag);
void handleBoom();
void getCard();
// 创建新节点
bag_t* create_node(int characterId, int score) {
    bag_t* new_node = (bag_t*)malloc(sizeof(bag_t));
    if (new_node == NULL) {
        perror("Failed to create a new node");
        exit(EXIT_FAILURE);
    }
    new_node->characterId = characterId;
    new_node->score = score;
    new_node->next = NULL;
    return new_node;
}

// 添加节点到链表末尾
void append_node(bag_t** head, int characterId, int score) {
    bag_t* new_node = create_node(characterId, score);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    bag_t* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

// 删除节点
void delete_node(bag_t** head, int characterId, int score) {
    bag_t* temp = *head;
    bag_t* prev = NULL;
    
    // 如果头节点需要删除
    if (temp != NULL && temp->characterId == characterId && temp->score == score) {
        *head = temp->next; // 头节点被删除
        free(temp);
        return;
    }

    // 搜索待删除的节点
    while (temp != NULL && temp->characterId != characterId || temp->score != score) {
        prev = temp;
        temp = temp->next;
    }

    // 如果没有找到节点
    if (temp == NULL) return;

    // 节点找到，删除节点
    prev->next = temp->next;
    free(temp);
    temp = NULL;
}

// 打印链表

void printCard(int characterId, int score)
{
    switch(characterId) {
        case 0:
            printf("美人鱼M%d, ", score);
            break;
        case 1:
            printf("船锚C%d, ", score);
            break;
        case 2:
            printf("占卜球Z%d, ", score);
            break;
        case 3:
            printf("海怪H%d, ", score);
            break;
        case 4:
            printf("钥匙Y%d, ", score);
            break;
        case 5: 
            printf("宝箱B%d, ", score);
            break;
        case 6:
            printf("钩子G%d, ", score);
            break;
        case 7:
            printf("藏宝图T%d, ", score);
            break;
        case 8:
            printf("刀D%d, ", score);
            break;
        case 9:
            printf("炮弹P%d, ", score);
            break;
        default:
            break;   
    }   
}
void printBag(bag_t *bag)
{
    bag_t *tmp = bag;
    while(tmp != NULL) {
        printCard(tmp->characterId, tmp->score);
        tmp = tmp->next;
    }
    printf("\n");
}

// 释放链表
void free_list(bag_t** head) {
    bag_t* temp = *head;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
int getNodeCnt(bag_t* head)
{
    bag_t* temp = head;
    int cnt = 0;
    while (temp != NULL) {
        cnt++;
        temp = temp->next;
    }
    return cnt;
}

bag_t* findNodeByIndex(bag_t *bag ,int index)
{
    bag_t *temp = bag;
    while(index--) {
        temp = temp->next;
    }
    return temp;
}

bag_t* findLastNode(bag_t* head)
{
    bag_t* temp = head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    return temp;
}

bag_t* findNodeByChara(bag_t *bag, int id)
{
    bag_t *temp = bag;
    while(temp != NULL) {
        if(temp->characterId == id) {
            break;
        }
        temp = temp->next;
    }
    return temp;
}
bag_t* findNodeByCharaAndScore(bag_t *bag, int id, int score)
{
    bag_t *temp = bag;
    while(temp != NULL) {
        if(temp->characterId == id && temp->score == score) {
            break;
        }
        temp = temp->next;
    }
    return temp;
}
void add_bag(bag_t **dst, bag_t *new)
{
    bag_t *temp2 = new;
    while (temp2 != NULL) {
        append_node(dst, temp2->characterId, temp2->score);
        temp2 = temp2->next;
    }
    return;
}
int generateRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

int getIntFromAlphabet(char id)
{
    switch(id) {
        case 'M':
            return 0;
        case 'C':
            return 1;
        case 'Z':
            return 2;
        case 'H':
            return 3;
        case 'Y':
            return 4;
        case 'B':
            return 5;
        case 'G':
            return 6;
        case 'T':
            return 7;
        case 'D':
            return 8;
        case 'P':
            return 9;
    }
}
void handleM(int scoreM)
{
    int bagCnt = getNodeCnt(hand);
    if(bagCnt < 2) {
        printf("当前手牌不足，美人鱼效果无法触发\n");
        return;
    }
REINPUT:{
    char input[2];
    printf("请挑选手牌中的一张牌与美人鱼更换位置\n");
    scanf("%s", input);
    int characterId = getIntFromAlphabet(input[0]);
    int score = input[1] - '0';
    bag_t *tmp = findNodeByCharaAndScore(hand, characterId, score);
    if(tmp != NULL) {
        tmp->characterId = 0;
        tmp->score = scoreM;
        bag_t *tmp1 = findLastNode(hand);
        tmp1->characterId = characterId;
        tmp1->score = score;
        printf("交换完毕，当前手牌为 ");
        printBag(hand);
        handleCard(characterId, score, 0);        
    } else {
        printf("没有找到该牌，请重新输入\n");
        goto REINPUT;
    }
}
}

void handleC()
{
    printf("恭喜抽到船锚，船锚之前的牌将会被保护，不受爆炸影响\n");
}

void handleZ()
{
    bag_t *nextCard = store;
    printf("占卜球看到下张牌为:");
    switch(nextCard->characterId) {
        case 0:
            printf("美人鱼\n");
            break;
        case 1:
            printf("船锚\n");
            break;
        case 2:
            printf("占卜球\n");
            break;
        case 3:
            printf("海怪\n");
            break;
        case 4:
            printf("钥匙\n");
            break;
        case 5: 
            printf("宝箱\n");
            break;
        case 6:
            printf("钩子\n");
            break;
        case 7:
            printf("藏宝图\n");
            break;
        case 8:
            printf("刀\n");
            break;
        case 9:
            printf("炮弹\n");
            break;
        default:
            break;
    }
    return;
}

void handleH()
{
    printf("抽取两张牌之前无法结束\n");
    getCard();
    if(flag == 1) {
        getCard();
    }
}

void boxReward()
{
    printf("宝箱奖励为从弃牌区获取两张牌直接放入战利品区\n");
    int discardCnt = getNodeCnt(discard);
    bag_t *reward = discard;
    if(discardCnt == 0) {
        printf("没有弃牌，无法触发奖励\n");
        return;
    } else if(discardCnt == 1){
        printf("弃牌区仅有一张卡牌。恭喜获取");
        printCard(reward->characterId, reward->score);
        printf("到战利品区\n");
        if(playerId == 1) {
            append_node(&player1, reward->characterId, reward->score);
        } else {
            append_node(&player2, reward->characterId, reward->score);
        }
        delete_node(&discard, reward->characterId, reward->score);
    } else {
        int cnt = 2;
        printf("恭喜获取");
        while(cnt--) {
            printCard(reward->characterId, reward->score);
            if(playerId == 1) {
                append_node(&player1, reward->characterId, reward->score);
            } else {
                append_node(&player2, reward->characterId, reward->score);
            }
            delete_node(&discard, reward->characterId, reward->score);
            reward = reward->next;            
        }
        printf("到战利品区\n");
    }
}
void handleY()
{
    bag_t *BX = findNodeByChara(hand, 5);
    if(BX != NULL) {
        printf("可用钥匙打开宝箱%d\n", BX->score);
        boxReward();

    } else {
        printf("当你抽到宝箱后，可触发奖励\n");
    }
}

void handleB()
{
    bag_t *YS = findNodeByChara(hand, 4);
    if(YS != NULL) {
        printf("宝箱可用钥匙%d打开\n", YS->score);
        boxReward();
    } else {
        printf("当你抽到钥匙后，可触发奖励\n");
    }
}
void handleG()
{    
    bag_t *tempPlayer = NULL;
    if(playerId == 1) {
        tempPlayer = player1;
    } else {
        tempPlayer = player2;
    }
    int PlayCnt = getNodeCnt(tempPlayer);
    if(PlayCnt == 0) {
        printf("战利品区为空，无法钩子效果\n");
        return;
    }    
    printf("当前战利品区有 ");
    printBag(tempPlayer);
    printf("请选择战利品区的一张牌，将其放入手牌\n");
REINPUT:{
    char input[2];
    scanf("%s", input);
    int characterId = getIntFromAlphabet(input[0]);
    int score = input[1] - '0';
    bag_t *tmp = findNodeByCharaAndScore(tempPlayer, characterId, score);
    if(tmp != NULL) {
        append_node(&hand, characterId, score);
        if(playerId == 1) {
            delete_node(&player1, characterId, score);
        } else {
            delete_node(&player2, characterId, score);
        }
        if(((bitMask >> characterId) & 1) == 0) {
            bitMask |= 1 << characterId;
        } else {
            //勾取的牌导致爆炸
            flag = -1;
            handleBoom();
        }
        handleCard(characterId, score, 0);
        if(flag == -1) {
            printf("勾取的牌导致爆炸\n");
        }
    } else {
        printf("未找到该牌，请重新输入\n");
        goto REINPUT;
    }
}
}

void handleT()
{
    printf("从弃牌堆的三张牌中选择一张，加入你的手牌\n");
    int discardCnt = getNodeCnt(discard);
    bag_t *reward = discard;
    if(discardCnt == 0) {
        printf("没有弃牌，无法触发藏宝图效果\n");
        return;
    } else if(discardCnt < 3){
        printf("弃牌区仅有%d张牌，请选择一张牌\n", discardCnt);
        printBag(discard);
        printf("\n");
    } else {
        int cnt = 3;
        while(cnt--) {
            printCard(reward->characterId, reward->score);
            reward = reward->next;
        }   
        printf("\n");       
    }
REINPUT:{
    char input[2];
    scanf("%s", input);
    int characterId = getIntFromAlphabet(input[0]);
    int score = input[1] - '0';
    bag_t *tmp = findNodeByCharaAndScore(discard, characterId, score);
    if(tmp != NULL) {
        printf("恭喜获取");
        printCard(tmp->characterId, tmp->score);
        printf("到手牌\n");
        append_node(&hand, characterId, score);
        delete_node(&discard, characterId, score);
        if(((bitMask >> characterId) & 1) == 0) {
            bitMask |= 1 << characterId;
        } else {
            flag = -1;
            handleBoom();
        }
        handleCard(characterId, score, 0);
        if(flag == -1) {
            printf("选择的牌导致爆炸\n");
        }
    }else{
        printf("未找到该牌，请重新输入\n");
        goto REINPUT;
    }
}
}

void handleD()
{   
    bag_t *tempPlayer;
    if(playerId == 1) {
        tempPlayer = player2;
    } else {
        tempPlayer = player1;
    }  
    int PlayCnt = getNodeCnt(tempPlayer);
    if(PlayCnt == 0) {
        printf("对方战利品区为空，无法抢劫\n");
        return;
    }
    printf("对方战利品区有 ");
    printBag(tempPlayer);
    printf("请选择对方战利品区的一张牌，将其放入你的手牌\n");
REINPUT:{
    char input[2];
    scanf("%s", input);
    int characterId = getIntFromAlphabet(input[0]);
    int score = input[1] - '0';
    bag_t *tmp = findNodeByCharaAndScore(tempPlayer, characterId, score);
    if(tmp != NULL) {
        printf("恭喜获取");
        printCard(tmp->characterId, tmp->score);
        printf("到手牌\n");
        append_node(&hand, tmp->characterId, tmp->score);
        if(playerId == 1) {
            delete_node(&player2, characterId, score);
        } else {
            delete_node(&player1, characterId, score);
        }
        if(((bitMask >> characterId) & 1) == 0) {
            bitMask |= 1 << characterId;
        } else {
            flag = -1;
            handleBoom();
        }
        handleCard(characterId, score, 0);
        if(flag == -1) {
            printf("选择的牌导致爆炸\n");
        }
    }else{
        printf("未找到该牌，请重新输入\n");
        goto REINPUT;
    }
}
}

void handleP()
{
    bag_t *tempPlayer;
    if(playerId == 1) {
        tempPlayer = player2;
    } else {
        tempPlayer = player1;
    }  
    int PlayCnt = getNodeCnt(tempPlayer);
    if(PlayCnt == 0) {
        printf("对方战利品区为空，无法炮击\n");
        return;
    }
    printf("对方战利品区有 ");
    printBag(tempPlayer);
    printf("请选择对方战利品区的一张牌，炮击将其丢入弃牌区\n");
REINPUT:{
    char input[2];
    scanf("%s", input);
    int characterId = getIntFromAlphabet(input[0]);
    int score = input[1] - '0';
    bag_t *tmp = findNodeByCharaAndScore(tempPlayer, characterId, score);
    if(tmp != NULL) {
        printf("击破对方战利品");
        printCard(tmp->characterId, tmp->score);
        printf("到弃牌区\n");
        append_node(&discard, characterId, score);
        delete_node(&tempPlayer, characterId, score);
    }else{
        printf("未找到该牌，请重新输入\n");
        goto REINPUT;
    }
}
}

void handleCard(int characterId, int newScore, int printFlag)
{
    if(printFlag) printf("抽到");
    //如果手牌没爆，触发效果。
    switch (characterId)
    {
    case 0:
        if(printFlag) printf("美人鱼M%d\n", newScore);
        if(flag == 1) handleM(newScore);
        break;
    case 1:
        if(printFlag) printf("船锚C%d\n", newScore);
        if(flag == 1) handleC();
        break;
    case 2:
        if(printFlag) printf("占卜球Z%d\n", newScore);
        if(flag == 1) handleZ();
        break;
    case 3:
        if(printFlag) printf("海怪H%d\n", newScore);
        if(flag == 1) handleH();
        break;
    case 4:
        if(printFlag) printf("钥匙Y%d\n", newScore);
        if(flag == 1) handleY();
        break;
    case 5:
        if(printFlag) printf("宝箱B%d\n", newScore);
        if(flag == 1) handleB();
        break;
    case 6:
        if(printFlag) printf("钩子G%d\n", newScore);
        if(flag == 1) handleG();  
        break;
    case 7:
        if(printFlag) printf("藏宝图T%d\n", newScore);
        if(flag == 1) handleT();
        break;
    case 8:
        if(printFlag) printf("刀D%d\n", newScore);
        if(flag == 1) handleD();
        break;
    case 9:
        if(printFlag) printf("炮弹P%d\n", newScore);
        if(flag == 1) handleP();
        break;
    default:
        break;
    }
}

void handleBoom()
{
    bag_t *temp = findNodeByChara(hand, 1);
    if(temp != NULL) {
        if(hand->characterId != 1) {
            //有船锚，将船锚及其之后的结点放入弃牌堆
            bag_t *temp2 = hand;
            while(temp2){
                while(temp2 && temp2->characterId != 1) {
                    temp2 = temp2->next;
                }
                while(temp2 != NULL) {
                    append_node(&discard, temp2->characterId, temp2->score);
                    delete_node(&hand, temp2->characterId, temp2->score);
                    temp2 = temp2->next;            
                }
            } 
            return;              
        }         
    }
    //没有船锚(或者船锚是第一张)，删除手牌所有牌，弃牌堆放入手牌
    add_bag(&discard, hand);
    free_list(&hand);
    hand = NULL;
}
void getCard()
{
    int characterId, newScore;
    int storeCnt = getNodeCnt(store);
    if(storeCnt == 0) {
        printf("牌库为空\n");
        flag = 0;
        return;
    } else {
        printf("牌库卡牌剩余%d张\n", storeCnt);
    }
    if(getNodeCnt(hand) > 0) {
        printf("当前手牌有");
        printBag(hand);  
    }
    
    //拿到总牌库的第一张卡作为手牌。
    bag_t *nextCard = store;
    characterId = nextCard->characterId;
    newScore = nextCard->score;
    append_node(&hand, characterId, newScore);
    delete_node(&store, characterId, newScore);
    if(((bitMask >> characterId) & 1) == 0) {
        bitMask |= 1 << characterId;
    } else {
        flag = -1;
        handleBoom();
    }
    handleCard(characterId, newScore, 1);
    if(flag == -1) {
        printf("boom!\n");
    }
}
int onePlayer()
{
    while(1) {
        getCard();
        if(flag != 1) {
            break;
        } else {
            int input = 0;
            printf("是否继续抽卡？[1/0]\n");
            scanf("%d", &input);
            if (input == 0) {
                flag = 0;
                break;
            }            
        }
    }
    if(hand != NULL) {
        if(playerId == 1) {
            add_bag(&player1, hand); 
        } else {
            add_bag(&player2, hand);
        }
        printf("本回合最终手牌有 ");
        printBag(hand);           
        free_list(&hand);  
        hand = NULL;      
    } else {
        printf("本回合最终手牌为空\n");
    }
}

int checkResult()
{
    int result0 = 0;
    int result1 = 0;
    bag_t* temp = player1;
    while (temp != NULL) {
        result0 += temp->score;
        temp = temp->next;
    }
    printf("玩家1的最终分数为:%d\n", result0);
    temp = player2;
    while (temp != NULL) {
        result1 += temp->score;
        temp = temp->next;
    }
    printf("玩家2的最终分数为:%d\n", result1);
    return result0>result1? 1:2;
}

void createStore(int **storeArr)
{ 
    int *tmp = malloc(sizeof(int) * 10 * CARD_CNT);
    for(int j = 0; j < 10; j++) {
        for(int i = 0; i < CARD_CNT; i++) {
            int score = generateRandomInRange(2, 7);
            tmp[j * 5 + i] = (score & 0xf) + (j << 4);
        }
    }
    *storeArr = tmp;
}

//打乱牌库
void shuffleStore(int **storeArr)
{
    
    int *tmp = (int *)(*storeArr);
    int c = 0;
    //先洗前一半
    for(int i = 0; i < 100 * CARD_CNT; i++) {   
        int a = generateRandomInRange(0, 5 * CARD_CNT - 1);
        int b = generateRandomInRange(0, 5 * CARD_CNT - 1);
        c = tmp[a];
        tmp[a] = tmp[b];
        tmp[b] = c;
    }
    //再洗后一半
    for(int i = 0; i < 100 * CARD_CNT; i++) {   
        int a = generateRandomInRange(5 * CARD_CNT, 10 * CARD_CNT - 1);
        int b = generateRandomInRange(5 * CARD_CNT, 10 * CARD_CNT - 1);
        c = tmp[a];
        tmp[a] = tmp[b];
        tmp[b] = c;
    }   
    //洗中间
    for(int i = 0; i < 100 * CARD_CNT; i++) {   
        int a = generateRandomInRange(3 * CARD_CNT, 7 * CARD_CNT - 1);
        int b = generateRandomInRange(3 * CARD_CNT, 7 * CARD_CNT - 1);
        c = tmp[a];
        tmp[a] = tmp[b];
        tmp[b] = c;
    } 
    //整体混洗
    for(int i = 0; i < 100 * CARD_CNT; i++) {  
        int a = generateRandomInRange(0, 10 * CARD_CNT - 1);
        int b = generateRandomInRange(0, 10 * CARD_CNT - 1);
        c = tmp[a];
        tmp[a] = tmp[b];
        tmp[b] = c;
    }
    for(int i = 0; i < 10 * CARD_CNT; i++) {
        append_node(&store, tmp[i] >> 4, tmp[i] & 0xf);
    }
}

//打印总牌库打乱情况，确认效果
void printArray(int *arr, int size)
{
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i] >> 4);
    }
    printf("\n");
}
int main()
{
    //设置随机数种子
    srand(time(NULL));
    //生成牌库
    int *storeArr = NULL;
    createStore((int**)&storeArr);
    // printArray(storeArr, 10 * CARD_CNT);
    //打乱牌库
    shuffleStore((int**)&storeArr);
    // printArray(storeArr, 10 * CARD_CNT);
    printf("开始游戏\n");
    while(1) {
        printf("第一位玩家开始抽牌\n");
        playerId = 1;
        onePlayer();
        printf("当前战利品有 ");
        printBag(player1);
        flag = 1;
        bitMask = 0;
        printf("=====================\n");
        printf("\n\n\n第二位玩家开始抽牌\n");
        playerId = 2;
        onePlayer();  
        printf("当前战利品有 ");
        printBag(player2);
        flag = 1;
        bitMask = 0;
        printf("=====================\n\n\n\n"); 
        int storeCnt = getNodeCnt(store);
        if(storeCnt == 0) {
            printf("游戏结束\n");
            break;
        }
    }
    printf("最终获胜玩家为%d\n", checkResult());
}
