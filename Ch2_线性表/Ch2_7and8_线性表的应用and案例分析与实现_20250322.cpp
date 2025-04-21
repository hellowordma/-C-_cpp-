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
* 2.7 线性表的应用
 *==================================*/


 /* ----------定义线性表的各种前置函数。参考Ch2_4_线性表的顺序表示_20250321.cpp ---------- */

// 定义线性表SqList
#define MAXSIZE 100 // 存储空间初始分配量
typedef struct{
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

 /* ----------end of 定义线性表的各种前置函数---------- */


/* ---------- 2.7.1 线性表的合并---------- */

// 1. 【算法2.15】线性表的合并(顺序表)。相当于求2个集合的并集。
// 参考视频：第04周08--2.7线性表的应用1--线性表的合并 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=42
// 参考课本：p42
// 时间复杂度：O(m*n)，m和n分别是两个线性表的长度
void MergeList(SqList &LA, SqList LB){ // LA和LB是线性表，求LA和LB的并集，结果存放在LA中
    int m = ListLength(LA); int n = ListLength(LB); // 求线性表的长度
    for (int i = 1; i <= n; i++){
        // e 初始化
        int e;
        GetElem(LB, i, e); // 取LB中第i个元素赋给e
        if (!LocateElem(LA, e)){ // 如果LA中不存在和e相同的元素
            ListInsert(LA, ++m, e); // 插入到LA的末尾。注意：++m而不是m++，是因为++m是先自增再赋值，m++是先赋值再自增。
            // 第一次ListInsert的时候，相当于e插入到LA的第m+1个位置，也就是LA的末尾。
            // 举例：如果LA的长度是3，m=3，那么++m=4，也就是插入到LA的第4个位置，也就是LA的末尾。
        }
    }
}

/* ---------- 2.7.2 有序表的合并:---------- */

// 有两种情况，一种是顺序有序表的合并，一种是链式有序表的合并。
// 1. 【算法2.16】顺序有序表的合并。求解有序集合的并集的问题。
// 参考视频：第04周09--2.7线性表的应用2--顺序有序表的合并 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=43
// 参考课本：p43-p44
// 时间复杂度：O(m+n)，m和n分别是两个线性表的长度；空间复杂度：O(m+n)
void MergeList_Sq(SqList LA, SqList LB, SqList &LC){ // LA和LB是线性有序表,这里是非递减有序表；LC是合并后的线性有序表，也是非递减有序表。
    int *pa=LA.elem, *pb=LB.elem; // 指针p和q分别指向LA和LB的第一个元素
    LC.length = LA.length + LB.length; // LC的长度为LA和LB的长度之和
    int *pc = LC.elem; // 指针pc指向LC的第一个元素    
    int *pa_last = LA.elem + LA.length - 1; // 指针pa_last指向LA的最后一个元素。注意：LA.elem是数组的首地址，LA.elem+LA.length-1是数组的最后一个元素的地址。
    int *pb_last = LB.elem + LB.length - 1; // 指针pb_last指向LB的最后一个元素。注意：LB.elem是数组的首地址，LB.elem+LB.length-1是数组的最后一个元素的地址。
    // 注意：在 C/C++ 中，指针的算术运算已经考虑了所指向类型的大小，因此不需要额外乘以 elem 类型的字节长度。

    while (pa <= pa_last && pb <= pb_last){ // pa和pb均未到达表尾
        if (*pa <= *pb) *pc++ = *pa++; // 将pa所指元素插入到pc所指位置，pa和pc同时后移。
        else *pc++ = *pb++; // 将pb所指元素插入到pc所指位置，pb和pc同时后移
        // 注意：*pc++ = *pa++; 是先赋值再自增。举例：如果*pa=1，*pb=2，那么*pc=1，pa和pc同时后移，此时*pa=2，*pb=2，那么*pc=2，pb和pc同时后移。
        // 进一步理解：*pc++ = *pa++;意味着先将*pa的值赋给*pc，然后pc指针后移，pa指针后移。
        // 再注意：++优先级高于*，所以*pc++ = *pa++;等价于*(pc++) = *(pa++)，也就是先*pc=*pa，然后pc后移，pa后移。
    }
    // 以下两个while只有一个会执行，因为pa和pb只有一个会先到达表尾
    while (pa <= pa_last) *pc++ = *pa++; // 插入LA的剩余元素
    while (pb <= pb_last) *pc++ = *pb++; // 插入LB的剩余元素
}

// 2. 【算法2.17】链式有序表的合并。求解有序集合的交集的问题。
// 参考视频：第04周10--2.7线性表的应用3--链式有序表的合并 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=44
// 参考课本：p44-p45
// 时间复杂度：O(m+n)，m和n分别是两个线性表的长度；空间复杂度：O(1)
void MergeList_L(LinkList &La, LinkList &Lb, LinkList &Lc){ // La和Lb是链式有序表,这里是非递减有序表；Lc是合并后的链式有序表，也是非递减有序表。 
    LNode *pa = La->next, *pb = Lb->next; // pa和pb分别指向La和Lb的首元结点
    Lc = La; // 用La的头结点作为Lc的头结点，相当于不额外分配空间，Lc借用La的头结点。
    LNode *pc = Lc; // pc指向Lc的头结点
    while (pa && pb){ // pa和pb均未到达表尾
        if (pa->data <= pb->data){ // pa的数据域小于等于pb的数据域
            pc->next = pa; // 将pa结点插入到pc结点之后
            pc = pa; // pc指向pa
            pa = pa->next; // pa后移
        }
        else{ // pa的数据域大于pb的数据域
            pc->next = pb; // 将pb结点插入到pc结点之后
            pc = pb; // pc指向pb
            pb = pb->next; // pb后移
        }
    }
    pc->next = pa ? pa : pb; // 插入剩余段。三目运算符，如果pa不为空，那么pc的next指向pa，否则指向pb。
    delete Lb; // 释放Lb的头结点
}


/*==================================
* 2.8 案例分析与实现
 *==================================*/

 /* ---------- 2.8.2 【案例2.2】稀疏多项式的表示与运算---------- */
// 定义多项式的结点
typedef struct PNode{
    float coef; // 系数
    int expn; // 指数
    struct PNode *next; // 指针域
}PNode, *Polyn;

// 1. 【算法2.18】多项式的创建（相当于链式有序表的构建）
// 参考视频：第04周12--2.8案例分析与实现2--稀疏多项式的表示与运算 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=46
// 参考课本：p47-p48
// 时间复杂度：O(n²)，因为涉及到排序，最坏情况下n次循环每次都有n次比较，所以是n²
void CreatePolyn(Polyn &P, int n){ // 输入n项的系数和指数，建立表示一元多项式的有序链表P
    P = new PNode; // 创建头结点
    P->next = NULL; // 头结点的指针域为空
    for (int i = 0; i < n; i++){ // 依次输入n个非零项
        PNode *s = new PNode; // 创建新结点
        cin >> s->coef >> s->expn; // 输入系数和指数
        PNode *pre = P; // pre指向头结点，相当于 当前指针的前一个指针。
        PNode *q = P->next; // q指向首元结点，相当于 当前指针。
        // 通过比较指数找到第一个大于输入项指数的项*q
        while (q && q->expn < s->expn){  // 当前指针q存在且q的指数小于输入项的指数时，q后移
            pre = q; // pre指向q
            q = q->next; // q后移
        }
        s->next = q; // 将输入项s插入到pre和q之间
        pre->next = s; // pre的next指向s
    }//for 
}

// 2. 【算法2.19】多项式的相加(两个多项式合并,相当于两个链式有序表的合并)
// 参考视频：第04周12--2.8案例分析与实现2--稀疏多项式的表示与运算 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=46
// 参考课本：p48-p49
// 时间复杂度：O(m+n)，m和n分别是两个多项式的项数
// 空间复杂度：O(1)，因为最后使用了已有的Pb的头结点，所以不需要额外的空间。
void AddPolyn(Polyn &Pa, Polyn &Pb){ // 多项式加法：Pa = Pa + Pb
    PNode *p1 = Pa->next; // p1指向Pa的首元结点
    PNode *p2 = Pb->next; // p2指向Pb的首元结点
    PNode *p3 = Pa; // p3指向Pa的头结点
    while (p1 && p2){ // p1和p2均未到达表尾
        if (p1->expn == p2->expn){ // 两者指数相等
            float sum = p1->coef + p2->coef; // 系数相加
            if (sum != 0){ // 系数和不为0
                p1->coef = sum; // 修改Pa的系数
                p3->next = p1; // 将p1插入到p3之后
                p3 = p1; // p3指向p1。
                p1 = p1->next; // p1后移。
                // 注意：此时p3不会变，因为这只是地址副本，p3=p1只是让p3指向p1，p3不会随着p1的变化而变化。
                PNode *r = p2; // 保存p2的地址
                p2 = p2->next; // p2后移
                delete r; // 删除p2
            }
            else{ // 系数和为0
                PNode *r = p1; p1 = p1->next; delete r; // 删除Pa当前结点，p1后移一位
                r = p2; p2 = p2->next; delete r; // 删除Pb当前结点，p2后移一位
            } 
        } 
        else if (p1->expn < p2->expn){ // p1的指数小于p2的指数
            p3->next = p1; // 将p1插入到p3之后
            p3 = p1; // p3指向p1
            p1 = p1->next; // p1后移
        }
        else{ // p1的指数大于p2的指数
            p3->next = p2; // 将p2插入到p3之后
            p3 = p2; // p3指向p2
            p2 = p2->next; // p2后移
        } 
    }//while
    p3->next = p1 ? p1 : p2; // 插入剩余段
    delete Pb; // 释放Pb的头结点
}
