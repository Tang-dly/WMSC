#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// {"美人鱼", "船锚", "占卜球", "海怪", "钥匙", "宝箱", "钩子", "藏宝图", "刀", "炮弹"};
int bitMask = 0;
int score = 0;
int flag = 1;
int ZBQ = 100;

typedef struct Bag {
    int characterId;
    int score;
    struct Bag *next;
} bag_t;

bag_t *player1 = NULL;
bag_t *player2 = NULL;

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
void delete_node(bag_t** head, int characterId) {
    bag_t* temp = *head;
    bag_t* prev = NULL;
    
    // 如果头节点需要删除
    if (temp != NULL && temp->characterId == characterId) {
        *head = temp->next; // 头节点被删除
        free(temp);
        return;
    }

    // 搜索待删除的节点
    while (temp != NULL && temp->characterId != characterId) {
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

void add_bag(bag_t **bag, bag_t *new)
{
    bag_t* temp = *bag;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new;
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
    ZBQ = generateRandomInRange(0, 9);
    printf("占卜球看到下张牌为 \n");
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
void getCard(bag_t *bag)
{
    int characterId;
    if(ZBQ == 100) {
        characterId = generateRandomInRange(0, 9);
    } else {
        characterId = ZBQ;
    }
    int newScore = generateRandomInRange(2, 7);
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
    if((bitMask >> characterId) & 1 == 0) {
        bitMask |= 1 << characterId;
        append_node(&bag, characterId, newScore);
    } else {
        printf("boom!\n");
        flag = 0;
        return;
    }
}
int onePlayer()
{
    bag_t *tmpBag = NULL;
    while(1) {
        getCard(tmpBag);
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
    add_bag(&player1, tmpBag);
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
int main()
{
    printf("开始游戏\n");
    printf("第一位玩家开始抽牌\n");
    onePlayer();
    printf("第二位玩家开始抽牌\n");
    onePlayer();
    printf("最终获胜玩家为%d", checkResult());
}
