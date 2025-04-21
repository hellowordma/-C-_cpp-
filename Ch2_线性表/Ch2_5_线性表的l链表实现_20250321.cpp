/***********************************************************
 * 第2章 线性表
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏
// 2. 视频：b站视频《数据结构》by王卓

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
* 2.5 线性表的链式表示和实现
 *==================================*/

 /* ---------- 2.5.1 单链表的定义---------- */

// 参考视频： 第3周02--2.5线性表的链式表示和实现2--单链表的定义 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=23
// 课本范围：p30-p31
// 代码实现

// 1. 定义单链表的结点类型
typedef struct LNode{ // struct 表示结构体，typedef表示给结构体取别名，如果在大括号后面有多个变量，则这些变量等价，都表示这个结构体。
    int data; // 结点的数据域
    struct LNode *next; // 结点的指针域。实际上是个嵌套struct LNode。
}LNode, *LinkList; // LNode是节点类型名，LinkList是指向节点的指针类型名。二者等价。一般用LinkList L表示一个链表，用LNode *p表示一个节点。

// 2. 例子：存储学生学号、姓名、成绩的单链表
// typedef struct Student{
//     char num[8]; // 学号
//     char name[8]; // 姓名
//     int score; // 成绩
//     struct Student *next; // 指向下一个学生的指针
// }LNode_Student, *LinkList_Student; // Student是学生类型名，LinkList_Student是指向学生的指针类型名。二者等价。

// 上面被注释掉的代码不常用。下面的代码更常用，因为更灵活地设置了ElemType，抽象出了数据域。
typedef struct ElemType{  
    char num[8]; // 学号
    char name[8]; // 姓名
    int score; // 成绩
}ElemType; 

typedef struct Student{
    ElemType data; // 数据域
    struct Student *next; // 指针域
}LNode_Student, *LinkList_Student; 

 /* ---------- 2.5.2 单链表的基本操作---------- */
// 参考视频： 第3周03--2.5线性表的链式表示和实现3--单链表的基本操作1--初始化和判断空表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=24
// 课本范围：p32-
// 代码实现

// 1. 初始化链表。意义在于添加一个头结点，头结点的数据域不存放数据，头指针指向头结点。
Status InitList(LinkList& L){
    L = new LNode; // 分配一个头结点。这是C++的写法。 或者 L = (LinkList)malloc(sizeof(LNode)); 这是C语言的写法。
    L->next = NULL; // 头结点的指针域为空
    return OK;
}

// 2. 【补充算法1】判断链表是否为空
int ListEmpty(LinkList L){
    if (L->next) return 0; // 链表不为空
    else return 1; // 链表为空
}

// 3. 【补充算法2】销毁单链表
// 参考视频： 第3周04--2.5线性表的链式表示和实现4--单链表的基本操作2--销毁单链表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=25

Status DestroyList(LinkList& L){
    LNode *p; // 临时指针. 也可以写成 LinkList p，但不常见。
    while (L){ // 链表不为空
        p = L;
        L = L->next; // 指向下一个节点
        delete(p); // 删除当前节点
    } // 头节点也要删除
    return OK;  
}

// 4. 【补充算法3】清空单链表
// 参考视频： 第3周05--2.5线性表的链式表示和实现5--单链表的基本操作3--清空单链表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=26
Status ClearList(LinkList& L){
    LNode *p, *q; // 临时指针
    p = L->next; // 指向第一个节点而不是头结点
    while (p){ // 头结点之后的节点不为空时
        q = p->next; // 保存下一个节点的地址
        delete(p); // 删除当前节点
        p = q; // 指向下一个节点
    }
    L->next = NULL; // 头结点指针域为空。保留头结点。这里是避免头结点指针悬空（悬空意味着指向的内存已经被释放，但指针还指向那块内存）。
    return OK;
}

//5. 【补充算法4】求单链表的长度
// 参考视频： 第3周06--2.5线性表的链式表示和实现6--单链表的基本操作4--求单链表的长度 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=27
int ListLength(LinkList L){
    int i = 0; // 计数器
    LNode *p = L->next; // 指向第一个节点
    while (p){ // 节点不为空,遍历链表
        i++; // 计数器加1
        p = p->next; // 指向下一个节点
    }
    return i; // 返回计数器的值
}

// 6. 【算法2.7】取值：获取单链表的第i个元素
// 参考视频： 第3周07--2.5线性表的链式表示和实现7--单链表的基本操作5--取单链表的第i个元素 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=28
// 参考课本：p32-p33
// 时间复杂度：O(n)，因为要从头到尾遍历链表。
Status GetElem_L(LinkList L, int i, int &e){ //获取单链表的第i个元素，通过变量e返回。注意 int &e 可以是ElemType &e。
    LNode *p = L->next; // 指向第一个节点
    int j = 1; // 计数器
    while (p && j < i){ // p不为空且计数器小于i时
        p = p->next; // 指向下一个节点
        j++; // 计数器加1
    }
    if (!p || j > i) return ERROR; // p为空或计数器大于i时，说明i值不合法。比如i=0的情况下不合法
    e = p->data; // 获取第i个元素的内容
    return OK;
}
// 或者改为 用我自己的以下方法，不需要额外的计数器j，更简洁。
// Status GetElem_L(LinkList L,int i, int &e){
//     LNode *p = L->next;
//     if(i<1) return ERROR;
//     while(p&&i>1){
//         p=p->next;
//         i--;
//     }
//     if (!p) return ERROR;
//     e = p->data;
//     return OK
// }

// 7. 【算法2.8】按值查找：在单链表中查找值为e的元素，返回指针
// 参考视频： 第3周08--2.5线性表的链式表示和实现8--单链表的基本操作6--按值查找 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=29
// 参考课本：p33-p34
// 时间复杂度：O(n)，因为要从头到尾遍历链表。
LNode *LocateElem_L(LinkList L, int e){ // 在单链表中查找值为e的元素。LNode *LocateElem这种定义方式表示返回的是一个指针，而且指向的是一个LNode类型的数据。
    LNode *p = L->next; // 指向第一个节点
    while(p && p->data!=e) // p不为空且p的数据域不等于e时
        p = p->next;
    return p; // 返回找到的节点的地址. 如果没找到，会自动返回NULL，因为此时指针已经到了链表的末尾的next，也就是NULL。
}

// 8. 【算法2.8的变化】按值查找：在单链表中查找值为e的元素，获取位置序号
// 参考视频： 第3周08--2.5线性表的链式表示和实现8--单链表的基本操作6--按值查找 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=29
// 时间复杂度：O(n)，因为要从头到尾遍历链表。
int LocateElem_L_id(LinkList L, int e){ // 在单链表中查找值为e的元素，返回位置序号
    LNode *p = L->next; // 指向第一个节点
    int i = 1; // 计数器，注意是从1开始而不是0，因为 *p 指向的是第一个节点而不是头结点。
    while(p && p->data!=e){ // p不为空且p的数据域不等于e时
        p = p->next;
        i++; // 计数器加1
    }
    if (p) return i; // 返回位置序号
    else return 0; // 未找到元素e,返回0
}

// 9. 【算法2.9】插入：在单链表的第i个位置插入元素e
// 参考视频： 第3周09--2.5线性表的链式表示和实现9--单链表的基本操作7--插入元素 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=30
// 参考课本：p34-p35
// 时间复杂度：O(1)，因为不需要移动元素，只用修改指针。注意这是针对插入操作本身，不包含查询遍历的时间。严格来说，时间复杂度是O(n)
Status ListInsert(LinkList& L, int i, int e){ // 在单链表的第i个位置插入元素e。i的合法值是1到n+1，n是链表长度。
    LNode *p = L; // 指向头结点。之所以不是指向第一个节点也就是L->next，是因为插入操作可能会在第一个节点之前插入。
    int j = 0; // 计数器
    while(p && j < i-1){ // p不为空且计数器小于i-1时
        p = p->next; // 指向下一个节点
        j++; // 计数器加1
    } // 循环结束时，p指向第i-1个结点
    if (!p || j > i-1) return ERROR; // p为空或计数器大于i-1时，说明i值不合法。比如i<=0的情况下不合法。
    LNode *s = new LNode; // 分配一个新结点
    s->data = e; // 新结点赋值
    s->next = p->next; // 将 新结点的指针域 指向p的下一个结点
    p->next = s; // 将p的指针域指向新结点
    return OK;
}

// 10. 【算法2.10】删除：删除单链表的第i个位置的元素
// 参考视频： 第3周10--2.5线性表的链式表示和实现10--单链表的基本操作8--删除元素 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=31
// 参考课本：p35-p36
//// 时间复杂度：O(1)，因为不需要移动元素，只用修改指针。注意这是针对删除操作本身，不包含查询遍历的时间。严格来说，时间复杂度是O(n)
Status ListDelete(LinkList& L ,int i){ // 删除单链表的第i个位置的元素。i的合法值是1到n，n是链表长度。
    LNode *p = L; // 指向头结点。之所以不是指向第一个节点也就是L->next，是因为删除操作可能会删除第一个节点，此时需要用到头结点去连接到第二个节点。
    int j = 0; // 计数器
    while(p->next && j < i-1){ // p的下一个结点不为空且计数器小于i-1时。
        // 注意，删除和插入的区别在于while和if这里是p->next，而不是p，因为插入允许下一个结点为空（下一个节点是要插入的位置），而删除不允许下一个结点为空（下一个节点是要删除的位置）
        p = p->next; // 指向下一个节点
        j++; // 计数器加1
    }// 循环结束时，p指向第i-1个结点
    if (!p->next || j > i-1) return ERROR; // p的下一个结点为空或计数器大于i-1时，说明i值不合法。比如i<=0的情况下不合法。
    LNode *q = p->next; // q指向要删除的结点
    p->next = q->next; // 将p的指针域指向q的下一个结点。相当于p->next = p->next->next;
    int e = q->data; // 保存被删除的元素, 也许之后会考虑return e，函数也改成int类型，此时函数返回的事被删除的结点的值(data)。
    delete(q); // 删除q结点
    return OK;
}

// 11. 【算法2.11】前插法创建单链表
// 参考视频： 第3周12--2.5线性表的链式表示和实现12--单链表的基本操作10--前插法创建单链表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=33
// 参考课本：p36-p37
// 时间复杂度：O(n)，因为要从头到尾遍历链表。
void CreateList_H(LinkList& L, int n){ // 前插法创建单链表，n是链表长度。H表示Head的意思，对应前插法。
    L = new LNode; // 创建头结点
    L->next = NULL; // 头结点的指针域为空
    // 以上两行相当于InitList(L)的功能，也就是初始化链表（一个只有头结点的空链表）。
    for (int i = 0; i < n; i++){
        LNode *p = new LNode; // 创建新结点
        cin >> p->data; // 输入新结点的数据。注意是逆序输入，即先输入的数据在链表的后面。比如为了创建线性表(1,2,3)，输入的数据应该是3,2,1。
        p->next = L->next; // 将新结点的指针域指向头结点的下一个结点
        L->next = p; // 将头结点的指针域指向新结点
    }
}

// 12. 【算法2.12】后插法创建单链表
// 参考视频： 第3周13--2.5线性表的链式表示和实现13--单链表的基本操作11--后插法创建单链表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=34
// 参考课本：p37-p38
// 时间复杂度：O(n)，因为要从头到尾遍历链表。
void CreateList_R(LinkList& L, int n){ // 后插法创建单链表，n是链表长度。R表示Rear的意思，对应后插法。
    L = new LNode; // 创建头结点
    L->next = NULL; // 头结点的指针域为空
    // 以上两行相当于InitList(L)的功能，也就是初始化链表（一个只有头结点的空链表）。
    LNode *r = L; // r指向尾结点，初始时指向头结点
    for (int i = 0; i < n; i++){
        LNode *p = new LNode; // 创建新结点
        cin >> p->data; // 输入新结点的数据。注意是顺序输入，即先输入的数据在链表的前面。比如为了创建线性表(1,2,3)，输入的数据应该是1,2,3。
        p -> next = NULL; // 新结点的指针域为空。也可以是p->next = r->next; 毕竟r->next此时是NULL。
        r->next = p; // 将尾结点的指针域指向新结点
        r = p; // r指向新结点，即新结点成为新的尾结点
    }
}

 /* ---------- 2.5.3 循环链表---------- */


// 13. 循环链表
// 参考视频： 第04周02--2.5.3循环链表2--两个链表合并 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=36
// 参考课本：p39
// 13.1. 初始化循环链表
Status InitList_C(LinkList& L){ // 初始化循环链表
    L = new LNode; // 分配一个头结点
    L->next = L; // 头结点的指针域指向头结点自己，而不是NULL
    return OK;
}
// 13.2. 判断循环链表是否为空  
int ListEmpty_C(LinkList L){ // 判断循环链表是否为空
    if (L->next == L) return 1; // 循环链表为空
    else return 0; // 循环链表不为空
}
// 13.3. 合并两个带尾指针的循环链表
LinkList Connect_Cir_Rear(LinkList A, LinkList B){ // 合并两个带尾指针的循环链表。假设A和B已经做过循环链表初始化，且A和B的尾结点的next指针指向头结点。
    LNode *p = A->next; // 指向A的头结点。因为A是尾指针，所以A->next是头结点。
    A->next = B->next->next; // A的尾结点指向B的首元结点
    delete(B->next); // 删除B的头结点
    B->next = p; // B的尾结点指向A的首元结点
    return B; // 返回合并后的循环链表。注意是返回B，因为B是尾指针，所以返回B就是返回合并后的循环链表。
}
// 下面是我写的另一种方法，更容易理解，与上面的代码等价。

// LinkList Connect_Cir_Rear(LinkList A, LinkList B){ 
//     LNode *p = B->next; // 指向B的头结点。因为B是尾指针，所以B->next是头结点。
//     B->next = A->next; // B的尾结点指向A的头结点
//     A->next = B->next->next; // A的尾结点指向B的首元结点
//     delete(p); // 删除B的头结点
//     return B; // 返回合并后的循环链表。注意是返回B，因为B是尾指针，所以返回B就是返回合并后的循环链表。
// }

 /* ---------- 2.5.4 双向链表---------- */

// 14. 双向链表
// 参考视频： 第04周03--2.5.4双向链表1--双向链表的定义和基本操作 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=37
// 参考课本：p39-p40

// 14.1. 定义双向链表
typedef struct DuLNode{ // 双向链表结点类型
    int data; // 数据域
    struct DuLNode *prior, *next; // 前驱指针和后继指针
}DuLNode, *DuLinkList; // DuLNode是结点类型名，DuLinkList是指向结点的指针类型名。

// 14.2 取值：获取双向链表的第i个元素
DuLNode *GetElemP_DuL(DuLinkList L, int i){ // 获取双向链表的第i个元素的地址。i的合法值是1到n，n是链表长度。
    DuLNode *p = L->next; // 指向首元结点
    int j = 1; // 计数器
    while (p && j < i){ // p不为空且计数器小于i时
        p = p->next; // 指向下一个结点
        j++; // 计数器加1
    }
    if (!p || j > i) return NULL; // p为空或计数器大于i时，说明i值不合法。比如i<=0的情况下不合法。  
    return p; // 返回第i个结点的地址
}

// 14.2.【算法2.13】双向链表的插入
// 参考视频： 第04周04--2.5.4双向链表2--双向链表的插入操作 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=38
// 参考课本：p40
// 时间复杂度：O(n)，因为GetElemP_DuL的时间复杂度是O(n)。
Status ListInsert_DuL(DuLinkList &L, int i, int e){ // 在双向链表的第i个位置 之前 插入元素e。i的合法值是1到n, n是链表长度。
    // 此时可能暂时没考虑插入到尾部节点的后面的情况。不像ListInsert，ListInsert是插入到第i个位置，包括尾部节点，i的合法值是1到n+1。
    DuLNode *p = GetElemP_DuL(L, i); // 获取第i个结点的地址
    if (!p) return ERROR; // 如果p为空，则表示第i个结点不存在，返回ERROR。
    DuLNode *s = new DuLNode; // 分配新结点
    s->data = e; // 新结点赋值
    s->prior = p->prior; // 新结点的前驱指针指向p的前驱结点
    p->prior->next = s; // p的前驱结点的后继指针指向新结点
    s->next = p; // 新结点的后继指针指向p
    p->prior = s; // p的前驱指针指向新结点
    return OK;
}

// 14.3.【算法2.14】双向链表的删除
// 参考视频： 第04周05--2.5.4双向链表3--双向链表的删除操作 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=39
// 参考课本：p40
// 时间复杂度：O(n)，因为GetElemP_DuL的时间复杂度是O(n)。
Status ListDelete_DuL(DuLinkList &L, int i){ // 删除双向链表的第i个位置的元素。i的合法值是1到n，n是链表长度。
    DuLNode *p = GetElemP_DuL(L, i); // 获取第i个结点的地址
    if (!p) return ERROR; // 如果p为空，则表示第i个结点不存在，返回ERROR。
    p->prior->next = p->next; // p的前驱结点的后继指针指向p的后继结点
    p->next->prior = p->prior; // p的后继结点的前驱指针指向p的前驱结点
    int e = p->data; // 保存被删除的元素，也许之后会考虑return e，函数也改成int类型，此时函数返回的事被删除的结点的值(data)。
    delete(p); // 删除p结点
    return OK;
}