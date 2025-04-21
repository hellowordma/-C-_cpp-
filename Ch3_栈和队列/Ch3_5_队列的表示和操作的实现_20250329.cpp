/***********************************************************
 * 第3章 栈和队列
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
#define True 1
#define False 0
typedef int Status; // Status是函数的类型，其值是函数结果状态代码


/*==================================
* 3.5 队列的表示和操作的实现
 *==================================*/


 /* ----------3.5.2.顺序（循环）队列的表示和实现------------------- */

// 定义顺序(循环)队列 SqQueue。
// !! 注意：1. 涉及到顺序循环队列，要考虑+ 和 % MAXQSIZE。
//      2. 采用少用1个元素空间的方法区分队满队空（需要+1来判断队满，比如 if((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR;）
#define MAXQSIZE 100 // 最大队列长度
typedef struct SqQueue{
    int *base; // 初始化的动态分配存储空间。int本来应该是QElemType，这是元素类型，可以是int、float、char等任意类型。
    int front; // 头指针，若队列不空，指向队列头元素
    int rear;  // 尾指针，若队列不空，指向队列尾元素的下一个位置
}SqQueue; 
// 比SqList多了一个队头指针front和一个队尾指针rear，少了length。
// 与SqStack相比，SqQueue首尾是数组的整数索引也就是front和rear，而SqStack首尾是指针 *base和*top。
//   这是因为队列是循环队列，使用整数索引（front 和 rear）能够更方便实现循环队列。
//   若使用指针，每次到数组末尾需要重新指向头部，这种循环指针实现较为复杂且容易出错，而索引方式非常清晰明了：

// 【算法3.11】初始化顺序循环队列 Queue
// 参考视频：第05周13--3.5队列的表示和实现2-3.5.2顺序队列表的表示和实现1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=60
// 参考课本：p71-p72
Status InitQueue(SqQueue &Q){
    Q.base = new int[MAXQSIZE]; // 分配MAXQSIZE个元素的数组空间。int本来应该是QElemType，这是元素类型，可以是int、float、char等任意类型。
    if(!Q.base) return OVERFLOW; // 存储空间分配失败
    Q.front = Q.rear = 0; // 头指针和尾指针置为0，表示队列为空
    return OK;
}
// ！！！注意，顺序表的Init函数一般都要判断是否分配内存成功（OVERFLOW），

// 【算法3.12】求顺序循环队列的长度 QueueLength。
// 参考视频：第05周13--3.5队列的表示和实现2-3.5.2顺序队列表的表示和实现1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=60
// 参考课本：p71-p72
int QueueLength(SqQueue Q){ // 记得加上% MAXQSIZE，防止负数。%MAXQSIZE是为了循环队列的实现。
    return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE; // 队列长度。
}
 
// 算法【3.13】循环队列的入队操作 EnQueue
// 参考视频：第05周14--3.5队列的表示和实现2-3.5.2顺序队列表的表示和实现2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=61
// 参考课本：p72
Status EnQueue(SqQueue &Q, int e){  // int本来应该是QElemType
    if((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR; // 队列已满
    Q.base[Q.rear] = e; // 新元素插入队尾
    Q.rear = (Q.rear + 1) % MAXQSIZE; // 队尾指针+1，若到最后则转到数组头部。因此不能直接++Q.rear。

    return OK;
}

// 算法【3.14】循环队列的出队操作 DeQueue
// 参考视频：第05周14--3.5队列的表示和实现2-3.5.2顺序队列表的表示和实现2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=61
// 参考课本：p72
Status DeQueue(SqQueue &Q, int &e){ // int本来应该是QElemType
    if(Q.front == Q.rear) return ERROR; // 队列为空
    e = Q.base[Q.front]; // 保存队头元素
    Q.front = (Q.front + 1) % MAXQSIZE; // 队头指针+1，若到最后则转到数组头部
    return OK;
}

// 算法【3.15】循环队列的获取队头元素操作 GetHead
// 参考视频：第05周14--3.5队列的表示和实现2-3.5.2顺序队列表的表示和实现2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=61
// 参考课本：p73
int GetHead(SqQueue Q){ // int本来应该是QElemType
    if(Q.front == Q.rear) return ERROR; // 队列为空
    return Q.base[Q.front]; // 获取队头元素
}


/* ----------3.5.3 .链队——队列的链式表示和实现------------------- */

// 定义链队列结点
// 参考课本：p73
typedef struct QNode{
    int data; // 数据域。int本来应该是QElemType
    struct QNode *next; // 指针域
}QNode, *QueuePtr;
// 与LNode类似，只是名字不同而已。

// 定义链队列
// 参考课本：p73
typedef struct LinkQueue{
    QueuePtr front; // 队头指针
    QueuePtr rear;  // 队尾指针
}LinkQueue;
// 此时LinkQueue不可类比LinkList，QueuePtr才是类比LinkList的。LinkQueue相当于包括了两个QueuePtr（LinkList）。

// 【算法3.16】初始化链队列 InitQueue_node
// 参考视频：第05周15--3.5队列的表示和实现3-3.5.3链队列的表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=62
// 参考课本：p73
Status InitQueue_node(LinkQueue &Q){
    // 建立头结点
    Q.front = Q.rear = new QNode; // 生成新结点作为头结点
    if(!Q.front) return OVERFLOW; // 内存分配失败
    Q.front->next = NULL; // 头结点的指针域置空，此时尾结点也指向头结点也就是空。
    return OK;
} 
// 与Stack相比，Queue额外设置了头结点

// 【补充算法1】销毁链队列 DestroyQueue_node
// 参考视频：第05周15--3.5队列的表示和实现3-3.5.3链队列的表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=62 
Status DestroyQueue_node(LinkQueue &Q){ // 从头结点开始，依次释放每个结点的空间
    QueuePtr p;
    while(Q.front){ // 释放链队列的所有结点
        p = Q.front->next; // p指向下一个结点。因为与Stack相比，Queue额外设置了头结点，所以这里要先指向下一个结点也就是Q.front->next而不是Q.front本身。
        delete Q.front; // 删除当前结点
        Q.front = p; // 指向下一个结点
        // 另一种方法：Q.rear替换p，因为闲着也是闲着，额外省去一个指针p
        // Q.rear = Q.front->next; // p指向下一个结点
        // delete Q.front; // 删除当前结点
        // Q.front = Q.rear; // 指向下一个结点
    }
    return OK;
}

// 【算法3.17】链队列 将元素e入队 EnQueue_node
// 参考视频：第05周15--3.5队列的表示和实现3-3.5.3链队列的表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=62
// 参考课本：p74
Status EnQueue_node(LinkQueue &Q, int e){ // int本来应该是QElemType
    QueuePtr p = new QNode; // 生成新结点
    if(!p) return OVERFLOW; // 内存分配失败
    p->data = e; // 赋值
    p->next = NULL; // 设置尾结点指针域为空
    Q.rear->next = p; // 插入到队尾
    Q.rear = p; // 修改队尾指针
    return OK;
} 

// 【算法3.18】链队列 出队 DeQueue_node
// 参考视频：第05周15--3.5队列的表示和实现3-3.5.3链队列的表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=62
// 参考课本：p74-p75
Status DeQueue_node(LinkQueue &Q, int &e){ // int本来应该是QElemType
    if(Q.front == Q.rear) return ERROR; // 队列为空
    QueuePtr p = Q.front->next; // p指向队头元素
    e = p->data; // 保存队头元素的值
    Q.front->next = p->next; // 修改头指针
    if(Q.rear == p) Q.rear = Q.front; // 注意特殊情况：如果队列只有一个元素，出队后队尾指针也要修改，指向头结点
    delete p; // 释放结点空间
    return OK;
}

// 【算法3.19】链队列 获取队头元素 GetHead_node
// 参考视频：第05周15--3.5队列的表示和实现3-3.5.3链队列的表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=62
// 参考课本：p75
Status GetHead_node(LinkQueue Q, int &e){
    if(Q.front == Q.rear) return ERROR; // 队列为空
    e = Q.front->next->data; // 获取队头元素
    return OK;
}
