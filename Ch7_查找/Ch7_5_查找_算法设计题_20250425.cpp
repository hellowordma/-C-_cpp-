/***********************************************************
 * 第7章 查找
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏 p232-p233.
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p157-p161.

//导入相关的头文件
# include <stdio.h> // 标准输入输出流
# include <iostream> // 输入输出流
# include <fstream>  // 文件输入输出流
# include <cstring>  // 字符串操作函数
using namespace std; // 使用标准命名空间   
// Status定义
#define OK 1
#define ERROR 0 
#define OVERFLOW -2
#define True 1
#define False 0
typedef int Status; // Status是函数的类型，其值是函数结果状态代码


/*==============================================================
* 算法设计题（见习题集)
 *==============================================================*/


 /* ---------- Start of 定义 ---------- */
 //  主要参考：D:\MacBook迁移\2025互联网准备\C++\数据结构_王卓_20250320\Ch6_图\Ch7_查找  目录下的
//   - Ch7_2_线性表的查找_20250421.cpp 
//   - Ch7_3_树表的查找_20250421.cpp
//   - Ch7_4_散列表的查找_20250423.cpp
//   - 以及  Ch2_线性表/Ch2_5_线性表的l链表实现_20250321.cpp

typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
 typedef struct ElemType{
    KeyType key; // 关键字类型
    InfoType otherinofo; // 其他数据域
} ElemType; // ElemType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。

typedef struct SSTable{
    ElemType *R; // 线性表的元素数组
    int length; // 线性表的长度
}SSTable; // SSTable是顺序表，包含关键字和其他数据域。ElemType是线性表的元素类型，包含关键字和其他数据域。

// 二叉排序树的二叉链表存储表示
typedef struct BSTNode{ // 二叉排序树结点类型
    ElemType data; // 数据域
    struct BSTNode *lchild, *rchild; // 左右孩子指针
} BSTNode, *BSTree; // BSTNode是二叉排序树结点类型，BSTree是指向二叉排序树结点的指针类型。

// 开放地址法，散列表的存储表示
#define m 20 // 散列表的表长
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型

typedef struct LNode {
    KeyType data;
    InfoType otherinfo;
    struct LNode *next;
} LNode, *LinkList; // 链表结点类型, 适配散列表的链地址法（拉链法）。

// 散列表定义（数组的每个元素是链表头指针）
typedef LNode* HashTable[m]; // 每个槽是 LNode*（即链表的头指针），适用于散列表的链地址法（拉链法）。

// 辅助函数：散列函数（除留余数法）
int Hash(KeyType key){
    // 散列函数，计算散列地址
    int n = 19; // n一般取≤散列表表长m的质数
    return key % n; // 除留余数法。
}


 /* ---------- End of 定义 ---------- */

//  1. 试写出折半查找的递归算法
// 参考习题:p157

// 递归实现折半查找（额外多了2个参数 low和high）
int Search_Bin_Recursion(SSTable ST, KeyType key, int low, int high) { // 折半查找
    if (low > high) return 0; // 没有找到，返回0
    int mid = low + ( high - low ) / 2; // 计算中间位置，防止溢出
    if (ST.R[mid].key == key) return mid; // 找到关键字，返回下标
    else if (ST.R[mid].key > key) return Search_Bin_Recursion(ST, key, low, mid - 1); // 在左半部分查找
    else return Search_Bin_Recursion(ST, key, mid + 1, high); // 在右半部分查找
}

// 2. 试写一个判别给定二叉树是否为二叉排序树的算法
// 思路：中序遍历递归，标志
// 参考习题:p157-p158
BSTNode *pre = NULL; // 全局变量，用于记录前驱结点，初始化为NULL（根节点的前驱结点）
void JudgeBST(BSTree T, int &flag){
    // 判断二叉树是否为二叉排序树，flag初值为1。若为二叉排序树，则flag保持为1；否则，flag置为0。
    if (T != NULL&&flag){ // 如果当前结点不为空且flag为1
        // 中序遍历,保持前驱结点的值小于当前结点的值，否则，flag置为0
        JudgeBST(T->lchild, flag); // 判断左子树
        if(pre==NULL) pre = T; // 如果前驱结点为空，则当前结点为前驱结点.中序遍历的第一个结点不必比较（此时相当于走到了最左边也就是最小的结点）
        else if (pre->data.key < T->data.key) pre = T; // 如果前驱结点小于当前结点，则更新前驱结点为当前结点
        else flag = 0; return; // 否则，flag置为0，表示不是二叉排序树。并且返回，结束该函数（不用再判断之后的rchild了）
        JudgeBST(T->rchild, flag); // 判断右子树
    }
}

// 3. 已知二叉排序树采用二叉链表存储结构,根节点指针为T，链结点的结构为(lchild,data,rchild),其中lchild、rchild 分别指
//      向该结点左、右孩子的指针，data域存放结点的数据，请写出递归算法，从小到大输出二叉排序树中所有数据值≥x的结点的数据，
//      要求先找到第一个满足条件的结点，然后依次输出其他满足条件的结点。
// 思路：中序遍历，递归。（我觉得答案有问题，不如直接最简单的）
// 参考习题:p158-p159

void PrintAllx(BSTree T, KeyType x){
    // 输出二叉排序树中所有数据值≥x的结点的数据
    if(T){ // 如果当前结点不为空，则继续遍历
        if(T->data.key>=x){ // 如果当前结点的数据值≥x，则
            PrintAllx(T->lchild, x); // 递归左子树（毕竟根节点的key是大于lchild的key的，所以如果连根节点都小于x，lchild也一定小于x）
            cout<<T->data.key<<" "; // 输出当前结点的数据
        }
        PrintAllx(T->rchild, x); // 递归右子树
    }
}

// 4. 已知二叉树T的结点形式为(lchild,data,count,rchild),在树中查找值为x的结点，若找到则记数(count)加1;否则，
// 作为一个新结点插入树中，插人后仍为二叉排序树。写出其非递归算法。
// 思路： while遍历迭代，并设置双亲节点（相当于链表的前驱节点）方便之后接入lchild或rchild。
// 参考习题:p159

// 定义一个带计数的二叉排序树结点类型BSTNode_count
typedef struct BSTNode_count{
    ElemType data;
    int count;
    struct BSTNode_count *lchild, *rchild;
}BSTNode_count, *BSTree_count;

// 正式开始
void SearchBST(BSTree_count &T, KeyType X){
    // 在二叉排序树中查找值为X的结点，若找到则记数(count)加1;否则，作为一个新结点插入树中。插人后仍为二叉排序树。
    if(!T){ // 如果T为空，则s作为根结点插入, 结束该函数
        // 新建一个结点s，初始化count为1，data为X，左右孩子为空
        BSTNode_count *s = new BSTNode_count; // 定义一个新结点s
        s->data.key = X; 
        s->count = 1; // 初始化计数为1（书上是0，但我觉得是1）
        s->lchild = s->rchild = NULL; 
        T = s; // 将s作为根结点插入
        return; // 结束该函数
    }
    BSTNode_count *q = NULL, *p = T; // f为q的双亲结点,初始化为NULL。q为当前结点，初始化为根结点。
    while(p){ // 如果q不为空，则继续查找
        if(p->data.key == X){ // 如果找到值为X的结点，则count加1，结束该函数。
            p->count++;
            return;
        }
        q = p; // 更新双亲结点
        if(X < p->data.key) p = p->lchild; // 如果X小于当前结点的关键字，则向左子树查找
        else p = p->rchild; // 否则，如果X大于当前结点的关键字，则向右子树查找
    } 
    // 如果没有找到值为X的结点，则插入一个新结点s
    BSTNode_count *s = new BSTNode_count; // 定义一个新结点s
    s->data.key = X; 
    s->count = 1; // 初始化计数为1（书上是0，但我觉得是1）
    s->lchild = s->rchild = NULL; 
    if(q->data.key > X) q->lchild = s; // 如果X小于双亲结点的关键字，则将s插入到左子树 
    else q->rchild = s; // 否则，将s插入到右子树 
} 


// 5. 假设一棵平衡二叉树的每个结点都标明了平衡因子b，试设计一个算法，求平衡二叉树高度。
// 思路：遍历或者递归
// 参考习题:p159-p160

// 定义一个带平衡因子的二叉排序树结点类型BSTNode_BF
typedef struct BSTNode_BF{
    int b; // 平衡因子。只有3种取值：-1、0、1，分别表示左子树比右子树低1、相等、高1
    KeyType data;
    struct BSTNode_BF *lchild, *rchild;
} BSTNode_BF, *BSTree_BF;

// 正式开始
int Height(BSTree_BF T){
    // 根据平衡因子求平衡二叉树的高度
    int level = 0; // 初始化高度为0
    BSTNode_BF *p = T; // 定义一个指针p，指向T
    while(p){
        ++level; // 每次循环高度加1
        if(p->b<0) p->rchild; // 如果平衡因子小于0，则向右子树移动
        else p->lchild; // 如果平衡因子大于等于0，则向左子树移动
    }
    return level; // 返回高度
}

// 我的方法:递归
int Height_custom(BSTree_BF T){
    // 根据平衡因子求平衡二叉树的高度
    if(T==NULL) return 0; // 如果T为空，则返回0
    if(T->b==0) return Height_custom(T->lchild)+1; // 如果b==0，则返回左子树的高度加1
    else if(T->b==1) return Height_custom(T->rchild)+1+1; // 如果b==1，则返回右子树的高度加1再加1
    else return Height_custom(T->lchild)+1+1; // 如果b==-1，则返回左子树的高度加1再加1
}

// 6.分别写出在散列表中插入和删除关键字为K的一个记录的算法，设散列函数为H,解决冲突的方法为链地址法。
// 思路：用到链表的插入和删除。
// 参考习题:p160-p161

bool Insert_K(HashTable HT, KeyType K){
    // 在散列表中插入关键字为K的一个记录
    int i = Hash(K);
    LNode *p = HT[i]; // 定义一个指针p，指向散列表的第i个元素
    while(p->next){ // 如果p的下一个元素不为空，则继续查找
        if(p->next->data==K) return false; // 如果关键字已经存在，则返回false
        p = p->next; // 否则，继续查找
    }
    // 使用尾插法插入新节点。
    LNode *s = new LNode; // 创建一个新的节点
    s->data=K; // 将关键字赋值给新节点
    s->next = p->next; // 将新节点插入到链表的末尾。此时p->next == NULL。
    p->next = s; // 更新链表指针
    return true; // 返回true，表示插入成功
}

bool Delete_K(HashTable HT, KeyType K){
    // 在散列表中删除关键字为K的一个记录
    int i = Hash(K);
    LNode *p = HT[i]; // 定义一个指针p，指向散列表的第ant个元素
    while(p->next){ // 如果p的下一个元素不为空，则继续查找
        if(p->next->data==K){ // 如果关键字存在，则删除
            LNode *s = p->next; // 定义一个指针s，指向要删除的节点
            p->next = s->next; // 将s的前一个节点的next指针指向s的下一个节点
            delete s; // 删除s节点
            return true; // 返回true，表示删除成功
        }
        p = p->next; // 否则，继续查找
    }
    return false; // 如果关键字不存在，则返回false
}