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
* 3.6 栈和队列的案例分析与实现
 *==================================*/


/* ---------- 从Ch3_3导入的栈的基本定义和操作---------- */

// 顺序栈定义
#define MAXSIZE 100 
typedef struct SqStack{
    int *base; 
    int *top; 
    int stacksize; 
}SqStack;

// InitStack函数
Status InitStack(SqStack &S){ 
    S.base = new int[MAXSIZE]; // 分配MAXSIZE个元素的数组空间
    if(!S.base) return OVERFLOW; // 存储空间分配失败
    S.top = S.base; // 栈顶指针指向栈底，作为初始化（栈空）
    S.stacksize = MAXSIZE; // 栈的大小
    return OK;
}
// StackEmpty函数
Status StackEmpty(SqStack S){ // 判断栈空
    if (S.top == S.base) return 1; // 栈空
    else return 0; // 栈非空
}
// Pop函数
Status Pop(SqStack &S, int &e){ // 出栈操作
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *--S.top; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top; e = *S.top;。
    // 注意不是 *S.top--，因为栈顶元素的值需要top指针下移后才能获取。
    return OK;
}
// Push函数
Status Push(SqStack &S, int e){ // 入栈操作。int本质上是SElemType
    if (S.top - S.base == S.stacksize) 
        return ERROR; // 栈满
    *S.top++ = e; // 将元素e压入栈顶，栈顶指针上移。相当于 *S.top = e; S.top++;
    return OK;
}
// GetTop函数
int GetTop(SqStack S){ // 获取栈顶元素。int本质上是SElemType
    if (S.top == S.base) 
        return ERROR; // 栈空
    return *(S.top - 1); // 栈顶元素的值// 改了下，返回值类型为int
}

// 链栈定义
typedef struct StackNode{ 
    int data; 
    struct StackNode *next; 
}StackNode, *LinkStack;


/* ---------- 从Ch3_5导入的队列的基本定义和操作---------- */

// 顺序队列定义
#define MAXQSIZE 100 
typedef struct SqQueue{
    int *base; 
    int front; 
    int rear;  
}SqQueue;

// 链队列定义
typedef struct QNode{
    int data; 
    struct QNode *next; 
}QNode, *QueuePtr;

typedef struct LinkQueue{
    QueuePtr front; 
    QueuePtr rear;  
}LinkQueue;


/* --------------------  4个案例 ------------------------------  */

// 【算法3.20】十进制转八进制
// 参考课本：p75-p76
// 时间复杂度：O(log8_n)
void conversion(int n){ // 十进制转八进制
    SqStack S; // 定义栈S
    InitStack(S); // 初始化栈S
    while(n){ // 循环除8取余，直到n为0
        Push(S, n % 8); // 入栈操作
        n /= 8; // 除8操作
    }
    while(!StackEmpty(S)){ // 栈不空时循环出栈
        int e; // 定义变量e
        Pop(S, e); // 出栈操作
        cout << e; // 输出栈顶元素
    }
    cout << endl; // 换行输出
}

// 【算法3.21】括号匹配
// 参考课本：p76-p77
// 时间复杂度：O(n)

/* ---------- 为Matching函数添加char类型的栈操作 ---------- */

// 定义char类型的顺序栈
typedef struct SqStack_char{
    char *base; 
    char *top; 
    int stacksize; 
}SqStack_char;

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
Status Matching(){
    SqStack_char S; // 改用char类型的栈
    char ch,e; // 定义变量ch, e
    int flag = 1; // 定义变量flag，初始值为1,表示匹配成功
    InitStack_char(S); // 初始化栈S
    cin>>ch; // 输入字符
 
    while(ch != '#' && flag){ // 当输入字符不为#时循环（假设表达式以#为结束符）
        switch(ch){
            // 如果输入字符为左括号或左中括号，则入栈
            case '(': 
            case '[':
                Push_char(S, ch);
                break; // 跳出switch语句
            case ')': // 如果输入字符为右括号
                // 如果栈非空且栈顶元素是左括号，则出栈
                if (!StackEmpty_char(S) && GetTop_char(S)=='(' ){
                    Pop_char(S,e);
                }
                else flag=0;//  匹配失败，flag置为0
                break; // 跳出switch语句
            case ']': // 如果输入字符为右中括号
                // 如果栈非空且栈顶元素是左中括号，则出栈
                if (!StackEmpty_char(S) && GetTop_char(S)=='[' ){
                    Pop_char(S,e);
                }
                else flag=0; // 匹配失败，flag置为0
                break; // 跳出switch语句
        } // 结束switch语句
        cin>>ch; // 继续输入下一个字符
    } // 结束while循环
    // 如果栈空，且flag为1，则匹配成功；否则匹配失败
    if(StackEmpty_char(S) && flag) return 1; 
    else return 0;
}

// 【算法3.22】表达式求值(中缀表达式求值)
// 参考课本：p79
// 时间复杂度：O(n)

/* ---------- 为EvaluateExpression函数添加前置函数 ---------- */

// 判断字符是否为运算符
bool In(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '#')
        return true;
    return false;
}

// 判断运算符优先级
char Precede(char a, char b) {
    // 运算符优先级表。
    // 经过查找，a对应第i行，b对应第j列，得到的priority[i][j]值就是二者的优先级符号。

    char priority[7][7] = {
        /*运算符    +   -    *    /    (     )   # */
        /* + */  {'>', '>', '<', '<', '<', '>', '>'},
        /* - */  {'>', '>', '<', '<', '<', '>', '>'},
        /* * */  {'>', '>', '>', '>', '<', '>', '>'},
        /* / */  {'>', '>', '>', '>', '<', '>', '>'},
        /* ( */  {'<', '<', '<', '<', '<', '=', ' '},
        /* ) */  {'>', '>', '>', '>', '=', '>', '>'},
        /* # */  {'<', '<', '<', '<', '<', ' ', '='}
    }; // 相当于一个二维数组，表示运算符的优先级关系
    //  注意：char[i][j]不一定=char[j][i]，因为优先级关系是单向的。
    // 其中，'>'表示栈顶操作符优先级高于当前操作符，'<'表示栈顶操作符优先级低于当前操作符，'='表示相等，' '表示错误
    // 而且同一档次的优先级，a（左边）＞b（右边）。
    // 甚至同一个char，比如a=b，也不一定=。一般是>，除了#是=，(是<。
    // 若i = j, 一般都是>,除了 #是= 。
    //  左括号和右括号优先级是 = ，# 和 #优先级是=
    // 左括号优先级最低，右括号优先级最高（除了左括号和右括号对比的时候会相等）
    // # 的优先级最低，表示表达式开始和结束。（除了# 和 # 会相等，以及 栈顶# 遇到右括号会报错，栈顶左括号遇到# 也会报错）
    
    int i, j;
    switch(a) {
        case '+': i = 0; break;
        case '-': i = 1; break;
        case '*': i = 2; break;
        case '/': i = 3; break;
        case '(': i = 4; break;
        case ')': i = 5; break;
        case '#': i = 6; break;
    }
    switch(b) {
        case '+': j = 0; break;
        case '-': j = 1; break;
        case '*': j = 2; break;
        case '/': j = 3; break;
        case '(': j = 4; break;
        case ')': j = 5; break;
        case '#': j = 6; break;
    }
    return priority[i][j];
}

// 执行运算
int Operate(int a, int b, char theta) {
    switch(theta) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if(b != 0)
                return a / b;
            else {
                cout << "错误：除数为0" << endl;
                return 0;
            }
        default: return 0;
    }
}
// 正式开始。
char EvaluateExpression(){
    SqStack OPND;
    SqStack_char OPTR; // 定义操作数栈和操作符栈
    InitStack(OPND); // 初始化操作数栈
    InitStack_char(OPTR); // 初始化操作符栈
    char ch,theta; // 定义变量ch，theta，用于存储操作符和运算符
    int a,b; // 定义变量a,b，用于存储操作数
    Push_char(OPTR, '#'); // 将#入栈，表示表达式结束
    cin >> ch; // 输入字符
    while(ch!= '#' || GetTop_char(OPTR) != '#'){ // 当输入字符不为#时或者栈顶操作符不为#时循环。
        if(!In(ch)){ // 如果输入字符不是操作符
            Push(OPND, ch); // 入栈操作数栈
            cin >> ch; // 输入下一个字符
        }
        else{ // 如果输入字符是操作符
            switch(Precede(GetTop_char(OPTR),ch)){ // 判断操作符的优先级
                case '<': // 如果栈顶操作符＜当前操作符 优先级，则入栈
                    Push_char(OPTR, ch); // 入栈操作符栈
                    cin >> ch; // 输入下一个字符
                    break; // 跳出switch语句
                case '=': // 如果栈顶操作符=当前操作符 优先级，则出栈
                    Pop_char(OPTR, ch); // 出栈操作符栈
                    cin >> ch; // 输入下一个字符
                    break; // 跳出switch语句
                case '>': // 如果栈顶操作符>当前操作符 优先级，则出栈操作数栈，进行计算
                    Pop_char(OPTR, theta); // 出栈操作符栈
                    Pop(OPND, b); // 出栈操作数栈，op2为栈顶元素
                    Pop(OPND, a); // 出栈操作数栈，op1为栈顶元素
                    Push(OPND, Operate(a, b, theta)); // 将计算结果入栈操作数栈
                    break; // 跳出switch语句。注意只有这个清空不需要输入下一个字符，因为ch还没被使用。
            } // 结束switch语句
        } // 结束else语句
    } // 结束while语句
    return GetTop(OPND); // 返回操作数栈顶元素 // 返回计算结果
}

// 【算法3.23】舞伴问题
// 参考课本：p81-p82
// 时间复杂度：O(n)，空间复杂度：O(n)

/* ---------- 为 DancePartner函数添加前置函数 ---------- */
typedef struct Person{
    char name[20]; // 人员姓名
    char sex; // 人员性别
}Person; // 定义人员结构体
# define MAXSIZE 100 // 最大人员数
typedef struct SqQueue_Person{
    Person *base; // 人员数组
    int front; // 队头指针
    int rear;  // 队尾指针
}SqQueue_Person; // 定义人员队列结构体

// InitQueue_Person函数
Status InitQueue_Person(SqQueue_Person &Q){ 
    Q.base = new Person[MAXSIZE]; // 分配MAXSIZE个元素的数组空间
    if(!Q.base) return OVERFLOW; // 存储空间分配失败
    Q.front = Q.rear = 0; // 头指针和尾指针置为0，表示队列为空
    return OK;
}

// QueueEmpty_Person函数
Status QueueEmpty_Person(SqQueue_Person Q){ 
    if(Q.front == Q.rear) return True;
    else return False;
}

// Enqueue_Person函数
Status Enqueue_Person(SqQueue_Person &Q, Person e){ 
    if((Q.rear + 1) % MAXSIZE == Q.front) return ERROR; // 队满
    Q.base[Q.rear] = e;
    Q.rear = (Q.rear + 1) % MAXSIZE;
    return OK;
}

// Dequeue_Person函数
Status Dequeue_Person(SqQueue_Person &Q, Person &e){ 
    if(Q.front == Q.rear) return ERROR; // 队空
    e = Q.base[Q.front];
    Q.front = (Q.front + 1) % MAXSIZE;
    return OK;
}

// GetHead_Person函数
Person GetHead_Person(SqQueue_Person Q){ 
    if(Q.front != Q.rear) // 队列非空
        return Q.base[Q.front];
    Person p = {"", ' '}; // 队列为空时返回空Person
    return p;
}

// 正式开始。
void DancePartner(Person dancer[],int num){ 
    SqQueue_Person Mdancer,Fdancer; // 定义男女舞伴队列
    InitQueue_Person(Mdancer); // 初始化男舞伴队列
    InitQueue_Person(Fdancer); // 初始化女舞伴队列
    Person p; // 定义人员结构体变量
    for(int i=0;i<num;i++){ // 循环输入人员信息
        p = dancer[i]; // 读取人员信息
        // 如果人员性别为F，则入女舞伴队列，否则入男舞伴队列
        if(p.sex == 'F') Enqueue_Person(Fdancer, p); // 
        else Enqueue_Person(Mdancer, p); 
    }
    cout << "The dance partners are:" << endl;
    // 循环出队，直到其中一个队列为空
    while(!QueueEmpty_Person(Mdancer) && !QueueEmpty_Person(Fdancer)){
        Dequeue_Person(Mdancer, p); // 出男舞伴队列
        cout << p.name << " ";
        Dequeue_Person(Fdancer, p); // 出女舞伴队列
        cout << p.name << endl;
    }
    if(!QueueEmpty_Person(Fdancer)) { // 如果女舞伴队列不空，则输出等待的女舞伴
        p = GetHead_Person(Fdancer);
        cout << p.name << " is the first woman to get a partner." << endl;
    }
    else if(!QueueEmpty_Person(Mdancer)) { // 如果男舞伴队列不空，则输出等待的男舞伴
        p = GetHead_Person(Mdancer);
        cout << p.name << " is the first man to get a partner." << endl;
    }
}
