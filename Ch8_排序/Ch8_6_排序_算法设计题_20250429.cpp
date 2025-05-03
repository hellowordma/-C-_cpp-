/***********************************************************
 * 第8章 排序
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏 p271
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p166

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
* 算法设计题（见习题集)
 *==============================================================*/


 /* ---------- Start of 定义 ---------- */
 //  主要参考：D:\MacBook迁移\2025互联网准备\C++\数据结构_王卓_20250320\Ch6_图\Ch7_查找  目录下的
//   - Ch7_2_线性表的查找_20250421.cpp 
//   - Ch7_3_树表的查找_20250421.cpp
//   - Ch7_4_散列表的查找_20250423.cpp
//   - 以及  Ch2_线性表/Ch2_5_线性表的l链表实现_20250321.cpp

typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
 typedef struct ElemType{
    KeyType key; // 关键字类型
    InfoType otherinofo; // 其他数据域
} ElemType; // ElemType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。

// 开放地址法，散列表的存储表示
#define m 20 // 散列表的表长
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型

typedef struct LNode {
    KeyType data;
    InfoType otherinfo;
    struct LNode *next;
} LNode, *LinkList; // 链表结点类型

typedef struct DuLNode{
    ElemType data; // 数据域
    struct DuLNode *prior, *next; // 前驱和后继指针
}DuLNode, *DuLinkList; // 双向链表结点类型

 /* ---------- End of 定义 ---------- */

//  1. 试以单链表为存储结构，实现简单选择排序算法。
// 思路：交换data域的值即可，不需要交换指针。
// 参考习题:p179
void SelectSort(LinkList &L){
    // 单链表的简单选择排序
    LNode *p = L->next; // p指向首元结点
    while(p){ // 遍历链表
        LNode *q = p->next; // q指向p的下一个结点
        LNode *tmp = p; // tmp指向p，tmp用于保存最小值的结点
        while(q){ // 遍历p的后续结点
            if(tmp->data>q->data) tmp = q; // 找到最小值的结点
            q = q->next; // q指向下一个结点
        }
        if(tmp != p){ // 如果最小值的结点不是p，则交换p和tmp的data域
            KeyType tempData = p->data;
            p->data = tmp->data;
            tmp->data = tempData;
        }
        p = p->next; // p指向下一个结点
    }
}


// 2. 有n个记录存储在带头结点的双向链表中，利用双向冒泡排序法对其按升序进行排序，请写出这种排序的算法。(注:双向冒泡排序即相邻两趟排序向相反方向冒泡)
// 思路：
// 参考习题:p179-p180
void DuplexSort(DuLinkList &L){
    // 双向链表的双向冒泡排序
    int exchange = 1; // 交换标志
    DuLNode *head = L, *tail = NULL; // head指向头结点（注意不是首元结点），tail指向尾结点。类似于顺序表的low和high（分别赋值为-1和n，其中有效下标是0~n-1）
    while(exchange){
        exchange = 0; // 初始化交换标志为0,假定没有交换。
        DuLNode *p = head->next; // p指向当前结点。
        while(p->next!=tail){ // 向下冒泡，直到尾结点。每一趟使得最大元素沉底。
            if(p->data.key>p->next->data.key){ // 如果当前结点的关键字大于下一个结点的关键字，则交换
                DuLNode *temp = p->next; // temp指向下一个结点
                exchange = 1; // 设置交换标志为1，表示发生了交换
                // 接下来进行双向链表的交换操作
                p->next = temp->next; temp->next->prior = p; // 先将p与p->next->next连接起来
                p->prior->next = temp; temp->prior = p->prior; // 再将p->prior与p->next连接起来
                temp->next = p; p->prior = temp; // 最后将p->next与p连接起来。实现了p与p->next的交换
            }
            else p = p->next; // 如果没有交换，则p指向下一个结点。（如果交换了，则p相当于调整到了下一个结点）
        }
        tail = p; // 更新尾结点为当前结点, 准备向上冒泡
        p = tail->prior; // p指向当前结点的前驱结点
        while(p->prior!=head){ // 向上冒泡，直到头结点。每一趟使得最小元素浮顶。
            if(p->data.key<p->prior->data.key){ // 如果当前结点的关键字小于前一个结点的关键字，则交换
                DuLNode *temp = p->prior; // temp指向前一个结点
                exchange = 1; // 设置交换标志为1，表示发生了交换
                // 接下来进行双向链表的交换操作 （与向下冒泡的交换操作的3行代码相比，只需要互换prior和next即可）
                p->prior = temp->prior; temp->prior->next = p; // 先将p与p->prior->prior连接起来
                p->next->prior = temp; temp->next = p->next; // 再将p->next与p->prior连接起来
                p->next = temp; temp->prior = p;  // 最后将p与p->prior连接起来。实现了p与p->prior的交换
            }
            else p = p->prior; // 如果没有交换，则p指向下一个结点。（如果交换了，则p相当于调整到了下一个结点）
        }
        head = p; // 更新头结点为当前结点，准备向下冒泡。注意下面没有p= head->next，因为下一趟while会在一开始实现这一点。
    }
}


// 3.设有顺序放置的n个桶,每个桶中装有一粒砾石,每粒砾石的颜色是红、白、蓝之一。要求重新安排这些砾石，使得所有红色砾石在前，
//  所有白色砾石居中，所有蓝色石在后。重新安排时对每粒砾石的颜色只能看一次，并且只允许交换操作来调整砾石的位置
// 思路：
//      设4个指针i,j和 k1、k2，分别用来指向第一块白色石头、红色石头、最后一块白色石头和蓝色石头。首先找到从左向右第一块不为红的石头和从右向左第一块不为蓝的石头。
//      如果从左向在一块不为红的石头是蓝色的，则把它与从右向左第一块不为蓝的石头交换。如果从右向左第一不为蓝的石头是红色的，则把它与从左向右第一块不为红的石头交换。
//      如果这两块石头都是白的，则把这两块石头之后出现的第一块红石头与第一块白石头交换或之后第一块蓝石头与最后一个石头交换。
//      重复上述步骤、直至所有石头都遍历完毕。
// 参考习题:p180-p182
void QkSort(char r[],int n){
    // r是存放砾石颜色的数组，n是砾石的数量，元素有红白蓝三种颜色。要求重新安排这些砾石，使得所有红色砾石在前，白色砾石居中，蓝色砾石在后。
    int i = 1, j = 1, k1 = n ,k2 =n; // i指向第一块白色石头，j指向红色石头，k1指向蓝石头，k2指向最后一块白色石头
    while(j<k1){ // 遍历所有石头。以j（指向红色石头）为基准，k1（指向蓝色石头）为结束条件
        while(r[k1]=='B'){ // 从右向左找到第一块不为蓝的石头
            --k1;--k2;
        }
        while(r[i]=='R'){ // 从左向右找到第一块不为红的石头
            ++i;++j;
        }
        if(r[i]=='B'){ // 如果从左向右第一块不为红的石头r[i]是蓝色的，则交换它与从右向左第一块不为蓝的石头r[k1]
            char temp = r[i]; // 交换r[i]和r[k2]
            r[i] = r[k2];
            r[k2] = temp; 
            while(r[i]=='R'){ // 如果交换后r[i]是红色的，则继续向右移动i和j
                ++i;++j;
            }
            while(r[k1]=='B'){ // 如果交换后r[k1]是蓝色的，则继续向左移动k1和k2
                --k1;--k2;
            }
        }
        else if(r[k1]=='R'){ // 否则（此时r[i]=='W'），如果从右向左第一块不为蓝的石头是红色的，则交换它与从左向右第一块不为红的石头
            char temp = r[i]; // 交换r[i]和r[k2]
            r[i] = r[k2];
            r[k2] = temp;
            // 下面2个while与上面2个while相同。
            while(r[i]=='R'){ // 如果r[i]是红色的，则继续向右移动i和j
                ++i;++j;
            }
            while(r[k1]=='B'){ // 如果r[k1]是蓝色的，则继续向左移动k1和k2
                --k1;--k2;
            }
        }
        else{ // 两块石头都是白色。此时i和j 可能要分离，k1和k2也可能要分离。
            while(r[j]=='W') ++j; // 如果r[j]是白色的，则继续向右移动j,直到找到第一块不为白的石头
            if(j>=k1) break; // 如果j大于等于k1，则退出循环。此时左右汇合了，说明所有的石头都已经遍历完毕。
            if(r[j]=='R'){ // 如果r[j]是红色的，则交换它与从左到右第一块白色石头
                char temp = r[i]; // 交换r[i]和r[j]
                r[i] = r[j];
                r[j] = temp;
                ++i; // 交换后，此时r[i]为红色，所以i向右移动1位
            }
            else{ // 如果r[j]是蓝色的，则交换它与从右到左的第一块白色石头
                char temp = r[k2]; // 交换r[k2]和r[j]
                r[k2] = r[j];
                r[j] = temp;
                --k2; // 交换后，此时r[k2]为蓝色，所以k2向左移动1位。
            }
        }
    }
}
// 我的方法：荷兰国旗问题，三指针法。
void QkSort_custom(char r[],int n){
    int low=0, mid =0, high = n-1;  // 三指针。红色区右边界（[0, low-1]是红色）；白色区右边界（[low, mid-1]是白色），蓝色区左边界（[high+1, n-1]是蓝色）。
    while(mid<=high){ // 注意是mid<=high，而不是mid<high。因为还要对最后一个元素进行判断。
        if (r[mid] =='R'){ // 如果r[mid]是红色
            char temp = r[mid]; // 交换r[mid]和r[low]
            r[mid] = r[low];
            r[low] = temp;
            ++low; ++mid; // 交换后，low和mid都要分别向后移动1位
        }
        else if (r[mid] =='B'){ // 如果r[mid]是蓝色
            char temp = r[mid]; // 交换r[mid]和r[high]
            r[mid] = r[high];
            r[high] = temp;
            --high; // 交换后，high向左移动1位
        }
        else { // 如果r[mid]是白色
            ++mid;
        }
    }
}

// 4.编写算法，对n个关键字取整数值的记录序列进行整理，以使所有关键字为负值的记录在关键字为非负值的记录之前，要求:
//  (1) 采用顺序存储结构，至多使用一个记录的辅助存储空间；
//  (2) 算法时间复杂度为O(n)；
// 思路：双指针法，交换。
// 参考习题:p182
void Process(int a[], int n){
    // 将a[0]~a[n-1]中所有负数的记录放在前面，非负数的记录放在后面
    int low = 0, high = n - 1; // low指向第一个非负数，high指向最后一个负数
    while(low<high){
        while(low<high && a[low]<0) ++low; // 从前往后，找到第一个非负数
        while(low<high && a[high]>=0) --high; // 从后往前，找到最后一个负数
        if(low<high){ // 如果low<high，说明找到了非负数和负数，交换它们的位置
            int temp = a[low];
            a[low] = a[high];
            a[high] = temp;
            ++low; --high; // 交换后，low和high都要分别向后和向前移动1位
        }
    }
}

// 我的方法：不用数组，用顺序表SqList。
#define MAXSIZE 20 // 顺序表的最大长度
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
typedef struct RedType{ // 记录类型
    KeyType key; // 关键字类型
    InfoType otherinfo; // 其他数据域
} RedType; // RedType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。
typedef struct SqList{ // 顺序表类型
    RedType r[MAXSIZE+1]; // r[0]闲置或者做哨兵。r[1]~r[length]是线性表的元素
    int length; // 线性表的长度
} SqList; // SqList是顺序表，包含关键字和其他数据域。RedType是线性表的元素类型，包含关键字和其他数据域。
void Porcess_sqlist(SqList &L){
    int n = L.length;
    int low =1,high=n;
    while(low<high){
        while(L.r[low].key<0 &&low<high) ++low;
        while(L.r[high].key>=0 && low<high) --high;
        KeyType temp = L.r[low].key;
        L.r[low].key = L.r[high].key;
        L.r[high].key = temp;
        ++low; --high; // 交换后，low和high都要分别向后和向前移动1位
    }
}


// 5.借助于快速排序的算法思想，在一组无序的记录中查找给定关键字值等于 key 的记录。设此组记录存放于数组r[1...n]中。
//     若查找成功，则输出该记录在r数组中的位置及其值，否则显示“not found”信息。请简要说明算法思想并编写算法。
// 思路：双指针
// 参考习题:p183
int Search(ElemType r[],int low, int high, KeyType k){
    // 在r[low]~r[high]中查找关键字为k的记录，返回其位置

    while(low<high){
        while(low<high && r[high].key>k) --high;
        if(r[high].key==k) return high; // 找到关键字为k的记录，返回其位置
        while(low<high && r[low].key<k) ++low;
        if(r[low].key==k) return low; // 找到关键字为k的记录，返回其位置
    }
    cout<<"not found"<<endl; // 没有找到关键字为k的记录，输出“not found”信息
    return -1; // 返回-1表示没有找到
}


// 6. 有一种简单的排序算法，叫做计数排序。这种排序算法对一个待排序的表进行排序,并将排序结果存放到另一个新的表中。必须注意的是，
//      表中所有待排序的关键字互不相同，计数排算法针对表中的每个记录，扫描待排序的表一趟，统计表中有多少个记录的关键字比该记录的关键字小。
//      假设针对某一个记录，统计出的计数值为c，那么，这个记录在新的有序表中的合适的存放位置即为c。
// （1）给出适用于计数排序的顺序表定义;
// （2）编写实现计数排序的算法;
// （3）对于有 n个记录的表，关键字比较次数是多少?
// （4）与简单选择排序相比较，这种方法是否更好?为什么?
// 思路：
// 参考习题:p

typedef struct RecType{
    KeyType key;
    InfoType otherinfo; // 其他数据域
}RecType; // 适用于计数排序的顺序表定义。RecType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。

void CountSort(RecType a[], RecType b[],int n){
    // 基数排序算法，将包括n个数据的数组a中的记录排序后放入数组b中
    int i,j,c;
    for(i=0; i<n; ++i){ // 针对数组中每个关键字
        for(j=0,c=0;j<n;++j){ // 统计关键字比当前关键字小的元素个数
            if(a[j].key<a[i].key) ++c;
        }
        b[c] = a[i]; // 将当前关键字放入新数组
    }
}
// 关键字比较次数是n²；简单选择排序由于本算法，因为简单选择排序的比较次数是 n(n-1)/2，且空间复杂度为O(1)。本算法空间复杂度为O(n).