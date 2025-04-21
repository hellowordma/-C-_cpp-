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
* 5.7 哈夫曼树及其应用
 *==============================================================*/

/* -------------- 5.7.2 哈夫曼树的构造算法--------------- */


// 1. 定义哈夫曼树结点类型
// 参考视频：第09周04--5.7哈夫曼树及其应用4-哈夫曼树的构造2-哈夫曼树算法实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=104
// 参考课本：p138
typedef struct HTNode{ // 哈夫曼树结点类型
    int weight; // 结点权值。可以是数组。
    int parent, lchild, rchild; // 双亲、左孩子、右孩子下标
} HTNode, *HuffmanTree; // HTNode是哈夫曼树结点类型，HuffmanTree是指向哈夫曼树结点的指针类型。动态分配数组存储哈夫曼树

// Select函数
void Select(HuffmanTree HT, int n, int &s1, int &s2) { // 在HT[1..n]中选择两个其双亲域为0且权值最小的结点并返回它们在HT中的下标s1和s2
    s1 = s2 = 0; // 初始化
    for (int i=1; i<=n; i++) { // 遍历HT[1..n]，让s1和s2分别指向双亲域为0且权值最小的两个结点
        if (HT[i].parent == 0) { // 如果双亲域为0
            if (s1 == 0 || HT[i].weight < HT[s1].weight) { // 如果s1为空或HT[i]的权值小于HT[s1]的权值
                s2 = s1; // 将s1赋值给s2
                s1 = i; // 更新s1
            } else if (s2 == 0 || HT[i].weight < HT[s2].weight) { // 如果s2为空或HT[i]的权值小于HT[s2]的权值
                s2 = i; // 更新s2
            }
        }
    }
}

// 【算法5.10】构造哈夫曼树
// 参考视频：第09周04--5.7哈夫曼树及其应用4-哈夫曼树的构造2-哈夫曼树算法实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=104
// 参考课本：p138-p139
void CreateHuffmanTree(HuffmanTree &HT, int n) {
    if (n<=1) return; // 结点个数小于等于1，不需要构造哈夫曼树
    int m = 2*n - 1; // 哈夫曼树的结点个数
    HT = new HTNode[m+1]; // 动态分配m+1个结点的数组空间，0号单元闲置不用
    for (int i=1; i<=m; i++) { // 初始化哈夫曼树结点, 将1~m号单元中的双亲、左孩子、右孩子下标初始化为0
        HT[i].parent = HT[i].lchild = HT[i].rchild = 0;
    } 
    for (int i=1; i<=n; i++) { // 输入前n个结点的权值
        cin >> HT[i].weight; // 输入权值
    }
    // 初始化工作结束，下面开始创建哈夫曼树
    int s1, s2; // s1和s2分别为最小的两个结点下标
    for (int i=n+1; i<=m; i++) {  // 从第n+1个结点开始创建哈夫曼树
        // 通过n-1次 选择、删除、合并来创建哈夫曼树
        Select(HT, i-1, s1, s2); // 在HT[1..i-1]（前i-1个结点）中选择两个其双亲域为0且权值最小的结点并返回它们在HT中的下标s1和s2
        // 注意是i-1，不能包含当前结点i，因为当前结点i还没有加入到哈夫曼树中
        HT[s1].parent = HT[s2].parent = i; // 得到新的结点i，从森林中删除s1，s2，将s1和s2的双亲指域从0改为i
        HT[i].lchild = s1; HT[i].rchild = s2; // s1和s2分别为i的左孩子和右孩子
        HT[i].weight = HT[s1].weight + HT[s2].weight; // i的权值为s1和s2的权值之和
    }
}
// 注意：HT[i]表示访问数组中第i个HTNode元素，这是一个结构体变量（非指针），因此使用.运算符访问成员weight是合法的。
// 这种写法与普通数组访问一致，例如int arr[10]; arr[0] = 1; 

// 【算法5.11】哈夫曼编码
// 参考视频：第09周06--5.7哈夫曼树及其应用6-哈夫曼编码2-哈夫曼编码的算法实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=106
// 参考课本：p141-p142

typedef char **HuffmanCode; // 哈夫曼编码类型，定义为字符指针的指针类型。之所以是**，是因为 要动态分配二维数组

void CreateHuffmanCode(HuffmanTree HT, HuffmanCode &HC, int n) { // 创建哈夫曼编码
    // HC是哈夫曼编码，n是结点个数
    HC = new char*[n+1]; // 动态分配n+1个字符指针的数组空间，0号单元闲置不用。
    // char*[n+1] 表示指向n+1个字符数组的指针数组
    // 也就是一个二维数组，第一维是n+1，第二维是字符数组（后续的cd）
    char *cd = new char[n]; // 动态分配n个字符的数组空间,临时存放每个字符编码
    cd[n-1] = '\0'; // 编码结束符号
    int start, i, c, f; // start是编码的起始位置，i是循环变量，c是当前结点下标，f是双亲结点下标
    for (i=1; i<=n; i++) { // 逐个字符求哈夫曼编码
        start = n-1; // start开始时指向编码的最后一个字符，即编码结束符号的位置
        c = i; f = HT[i].parent; // c指向当前结点下标，f指向 双亲结点下标
        while(f != 0) { // 从当前结点向上遍历到根结点
            --start; // 编码从后往前存放
            if (HT[f].lchild == c) cd[start] = '0'; // 如果当前结点是父结点的左孩子，则编码为0
            else cd[start] = '1'; // 如果当前结点是父结点的右孩子，则编码为1
            c = f; f = HT[f].parent; // 继续向上回溯
        }
        HC[i] = new char[n - start]; // 为第一个字符编码，动态分配n-start个字符的数组空间
        strcpy(HC[i], &cd[start]); // 将编码复制到HC[i]中。注意：strcpy只复制从cd[start]到字符串结尾的内容。前面空着的不会复制。
    }
    delete[] cd; // 释放临时存放每个字符编码的数组空间
}

/*==============================================================
* 5.8 案例分析与实现
 *==============================================================*/

// 【算法5.12】表达式树的创建
// 参考课本：p143-p144
// 时间复杂度：O(n)；空间复杂度：O(n)
#define MAXSIZE 100 // 最大栈长度

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

typedef struct BiTNode{ // 二叉树结点类型
    int data; // 数据域。int其实是TElemType，只不过这里写规定TElemType会报错。
    struct BiTNode *lchild, *rchild; // 左右孩子指针
} BiTNode, *BiTree; // BiTNode是二叉树结点类型，BiTree是指向二叉树结点的指针类型。
//  非常类似LNode，只不过多了1个指针变成了2个指针而已。
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
Status In(char ch){ // 判断字符是否是操作符
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') return True; // 是操作符
    else return False; // 不是操作符
}

void CreateExpTree(BiTree &T, BiTree l, BiTree r, char ch){ // 创建表达式树
    T->data = ch; // 赋值
    T->lchild = l; // 左孩子指针指向左子树
    T->rchild = r; // 右孩子指针指向右子树
}

// 判断运算符优先级
char Precede(char a, char b) {
    // 运算符优先级表。
    //  注意：char[i][j]不一定=char[j][i]，因为优先级关系是单向的。
    // 而且同一档次的优先级，a（左边）＞b（右边）。
    // 甚至同一个char，比如a=b，也不一定=。一般是>，除了#是=，(是<。
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
    // 其中，'>'表示栈顶操作符优先级高于当前操作符，'<'表示栈顶操作符优先级低于当前操作符，'='表示相等，' '表示错误
    // 若i = j, 一般都是>,除了 #是= 。
    //  左括号和右括号优先级是 = 。
    
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
// 正式开始
// 初始化表达式树（操作符必然有左右孩子，操作数没有左右孩子），暂时只支持加减乘除的运算符，以及个位数的字符串比如'0'、'1'、'2'、'3'、'4'、'5'、'6'、'7'、'8'、'9'
void InitExpTree(){
    SqStack_btree EXPT; // 定义表达式树栈
    SqStack_char OPTR; // 定义操作符栈
    InitStack_btree(EXPT); // 初始化表达式树栈
    InitStack_char(OPTR); // 初始化操作符栈
    Push_char(OPTR, '#'); // 将表达式起始符 # 压入操作符栈
    char ch; // 定义字符变量ch
    cin >> ch; // 输入字符
    while(ch!='#'||GetTop_char(OPTR)!='#'){ // 当输入的字符不是结束符或者操作符栈顶元素不是结束符时
        if(!In(ch)){ // 如果ch不是运算符
            BiTNode *T; // 定义二叉树结点指针T
            CreateExpTree(T,NULL,NULL,ch); // 以ch为根创建一颗只有根节点的二叉树
            Push_btree(EXPT, T); // 将二叉树结点指针T压入表达式树栈
            cin >> ch; // 输入下一个字符
        }
        else{ // 如果ch是运算符
            switch(Precede(GetTop_char(OPTR),ch)){ // 比较操作符的优先级
                case '<': // 如果栈顶操作符优先级小于当前操作符
                    Push_char(OPTR, ch); cin >> ch;// 将当前操作符压入操作符栈，输入下一个字符
                    break;
                case '=': // 如果栈顶操作符优先级等于当前操作符。只有在栈顶操作符='('且ch=')'时才成立
                    Pop_char(OPTR, ch);cin >> ch; // 弹出栈顶操作符，输入下一个字符
                    break;
                case '>': // 如果栈顶操作符优先级大于当前操作符
                    char theta; // 定义字符变量theta
                    BiTNode * a, * b; // 定义二叉树结点指针a和b
                    Pop_char(OPTR, theta); // 弹出栈顶操作符
                    Pop_btree(EXPT, b); Pop_btree(EXPT, a); // 弹出表达式树栈顶元素，分别赋值给a和b
                    BiTNode *T; // 定义二叉树结点指针T
                    CreateExpTree(T, a, b, theta); // 以theta为根创建一颗以a和b为左右子树的二叉树
                    Push_btree(EXPT, T); // 将二叉树结点指针T压入表达式树栈
                    break; // 注意：此时不用再cin>> ch了，因为ch还需要继续跟OPTR的栈顶元素进行比较。
            }
        }
    }
}

// 【算法5.13】表达式树的求值
// 相当于后序遍历
// 参考课本：p145
// 时间复杂度：O(n)；空间复杂度：O(n)

// 执行运算，暂时只考虑加减乘除的运算符。
float GetValue(char op, int lvalue, int rvalue){
    switch(op){ // 根据操作符进行运算
        case '+': return lvalue + rvalue; // 加法
        case '-': return lvalue - rvalue; // 减法
        case '*': return lvalue * rvalue; // 乘法
        case '/': 
            if(rvalue != 0) return lvalue / rvalue; // 除法
            else {
                cout << "错误：除数为0" << endl; // 除数为0，输出错误信息
                return 0;
            }
        default: return 0; // 默认返回0
    }
}

// 正式开始
// 求值表达式树，暂时只支持加减乘除的运算符。
float EvaluateExpTree(BiTree T){
    int lvalue = 0, rvalue = 0; // 定义左值和右值, 初始值为0
    if (T->lchild == NULL && T->rchild == NULL) // 如果是结点是操作数（因为如果没有左右孩子，必然是操作数）
        return T->data-'0'; // 返回结点的值。之所以 要减去'0'，是因为输入的是字符型的数字，比如'1'、'2'、'3'、'4'、'5'、'6'、'7'、'8'、'9'，而不是int类型的数字
        // 而 '1'-'0'=1, 可以转化成int类型的数字。
    else{ // 如果是结点是操作符
        lvalue = EvaluateExpTree(T->lchild); // 递归求左子树的值
        rvalue = EvaluateExpTree(T->rchild); // 递归求右子树的值
        return GetValue(T->data, lvalue, rvalue); // 返回操作符的值
    }
}
