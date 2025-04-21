/***********************************************************
 * 第7章 查找
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
* 7.2 线性表的查找
 *==============================================================*/


 /* -------------- 7.2.1 顺序查找--------------- */
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型
 typedef struct ElemType{
    KeyType key; // 关键字类型
    InfoType otherinofo; // 其他数据域
} ElemType; // ElemType是线性表的元素类型，包含关键字和其他数据域。KeyType是关键字类型。

typedef struct SSTable{
    ElemType *R;
    int length; // 线性表的长度
}SSTable;

// 【算法7.1】顺序查找
// 参考视频：第12周02--7.2线性表的查找1-7.2.1顺序查找1--顺序查找算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=138
// 参考课本：p192-p193
// 时间复杂度：O(n)
int Search_Seq(SSTable ST, KeyType key) { // 顺序查找
    for (int i = ST.length;i>=1;i--) { // 从后往前查找
        if (ST.R[i].key == key) return i; // 找到关键字，返回下标
    }
    return 0; // 没有找到，返回0
} // ！！！注意：返回0表示没有找到，返回1表示找到。

// 【算法7.2】设置监视哨的顺序查找
// 参考视频：第12周02--7.2线性表的查找1-7.2.1顺序查找1--顺序查找算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=138
// 参考课本：p193
// 时间复杂度：O(n)

// 监视哨的作用是减少比较次数（i与1的比较），提高查找效率。
int Search_Seq_Sentinel(SSTable ST, KeyType key) { // 设置监视哨的顺序查找
    ST.R[0].key = key; // 设置监视哨，关键字为key
    int i;
    for(i = ST.length; ST.R[i].key != key; --i); // 从后往前查找，直到找到关键字为key的元素
    // 注意for循环是个空循环，最后是个;符号。
    return i; // 返回下标。如果没找到，则自然返回的是0（监视哨的下标）。如果找到，则返回的是i（监视哨的下标）。
} 

// 【算法7.3】折半查找
// 参考视频：第12周04--7.2线性表的查找3-7.2.2折半查找1--折半查找算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=140
// 参考课本：p194
// 时间复杂度：O(log2n)
int Search_Bin(SSTable ST, KeyType key) { // 折半查找
    int low = 1, high = ST.length; // low和high分别是查找范围的下限和上限
    while (low <= high) { // 当low小于等于high时，继续查找
        int mid = low + ( high - low ) / 2; // 计算中间位置，防止溢出
        if (ST.R[mid].key == key) return mid; // 找到关键字，返回下标
        else if (ST.R[mid].key > key) high = mid - 1; // 如果中间元素大于关键字，则在左半部分查找
        else low = mid + 1; // 如果中间元素小于关键字，则在右半部分查找
    }
    return 0; // 没有找到，返回0
}

// 以上课本内容是闭区间方法。或者可以用开区间方法：low =-1，high = n; while(low + 1) < high; low或者high=mid;
int Search_Bin_open(SSTable ST, KeyType key) { // 折半查找
    int low = -1, high = ST.length; // low和high分别是查找范围的下限和上限
    while (low < high) { // 当low小于high时，继续查找
        int mid = low + ( high - low ) / 2; // 计算中间位置，防止溢出
        if (ST.R[mid].key == key) return mid; // 找到关键字，返回下标
        else if (ST.R[mid].key > key) high = mid; // 如果中间元素大于关键字，则在左半部分查找
        else low = mid; // 如果中间元素小于关键字，则在右半部分查找
    }
    return 0; // 没有找到，返回0
}

// 递归实现折半查找（额外多了2个参数 low和high）
int Search_Bin_Recursion(SSTable ST, KeyType key, int low, int high) { // 折半查找
    if (low > high) return 0; // 没有找到，返回0
    int mid = low + ( high - low ) / 2; // 计算中间位置，防止溢出
    if (ST.R[mid].key == key) return mid; // 找到关键字，返回下标
    else if (ST.R[mid].key > key) return Search_Bin_Recursion(ST, key, low, mid - 1); // 在左半部分查找
    else return Search_Bin_Recursion(ST, key, mid + 1, high); // 在右半部分查找
}


