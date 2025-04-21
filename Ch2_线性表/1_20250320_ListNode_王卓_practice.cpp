#include <stdio.h> // 标准输入输出流
#include <iostream> // 输入输出流

using namespace std; // 使用标准命名空间

// Status定义
#define OK 1
#define ERROR 0
#define OVERFLOW -2

typedef int Status; // Status是函数的类型，其值是函数结果状态代码

// 定义节点
typedef struct LNode {
    int data;
    struct LNode* next;
} LNode, *LinkList;

// 初始化链表。意义在于添加一个头结点，头结点的数据域不存放数据，头指针指向头结点。
Status InitList(LinkList& L) {
    L = new LNode; // 分配一个头结点
    L->next = NULL; // 头结点的指针域为空
    return OK;
}
// 删除链表
Status DestroyList(LinkList& L) {
    LNode* p; // 临时指针
    while (L) { // 链表不为空
        p = L->next; // 保存下一个节点的地址
        delete(L); // 删除当前节点
        L = p; // 指向下一个节点
    } // 头节点也要删除
    return OK;
}

// 清空链表
Status ClearList(LinkList& L) {
    LNode* p, * q; // 临时指针
    p = L->next; // 指向第一个节点而不是头结点
    while (p) { // 头结点之后的节点不为空时
        q = p->next; // 保存下一个节点的地址
        delete(p); // 删除当前节点
        p = q; // 指向下一个节点
    }
    L->next = NULL; // 头结点指针域为空。保留头结点。
    return OK;
}

// 实例化
int main() {
    LinkList L;
    InitList(L); // 初始化链表
    cout << "初始化成功" << endl;
    printf("L->next = %p\n", (void*)L->next); // 打印指针的值
    ClearList(L); // 清空链表
    printf("清空成功\n");
    printf("L->next = %p\n", (void*)L->next); // 打印指针的值
    return 0;
}