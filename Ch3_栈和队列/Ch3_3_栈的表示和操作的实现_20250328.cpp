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
* 3.3 栈的表示和操作的实现
 *==================================*/


 /* ----------3.3.2.顺序栈的表示和实现------------------- */

// 定义顺序栈 SqStack
#define MAXSIZE 100 // 最大栈长度
typedef struct SqStack{
    int *base; // 栈底指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int *top; // 栈顶指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int stacksize; // 栈的大小
}SqStack;
//  比SqList多了一个栈底指针*base。

// 【算法3.1】初始化顺序栈InitStack 
// 参考视频：第05周07--3.3栈的表示和实现3--3.3.2栈的顺序栈操作1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=54
// 参考课本：p58
Status InitStack(SqStack &S){ 
    S.base = new int[MAXSIZE]; // 分配MAXSIZE个元素的数组空间。如果SqStack的元素类型是int，则分配int类型的数组空间；如果SqStack的元素类型是char，则分配char类型的数组空间。
    if(!S.base) return OVERFLOW; // 存储空间分配失败
    S.top = S.base; // 栈顶指针指向栈底，作为初始化（栈空）
    S.stacksize = MAXSIZE; // 栈的大小
    return OK;
}
// ！！！注意，顺序表的Init函数一般都要判断是否分配内存成功（OVERFLOW），
// 而链表的Init函数一般不需要判断，因为链表是动态分配内存的，系统会自动释放内存。

// 【补充算法1】判断顺序栈空
// 参考视频：第05周08--3.3栈的表示和实现4--3.3.2栈的顺序栈操作2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=55
Status StackEmpty(SqStack S){ // 判断栈空
    if (S.top == S.base) return 1; // 栈空
    else return 0; // 栈非空
}
// 【补充算法2】求顺序栈的长度
// 参考视频：第05周08--3.3栈的表示和实现4--3.3.2栈的顺序栈操作2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=55
int StackLength(SqStack S){ // 求栈的长度
    return (S.top - S.base); // 栈顶指针减去栈底指针
}
// 【补充算法3】清空顺序栈
// 参考视频：第05周08--3.3栈的表示和实现4--3.3.2栈的顺序栈操作2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=55
Status ClearStack(SqStack &S){ // 清空栈
    if(S.base){ // 如果栈底指针不为空。之所以要判断S.base,是因为可能这个栈已经被销毁了，不需要额外清空。
        S.top = S.base; // 栈顶指针指向栈底
    }
    return OK;
}
// 【算法3补充4】销毁顺序栈
// 参考视频：第05周08--3.3栈的表示和实现4--3.3.2栈的顺序栈操作2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=55
Status DestroyStack(SqStack &S){ // 销毁栈
    if(S.base){ // 如果栈底指针不为空。
        delete S.base; // 释放栈底指针指向的内存(把数组回归内存池)。 delete[] S.base更好。
        // 之所以删除S.base而不是S.top，是因为数组的第一个元素的地址就是栈底指针S.base，delete S.base就可以释放整个数组的内存。     
        // 注意： 对于像int/char/long/int*/struct等简单数据类型，由于对象没有destructor，所以用delete和delete[]都可以，
        // 但是如果是数组就只能通过delete[]对数组中的每个元素逐个释放内存。
        S.top = S.base = NULL; // 栈顶指针和栈底指针都置空
        S.stacksize = 0; // 栈的大小置为0
    }
    return OK;
}

// 【算法3.2】顺序栈的入栈Push
// 参考视频：第05周08--3.3栈的表示和实现4--3.3.2栈的顺序栈操作2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=55
// 参考课本：p59
Status Push(SqStack &S, int e){ // 入栈操作。int本质上是SElemType
    if (S.top - S.base == S.stacksize) 
        return ERROR; // 栈满
    *S.top++ = e; // 将元素e压入栈顶，栈顶指针上移。相当于 *S.top = e; S.top++;
    return OK;
}
// 参考视频：第05周09--3.3栈的表示和实现4--3.3.2栈的顺序栈操作3 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=56
// 参考课本：p59
Status Pop(SqStack &S, int &e){ // 出栈操作
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *--S.top; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top; e = *S.top;。
    // 注意不是 *S.top--，因为栈顶元素的值需要top指针下移后才能获取。
    return OK;
}
// 【算法3.4】获取栈顶元素GetTop
// 参考课本：p59
int GetTop(SqStack S, int &e){ // 获取栈顶元素。int本质上是SElemType
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *(S.top - 1); // 栈顶元素的值
    return OK;
}

/* ----------3.3.3.链栈的表示和实现------------------- */
// !!! 注意：与链表相比，链栈没有头结点，只有栈顶指针S。链栈的栈顶指针S指向栈顶元素的地址，而不是头结点的地址。

// 定义链栈结点StackNode
typedef struct StackNode{ // 链栈结点类型
    int data; // 数据域。int本质上是SElemType
    struct StackNode *next; // 指针域
}StackNode, *LinkStack; // StackNode是结点类型名，LinkStack是指向结点的指针类型名。
// 其实跟LNode是一样的，只是名字不同而已。

// 【算法3.5】初始化链栈
// 参考视频：第05周10--3.3栈的表示和实现4--3.3.3链栈表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=57
// 参考课本：p60
Status InitStack_node(LinkStack &S){ // 初始化链栈
    S = NULL; // 栈顶指针置空。没必要设置头结点（L->next = NULL）
    return OK; // 
}
// 【补充算法1】判断链栈空
// 参考视频：第05周10--3.3栈的表示和实现4--3.3.3链栈表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=57
Status StackEmpty_node(LinkStack S){ // 判断链栈空
    if (S == NULL) return 1; // 栈空
    else return 0; // 栈非空
}
// 【算法3.6】链栈的入栈Push
// 参考视频：第05周10--3.3栈的表示和实现4--3.3.3链栈表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=57
// 参考课本：p60
Status Push_node(LinkStack &S, int e){ // 入栈操作。int本质上是SElemType。注意没有头结点。
    StackNode *p = new StackNode; // 创建新结点
    p->data = e; // 新结点赋值
    p->next = S; // 新结点的插入栈顶。注意是 = S而不是s->next。因为S是栈顶指针，指向栈顶元素的地址，而不是头结点的地址。
    S = p; // 栈顶指针修改，指向新结点
    return OK;
}// 与顺序栈的Push操作不同的是，链栈的Push操作不需要判断栈满，因为链栈是动态分配内存的。
// 【算法3.7】链栈的出栈Pop
// 参考视频：第05周10--3.3栈的表示和实现4--3.3.3链栈表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=57
// 参考课本：p60-p61
Status Pop_node(LinkStack &S, int &e){ // 出栈操作
    if (S == NULL) 
        return ERROR; // 栈空
    StackNode *p = S; // p指向栈顶结点
    e = p->data; // 将栈顶元素赋值给e
    S = S->next; // 栈顶指针指向下一个结点
    delete p; // 释放栈顶结点的内存
    return OK;
}
// 注意：对于指针可以先delete再赋值，但是不能先delete再访问比如S->next, 因为delete会释放内存，导致指针悬空。

// 【算法3.8】获取栈顶元素GetTop
// 参考视频：第05周10--3.3栈的表示和实现4--3.3.3链栈表示和实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=57
// 参考课本：p61
int GetTop_node(LinkStack S){ // 获取栈顶元素
    if(S!=NULL){ // 栈非空
        return S->data; // 返回栈顶元素
    }
}
// 【算法3.9】遍历链表中各个结点的算法（递归算法，链表的数据结构是递归的）
// 参考课本：p63-p64
// 定义单链表的结点类型
typedef struct LNode{ // struct 表示结构体，typedef表示给结构体取别名，如果在大括号后面有多个变量，则这些变量等价，都表示这个结构体。
    int data; // 结点的数据域
    struct LNode *next; // 结点的指针域。实际上是个嵌套struct LNode。
}LNode, *LinkList; // LNode是节点类型名，LinkList是指向节点的指针类型名。二者等价。一般用LinkList L表示一个链表，用LNode *p表示一个节点。

void TraverseList(LinkList p){ // 遍历链表
    if (p != NULL){ // p不为空时
        cout << p->data << endl; // 输出结点的数据域
        TraverseList(p->next); // 递归遍历下一个结点
    }
    else return; // p为空时，递归结束
}
// 【算法3.10】Hanoi塔问题
// 参考课本：p64-p65
void move(char A, char C, int n){
    cout << "将第" << n << "个盘子从" << A << "移动到" << C << endl; // 输出移动信息
}
void Hanoi(int n, char A, char B, char C){ // n表示盘子数，A、B、C分别表示三个柱子
    // 将n个盘子从A移动到C，B为辅助柱子。每次只能移动一个盘子，且大盘子不能放在小盘子上面。
    if (n == 1) move(A, C, n); // 只有一个盘子时，直接移动
    else{
        Hanoi(n - 1, A, C, B); // 将上面n-1个盘子从A移动到B
        move(A, C, n); // 将第n个盘子从A移动到C
        Hanoi(n - 1, B, A, C); // 将上面n-1个盘子从B移动到C
    }
}