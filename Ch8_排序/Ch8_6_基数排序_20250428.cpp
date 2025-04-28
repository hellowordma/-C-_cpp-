/***********************************************************
 * 第8章 排序
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

#define MAXNUM_KEY 8 // 关键字项数的最大值
#define RADIX 10 // 关键字基数。例如，十进制数的基数为10
#define MAX_SPACE 10000 // 最大空间容量
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型

typedef struct SLCell{
    KeyType keys[MAXNUM_KEY]; // 关键字项
    InfoType otherinfo; // 其他数据域
    int next; // 指向下一个结点的指针
}SLCell; // 静态链表结点类型

typedef struct SLList{
    SLCell r[MAX_SPACE]; // 静态链表的可利用空间，r[0]是头结点
    int keynum; // 记录的当前关键字个数，相当于n
    int recnum; // 静态链表的当前长度（结点个数）
}SLList; // 静态链表类型

typedef int ArrType[RADIX]; // 数组类型


/*==============================================================
* 8.6 基数排序（Radix Sort）
- 基本思路：属于典型的分配类排序，不需要比较元素之间的大小关系，而是将元素分配到不同的桶中，然后再将桶中的元素进行排序。
          通过对待排序的元素进行若干趟分配和收集，完成排序。
          有点类似散列表查找的意思（也是不比较）
- 例子：
    - 扑克牌可以分成4个花色，每个花色有13张牌，共52张牌。可以按照花色进行排序，也可以按照点数进行排序。
    - 5位10进制数的排序：将每个数分成5位，每位的取值范围是0~9。可以按照个位、十位、百位、千位、万位这5个关键字进行排序。
*==============================================================*/

//【算法8.12】基数排序（Radix Sort）（用静态链表实现）
// 参考视频：第14周13--第8章排13--8.6基数排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=172
// 参考课本：p256-257
// 思路：
// 时间复杂度：O(d(n+rd)) 。d是关键字的位数，n是记录的个数，rd是关键字的取值范围（基数）。每1趟分配的时间复杂度是O(n)，每1趟收集的时间复杂度是O(rd)，共d趟。
// 空间复杂度：O(n+rd)。所需辅助空间为2rd个队列指针,另外由于需用链表做存储结构,则相对于其他以顺序结构为基础的排序算法,还增加了n个结点指针的空间。
// 性能对于顺序、无序、逆序的区别：没有区别。
// 排序稳定性：稳定。
// 适用性：适用于顺序表和链表。


int ord(int x) {
    return x; // 简单映射，如果需要复杂处理，比如 x%10，可以改这里
}

void Distribute(SLCell r[], int i, ArrType &f, ArrType &e){
    // 静态链表L的r域中记录已按（key[0],key[1],...,key[i-1]）有序
    // 本算法按照第i位关键字key[i]的值建立RADIX个子表，使得同一个子表中记录的关键字key[i]相同
    // f[0,...RADIX-1]和e[0,...RADIX-1]分别纸箱各子表中第一个和最后一个记录
    int j,p;
    for(j=0; j<RADIX; j++) f[j]=0; // 初始化，让各个子表为空
    for(p=r[0].next; p; p=r[p].next){ // 遍历静态链表L
        j = ord(r[p].keys[i]); // ord函数将记录中第i个关键字映射到0~RADIX-1
        if(!f[j]) f[j] = p; // 如果子表j为空，则将当前记录p作为子表j的第一个记录
        else r[e[j]].next = p; // 否则，将当前记录p链接到子表j的最后一个记录后面
        e[j] = p; // 更新子表j的最后一个记录
    }
}

void Collect(SLCell r[], ArrType &f, ArrType &e){
    // 本算法按照keys[i]从小到大地将f[0]~f[RADIX-1]所指的RADIX个子表，依次链接成一个静态链表
    // e[0]~e[RADIX-1]是各个子表的尾指针
    int j,t;
    for(j=0; !f[j]; j= succ(j)); // 找到第一个非空子表, succ是求后继函数
    r[0].next = f[j]; t = e[j]; // r[0].next指向第一个非空子表中的第一个结点
    while(j<RADIX){ // 遍历所有子表
        for(j=succ(j); j<RADIX-1&&!f[j]; j=succ(j)); // 找到下一个非空子表
        if(f[j]) { // 链接2个非空子表
            r[t].next = f[j];  // 将当前子表的第一个结点链接到前一个子表的最后一个结点后面
            t = e[j]; // 更新前一个子表的最后一个结点
        }
    }
    r[t].next = 0; // t指向最后一个非空子表的最后一个结点，将其next域置为0，表示链表结束
}

void RadixSort(SLList &L){
    // L是采用静态链表存储的顺序表
    // 对L做基数排序，使得L成为按关键字自小到大的有序静态链表，L.r[0]是头结点
    int i;
    ArrType f,e; 
    for(i=0; i<L.recnum;++i) L.r[i].next=i+1;// 对每个关键字进行排序
    L.r[L.recnum].next = 0; // 尾结点next域置为0
    for(i=0; i<L.keynum; ++i){ // 按照低位优先依次对各个关键字进行分配和收集
        // 第i趟分配和收集
        Distribute(L.r,i,f,e); // 将L.r[0].next所指的静态链表按照第i个关键字进行分配
        Collect(L.r,f,e); // 将分配好的静态链表进行收集
    }
}

