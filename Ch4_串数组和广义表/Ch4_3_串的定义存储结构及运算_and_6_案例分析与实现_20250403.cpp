/***********************************************************
 * 第4章 串、数组和广义表
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


/*==================================
* 4.3 串的定义、存储结构及运算
 *==================================*/


 /* ----------4.3.2.串的存储结构------------------- */

// 1. 串的顺序存储

// 1.1 串的定长顺序存储结构
// 参考视频：第06周03--第4章串、数组和广义表3-4.3串的类型定义、存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=65
// 参考课本：p90 
#define MAXSTRLEN 255 // 最大串长
typedef struct SString{ // 顺序存储结构
    char ch[MAXSTRLEN+1]; // 串的存储空间，之所以+1是因为下标为0的分量闲置不用，设为'\0'。
    int length; // 字符串的长度。记录的是净长度，也就是不包括'\0'的长度。
} SString; // SString是顺序存储结构的类型名。ch是字符数组，length是字符串的长度。

// 1.2 串的堆式顺序存储结构
// 参考视频：第06周03--第4章串、数组和广义表3-4.3串的类型定义、存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=65
// 参考课本：p90
typedef struct HString{ // 堆式顺序存储结构
    char *ch; // 串的存储空间，指向动态分配的内存。若是非空串，则ch指向存储串的首地址；若是空串，则ch指向NULL。
    int length; // 字符串的长度
} HString; // HString是堆式顺序存储结构的类型名。ch是字符指针，length是字符串的长度。H=Heap，表示堆式存储结构。

// 本质上和SqList是一样的，只是数据类型特定而已，即ElemType规定为char。 

// 2. 串的链式存储——块链结构
// 参考视频：第06周03--第4章串、数组和广义表3-4.3串的类型定义、存储结构 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=65
// 参考课本：p91
#define CHUNKSIZE 80 // 块的大小，用户自定义。
typedef struct Chunk{ // 块的定义
    char ch[CHUNKSIZE]; // 字符串的存储空间
    struct Chunk *next; // 指向下一个块的指针
} Chunk; // Chunk是块的类型名。ch是字符数组，next是指向下一个块的指针。
// Chunk其实和LNode是一样的，只是数据类型特定而已，即ElemType规定为char。

typedef struct LString{ // 链式存储结构
    Chunk *head, *tail; // 串的头指针和尾指针。head指向第一个块，tail指向最后一个块。
    int length; // 字符串的长度
} LString; // LString是链式存储结构的类型名。head是指向第一个块的指针，length是字符串的长度。

/* ---------------4.3.3 串的模式匹配算法（用定长顺序存储结构实现）------------------- */
// 【算法4.1】 BF算法
// 参考视频：第06周03--第4章串、数组和广义表3-4.3串的操作 -串的匹配算法1--BF算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=66
// 参考课本：p92
int Index_BF(SString S, SString T, int pos){ // BF算法
    // 返回模式串T在主串S中第pos个字符之后第一次出现的位置，若不存在，则返回0。
    // S是主串，T是模式串，pos是主串的起始位置，1<=pos<=S.length。
    // 注意：pos是从1开始的，而不是从0开始的。
    int i = pos; // 主串的起始位置
    int j = 1; // 模式串的起始位置
    while (i <= S.length && j <= T.length){ // 主串和模式串都没有匹配完
        if (S.ch[i] == T.ch[j]){ // 如果主串和模式串的字符相等
            i++,j++; // 主串指针后移, 模式串指针后移
        }
        else{ // 如果主串和模式串的字符不相等
            i = i - j + 2; // 主串指针回溯到下一个字符的位置.这里的i = i - j + 2是因为此时已经往后移动了j-1个字符，变成i-j+2
            // 而 i-j+1是当前主串指针的位置, 要继续往后移动一个字符，所以要再+1，变成i-j+2
            j = 1; // 模式串指针回溯到第一个字符的位置
        }
    }
    if (j > T.length) return i - T.length; // 匹配成功，返回匹配位置。注意不需要-1，因为i是从1而不是0开始的。
    else return 0; // 匹配失败，返回0
}

// 【算法4.2】 KMP算法
// !!! 注意：因为教材用C语言，字符串第0个元素置空，S和T的长度都是n+1，下标从1开始，所以没法像python一样让next[0] = -1,必须next[1] = 0, 后续的i和j初始化也要改的。
// 参考视频：第06周03--第4章串、数组和广义表3-4.3串的操作 -串的匹配算法2--KMP算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=67
// 参考课本：p95-p98
void get_next(SString T, int next[]){ // 求模式串T的next数组
    // next[i]表示模式串T的前缀和后缀相等的最大长度，本质上是告诉我们，
    // 在kmp算法中，当模式串的j指针不匹配时，应该将模式串的j指针回溯到哪个位置（也就是next[j]）。
    // next数组的长度为模式串T的长度+1
    int i = 1, j = 0; // 模式串指针和前缀指针。如果是python，那么i = 0, j = -1
    next[1] = 0;  // 如果是python，那么next[0] = -1
    while (i < T.length){ // 模式串没有匹配完。  如果是python，那么while i < T.length-1
        if (j == 0 || T.ch[i] == T.ch[j]){ // 如果前缀指针为0或者模式串和前缀的字符相等。 
            // 如果是python，那么j == -1
            i++,j++,next[i] = j; // 模式串指针后移，前缀指针后移，更新next数组
        }
        else j = next[j]; // 如果前缀指针不为0且模式串和前缀的字符不相等,前缀指针回溯到上一个位置
    }
}

void get_nextval(SString T, int nextval[]){ // 求模式串T的next函数修正值
    // 在get_next函数的基础上，如果模式串有连续相同的字符，则nextval[i] = nextval[j]，否则nextval[i] = j
    int i = 1, j = 0; // 模式串指针和前缀指针。如果是python，那么i = 0, j = -1
    nextval[1] = 0; // 如果是python，那么nextval[0] = -1
    while (i < T.length){ // 模式串没有匹配完。  如果是python，那么while i < T.length-1
        if (j == 0 || T.ch[i] == T.ch[j]){ // 如果前缀指针为0或者模式串和前缀的字符相等。 
            // 如果是python，那么j == -1
            i++,j++; // 模式串指针后移，前缀指针后移
            if (T.ch[i] != T.ch[j]){ // 如果++后的模式串和前缀的字符不相等
                nextval[i] = j; // 赋值为j
            }
            else{ // 如果++后的模式串和前缀的字符仍然相等
                nextval[i] = nextval[j]; // 赋值为nextval[j]
            } // !!!! 这里就是与get_next函数的区别。其他地方是一样的。
        }
        else j = nextval[j]; // 如果前缀指针不为0且模式串和前缀的字符不相等,前缀指针回溯到上一个位置
    }
}

int Index_KMP(SString S, SString T, int pos){ // KMP算法
    // 返回模式串T在主串S中第pos个字符之后第一次出现的位置，若不存在，则返回0。
    // S是主串，T是模式串，pos是主串的起始位置，1<=pos<=S.length，即[1,n]的区间。
    int i = pos, j = 1; // 主串指针和模式串指针。如果是python，那么j = 0
    int next[T.length + 1]; // next数组的长度为模式串T的长度+1，因为next[0]是闲置不用的。
    get_next(T, next); // 求模式串T的next数组
    //  get_nextval(T, next); // 求模式串T的next函数修正值
    // !!!注意：get_nextval(T, next)和get_next(T, next)是互斥的，只能选择一个, 前者是对后者的优化。
    while (i <= S.length && j <= T.length){ // 主串和模式串都没有匹配完。 如果是python，那么while i < S.length and j < T.length
        if (j == 0 || S.ch[i] == T.ch[j]){ // 如果模式串指针为0或者主串和模式串的字符相等。如果是python，那么j == -1
            i++,j++; // 主串指针后移，模式串指针后移
        }
        else j = next[j]; // 如果模式串指针不为0且主串和模式串的字符不相等,模式串指针回溯到上一个位置
    }
    if (j > T.length) return i - T.length; // 匹配成功，返回匹配位置。注意不需要-1，因为i是从1而不是0开始的。
    // 如果是python，那么if j == T.length return i - T.length
    else return 0; // 匹配失败，返回0
}

/*==================================
* 4.6 案例分析与实现
 *==================================*/

//  【算法4.5】 病毒感染检测
// 参考视频：第06周03--第4章串、数组和广义表3-4.6案例分析与实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=75

// 参考课本：p106-p107

void Virus_detection(){
    // 利用BF算法实现
    ifstream inFile("virus.txt"); // 打开病毒DNA和人体DNA的文件
    ofstream outFile("result.txt"); // 打开存储结果的文件
    int num; // 待读取检测文件的个数
    int i,j,m,flag; // i,j,m,flag是循环变量
    SString Virus,Person,temp; // 定义病毒DNA的顺序存储结构以及人的DNA的顺序存储结构
    char* Vir; // 定义一个字符指针用于暂存病毒DNA
    inFile >> num; // 读取检测文件的个数
    while(num--){
        inFile >> Virus.ch+1; // 读取病毒DNA, 字符串的下标从1开始
        inFile >> Person.ch+1; // 读取人的DNA
        Vir = Virus.ch; // 将病毒DNA暂存到Vir中，以备后续输出
        flag = 0; // 初始化flag为0,表示没有感染.匹配后flag=1
        Virus.length = strlen(Virus.ch+1); // 获取病毒DNA的长度,+1是因为下标从1开始
        m = Virus.length; // 获取病毒DNA的长度
        for(i=m+1;i<=2*m;i++){ // 将病毒字符串长度扩大为之前的2倍也就是m*2
            Virus.ch[i] = Virus.ch[i-m]; 
        } // 将Virus.ch[1:m]复制到Virus.ch[m+1:2*m]
        Virus.length = m * 2;
        Virus.ch[2*m+1] = '\0'; // 字符串的结束标志
        for (i =0;i<m;i++){ // 循环遍历病毒DNA(已经扩大为2倍)
            for(j = 1;j<=m;j++){ // 循环遍历人的DNA
                temp.ch[j] = Virus.ch[i+j]; // 将病毒DNA的第i+j个字符赋值给temp
            } // 将Virus.ch[1+i:1+i+m]赋值给temp.ch[1:m],作为匹配的模式串
            temp.ch[m+1] = '\0'; // 字符串的结束标志
            flag = Index_BF(Person,temp,1); // 调用BF算法进行匹配，flag为返回的位置，如果是0则说明不匹配，>=1则说明匹配成功
            if (flag) break; // 如果匹配成功，则跳出循环
        } // 结束内层循环    
        // 根据flag的值判断是否匹配成功，非0表示匹配成功，0表示匹配失败
        if (flag) outFile << Vir+1 << "  "<< Person.ch+1 << "  " << "Yes" << endl; // 如果匹配成功，则输出感染信息
        else outFile << Vir+1 << "  "<< Person.ch+1 << "  " << "No" << endl; // 如果匹配失败，则输出未感染信息
         // 之所以是Vir+1是因为Virus.ch[0]是闲置不用的，设为'\0'。
    }
}
