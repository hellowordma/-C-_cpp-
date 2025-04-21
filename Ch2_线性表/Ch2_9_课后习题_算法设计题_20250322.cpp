/***********************************************************
 * 第2章 线性表
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏；
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p18-p20

//导入相关的头文件
# include <stdio.h> // 标准输入输出流
# include <iostream> // 输入输出流
using namespace std; // 使用标准命名空间   
// Status定义
#define OK 1
#define ERROR 0 
#define OVERFLOW -2
typedef int Status; // Status是函数的类型，其值是函数结果状态代码


/*==================================
* 算法设计题（见习题集)
 *==================================*/


 /* ----------定义线性表的各种前置函数。参考Ch2_4_线性表的顺序表示_20250321.cpp ---------- */

// 定义线性表SqList
#define MAXSIZE 100 // 存储空间初始分配量
typedef struct SqList{
    // int elem[MAXSIZE]; // 数组存储数据元素，最大值为MAXSIZE
    int *elem;
    int length; // 线性表当前长度
}SqList;

// InitList 
Status InitList(SqList &L){
    L.elem = new int[MAXSIZE]; // 分配内存。
    if(!L.elem) return OVERFLOW; // 存储空间分配失败
    L.length = 0; // 空表长度为0
    return OK;
}
// ListLength
int ListLength(SqList L){
    return L.length;
}
// GetElem
int GetElem(SqList L, int i, int &e){
    if (i < 1 || i > L.length) return ERROR; // i值不合法
    e = L.elem[i-1]; // 取第i个元素的值
    return OK;
}   
// LocateElem
int LocateElem(SqList L, int e){
    for (int i = 0; i < L.length; i++){
        if (L.elem[i] == e) return i+1; // 找到元素e，返回其位序, 位序从1开始而不是0，所以要+1
    }
    return 0; // 未找到元素e
}
// ListInsert
Status ListInsert(SqList &L, int i, int e){
    if (i < 1 || i > L.length+1) return ERROR; // i值不合法。 i应该在1到L.length+1之间
    if (L.length >= MAXSIZE) return ERROR; // 当前存储空间已满
    for (int j = L.length; j >= i; j--){
        L.elem[j] = L.elem[j-1]; // 插入位置及之后的元素后移
    }
    L.elem[i-1] = e; // 插入e。注意是i-1而不是i
    L.length++; // 表长增1
    return OK;
}

// 定义链表
typedef struct LNode{
    int data; // 数据域
    struct LNode *next; // 指针域
}LNode, *LinkList;
// 定义双向链表
typedef struct DuLNode{ // 双向链表结点类型
    int data; // 数据域
    struct DuLNode *prior, *next; // 前驱指针和后继指针
}DuLNode, *DuLinkList; // DuLNode是结点类型名，DuLinkList是指向结点的指针类型名。

 /* ----------end of 定义线性表的各种前置函数---------- */


// 1. 将2个递增的有序链表合并成一个递增的有序链表，要求结果链表仍使用原来的2个链表的存储空间，不另外占用其他的存储空间。
// 表中不允许有重复元素。
// 参考习题 p29-p30
// 时间复杂度：O(m+n)，m和n分别为2个链表的长度；空间复杂度：O(1)
void MergeList1(LinkList &La, LinkList &Lb, LinkList &Lc){
    // 将2个递增的有序链表La和Lb合并成一个递增的有序链表Lc，结果链表仍使用原来的2个链表的存储空间，不另外占用其他的存储空间。
    LNode *pa = La->next, *pb = Lb->next, *pc; // pa和pb分别指向La和Lb的首元结点
    Lc = La; // 用La的头结点作为Lc的头结点，相当于不额外分配空间，Lc借用La的头结点。
    pc = Lc; // pc指向Lc的头结点
    while (pa && pb){ // pa和pb均未到达表尾
        if (pa->data < pb->data){ // pa的数据域小于pb的数据域
            pc->next = pa; // 将pa结点插入到pc结点之后
            pc = pa; // pc指向pa
            pa = pa->next; // pa后移
        }
        else if (pa->data > pb->data) { // pa的数据域大于pb的数据域
            pc->next = pb; // 将pb结点插入到pc结点之后
            pc = pb; // pc指向pb
            pb = pb->next; // pb后移
        }
        else { // pa的数据域等于pb的数据域
            pc->next = pa; // 将pa结点插入到pc结点之后
            pc = pa; // pc指向pa
            pa = pa->next; // pa后移
            LNode *temp = pb; // 临时指针temp指向pb
            pb = pb->next; // pb后移
            delete temp; // 删除pb结点
        }
    }   
    pc->next = pa ? pa : pb; // 插入剩余段给Lc
    delete Lb; // 释放Lb的头结点
}

// 2. 将2个非递减的有序链表合并成一个非递增的有序链表，要求结果链表仍使用原来的2个链表的存储空间，不另外占用其他的存储空间。
// 允许 重复元素。
// 参考习题 p30-p31
// 时间复杂度：O(m+n)，m和n分别为2个链表的长度；空间复杂度：O(1)
void MergeList2(LinkList &La, LinkList &Lb, LinkList &Lc){
    // 将2个非递减的有序链表La和Lb合并成一个非递增的有序链表Lc，结果链表仍使用原来的2个链表的存储空间，不另外占用其他的存储空间。
    // 方法：前插法建立链表（参考ch2_5_线性表的链表实现_20250321.cpp),通过q指向待插入的结点，将q插入到Lc的头结点之后来实现。
    // 此时不能在某一个表为空时直接将另一个表的剩余段插入到Lc的尾部，而是需要对另一个表按元素逐个插入。
    LNode *pa = La->next, *pb = Lb->next, *q; // pa和pb分别指向La和Lb的首元结点。q初始化。
    Lc =  La; // 用La的头结点作为Lc的头结点，相当于不额外分配空间，Lc借用La的头结点。
    Lc->next = NULL; // Lc的头结点的指针域为空。
    while (pa || pb){ // 只要有一个链表未到达表尾，则用q指向待摘取的结点
        if (!pa) {// La为空，用q指向pb，pb后移
            q = pb;
            pb = pb->next;
        }
        else if (!pb) { // Lb为空，用q指向pa，pa后移
            q = pa;
            pa = pa->next;
        }
        else if (pa->data <= pb->data){ // pa的数据域小于等于pb的数据域，用q指向pa，pa后移。与(!pb) 的操作等价。
            q = pa;
            pa = pa->next;
        }
        else { // pa的数据域大于pb的数据域，用q指向pb，pb后移。与(!pa) 的操作等价。
            q = pb;
            pb = pb->next;
        }
        // 下面2行是 前插法建立链表的核心操作
        q->next = Lc->next; // 将q结点插入到Lc的头结点之后
        Lc->next = q; // Lc的头结点指向q
    } // while
    delete Lb; // 释放Lb的头结点
}

// 3. 已知2个链表A和B分别表示两个集合，其元素递增排列。编写一个算法，求A和B的交集，并存放于A链表中。
// 参考习题 p30-p31
// 时间复杂度：O(m+n)，m和n分别为2个链表的长度；空间复杂度：O(1)
void Insection(LinkList &La, LinkList &Lb,LinkList &Lc){
    // 已知2个链表La和Lb分别表示两个集合，其元素递增排列。求A和B的交集，并存放于A链表中。
    // 方法：遍历La和Lb，将相同的元素插入到Lc中。注意：La和Lb的元素是递增排列的。
    LNode *pa = La->next, *pb = Lb->next, *pc=nullptr, *temp=nullptr; // pa和pb分别指向La和Lb的首元结点。初始化pc和temp。
    Lc = La; // 用La的头结点作为Lc的头结点，相当于不额外分配空间，Lc借用La的头结点。
    pc = Lc; // pc指向Lc的头结点
    while (pa && pb){ // pa和pb均未到达表尾
        if (pa->data < pb->data){ // pa的数据域小于pb的数据域,则pa后移，删除pa结点
            temp = pa;
            pa = pa->next;
            delete temp;        
        }
        else if (pa->data > pb->data) { // pa的数据域大于pb的数据域，则pb后移，删除pb结点
            temp = pb;
            pb = pb->next;
            delete temp;
        }
        else { // pa的数据域等于pb的数据域，则将pa结点插入到pc结点之后，pa后移，删除pb结点
            pc->next = pa; // 将pa结点插入到pc结点之后
            pc = pa; // pc指向pa
            pa = pa->next; // pa后移
            LNode *temp = pb; // 临时指针temp指向pb
            pb = pb->next; // pb后移
            delete temp; // 删除pb结点
        }
    } // while
    while(pa) { // Lb为空，删除La中剩余的元素
        temp = pa;
        pa = pa->next;
        delete temp;
    }
    while(pb) { // La为空，删除Lb中剩余的元素
        temp = pb;
        pb = pb->next;
        delete temp;
    }
    // 因为没有像之前那样对剩下的a和b中的非空链表插入到Lc中，所以这里需要将pc的next指向NULL。
    pc->next = NULL; // 插入剩余段给Lc。
    delete Lb; // 释放Lb的头结点
}

// 4. 已知2个链表A和B分别表示两个集合，其元素递增排列。编写一个算法，求A和B的差集，并存放于A链表中。
// 参考习题 p33
// 时间复杂度：O(m+n)，m和n分别为2个链表的长度；空间复杂度：O(1)
void Difference(LinkList &La, LinkList &Lb){
    // La和Lb差集存储于La中，n为结果集合的元素个数，调用时为0。
    LNode *pa = La->next, *pb = Lb->next, *pre = La, *temp= nullptr; // pa和pb分别指向La和Lb的首元结点。pre指向La的头结点，q初始化。
    while (pa && pb){ // pa和pb均未到达表尾
        if (pa->data < pb->data){ // pa的数据域小于pb的数据域
            pre = pa; // pre指向pa
            pa = pa->next; // pa后移
        }
        else if (pa->data > pb->data) { // pa的数据域大于pb的数据域
            pb = pb->next; // pb后移
        }
        else { // （pa->data == pb->data）pa的数据域等于pb的数据域
            pre->next = pa->next;  // pre的next指向pa的next
            temp = pa; // q指向pa
            pa = pa->next; // pa后移
            delete temp;// 删除q结点
            pb = pb->next; // pb后移。这个是与答案不同的一行，我的理解是，如果pa和pb的数据域相等，那么删除pa结点，pb后移。
        }
    } // while
    // 如果pa还有剩余元素，则无需额外修改（因为pre->next本来就指向pa）
}

// 5. 设计算法将一个带头结点的单链表A分解为两个具有相同结构的单链表B和C，其中表B中含有A中小于0的元素，而表C中含有A中大于0的元素。（链表A的元素
//  为非零整数）。要求B、C利用A表的结点。
// 参考习题 p33-p34
// 思路：遍历A，将小于0的元素插入到B中，大于0的元素插入到C中。而且用的是！！！前插法建立链表。
// 时间复杂度：O(n)，n为链表长度；空间复杂度：O(1)
void Decompose(LinkList &A, LinkList &B, LinkList &C){
    // 将一个带头结点的单链表A分解为两个具有相同结构的单链表B和C，其中表B中含有A中小于0的元素，而表C中含有A中大于0的元素。
    // 方法：遍历A，将小于0的元素插入到B中，大于0的元素插入到C中。而且用的是前插法建立链表。
    LNode *p = A->next, *r; // p指向A的首元结点。
    // 注意 LNode *p = A->next,早于B =A，所以之后B->next = NULL虽然会让A的头结点也指向NULL，
    // 但是不会影响p的指向,while(p!=NULL)还是可以正常执行。可以正常进行
    B = A; // 用A的头结点作为B的头结点，相当于不额外分配空间，B借用A的头结点。
    C = new LNode; // 创建C的头结点
    C->next = NULL; // C的头结点的指针域为空。前插法别忘了这一步。
    B->next = NULL; // B的头结点的指针域为空。前插法别忘了这一步。
    while(p!=NULL){ // A未到达表尾
        r = p->next; // 保存p的后继结点
        if(p->data < 0){ // 小于0的元素插入到B中
            p->next = B->next; // 将p结点插入到B的头结点之后
            B->next = p; // B的头结点指向p
        }
        else{ // 大于0的元素插入到C中
            p->next = C->next; // 将p结点插入到C的头结点之后
            C->next = p; // C的头结点指向p
        }
        p = r; // p后移
    }
}

// 6. 设计一个算法，通过一趟遍历，确定长度为n的单链表中值最大的结点，返回该结点的数据域。
// 参考习题 p34
// 时间复杂度：O(n)，n为链表长度；空间复杂度：O(1)
typedef struct ElemType{ // 自定义数据类型
    int data;
    bool operator<(const ElemType& other) const {
        return data > other.data; // 重载<运算符，用于比较大小
    }
    bool operator!=(const ElemType& other) const {
        return data != other.data; // 重载!=运算符，用于比较是否相等

}ElemType; 
typedef struct LNodeElemType{ // 自定义结点类型
    ElemType data;
    struct LNodeElemType *next;
}LNodeElemType, *LinkListElemType;
// 上面的两个结构体是我自定义的，习题没有给出ElemType的定义，是个通用的数据类型。我在这里添加是为了方便编译。
ElemType MaxNode(LinkListElemType L){
    LNodeElemType *p = L->next, *pmax = p; // p指向首元结点，pmax指向首元结点
    while (p->next != NULL){ // 顺着链表向后走，直到p指向NULL
        if (p->data < p->next->data) pmax = p->next; // 找到值最大的结点,更新pmax指针
        p = p->next;
    }
    return pmax->data; // 返回值最大的结点的数据域
}

// 7. 设计一个算法，将链表中所有结点的链接方向逆转，要求仅仅利用原表空间。算法的时间复杂度为O(n)，n为链表的长度，时间复杂度为O(1)。
// 参考习题 p35
// 思路：前插法建立链表。从头到尾遍历链表，将每个结点插入到头结点之后。
// 时间复杂度：O(n)，n为链表长度；空间复杂度：O(1)
void Inverse(LinkList &L){
    // 将链表中所有结点的链接方向逆转，要求仅仅利用原表空间。
    LNode *p = L->next, *q; // p指向首元结点
    L->next = NULL; // 头结点的指针域为空
    while(p != NULL){ // 顺着链表向后走，直到p指向NULL
        q = p->next; // 保存p的后继结点
        p->next = L->next; // 将p结点插入到L的头结点之后
        L->next = p; // L的头结点指向p
        p = q; // p后移
    }
}

// 8. 设计一个算法，删除递增有序链表中值大于mink且小于maxk的所有元素。
// （mink和maxk是给定的两个参数，其值可以和表中的元素相同，也可以不同）
// 参考习题 p35
// 时间复杂度：O(n)，n为链表长度；空间复杂度：O(1)
void DeleteMinMax(LinkList &L, int mink, int maxk){
    // 删除递增有序链表中值大于mink且小于maxk的所有元素。
    LNode *p = L->next, *pre = L,*q; // p指向首元结点，pre指向头结点
    while(p!=NULL & p->data <= mink) { // 找到第一个大于等于mink的结点
        pre = p; // pre指向p
        p = p->next; // p后移
    }
    while(p && p->data < maxk){ // 删除大于mink且小于maxk的结点
        q = p; // q指向p
        p = p->next; // p后移
        delete q; // 删除q结点
    }
    pre->next = p; // 此时 p指向的data都大于等于maxk或者指向NULL，可以将pre的next指向p。
}

// 9. 已知p指向双向循环链表中的一个结点，其结点结构为(data, prior, next)，请写出算法Exchang(p)，交换p结点与其前驱结点的顺序。
// 参考习题 p36-37
// 思路：画图即可。
// 时间复杂度：O(1)；空间复杂度：O(1)
// 注意：可能存在一些corner case，比如链表只有2个结点。之后再考虑。
void Exchang(DuLinkList p){
    // 交换p结点与其前驱结点的顺序
    DuLNode *q = p->prior; // q指向p的前驱结点
    p->next->prior = q; // p的后继结点的前驱指针指向q
    q->next = p->next; // q的后继指针指向p的后继结点
    q->prior->next = p; // q的前驱结点的后继指针指向p
    p->next = q; // p的后继指针指向q
    p->prior = q->prior; // p的前驱指针指向q的前驱结点
    q->prior = p; // q的前驱指针指向p
}

// 10.已知长度为n的线性表A采用顺序存储结构，请写一个时间复杂度为O(n)，空间复杂度为O(1)的算法，该算法删除线性表中所有值为item的数据元素。
// 参考习题 p37
// 思路：快慢指针法。利用原表空间记录不等于item的元素。
// 时间复杂度：O(n)，n为线性表长度；空间复杂度：O(1)
typedef struct SqListElemType{
    ElemType *elem;
    int length;
}SqListElemType;

void DeleteItem(SqListElemType &A, ElemType item){
    // 删除线性表中所有值为item的数据元素
    int k = 0; // 计数器,不等于item的元素个数
    // i和k分别是快慢指针。
    for (int i = 0; i < A.length; i++){ // 顺序扫描线性表
        if (A.elem[i] != item){
            A.elem[k] = A.elem[i]; // 利用原表空间记录不等于item的元素
            k++; // 计数器加1
        }
    }
    A.length = k; // 表长更新，即为不等于item的元素个数。还起到一个作用：将后面多余的元素删除。
}

// 11. 【2009年第42题】已知一个带有表头结点的单链表，结构为(data,link), 假设改链表只给出了头指针list。
// 在不改变链表的前提下，设计一个算法，查找链表中倒数第k个位置上的结点（k为正整数）。若查找成功，算法输出该结点的data域的值，并返回1；否则，只返回0。
// 参考习题 p37-p38
// 时间复杂度：O(n)，n为链表长度；空间复杂度：O(1)

// // 课本方法
// // 思路：快慢双指针法。先让指针p1指向第k+1个结点，然后让指针p2指向首元结点，然后p1和p2同时后移，直到p1指向NULL，此时p2指向倒数第k个结点。
// int FindLastK(LinkList list, int k){
//     // 查找链表中倒数第k个位置上的结点（k为正整数）
//     int i = 0; // 计数器
//     LNode *p = list->next, *q = list->next; // p1和p2指向首元结点
//     while(p!=NULL){ // 顺着链表向后走，直到p指向NULL
//         p = p->next; // p后移
//         if (i < k) i++; // 计数器i小于k时，i增加
//         else q = q->next; // i大于等于k时，q后移
//     }
//     if(i==k) { // 找到倒数第k个结点，输出其data域的值
//         cout << q->data << endl;
//         return 1;
//     }
//     else return 0; // 未找到倒数第k个结点
// }

// ************上面是课本的方法，但我觉得下面我的方法可能更好，因为我不需要计数器i，直接用k来控制p1和p2的移动。

// 思路：快慢双指针法。
// 先让指针fast指向首元结点，然后让指针slow指向头节点。在fast指向第k个结点之前，fast和slow同时后移，直到fast指向NULL，
// 此时slow指向倒数第k个结点。然后fast和slow同时后移，直到fast指向NULL，此时slow指向倒数第k个结点。

int FindLastK(LinkList list, int k) {
    LNode *fast = list->next, *slow = list; // fast 指向首元节点，slow 指向头节点
    while (fast != NULL) {
        fast = fast->next; // fast 后移
        if (k > 1) k--; // fast 移动 k 步
        else slow = slow->next; // slow 开始移动
    }
    if (k == 1) { // 找到倒数第 k 个节点
        cout << slow->data;
        return 1;
    } else return 0; // 未找到倒数第 k 个节点
}


// 12. 【2010年第42题】设将n(n>1)个整数存放到一维数组R中，试设计一个在时间和空间两方面都尽可能高效的算法。
// 将R中保存的序列循环左移p（0<p<n）个位置，即将R中的数据由(X0,X1,...,Xn-1)变换为(Xp,Xp+1,...,Xn-1,X0,X1,...,Xp-1)。要求：
// （1）给出算法的基本设计思想。
// （2）根据设计思想，采用C或C++语言描述算法，关键之处给出注释。
// 参考习题 p38

// 思路： 三步翻转法。先将整个数组翻转，然后将前n-p个元素翻转，再将后p个元素翻转。
// 时间复杂度：O(n)，n为数组长度；空间复杂度：O(1)
void Reverse(int R[], int left, int right){
    // 将数组R中from到to的元素翻转
    while(left < right){
        int temp = R[left];
        R[left] = R[right];
        R[right] = temp;
        left++;
        right--;
    }
}
void Converse(int R[], int n, int p){
    // 将数组R中保存的序列循环左移p个位置
    if(p>0 && p<n){
    Reverse(R, 0, n-1); // 将整个数组翻转
    Reverse(R, 0, n-p-1); // 将前n-p个元素翻转
    Reverse(R, n-p, n-1); // 将后p个元素翻转
    }   
}

// 13. 【2011年第42题】一个长度为L(L>1)的升序序列S，处在第[L/2]个位置的数称为S的中位数。
// 例如，若序列S1={11,13,15,17,19}，则S1的中位数是15，两个序列的中位数是含它们所有元素的升序序列的中位数。
// 例如，若S2={2,4,6,8,20}，则S1和S2的中位数是11。
// 现在有两个等长升序序列A和B，设计一个在时间和空间两方面都尽可能高效的算法，找出两个序列A和B的中位数。
// 参考习题 p42.

// 思路：二分法。每次排除掉两个序列中的一半元素，直到剩下一个元素或两个元素。
// 时间复杂度：O(logn)，n为数组长度；空间复杂度：O(1)

int FindMedian(int A[], int B[], int n){
    // 找出两个等长升序序列A和B的中位数
    int left1 = 0, right1 = n-1, mid1, left2 = 0, right2 = n-1, mid2; // 初始化A和B的首尾指针为0和n-1，以及中位数
    while(left1 < right1){ // A的首尾指针未相遇（此时B的首尾指针也未相遇）时，循环
        mid1 = left1 + (right1 - left1) / 2; // 求A的中位数
        mid2 = left2 + (right2 - left2) / 2; // 求B的中位数
        if(A[mid1] == B[mid2]) return A[mid1]; // A的中位数等于B的中位数
        if(A[mid1] < B[mid2]){ // A的中位数小于B的中位数
            if((left1 + right1) % 2 == 0){ // 若元素个数为奇数
                left1 = mid1; // 舍弃A中位数左边的部分
                right2 = mid2; // 舍弃B中位数右边的部分
            }
            else{ // 若元素个数为偶数
                left1 = mid1 + 1; // 舍弃A中位数及其左边的部分,注意要+1，因为要包含中间的元素
                right2 = mid2; // 舍弃B中位数右边的部分
            }
        }
        else{ // A的中位数大于B的中位数
            if((left2 + right2) % 2 == 0){ // 若元素个数为奇数
                left2 = mid2; // 舍弃B中位数左边的部分
                right1 = mid1; // 舍弃A中位数右边的部分
            }
            else{ // 若元素个数为偶数
                left2 = mid2 + 1; // 舍弃B中位数及其左边的部分，注意要+1，因为要包含中间的元素
                right1 = mid1; // 舍弃A中位数右边的部分
            }
        }
    }
    return A [left1] < B[left2] ? A[left1] : B[left2]; // 返回A和B中较小的中位数。
    // 注意是left，也可以改成right，但是不能是mid，因为要考虑极端情况，如n=1时，mid并未赋值（因为while循环未执行），所以不能用mid。
    // 除非初始化mid1和mid2为0，但是这样会增加不必要的计算。
}

// 14. 【2012年 42题】假定采用带头结点的单链表保存单词，当两个单词有相同的后缀时，可共享相同的后缀存储空间，
// 例如“loading”和“being”。设str1和str2分别指向两个单词所在的单链表的头结点，链表结构为(data,link)。
// 设计一个高效算法，找出由str1和str2所指向两个链表共同后缀的起始位置。

// 思路：先遍历两个链表，得到两个链表的长度，然后让长的链表先走两个链表长度之差的步数，然后两个链表同时后移，直到两个链表的结点相同。
// 时间复杂度：O(m+n)，m和n分别是两个链表的长度；空间复杂度：O(1)

// 定义链表，数据域为char
typedef struct LNode_char{
    char data; // 数据域
    struct LNode_char *next; // 指针域
}LNode_char, *LinkList_char;

LinkList_char FindCommonSuffix(LinkList_char str1, LinkList_char str2){
    // 找出由str1和str2所指向两个链表共同后缀的起始位置
    LNode_char *p = str1->next, *q = str2->next; // p和q分别指向str1和str2的首元结点
    int len1 = 0, len2 = 0; // 初始化链表长度
    while(p != NULL){ // 遍历str1，得到链表长度
        len1++;
        p = p->next;
    }
    while(q != NULL){ // 遍历str2，得到链表长度
        len2++;
        q = q->next;
    }
    p = str1->next; // p重新指向str1的首元结点
    q = str2->next; // q重新指向str2的首元结点
    if(len1 > len2){ // str1比str2长
        for(int i = 0; i < len1-len2; i++) p = p->next; // 让str1先走len1-len2步
    }
    else{ // str2比str1长
        for(int i = 0; i < len2-len1; i++) q = q->next; // 让str2先走len2-len1步
    }
    while(p != NULL && q != NULL){ // 两个链表同时后移，直到两个链表的结点相同
        if(p == q) return p; // 找到共同后缀的起始位置
        p = p->next;
        q = q->next;
    }
    return NULL; // 未找到共同后缀的起始位置
}

// 15. 【2013年 41题】已知一个整数序列A=(a0,a1,...,an-1)，其中0<=ai<n(0<=i<n)，
//  若存在ap1=ap2=...=apm=x且m>n/2(0<=pk<n,1<=k<=m)，则称x为A的主元素。例如，A=(0,5,5,3,5,7,5,5)，则5为主元素；
//  又如，A=(0,5,5,3,5,1,5,7)，则A中没有主元素。假设A中的n个元素保存在一个一维数组中，请设计一个尽可能高效的算法，找出A的主元素。
// 参考习题 p41
// 思路：用Key的增减法: 选取候选主元素，遍历数组，如果当前元素等于候选主元素，count加1，否则count减1，如果count为0，更新候选主元素为当前元素，count重新计数。
//      遍历完数组后，再次遍历数组，计算候选主元素的个数，如果大于n/2，返回候选主元素，否则返回-1。
// 时间复杂度：O(n)，n为数组长度；空间复杂度：O(1)

int MainElement(int A[], int n){
    // 找出A的主元素
    int key = A[0], count = 1; // 初始化key为A的第一个元素，count为1(而不是0)
    for(int i = 1; i < n; i++){ // 从第二个元素开始遍历数组
        if(A[i] == key) count++; // 如果A[i]等于key，count加1
        else{ // 如果A[i]不等于key
            count--; // count减1
            if(count == 0){ // 如果count为0，更新key为A[i]，count重新计数，初始化为1
                key = A[i];  
                count = 1; 
            }
        }
    }
    if(count > 0){ // 遍历完数组后，key为主元素。注意只有在count>0时，才有必要再次遍历数组，计算key的个数。
        count = 0; // 重新计数
        for(int i = 0; i < n; i++){ // 遍历数组，计算key的个数
            if(A[i] == key) count++;
        }
        if(count > n/2) return key; // 如果key的个数大于n/2，返回key
        else return -1; // 如果key的个数小于等于n/2，返回-1
    }
    else return -1; // 如果count为0，返回-1
}

// 16. 【2015年 41题】用单链表保存m个整数，结点的结构为(data,link), 且|data|<=n(n为正整数)。
// 现要求设计一个时间复杂度尽可能高效的算法，对于链表中data的绝对值相等的结点，仅保留第一次出现的结点而删除其余绝对值相等的结点。
// 例如，若给定的单链表head如下：head->21->-15->-15->-7->15->15->7
// 则删除后的单链表应为：       head->21->-15->-7->15->7
// 参考习题 p42
// 思路：辅助数组计数。
// 时间复杂度：O(m)，m为链表长度；空间复杂度：O(1)，因为只用了一个辅助数组t。

void DeleteEqualAbsNode(LinkList head,int n){
    // 删除绝对值相等的结点
    int *t = new int[n+1]; // 申请一个数组t，用来存放绝对值出现的次数
    LNode *r = new LNode; // 申请一个新结点r，用来删除结点
    for(int i = 0; i < n; i++) *(t+i) = 0; // 初始化数组t
    LNode *p = head->next; // p指向首元结点
    while(p != NULL){
        if(t[abs(p->data)] ==1){  // 如果p的绝对值出现过
            r->next = p->next; // 删除p结点
            delete p;
            p = r->next;
        }
        else{ // 如果p的绝对值未出现过
            t[abs(p->data)] = 1; // 标记p的绝对值出现过
            r = p; // r指向p
            p = p->next; // p后移
        }
    }
    delete[] t; // 释放辅助数组的内存
}

// 17. 【2016年 43题】已知由n(n>=2)个正整数构成的集合A={ak|k=0,1,2,...,n-1}，
// 将其划分为两个不相交的子集A1和A2，元素个数分别为n1和n2, A1和A2中元素之和分别为S1和S2，
// 设计一个尽可能高效的算法，找出集合A的两个子集，使得|n1-n2|最小且|S1-S2|最大。
// 参考习题 p43
// 思路：将最小的n//2个数放到A1中，其余的放到A2中。仿照快速排序的思想，基于枢轴将数组分为两部分，根据划分后枢轴的位置i，分以下3种情况：
// (1). i = n//2，即A1和A2的元素个数相等，直接返回；
// (2). i < n//2，即A1的元素个数小于A2的元素个数，将A[i+1:n-1]中的最小的n//2-i个数放到A1中，其余的放到A2中；
// (3). i > n//2，即A1的元素个数大于A2的元素个数，将A[0:i-1]中的最小的i-n//2个数放到A2中，其余的放到A1中。
// 时间复杂度：O(nlogn)，n为数组长度；空间复杂度：O(1)

int Partition(int a[],int n){
    // 将正整数构成的集合A划分为两个不相交的子集A1和A2
    int low = 0, high = n-1; // low和high分别指向数组的上界和下界
    int low0 = 0, high0 = n-1; // low0和high0分别指向数组的上界和下界
    int left1 = 0, left2 = 0; // left1和left2分别表示A1和A2的和
    int flag = 0; // 标志位
    int k = n/2; // k为数组的中间位置
    while(flag){
        // 选取枢轴pivotkey
        int pivotkey = a[low]; // 选取第一个元素作为枢轴
        while(low < high){
            while(low < high && a[high] >= pivotkey) high--; // 从右向左找第一个小于pivotkey的元素
        if(low!=high) a[low] = a[high]; // 将a[high]调整到a[low]的位置
        while(low < high && a[low] <= pivotkey) low++; // 从左向右找第一个大于pivotkey的元素
        if(low!=high) a[high] = a[low]; // 将a[low]调整到a[high]的位置
        } // end of while(low < high)
        a[low] = pivotkey; // 枢轴元素放到正确的位置
        if(low==k-1) flag = 0; // 满足条件(1), 下次循环退出划分
        else{ 
            if(low < k-1){ // 满足条件(2),枢轴low及之前的元素放到A1中，枢轴low之后的元素放到A2中
                low0 = ++low; // low0指向low的下一个位置，继续对low之后的元素进行划分
                high = high0; // high指向high0
            }
            else{ // 满足条件(3)，枢轴low及之后的元素放到A2中，枢轴low之前的元素放到A1中
                high0 = --high; // high0指向high的前一个位置，继续对high之前的元素进行划分
                low = low0; // low指向low0
            }
        }
    } // end of while(flag)
    for(int i = 0; i < k; i++) left1 += a[i]; // 计算A1的和
    for(int i = k; i < n; i++) left2 += a[i]; // 计算A2的和
    return left2-left1; // 返回A2和A1的差值
}
