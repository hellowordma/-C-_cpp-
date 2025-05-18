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
* 5.4 二叉树的性质和存储结构
 *==============================================================*/


 /* -------------- 5.4.2 二叉树的存储结构--------------- */

// 1. 二叉树的顺序存储

// 参考视频：第07周09--5.4二叉树的性质和存储结构4-- 二叉树的存储结构1-顺序存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=84
// 参考课本：p120
#define MAXSIZE 100 // 最大二叉树的结点数
typedef int SqBiTree[MAXSIZE]; // 二叉树的顺序存储结构。int其实是TElemType，只不过这里写规定TElemType会报错。
SqBiTree bt; // 将bt定义为顺序存储结构的二叉树。0号单元存储根节点。bt是个数组。

// 2. 二叉树的链式存储（二叉链表）
// 参考视频：第08周03--第5章树和二叉树3-5.4二叉树的性质和存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=85
// 参考课本：p121
typedef struct BiTNode{ // 二叉树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    struct BiTNode *lchild, *rchild; // 左右孩子指针
} BiTNode, *BiTree; // BiTNode是二叉树结点类型，BiTree是指向二叉树结点的指针类型。
//  非常类似LNode，只不过多了1个指针变成了2个指针而已。



/*==============================================================
* 5.5 遍历二叉树和线索二叉树
 *==============================================================*/

/* -------------- 5.5.1 遍历二叉树--------------- */

// 【算法5.1】中序遍历的递归算法
// 参考视频：第08周02--5.5遍历二叉树和线索二叉树4-二叉树的递归遍历算法及分析 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=89
// 参考课本：p123
void InOrderTraverse(BiTree T){ // 中序遍历
    if (T){ // 如果二叉树不空
        InOrderTraverse(T->lchild); // 递归遍历左子树
        cout << T->data << " "; // 访问根结点。或者改为visit(T)
        InOrderTraverse(T->rchild); // 递归遍历右子树
    }
}

// 【算法补充1】先序遍历的递归算法
// 参考视频：第08周01--5.5遍历二叉树和线索二叉树3-二叉树的先序递归遍历算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=88
void PreOrderTraverse(BiTree T){ // 先序遍历的递归算法
    if (T){ // 如果二叉树不空
        cout << T->data << " "; // 访问根结点。或者改为visit(T)
        PreOrderTraverse(T->lchild); // 递归遍历左子树
        PreOrderTraverse(T->rchild); // 递归遍历右子树
    }
}

// 【算法补充2】后序遍历的递归算法
// 参考视频：第08周02--5.5遍历二叉树和线索二叉树4-二叉树的递归遍历算法及分析 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=89
void PostOrderTraverse(BiTree T){ // 后序遍历的递归算法
    if (T){ // 如果二叉树不空
        PostOrderTraverse(T->lchild); // 递归遍历左子树
        PostOrderTraverse(T->rchild); // 递归遍历右子树
        cout << T->data << " "; // 访问根结点。或者改为visit(T)
    }
}

// **总结**：
// 可以看出如果去掉输出语句，以上3种遍历算法的代码是一样的，只是输出语句的位置不同而已。
// 经过 1个结点第1、2、3次之后再访问，分别为先序、中序、后序遍历。
// 遍历的时间复杂度：O(n)，每个结点都要访问一次；空间复杂度：O(n)，递归调用栈的深度为n。


// 【算法5.2】中序遍历的非递归算法
// 参考视频：第08周03--5.5遍历二叉树和线索二叉树5-二叉树的遍历算法--中序非递归算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=90
// 参考课本：p124

// -----------------先把顺序栈的定义和操作函数写出来，然后再写中序遍历的非递归算法。参考 Ch3_栈和队列\Ch3_3_栈的表示和操作的实现_20250328.cpp

// 定义二叉树的顺序栈 SqStack_btree
#define MAXSIZE 100 // 最大栈长度
typedef struct SqStack_btree{ // 定义顺序栈
    BiTNode **base; // 栈底指针. 改为存储BiTNode*类型的指针。 **是因为指向指针的指针。
    // 也可以改为int *base; 但是这样就不能存储BiTNode*类型的指针了。
    BiTNode **top; // 栈顶指针. 改为存储BiTNode*类型的指针。
    int stacksize; // 栈的大小
}SqStack_btree;
//  比SqList多了一个栈底指针*base。

Status InitStack_btree(SqStack_btree &S){ 
    S.base = new BiTNode*[MAXSIZE]; // 分配MAXSIZE个元素的数组空间
    if(!S.base) return OVERFLOW; // 存储空间分配失败
    S.top = S.base; // 栈顶指针指向栈底，作为初始化（栈空）
    S.stacksize = MAXSIZE; // 栈的大小
    return OK;
}

Status StackEmpty_btree(SqStack_btree S){ // 判断栈空
    if (S.top == S.base) return 1; // 栈空
    else return 0; // 栈非空
}

Status Push_btree(SqStack_btree &S, BiTNode *e){ // 入栈操作。改为处理BiTNode*类型
    if (S.top - S.base == S.stacksize) 
        return ERROR; // 栈满
    *S.top++ = e; // 将元素e压入栈顶，栈顶指针上移。相当于 *S.top = e; S.top++;
    return OK;
}

Status Pop_btree(SqStack_btree &S, BiTNode *&e){ // 出栈操作。改为处理BiTNode*类型
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *--S.top; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top; e = *S.top;。
    return OK;
}
    
//---------------
// 正式开始
void InOrderTraverse_norecurse(BiTree T){ // 中序遍历的非递归算法
    SqStack_btree S; // 定义栈S
    InitStack_btree(S); // 初始化栈S
    BiTNode *p = T; // 定义指针p，指向二叉树的根结点
    BiTNode *q = new BiTNode; // 创建一个新的结点
    while (p || !StackEmpty_btree(S)){ // 如果p不为空或者栈不空
        if (p){ // 如果p不为空。 左。
            Push_btree(S, p); // 根指针p入栈。
            p = p->lchild; // p指向左子树。
        }
        else{ // 如果p为空
            Pop_btree(S, q); // 将栈顶元素弹出到q，退栈
            cout << q->data << " "; // 访问根结点。根。
            p = q->rchild; // p指向右子树,遍历右子树。右。
        }
    }   
    delete q; // 释放q的内存
}

// 【算法补充3】层序遍历算法
// 参考视频：第08周03--第5章树和二叉树3-5.4二叉树的性质和存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=91
// 参考课本：p125

// -----------------先把顺序队列的定义和操作函数写出来，然后再写层序遍历算法。参考 Ch3_栈和队列\Ch3_5_队列的表示和操作的实现_20250329.cpp
// 定义二叉树的顺序队列 SqQueue_btree
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
// 正式开始
void LevelOrderTraverse(BiTNode *b){ // 层序遍历算法
    SqQueue_btree qu; // 定义队列
    BiTNode *p; // 定义二叉树结点指针p
    InitQueue_btree(qu); // 初始化队列
    EnQueue_btree(qu, b); // 根结点指针入队
    while (!QueueEmpty_btree(qu)){ // 如果队列不空则循环
        DeQueue_btree(qu, p); // 队头元素p出队
        cout << p->data << " "; // 访问刚出队的结点p
        if (p->lchild) EnQueue_btree(qu, p->lchild); // p的左子树入队, 如果左子树不空
        if (p->rchild) EnQueue_btree(qu, p->rchild); // p的右子树入队，如果右子树不空
    }
}

// 【算法5.3】先序遍历的顺序建立二叉链表
// 参考视频：第08周05--5.5遍历二叉树和线索二叉树7-二叉树的遍历算法应用1 --建立二叉树算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=92
// 参考课本：p126

void CreateBiTree(BiTree &T) {
    // 按先序次序输入二叉树的结点值（1个字符），#表示空树，创建二叉链表表示的二叉树T。用到递归：先创建根结点，再创建左子树，再创建右子树。
    char ch; // 定义字符变量ch
    cin >> ch; // 输入字符
    if (ch == '#') T = NULL; // 如果输入#，则结点为空
    else{ // 如果输入的不是#，则创建结点
        T = new BiTNode; // 创建新结点
        T->data = ch; // 赋值
        CreateBiTree(T->lchild); // 递归创建左子树
        CreateBiTree(T->rchild); // 递归创建右子树
    }
}

// 【算法5.4】复制二叉树
// 参考视频：第08周06--5.5遍历二叉树和线索二叉树8-二叉树的遍历算法应用2 --复制、求结点数、求叶子结点数 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=93
// 参考课本：p127
void Copy(BiTree T,BiTree &NewT){ 
    // 复制二叉树, T是原树，NewT是新树,用到先序遍历。仍然是递归：先复制根结点，再复制左子树，再复制右子树。
    if (T == NULL) { // 如果二叉树为空，则返回空
        NewT = NULL; 
    } 
    NewT = new BiTNode; // 创建新结点。像这种用&修饰的变量NewT，就不用在本函数内部delete了，但最终会在main函数中手动delete，比如DestroyBiTree。
    NewT->data = T->data; // 赋值
    Copy(T->lchild, NewT->lchild); // 递归复制左子树
    Copy(T->rchild, NewT->rchild); // 递归复制右子树
}

// 【算法5.5】求二叉树的深度
// 参考视频：第08周06--5.5遍历二叉树和线索二叉树8-二叉树的遍历算法应用2 --复制、求结点数、求叶子结点数 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=93
// 参考课本：p128-p129
int Depth(BiTree T){ // 求二叉树的深度，左子树和右子树的深度之和加1（根结点也算1层）
    if (T == NULL) return 0; // 如果二叉树为空，则深度为0
    else{ // 如果二叉树不空，则递归求左子树和右子树的深度，取较大值加1
        int m = Depth(T->lchild); // 求左子树的深度
        int n = Depth(T->rchild); // 求右子树的深度
        return (m > n ? m : n) + 1; // 返回较大值加1,因为根结点也算1层
    }
}

// 【算法5.6】求二叉树的结点个数
// 参考视频：第08周06--5.5遍历二叉树和线索二叉树8-二叉树的遍历算法应用2 --复制、求结点数、求叶子结点数 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=93
// 参考课本：p128
int NodeCount(BiTree T){ // 求二叉树的结点个数，左子树和右子树的结点个数之和加1（根结点也算1个结点）
    if (T == NULL) return 0; // 如果二叉树为空，则结点个数为0
    else{ // 如果二叉树不空，则递归求左子树和右子树的结点个数，取较大值加1
        int m = NodeCount(T->lchild); // 求左子树的结点个数
        int n = NodeCount(T->rchild); // 求右子树的结点个数
        return m + n + 1; // 返回左子树和右子树的结点个数之和加1,因为根结点也算1个结点
    }
}

// 【算法补充4】求二叉树的叶子结点个数
// 参考视频：第08周06--5.5遍历二叉树和线索二叉树8-二叉树的遍历算法应用2 --复制、求结点数、求叶子结点数 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=93

int LeafCount(BiTree T){ // 求二叉树的叶子结点个数，左子树和右子树的叶子结点个数之和（不用加1，因为根结点不算叶子结点）
    if (T == NULL) return 0; // 如果二叉树为空，则叶子结点个数为0
    if (T->lchild == NULL && T->rchild == NULL) return 1; // 如果二叉树是叶子结点，则返回1
    else return LeafCount(T->lchild) + LeafCount(T->rchild); // 如果二叉树不空，则递归求左子树和右子树的叶子结点个数之和
}

// 1. 线索二叉树
// 参考视频：第08周07--5.5遍历二叉树和线索二叉树9-线索二叉树 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=94
// 参考课本：p129

typedef struct BiThrNode{ // 线索二叉树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    struct BiThrNode *lchild, *rchild; // 左右孩子指针
    int LTag, RTag; // 左右标志域，0表示指向孩子，1表示指向前驱或后继
} BiThrNode, *BiThrTree; // BiThrNode是线索二叉树结点类型，BiThrTree是指向线索二叉树结点的指针类型。

// 【算法5.7】以结点p为根的子树的中序线索化
// 参考课本：p130

// 定义pre，pre是全局变量，（在main函数中）初始化时其右孩子指针指向NULL，便于在树的最左点开始建立线索
BiThrNode* pre = NULL; // 声明pre为全局变量
void InitializePre() {
    pre = new BiThrNode; // 创建一个新的结点
    pre->rchild = NULL; // 初始化pre的右孩子指针指向NULL，便于在树的最左点开始建立线索
}
void InThreading(BiThrTree p){  // // 以结点p为根的子树的中序线索化
    // pre是全局变量，初始化时其右孩子指针指向NULL，便于在树的最左点开始建立线索
    if(p){ // 如果结点p不空
        InThreading(p->lchild); // 递归线索化左子树
        if (!p->lchild){ // 如果结点p的左孩子为空
            p->lchild = pre; // 将结点p的左孩子指针指向前驱结点pre
            p->LTag = 1; // 设置左标志域为1，表示指向前驱
        }
        else p->LTag = 0; // 如果结点p的左孩子不为空，则设置左标志域为0，表示指向左孩子

        if (!pre->rchild){ // 如果前驱结点的右孩子为空
            pre->rchild = p; // 将前驱结点的右孩子指针指向结点p
            pre->RTag = 1; // 设置右标志域为1，表示指向后继
        }
        else pre->RTag = 0; // 如果前驱结点的右孩子不为空，则设置右标志域为0，表示指向右孩子

        pre = p; // 更新前驱结点为当前结点p，保持pre指向p的前驱结点
        InThreading(p->rchild); // 递归线索化右子树
    }
}

// 【算法5.8】带头结点的二叉树中序线索化(调用【算法5.7】的InThreading函数)
// 参考课本：p130-p131
void InOrderThreading(BiThrTree &Thrt, BiThrTree T){ // 带头结点的二叉树中序线索化
    // 中序遍历二叉树T，并将其中序线索化，Thrt指向头结点。Thrt的lchild指向根结点，rchild指向最后一个结点
    // 同时第一个结点的lchild指向头结点，最后一个结点的rchild指向头结点
    // 可以参考双向循环链表。
    Thrt = new BiThrNode; // 创建头结点
    Thrt->LTag = 0; // 头结点有左孩子，若树非空，则其左孩子指向根结点
    Thrt->RTag = 1; // 头结点的右孩子指针为右线索
    Thrt->rchild = Thrt; // 初始化时，头结点的右孩子指针指向头结点。相当于头结点的rchild指向最后一个结点
    if (!T) Thrt->lchild = Thrt; // 如果二叉树为空，则头结点的左孩子指针指向头结点。相当于第一个结点的lchild指向头结点
    else{ // 如果二叉树不空，则线索化二叉树
        Thrt->lchild = T; // 头结点的lchild指针指向根结点
        pre = Thrt; // 初始化pre为头结点
        InThreading(T); // 线索化二叉树，调用【算法5.7】的InThreading函数.
        // 注意，在InThreading(T)的过程中，会实现 第一个结点的lchild指向头结点(当时是pre也就是Thrt)。
        pre->rchild = Thrt; // 【算法5.7】结束后，pre为最右结点也就是最后一个结点，rchild指向头结点
        pre->RTag = 1; // 最后一个结点pre的右标志域为1，呼应上一行的rchild指向头结点。
        Thrt->rchild = pre; // 头结点的rchild指针指向最后一个结点pre
    }
}

// 【算法5.9】遍历中序线索二叉树
// 参考课本：p132
// 时间复杂度：O(n)，空间复杂度：O(1)
void InOrderTraverse_Thr(BiThrTree T){ // 遍历中序线索二叉树
    // 此时的T是经过线索化的二叉树了，T指向头结点，头结点的左链lchild指向根结点，可参见【算法5.8】的InOrderThreading函数
    // 中序遍历二叉线索树T的非递归算法，对每个数据元素直接输出
    // 中序遍历的后继查找方法：
    // 1. 若p->RTag ==1，则p的右孩子指针指向后继结点，直接访问p->rchild
    // 2. 若p->RTag ==0，则p的右孩子指针指向右子树，访问右子树的最左下结点，就是后继结点
    BiThrNode *p = T->lchild; // p指向第一个结点也就是根结点
    while (p != T){ // 空树或者遍历完毕时，p==T，退出循环。类似循环链表的终止条件(p!=L)。因为最后一个节结点的rchild指向头结点T。
        while (p->LTag == 0) p = p->lchild; // 从根节点开始，沿着左孩子向下,直到中序遍历的第1个结点
        cout << p->data << " "; // 输出值
        // 接下来从中序遍历的第1个结点开始，遍历后继结点，直到最后一个结点
        while (p->RTag == 1 && p->rchild != T) { // 如果p的右标志域为1，且p的右孩子不指向头结点T
            p = p->rchild; // 沿着右线索访问后继结点
            cout << p->data << " ";  // 输出值
        }
        p = p->rchild; // 如果上面的 while 终止（即 RTag == 0），说明要跳到右子树。此时如果能继续进行外面的 while 循环，
        // （接上一行）那就可以通过右子树中最左下的结点（右子树的访问时的第一个结点）来访问后继结点了。
    }
}

