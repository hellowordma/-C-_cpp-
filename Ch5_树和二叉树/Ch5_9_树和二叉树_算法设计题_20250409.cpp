/***********************************************************
 * 第5章 树和二叉树
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏；p147-p148；
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p92；


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


/*==================================
* 算法设计题（见习题集)
 *==================================*/

 

 /* ---------- Start of 定义 ---------- */
//  主要参考：D:\MacBook迁移\2025互联网准备\C++\数据结构_王卓_20250320\Ch5_树和二叉树\Ch5_4and5_二叉树的性质和存储结构and遍历二叉树和线索二叉树_20250406.cpp

// 二叉树的链式存储（二叉链表）
typedef struct BiTNode{ // 二叉树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    struct BiTNode *lchild, *rchild; // 左右孩子指针
} BiTNode, *BiTree; // BiTNode是二叉树结点类型，BiTree是指向二叉树结点的指针类型。


#define MAXQSIZE 100 // 最大队列长度
typedef struct SqQueue_btree{ // 定义顺序队列
    BiTNode* data[MAXQSIZE]; // 队列元素类型，改为存储BiTNode*类型的指针
    int front, rear; // 队头和队尾指针
}SqQueue_btree; // front指向队头元素，rear指向队尾元素的下一个位置。注意front和rear都是下标，从0开始。

Status InitQueue_btree(SqQueue_btree &Q){
    // Q.data 已经是一个固定大小的数组，因此不需要动态分配内存。
    Q.front = Q.rear = 0; // 头指针和尾指针置为0，表示队列为空
    return OK;
}
Status EnQueue_btree(SqQueue_btree &Q, BiTNode *e){  // BiTNode*本来应该是QElemType
    if((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR; // 队列已满
    Q.data[Q.rear] = e; // 新元素插入队尾，直接存储指针
    Q.rear = (Q.rear + 1) % MAXQSIZE; // 队尾指针+1，若到最后则转到数组头部。因此不能直接++Q.rear。

    return OK;
}
Status DeQueue_btree(SqQueue_btree &Q, BiTNode *&e){ // BiTNode*本来应该是QElemType
    if(Q.front == Q.rear) return ERROR; // 队列为空
    e = Q.data[Q.front]; // 保存队头元素，直接获取指针
    Q.front = (Q.front + 1) % MAXQSIZE; // 队头指针+1，若到最后则转到数组头部
    return OK;
}

Status QueueEmpty_btree(SqQueue_btree Q){ // 判断队列空
    if (Q.front == Q.rear) return 1; // 队列空
    else return 0; // 队列非空
}
 /* ---------- End of 定义 ---------- */

//  1. 以二叉链表作为二叉树的存储结构，统计二叉树的叶子结点个数。
// 思路：递归
//  - 参考习题：p103;

int LeafNode(BiTree T){
    if (T == NULL) return 0; // 如果二叉树为空，返回0
    if (T->lchild == NULL && T->rchild == NULL) return 1; // 如果是叶子结点，返回1
    else return LeafNode(T->lchild) + LeafNode(T->rchild); // 递归统计左子树和右子树的叶子结点个数
}

// 2. 以二叉链表作为二叉树的存储结构，判断两棵树是否相同。
// 思路：递归
//  - 参考习题：p103;
int CmpTree(BiTree T1, BiTree T2){
    if (T1 == NULL && T2 == NULL) return 1; // 如果两棵树都为空，返回1
    if (T1 == NULL || T2 == NULL) return 0; // 如果其中一棵树为空，返回0
    if (T1->data != T2->data) return 0; // 如果根结点不相同，返回0
    return CmpTree(T1->lchild, T2->lchild) && CmpTree(T1->rchild, T2->rchild); // 递归判断左子树和右子树是否相同
}

// 3. 以二叉链表作为二叉树的存储结构，交换二叉树每个结点的左右孩子。
// 思路：递归
//  - 参考习题：p103-p104;
void ChangeLR(BiTree &T) {
    if (T == NULL) return; // 如果二叉树为空，不用交换，直接返回

    BiTNode *temp = T->lchild; // 临时变量保存左孩子
    T->lchild = T->rchild; // 交换左右孩子
    T->rchild = temp; 

    ChangeLR(T->lchild); // 递归交换左子树的左右孩子
    ChangeLR(T->rchild); // 递归交换右子树的左右孩子
}

// 4. 以二叉链表作为二叉树的存储结构，设计二叉树的双序遍历算法。
// （双序遍历是指对于二叉树的每个结点来说，先访问这个结点，然后再按双序遍历它的左子树
// 然后再一次访问这个结点，然后再按双序遍历它的右子树。）

// 思路：递归
//  - 参考习题：p104；
void DoubleOrderTraverse(BiTree T) {
    if (T == NULL) return; // 如果二叉树为空，直接返回

    cout << T->data << " "; // 先访问结点
    DoubleOrderTraverse(T->lchild); // 递归遍历左子树
    cout << T->data << " "; // 再访问结点
    DoubleOrderTraverse(T->rchild); // 递归遍历右子树
}

// 5. 以二叉链表作为二叉树的存储结构，计算二叉树的最大宽度（即二叉树所有层中结点个数的最大值）
// 思路：层序遍历+队列
//  - 参考习题：p104-p105;

int MaxWidth(BiTree T){
    // 求二叉树所有层的最大宽度
    if(T==NULL) return 0; //  如果二叉树为空，则返回0
    BiTree Q[MAXQSIZE]; // 定义一个顺序队列，用于存储二叉树的结点
    int front=1, rear=1; // 队列的头指针和尾指针，从1开始，而不是从0开始。
    int last=1; // last表示当前层的最右结点在队列中的位置，从1开始，而不是从0开始。之后会更新last，使其指向下一层的最右结点rear。
    int temp=0, maxw=0; // temp表示当前层的宽度，从0开始。maxw表示最大宽度。

    Q[rear] = T; // 根结点入队
    while(front<=rear){ // 队列不为空时循环
        BiTNode *p = Q[front++]; // 出队，并将front指针后移
        temp++; // 当前层的宽度加1
        if(p->lchild) Q[++rear] = p->lchild; // 非空左孩子入队
        if(p->rchild) Q[++rear] = p->rchild; // 非空右孩子入队
        if(front>last){ // 当前层遍历完毕
            last = rear; // 更新last为下一层的最右结点在队列中的位置
            if (temp>maxw) maxw = temp; // 更新最大宽度
                temp = 0; // 重置当前层的宽度
        }
    }
    return maxw;
}


// 6. 以二叉链表作为二叉树的存储结构，用按层次顺序遍历二叉树的方法，统计树种度尾1的结点数目
// 思路：层序遍历+队列
//  - 参考习题：p105-p106;
int CountDegree1(BiTree T) {
    int num = 0; // 初始化度为1的结点数目为0
    SqQueue_btree Q; // 定义队列
    if(T){
        InitQueue_btree(Q); // 初始化队列
        EnQueue_btree(Q, T); // 根结点入队
        while (!QueueEmpty_btree(Q)) { // 当队列不空时
            BiTNode *p; // 定义二叉树结点指针p
            DeQueue_btree(Q, p); // 出队一个结点
            if ((p->lchild == NULL && p->rchild != NULL)|| // 如果结点p的左孩子为空，右孩子不为空,或者
                (p->lchild != NULL && p->rchild == NULL)) { // 如果结点p的左孩子不为空，右孩子为空
                num++; // 度为1的结点数目加1
            }
            if (p->lchild != NULL) EnQueue_btree(Q, p->lchild); // 如果有左孩子，入队
            if (p->rchild != NULL) EnQueue_btree(Q, p->rchild); // 如果有右孩子，入队
        }
    }
    return num; // 返回度为1的结点数目
}

// 7. 以二叉链表作为二叉树的存储结构，求任意二叉树中第一条最长的路径的长度，并输出路径上的结点值。
// 思路：栈。
//  - 参考习题：p106;
# define MAXSSIZE 100 // 最大栈长度
void LongestPath(BiTree T){
    // 求二叉树种的第一条最长路径
    // 所有数组扩大一个位置以适应从1开始的索引，因为是从1开始。
    BiTNode *p = T, *l[MAXSSIZE+1], *s[MAXSSIZE+1]; // l数组用于记录最长路径上的结点,s数组用于记录当前路径上的结点。p为T也就是根结点。
    int tag[MAXSSIZE+1]; // tag数组用于记录s数组中结点的右分支是否被访问过（隐含了左分支也被访问过）。0表示未访问，1表示已访问。
    int top=0,longest=0; // top为s数组栈顶指针,longest为最长路径长度
    while(p || top){ // 当p不为空或栈不为空时循环
        while(p){ // 当p不为空时，将p入栈，并将p的左孩子入栈，直到p为空.相当于沿着左分支向下遍历到最左边的结点。
            s[++top] = p; // 将p入栈。之所以是++top，是因为后面要用到top作为下标，否则修改成 s[top++] = p;tag[top-1] = 0; 也可以。
            // 此时相当于s[1]存储了根节点。
            tag[top] = 0; // 将p标记为右节点未访问。此时tag[1] = 0，相当于标记了根节点的右结点为未访问。
            p = p->lchild; // 将p指向p的左孩子
        }
        if(tag[top] == 1){  // 当前结点的右分支已经访问过
            if(!s[top]->lchild && !s[top]->rchild)  // 只有到达叶子结点时才查看路径长度，并考虑是否更新最长路径长度
                if(longest < top){ // 如果当前路径长度大于最长路径长度
                    for(int i=1; i<=top; i++) // 保存当前最长路径到l栈，并更新最长路径长度
                        l[i] = s[i];
                    longest = top;  // top从1开始而不是从0开始，也是为了方便与longest比较。top此时就能表示路径长度了。
                }
            top--; // 出栈，相当于回溯到上一个结点
        }
        else{ // 当前结点的右分支未被访问过，则沿着右分支向下遍历
            p = s[top]->rchild; // 将p指向当前结点的右孩子。注意是p=s[top]->rchild，而不是p=p->rchild。因为此时p已经指向了当前结点的左孩子，所以需要重新赋值。
            tag[top] = 1; // 将当前结点标记为右分支已访问
        }
    }
} 

// 8. 以二叉链表作为二叉树的存储结构，输出二叉树中从每个叶子结点到根结点的路径。
// 思路：递归
//  - 参考习题：p106-p107;
void AllPath(BiTree T, int path[], int pathlen) { // int本来是TElemType。
    // path是路径数组，pathlen是路径长度,初始化为0（一般在main函数中调用AllPath之前初始化为0）
    if (T == NULL) return; // 如果二叉树为空，直接返回
    if (T->lchild == NULL && T->rchild == NULL) { // 如果是叶子结点
        cout<<" "<<T->data<<"到根节点的路径为："<<T->data; // 输出路径
        for (int i = pathlen - 1; i >= 0; i--) { // 从路径数组中逆序输出路径
            cout<<"->"<<path[i]; // 输出路径
        }
    }
    else{
        path[pathlen] = T->data; // 将当前结点值存入路径数组
        pathlen++; // 路径长度加1
        AllPath(T->lchild, path, pathlen); // 递归遍历左子树
        AllPath(T->rchild, path, pathlen); // 递归遍历右子树
        pathlen--; // 路径长度减1，恢复环境。
    }
}

// 9. 给出二叉树结点的数据类型定义，并设计WPL（带权路径长度）的计算算法。二叉树结点采用二叉链表存储，结点结构为(left,weight,right)
// 思路：递归
//  - 参考习题：p107;

int WPL(BiTree T, int depth) { // depth是当前结点的深度
    // T是二叉树的根结点, depth是当前结点的深度, 初始化为0（一般在main函数中调用WPL之前初始化为0）
    if (T == NULL) return 0; // 如果二叉树为空，返回0
    if (T->lchild == NULL && T->rchild == NULL) return T->data * depth; // 如果是叶子结点，返回带权路径长度
    return WPL(T->lchild, depth + 1) + WPL(T->rchild, depth + 1); // 递归计算左子树和右子树的带权路径长度
}
