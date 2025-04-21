/***********************************************************
 * 第7章 查找
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
* 7.3 树表的查找
 *==============================================================*/


/* -------------- 7.3.1 二叉排序树--------------- */

// 二叉排序树的二叉链表存储表示
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
 typedef struct ElemType{
    KeyType key; // 关键字类型
    InfoType otherinofo; // 其他数据域
} ElemType; // ElemType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。

typedef struct BSTNode{ // 二叉排序树结点类型
    ElemType data; // 数据域
    struct BSTNode *lchild, *rchild; // 左右孩子指针
} BSTNode, *BSTree; // BSTNode是二叉排序树结点类型，BSTree是指向二叉排序树结点的指针类型。

// 【算法7.4】二叉排序树的查找
// 参考视频：第11周03--7.3树表的查找3-2二叉排序树的查找 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=118
// 参考课本：p199
// 思路：递归左右子树查找。
// 时间复杂度：近似O(log2n)，最坏O(n)，平均O(log2n)。
// 近似O(log2n)是因为二叉排序树的高度大约是log2n，最坏O(n)是因为二叉排序树退化成链表（单支树），平均O(log2n)是因为二叉排序树的平均高度大约是log2n。

BSTree SearchBST(BSTree T, KeyType key) { // 二叉排序树的查找
    // 在根指针T所指的二叉排序树中查找关键字为key的结点，返回该结点的指针
    if((!T) || (key == T->data.key)) return T; // 如果树为空或找到关键字，返回该结点的指针
    // 如果T==NULL，则也返回NULL，表示没有找到关键字。
    else if(key < T->data.key) return SearchBST(T->lchild, key); // 如果关键字小于根结点的关键字，递归查找左子树
    else return SearchBST(T->rchild, key); // 如果关键字大于根结点的关键字，递归查找右子树
} 


