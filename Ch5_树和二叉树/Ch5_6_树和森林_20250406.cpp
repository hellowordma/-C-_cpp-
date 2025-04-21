/***********************************************************
 * 第5章 树和二叉树
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏
// 2. 视频：b站视频《数据结构》by王卓

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
* 5.6 树和森林
 *==============================================================*/

/* -------------- 5.6.1 树的存储结构（常见3种）--------------- */

// 1. 树的双亲表示法
// 参考视频：第08周08--5.6树和森林1-树的存储结构1-双亲表示法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=95
// 参考课本：p133
#define MAX_TREE_SIZE 100 // 树的最大结点数
typedef struct PTNode{ // 树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    int parent; // 双亲指针，指向双亲结点在数组中的下标
} PTNode;
typedef struct PTree{ // 树的双亲表示法
    PTNode nodes[MAX_TREE_SIZE]; // 结点数组
    int n,r; // n为结点个数,类似于SqList的length，r为根结点的下标位置
} PTree; 

// 2. 树的孩子表示法(还有带双亲的孩子表示法)
// 参考视频：第08周09--5.6树和森林2-树的存储结构2-孩子表示法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=96
// 参考课本：p134
typedef struct CTNode{
    int child;
    struct CTNode *next; 
} CTNode, *ChildPtr; // CTNode是树结点类型，ChildPtr是指向树结点的指针类型。
typedef struct CTBox{ // 树的孩子表示法
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    ChildPtr firstchild; // 指向第一个孩子结点的指针
} CTBox;
typedef struct CTree{ // 树的孩子表示法
    CTBox nodes[MAX_TREE_SIZE]; // 结点数组
    int n,r; // n为结点个数,类似于SqList的length，r为根结点的下标位置
} CTree; // CTree是树的孩子表示法，nodes是结点数组，n是结点个数。

// 3. 树的孩子兄弟表示法（又称二叉树表示法，二叉链表表示法）(较为常用，因为可用到二叉树的操作）
// 参考视频：第08周10--5.6树和森林3-树的存储结构3-孩子兄弟表示法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=97
// 参考课本：p135
typedef struct CSNode{ // 树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    struct CSNode *firstchild, *nextsibling; // firstchild指向第一个孩子结点的指针，nextsibling指向下一个兄弟结点的指针
} CSNode, *CSTree; // CSNode是树结点类型，CSTree是指向树结点的指针类型。
// 与二叉树的链式存储结构类似，只不过BiTNode的两个指针分别指向lchild和rchild，
// 而CSNode的两个指针分别指向firstchild和nextsibling。

