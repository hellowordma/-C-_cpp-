/***********************************************************
 * 第3章  栈和队列
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏；
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p48-p49

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


 /* ---------- Start of 定义 栈和队列的各种前置函数。参考Ch3_3和Ch3_5 ---------- */
// 定义链队列结点
// 参考课本：p73
typedef struct QNode{
    int data; // 数据域。int本来应该是QElemType
    struct QNode *next; // 指针域
}QNode, *QueuePtr;
 /* ---------- End of 定义 栈和队列的各种前置函数  的各种前置函数---------- */

// 1. 双栈的ADT
// 将编号为0和1的两个栈存放于一个数组空间V[m],栈底分别位于数组两端(V[0]和V[m-1]).
//  - 当第0栈空时，栈顶指针top[0] =-1；当第1栈空时，栈顶指针top[1] = m。
//  - 两个栈均从两端向中间生长（见图3.17）。请编写双栈的初始化，判断栈空，栈满，入栈和出栈的算法函数。
//  - 双栈数据结构的定义如下：
// 参考习题：p60-61；

typedef struct DblStack{
    int bot[2],top[2]; // 栈底指针和栈顶指针
    int *V; // 动态分配的数组空间. int其实是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int m; // 栈的大小
}DblStack;

// InitStack_Dbl函数
Status InitStack_Dbl(DblStack &S, int m){ // 初始化双栈
    S.V = new int[m]; // 分配m个元素的数组空间
    if(!S.V) return OVERFLOW; // 存储空间分配失败
    S.bot[0] = -1; // 栈底指针bot[0]指向栈底
    S.bot[1] = m; // 栈底指针bot[1]指向栈底
    S.top[0] = -1; // 栈顶指针top[0]指向栈底
    S.top[1] = m; // 栈顶指针top[1]指向栈底
    return OK;
}
// StackEmpty_Dbl函数
Status StackEmpty_Dbl(DblStack S, int i){ // 判断栈空
    return S.top[i] == S.bot[i]; // 栈空
}
// StackFull_Dbl函数
Status StackFull_Dbl(DblStack S){ // 判断栈满
    return S.top[0] + 1 == S.top[1]; // 栈满
}
// Pop_Dbl函数
Status Pop_Dbl(DblStack &S, int i, int &e){ // 出栈操作
    if (S.top[i] == S.bot[i]) return ERROR; // 栈空
    if(i==0){ // 如果是第0栈
        e = S.V[S.top[0]--]; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top[0]; e = S.V[S.top[0]];。
    }
    else{ // 如果是第1栈
        e = S.V[S.top[1]++]; // 将栈顶元素赋值给e，栈顶指针上移。相当于 ++S.top[1]; e = S.V[S.top[1]];。
    }
    return OK;
}
// Push_Dbl函数
Status Push_Dbl(DblStack &S, int i, int e){ // 入栈操作。int本质上是SElemType
    if (S.top[0] + 1 == S.top[1]) return ERROR; // 栈满
    if(i==0){ // 如果是第0栈
        S.V[++S.top[0]] = e; // 将元素e压入栈顶，栈顶指针上移。相当于 S.V[S.top[0]] = e; S.top[0]++;
    }
    else{ // 如果是第1栈
        S.V[--S.top[1]] = e; // 将元素e压入栈顶，栈顶指针下移。相当于 S.V[S.top[1]] = e; S.top[1]--;
    }
    return OK;
}

// 2.回文串的判断。
// 回文是指正读和反读都一样的字符串，如“abcba”、“12321”都是回文，但是“abcde”、“12345”不是回文。
// 请编写一个算法，判断一个字符串是否是回文串。（提示：可以将一半的字符入栈）。
// 参考习题：p61-62；

// strlen函数：计算字符串长度。请提供这个函数
#include <cstring> // 字符串处理函数库
// 定义char类型的顺序栈
typedef struct SqStack_char{
    char *base; 
    char *top; 
    int stacksize; 
}SqStack_char;
# define MAXSIZE 100 // 栈的最大容量
// InitStack_char函数
Status InitStack_char(SqStack_char &S){ 
    S.base = new char[MAXSIZE];
    if(!S.base) return OVERFLOW;
    S.top = S.base;
    S.stacksize = MAXSIZE;
    return OK;
}
// StackEmpty_char函数
Status StackEmpty_char(SqStack_char S){ 
    if (S.top == S.base) return 1;
    else return 0;
}
// Pop_char函数
Status Pop_char(SqStack_char &S, char &e){ 
    if (S.top == S.base) 
        return ERROR;
    e = *--S.top;
    return OK;
}
// Push_char函数
Status Push_char(SqStack_char &S, char e){ 
    if (S.top - S.base == S.stacksize) 
        return ERROR;
    *S.top++ = e;
    return OK;
}
// GetTop_char函数
char GetTop_char(SqStack_char S){ 
    if (S.top == S.base) 
        return ERROR;
    return  *(S.top - 1);
}
// 正式开始。
int IsPalindrome(char str[]){ // 判断字符串是否是回文串
    int i; // 定义变量i
    char e; // 定义变量e
    int len = strlen(str); // 字符串长度
    SqStack_char S; // 定义顺序栈S
    InitStack_char(S); // 初始化栈S

    for(i = 0; i < len/2; i++){ // 将前一半的字符入栈
        Push_char(S, str[i]); // 入栈操作
    }
    if(len%2 == 1){ // 如果字符串长度为奇数，则跳过中间的字符
        i++;
    }
    while(!StackEmpty_char(S)){ // 当栈不空时循环
        Pop_char(S, e); // 出栈操作
        if(e != str[i]){ // 如果出栈的字符和当前字符不相等，则不是回文串
            return 0; // 不是回文串
        }
        i++; // 下一个字符
    }
    return 1; // 是回文串
}

// 3. 设从键盘输入一个 整数的序列：a1,a2,a3,...,an，试编写算法实现：用栈结构存储输入的
// 整数，当ai!=-1时，将ai入栈；当ai=-1时，输出栈顶元素并出栈。算法应对异常情况进行处理。（如栈满、栈空等）
// 参考习题：p62-63；
void InOutS(int s[],int n){ 
    // s是元素为int的栈，n是栈的大小。
    int top = 0; // 栈顶指针.相当于初始化stack的操作之一。
    int x;
    for (int i = 0; i < n; i++){
        cin >> x; // 输入整数
        if(x == -1){ // 如果输入-1，则输出栈顶元素并出栈
            if(top == 0){ // 栈空
                cout << "栈空" << endl;
                exit(0); // 退出程序
            }
            else{
                cout << s[--top] << endl; // 输出栈顶元素并出栈
            }
        }
        else{ // 如果输入的不是-1，则入栈
            if(top == MAXSIZE){ // 栈满
                cout << "栈满" << endl;
                exit(0); // 退出程序
            }
            else{
                s[top++] = x; // 入栈操作
            }
        }
    }
    cout << "输入结束" << endl; // 输入结束
}

// 4. 从键盘上输入一个后缀表达式，试编写算法计算该后缀表达式的值。规定：逆波兰表达式的长度
//  不超过1行，以"$"结束, 操作数之间用空格隔开，操作符只有+、-、*、/ 这四种。例如：234 34 + 2*$
// 参考习题：p63-64；

// 定义顺序栈 SqStack
#define MAXSIZE 100 // 最大栈长度
typedef struct SqStack_double{
    double *base; // 栈底指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    double *top; // 栈顶指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int stacksize; // 栈的大小
}SqStack;
//  比SqList多了一个栈底指针*base。

// 【算法3.1】初始化顺序栈InitStack 
// 参考视频：第05周07--3.3栈的表示和实现3--3.3.2栈的顺序栈操作1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=54
// 参考课本：p58
Status InitStack_double(SqStack_double &S){ 
    S.base = new double[MAXSIZE]; // 分配MAXSIZE个元素的数组空间。如果SqStack的元素类型是int，则分配int类型的数组空间；如果SqStack的元素类型是char，则分配char类型的数组空间。
    if(!S.base) return OVERFLOW; // 存储空间分配失败
    S.top = S.base; // 栈顶指针指向栈底，作为初始化（栈空）
    S.stacksize = MAXSIZE; // 栈的大小
    return OK;
}
Status Push_double(SqStack_double &S, double e){ // 入栈操作。int本质上是SElemType
    if (S.top - S.base == S.stacksize) return ERROR; // 栈满
    *S.top++ = e; // 将元素e压入栈顶，栈顶指针上移。相当于 S.V[S.top[0]] = e; S.top[0]++;
}
Status Pop_double(SqStack_double &S, double &e){ // 出栈操作
    if (S.top == S.base) return ERROR; // 栈空
    e = *--S.top; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top; e = *S.top;。
    return OK;
}
double GetTop_double(SqStack_double S){ // 获取栈顶元素
    if (S.top == S.base) return ERROR; // 栈空
    return *(S.top - 1); // 栈顶元素的值
}
// 正式开始
// 后缀表达式求值
double Postfix(){
    SqStack_double OPND;
    InitStack_double(OPND);
    double num = 0,a,b; // 定义变量num、a、b
    char ch = getchar(); // 输入字符.getchar表示从键盘输入一个字符
    // 与cin不同的是，getchar是一个函数，而cin是一个对象。getchar是从键盘输入一个字符，而cin是从键盘输入一个字符串。
    while(ch!='$'){
        // 针对ch是数字的情况，比如'0'、'1'、'2'、'3'、'4'、'5'、'6'、'7'、'8'、'9',
        // 而且考虑连续数字（含小数点），拼接在一起
        int i = 0; // 定义变量i
        char data[100]; // 定义字符数组data，大小为100,可变
        while(ch>='0' && ch<='9'||ch=='.'){ // 如果是数字或者小数点
            data[i++] = ch; // 将字符存入数组data中
            ch = getchar(); // 继续输入字符
        }
        num = atof(data); // 将字符数组转换为浮点数
        Push_double(OPND, num); // 入栈操作
        //  如果不是数字，则判断是什么操作符。
        switch(ch){
            // 遇到操作符时，出栈两个操作数进行运算，然后再入栈结果
            case ' ': break;// 如果是空格，则跳过, 继续输入下一个字符
            case '+': Pop_double(OPND,b);Pop_double(OPND,a);Push_double(OPND,a+b);break; // 加法
            case '-': Pop_double(OPND,b);Pop_double(OPND,a);Push_double(OPND,a-b);break; // 减法
            case '*': Pop_double(OPND,b);Pop_double(OPND,a);Push_double(OPND,a*b);break; // 乘法
            case '/': Pop_double(OPND,b);Pop_double(OPND,a);Push_double(OPND,a/b);break; // 除法
        }
        ch = getchar(); // 继续输入字符
    }
    return GetTop_double(OPND); // 返回栈顶元素
}

// 5.假设以I和O分别表示栈的入栈和出栈操作，栈的初态和终态均为空栈，入栈和出战的操作序列可表示为仅有I和O组成的序列，
//   称可以操作的序列为合法序列。试编写算法，判断一个操作序列是否合法。例如：'IOIIOO'是合法的，'IOOII'是非法的。
// 参考习题：p63-p64；
bool Judge(char A[]){
    // A是操作序列。
    int i = 0; // 定义变量i
    int j =0, k = 0; // 定义变量j和k
    while(A[i] != '\0'){ // 当操作序列不结束时循环。之所以是'\0'，是因为字符串是以'\0'结尾的。
        if(A[i] == 'I'){ // 如果是入栈操作
            j++; // 入栈计数器加1
        }
        else if(A[i] == 'O'){ // 如果是出栈操作
            k++; // 出栈计数器加1
        }
        if(k>j){ // 如果出栈计数器大于入栈计数器，则不合法
            cout << "不合法" << endl; // 输出不合法
            return 0; // 不合法
        }
        i++; // 下一个字符
    }
    if(j == k) {cout << "合法" << endl;return 1;} // 如果入栈计数器和出栈计数器相等，则合法
    else {cout << "不合法" << endl;return 0;} // 否则不合法
}

// 6. 假设以带头结点的循环链表表示队列，并且只设置一个指针指向队尾元素结点（注意：不设头指针），试编写相应的置空队列、判断队列空、队列满、入队和出队的算法函数。
// 参考习题：p64-65；
typedef struct LinkQueue_rear{ // 定义循环链队列
    QueuePtr rear; // 队尾指针only。（不含头指针）
}LinkQueue_rear; // 定义循环链队列结构体

// InitQueue_rear函数
void InitQueue_rear(LinkQueue_rear &Q){ 
    // 置空队列
    // Q.rear = new QNode; // 创建头结点
    QueuePtr s = new QNode; // 
    Q.rear = Q.rear->next; // 队尾指针指向头结点。之后可以视为头结点了。
    while(Q.rear->next != Q.rear){ // 循环删除队列中的元素, 直到队列为空（也就是头结点的指针域指向自己）
        s = Q.rear->next; // s指向首元结点
        Q.rear->next = s->next; // 尾结点的指针域指向新的队头结点
        delete s; // 释放内存
    }
}
// EmptyQueue_rear函数
int EmptyQueue_rear(LinkQueue_rear Q){ 
    // 判断队列空
    // 队列只有一个头结点，即当头结点的指针域指向自己时，队列为空。
    return Q.rear->next->next == Q.rear->next; 
    // 或者者 return Q.rear->next == Q.rear; 因为头结点的指针域指向自己时，队列为空。
}
// Enqueue_rear函数
Status Enqueue_rear(LinkQueue_rear &Q, int e){ 
    // 入队操作
    QueuePtr p = new QNode; // 创建新结点
    p->data = e; // 将数据赋值给新结点
    p->next = Q.rear->next; // 新结点指向队头结点
    Q.rear->next = p; // 队尾指针指向新结点
    Q.rear = p; // 队尾指针指向新结点
    return OK;
}
// Dequeue_rear函数
Status Dequeue_rear(LinkQueue_rear &Q, int &e){ 
    // 出队操作 
    if(Q.rear->next->next == Q.rear->next) return ERROR; // 队空
    QueuePtr p = Q.rear->next->next; // p指向队头结点 
    e = p->data; // 将数据赋值给e
    if (p == Q.rear) { // 如果队列只有一个结点（即出队之后就只剩头结点了
        Q.rear = Q.rear->next; // 修改尾指针指向头结点
        Q.rear->next = p->next; // 
    }
    else { // 如果队列不止一个结点
        Q.rear->next->next = p->next; // 队头指针指向下一个结点
    }
    delete p; // 释放内存
    return OK;
}

// 7.假设数组Q[m]表示一个循环队列的元素，同时设置一个标志tag，以tag==0和tag==1来区别在队头指针
//  和队尾指针（front和rear）相等时，队列是空还是满。试编写相应的enqueue、dequeue算法。
// 参考习题：p66；

// 定义循环队列SqQueue_tag
typedef struct SqQueue_tag{ // 定义循环队列
    int *base; // 队列元素数组
    int front, rear,tag; // 队头指针、队尾指针和标志
}SqQueue_tag; // 定义循环队列结构体

// InitQueue_tag函数
Status InitQueue_tag(SqQueue_tag &Q){ 
    // 初始化循环队列
    Q.base = new int[MAXSIZE]; // 分配m个元素的数组空间
    if(!Q.base) return OVERFLOW; // 存储空间分配失败
    Q.front = Q.rear = 0; // 队头指针和队尾指针置为0，表示队列为空
    Q.tag = 0; // 队列标志置为0，表示队列为空
    return OK;
}

// Enqueue_tag函数
Status Enqueue_tag(SqQueue_tag &Q, int e){ 
    // 入队操作
    if((Q.tag == 1) && Q.front == Q.rear) return ERROR; // 队满
    Q.base[Q.rear] = e; // 将数据赋值给队尾指针
    Q.rear = (Q.rear + 1) % MAXSIZE; // 队尾指针下移
    if (Q.tag == 0) { // 如果队列为空
        Q.tag = 1; // 修改队列标志为1，表示队列不空
    }
    return OK;
}
// Dequeue_tag函数
Status Dequeue_tag(SqQueue_tag &Q, int &e){ 
    // 出队操作
    if((Q.tag == 0) && Q.front == Q.rear) return ERROR; // 队空
    e = Q.base[Q.front]; // 将数据赋值给队头指针
    Q.front = (Q.front + 1) % MAXSIZE; // 队头指针下移
    if(Q.tag==1)
        Q.tag = 0; // 修改队列标志为0，表示队列为空
    return OK;      
}

// 8. 如果允许在循环队列的两端都进行插入和删除操作，试编写相应的算法。要求：
//  - 1. 写出循环队列的定义；
//  - 2. 写出从队尾删除 和 从队头插入的算法；
// 参考习题：p66-67；

typedef struct SqQueue_dbl{ // 定义循环双端队列
    int *base; // 队列元素数组
    int front, rear; // 队头指针和队尾指针
}SqQueue_dbl; // 定义循环双端队列结构体
// 假设front指向队头元素的前一个元素，rear指向队尾元素。
// （一般的循环队列则是front指向队头元素，rear指向队尾元素的下一个元素）

// Enqueue_dbl函数
Status Enqueue_dbl(SqQueue_dbl &Q, int e){ 
    // 从队头插入操作
    if((Q.rear + 1) % MAXSIZE == Q.front) return ERROR; // 队满
    Q.base[Q.front] = e; // 将数据赋值给队头指针
    Q.front = (Q.front - 1+ MAXSIZE) % MAXSIZE; // 队头指针下移
    return OK;
}
// Dequeue_dbl函数
Status Dequeue_dbl(SqQueue_dbl &Q, int &e){ 
    // 从队尾删除操作
    if(Q.front == Q.rear) return ERROR; // 队空
    e = Q.base[Q.rear]; // 将数据赋值给队尾指针
    Q.rear = (Q.rear - 1 + MAXSIZE) % MAXSIZE; // 队尾指针下移
    return OK;
}

// 9. 已知Ackermann函数的递归定义如下：
//  - A(m,n) = n+1, m=0；
//  - A(m,n) = A(m-1,1), m=1；
//  - A(m,n) = A(m-1,A(m,n-1)), m>1；
// - 1. 请用递归算法实现Ackermann函数；
// - 2. 请用非递归算法实现Ackermann函数；
// 参考习题：p67-68；

// 9（1）. Ackermann函数的递归算法实现
int Ackermann(int m, int n){ // Ackermann函数的递归算法实现
    if(m == 0) return n + 1; // 当m=0时，返回n+1
    else if( n ==0) return Ackermann(m - 1, 1); // 当m=1时，返回Ackermann(m-1,1)
    else return Ackermann(m - 1, Ackermann(m, n - 1)); // 当m>1时，递归调用Ackermann函数
} // 返回Ackermann(m,n)的值

// 9（2）. Ackermann函数的非递归算法实现
int Ackermann_non_recursive(int m, int n){ // Ackermann函数的非递归算法实现
    // ack[i][j]初始化
    int ack[m + 1][n + 1]; // 定义二维数组ack
    for(int j = 0; j <= n; j++){ // 初始化ack数组
        ack[0][j] = j + 1; // 当m=0时，返回j+1
    } // 得到ack[0][j]的值也就是Ackermann(0,n)的值

    for(int i = 1; i <= m; i++){ // 这是m!=0的情况
        ack[i][0] = ack[i - 1][1]; // 当n=0时，返回ack[i-1][1]的值，也就是Ackermann(m,0)的值
        for(int j = 1; j <= n; j++){ // 初始化ack数组
            ack[i][j] = ack[i - 1][ack[i][j - 1]]; // 当m>1时，递归调用Ackermann函数
        }
    }
    return ack[m][n]; // 返回ack[m][n]的值，也就是Ackermann(m,n)的值
}

// 10. 已知f为单链表的表头指针，链表中存储的数据为整数，试编写递归算法：
//  - 1. 求链表中的最大值；
//  - 2. 求链表的结点个数；
//  - 3. 求链表的平均值；

// 参考习题：p68-p69；

// 10（1）. 求链表中的最大值
typedef struct LNode{ // struct 表示结构体，typedef表示给结构体取别名，如果在大括号后面有多个变量，则这些变量等价，都表示这个结构体。
    int data; // 结点的数据域
    struct LNode *next; // 结点的指针域。实际上是个嵌套struct LNode。
}LNode, *LinkList; // LNode是节点类型名，LinkList是指向节点的指针类型名。二者等价。一般用LinkList L表示一个链表，用LNode *p表示一个节点。

int GetMax(LinkList f){ // 求链表中的最大值
    if(f == NULL) return 0; // 链表为空，返回0
    if(!f->next) return f->data; // 链表只有一个结点，返回该结点的值
    int max = GetMax(f->next); // 递归调用GetMax函数，求链表中剩余结点的最大值
    return f->data > max ? f->data : max; // 返回链表中最大值
}
// 10（2）. 求链表的结点个数
int GetLength(LinkList f){ // 求链表的结点个数
    if(f == NULL) return 0; // 链表为空，返回0
    if (!f->next) return 1; // 链表只有一个结点，返回1
    else return 1 + GetLength(f->next); // 递归调用GetLength函数，求链表中剩余结点的个数
}
// 10（3）. 求链表的平均值
double GetAverage(LinkList f, int n){ // 求链表的平均值
    if(f == NULL) return 0; // 链表为空，返回0
    if (!f->next) return f->data; // 链表只有一个结点，返回该结点的值
    else{
        double sum = f->data + (n - 1) * GetAverage(f->next, n - 1); // 递归调用GetAverage函数，求链表中剩余结点的平均值
        return sum / n; // 返回链表中平均值
    }
} 