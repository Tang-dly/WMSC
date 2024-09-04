#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define CARD_CNT 5
// {"美人鱼", "船锚", "占卜球", "海怪", "钥匙", "宝箱", "钩子", "藏宝图", "刀", "炮弹"};
int bitMask = 0;
int score = 0;
int flag = 1;
// 占卜球   
int ZBQTYPE = 100;
int ZBQSCORE = 0;

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
}

// 打印链表
void print_list(bag_t* head) {
    bag_t* temp = head;
    while (temp != NULL) {
        printf("CharacterId: %d, Score: %d\n", temp->characterId, temp->score);
        temp = temp->next;
    }
}

// 释放链表
void free_list(bag_t* head) {
    bag_t* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
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

bag_t* findNodeByIndex(int index)
{
    bag_t *temp = store;
    while(index--) {
        temp = temp->next;
    }
    return temp;
}

bag_t* findFirstNodeByChara(int id)
{
    bag_t *temp = store;
    while(temp != NULL) {
        if(temp->characterId == id) {
            break;
        }
        temp = temp->next;
    }
    return temp;
}
void add_bag(bag_t **bag, bag_t *new)
{
    bag_t *temp2 = new;
    while (temp2 != NULL) {
        append_node(bag, temp2->characterId, temp2->score);
        temp2 = temp2->next;
    }
    return;
}
int generateRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

void handleM()
{

}

void handleC()
{

}

void handleZ()
{
    int storeCnt = getNodeCnt(store);
    int tmp = generateRandomInRange(0, storeCnt - 1);
    bag_t *node = findNodeByIndex(tmp);
    ZBQTYPE = node->characterId;
    ZBQSCORE = node->score;
    printf("占卜球看到下张牌为:");
    switch(ZBQTYPE) {
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
            printf("钥匙\n");
            break;
        case 4:
            printf("宝箱\n");
            break;
        case 5: 
            printf("刀\n");
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

}

void handleY()
{

}

void handleB()
{
    
}
void handleG()
{

}

void handleT()
{

}

void handleD()
{
    
}

void handleP()
{
    
}
void getCard(bag_t **bag)
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
    if(ZBQTYPE == 100) {
        int index = 0;
        index = generateRandomInRange(0, storeCnt-1);
        bag_t *node = findNodeByIndex(index);
        characterId = node->characterId;
        newScore = node->score;
    } else {
        characterId = ZBQTYPE;
        newScore = ZBQSCORE;
        ZBQTYPE = 100;
    }

    switch (characterId)
    {
    case 0:
        printf("美人鱼%d\n", newScore);
        handleM();
        break;
    case 1:
        printf("船锚%d\n", newScore);
        handleC();
        break;
    case 2:
        printf("占卜球%d\n", newScore);
        handleZ();
        break;
    case 3:
        printf("海怪%d\n", newScore);
        handleH();
        break;
    case 4:
        printf("钥匙%d\n", newScore);
        handleY();
        break;
    case 5:
        printf("宝箱%d\n", newScore);
        handleB();
        break;
    case 6:
        printf("钩子%d\n", newScore);
        handleG();  
        break;
    case 7:
        printf("藏宝图%d\n", newScore);
        handleT();
        break;
    case 8:
        printf("刀%d\n", newScore);
        handleD();
        break;
    case 9:
        printf("炮弹%d\n", newScore);
        handleP();
        break;
    default:
        break;
    }
    if(((bitMask >> characterId) & 1) == 0) {
        bitMask |= 1 << characterId;
        append_node(bag, characterId, newScore);
        delete_node(&store, characterId, newScore);
    } else {
        printf("boom!\n");
        flag = 0;
        add_bag(&store, *bag);
        free_list(*bag);
        *bag = NULL;
        return;
    }
}
int onePlayer(int player)
{
    bag_t *tmpBag = NULL;
    while(1) {
        getCard(&tmpBag);
        if(flag == 0) {
            break;
        } else {
            printf("是否继续抽卡？[1/0]\n");
            scanf("%d", &flag);
            if (flag == 0)
            {
                break;
            }            
        }
    }
    if(tmpBag != NULL) {
        if(player == 1) {
            add_bag(&player1, tmpBag); 
        } else {
            add_bag(&player2, tmpBag);
        }
        free_list(tmpBag);        
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
            score = generateRandomInRange(2, 7);
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
    for(int i = 0; i < 100 * CARD_CNT; i++) {    //十倍次数洗牌
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
int main()
{
    //设置随机数种子
    srand(time(NULL));
    //生成牌库
    int *storeArr = NULL;
    createStore((int**)&storeArr);
    //打乱牌库
    shuffleStore((int**)&storeArr);
    printf("开始游戏\n");
    while(1) {
        printf("第一位玩家开始抽牌\n");
        onePlayer(1);
        flag = 1;
        ZBQTYPE = 100;
        bitMask = 0;
        printf("=====================\n");
        printf("\n\n\n第二位玩家开始抽牌\n");
        onePlayer(2);  
        flag = 1;
        ZBQTYPE = 100;
        bitMask = 0;
        printf("=====================\n"); 
        int storeCnt = getNodeCnt(store);
        if(storeCnt == 0) {
            printf("游戏结束\n");
            break;
        }
    }
    printf("最终获胜玩家为%d\n", checkResult());
}
