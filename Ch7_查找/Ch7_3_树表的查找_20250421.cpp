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
// 参考视频：第12周09--第7章查找9--7.3树表的查找2-7.3.1二叉排序树2--二叉排序树的查找--递归算法 https://www.bilibili.com/video/BV1nJ411V7bd/?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=145
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


// 【算法7.5】二叉排序树的插入（待定）
// 参考视频：第12周11--第7章查找11--7.3树表的查找4-7.3.1二叉排序树4--二叉排序树的插入和生成 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=147
// 参考课本：p？
// 思路：用到查找函数
// 时间复杂度：近似O(log2n)，最坏O(n)，平均O(log2n)。（和查找函数一样）

Status InsertBST(BSTree &T, BSTNode *s){
    // 在二叉排序树T中插入结点s
    if(!T){ // 如果树为空，则插入结点s
        T = s; // 将s赋值给T
        return OK; // 返回成功
    }
    else if(s->data.key == T->data.key) return ERROR; // 如果关键字相同，则返回重复
    else if(s->data.key < T->data.key) return InsertBST(T->lchild, s); // 如果关键字小于根结点的关键字，递归插入左子树
    else return InsertBST(T->rchild, s); // 如果关键字大于根结点的关键字，递归插入右子树
}

// 【算法7.6】看看课本

// 【算法7.7】二叉排序树的删除
// 参考视频：第12周12--第7章查找12--7.3树表的查找5-7.3.1二叉排序树5--二叉排序树的删除 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=148
// 参考课本：p203-p204
// 思路：1.如果被删除结点z是叶子结点，直接删除，不会破坏二叉排序树的性质
//       2.如果被删除结点z只有左子树或只有右子树，用其左子树或右子树替代被删除结点
//       3.如果被删除结点z既有左子树又有右子树，用其左子树的最大值替代被删除结点，
//         然后删除左子树的最大值所在的结点（此时该节点只能有左子树，所以按照思路2删除即可）。或者用右子树的最小值也行。
// 时间复杂度：近似O(log2n)，最坏O(n)，平均O(log2n)。（和查找函数一样）

void DeleteBST(BSTree &T, KeyType key){
    // 删除二叉排序树T中关键字为key的结点
    BSTNode *p = T, *f= NULL; // 初始化：p指向根结点，f指向p的双亲结点。
    BSTNode *q, *s; // s指向第1种情况的要被删除的节点，q一开始指向p（之后在第1种情况指向s的双亲结点），在第2、3种情况中指向p的原始位置
    /*下面的while循环从根开始查找，直到找到关键字为key的结点或到达叶子结点*/
    while(p && p->data.key != key){ // 如果p不为空且p的值不等于key，则继续查找
        f = p; // f指向p的双亲结点
        if(key < p->data.key) p = p->lchild; // 如果key小于p的值，则p指向左子树，继续查找
        else p = p->rchild; // 如果key大于p的值，则p指向右子树，继续查找
    }
    if(p == NULL) return; // 没找到关键字为key的结点，直接返回
    /*考虑3种情况：p左右子树都不为空、p左子树为空、p右子树为空。此时已经涵盖了左右子树都为空的情况*/
    q = p; // q指向p初始化。在第1种情况中，q会指向s的双亲结点；在第2、3种情况中，q会指向p的原始位置
    if(p->lchild && p->rchild){ // 第1种情况：如果p的左右子树都不为空
        s = p->lchild; // s指向p的左子树。
        while(s->rchild){ // 如果s的右子树不为空，s向右走到尽头, 此时s没有右子树
            q = s; s = s->rchild; // q指向s的双亲结点，s指向s的右子树, 向右走到尽头
        }
        p->data = s->data; // 用s的值替代p的值。也就是用p的左子树的最大值替代p的值
        if(q!= p){ // 如果q不等于p，说明s不是p的左子树的根结点
            q->rchild = s->lchild; // 重新连接q的右子树为s的左子树
        }
        else q->lchild = s->lchild; // 如果q等于p，说明s是p的左子树的根结点, 重新连接p的左子树为s的左子树
        // 这里的q->lchild = s->lchild; 也可以写成p->lchild = s->lchild; 因为q和p是同一个结点。
        delete s; // 删除s
        return; // 删除完成，返回, 之后的代码就不会执行了
    }
    // 以上两种情况其实也涵盖了左右子树都为空的情况，也就是说如果做右子树都是空，那么p直接指向NULL。
    if(p->lchild == NULL){ // 第2种情况： 如果p的左子树为空
        p = p->rchild; // 用p的右子树替代p
    }
    else if(p->rchild == NULL){ // 第3种情况：如果p的右子树为空
        p = p->lchild; // 用p的左子树替代p
    }
    /* 将p所指的子树连接到其双亲结点f的相应位置（只会在第2、3种情况中执行）*/
    if(!f) T = p; // 如果f为空，说明p是根结点，将T指向p(此时为NULL)
    else if(q == f->lchild) f->lchild = p; // 如果q（一开始保存的p的位置）是f的左子树，将f的左子树指向p的左子树
    else f->rchild = p; // 如果q（一开始保存的p的位置）是f的右子树，将f的右子树指向p的左子树
    delete q; // 删除q，其实就是删除p的原始位置。不用担心影响到p，因为这属于第2、3种情况。
} 

/* -------------- 7.3.3 B-树--------------- */

// 【算法7.8】B-树的查找

// 【算法7.9】B-树的插入

