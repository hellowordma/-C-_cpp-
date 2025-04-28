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


/*==============================================================
* 8.3 交换排序
- 基本思想：通过交换相邻元素的方式进行排序。
 *==============================================================*/

/* -------------- 8.3.1 冒泡排序（Bubble Sort）--------------- */

// 【算法8.4】冒泡排序（Bubble Sort）
// 参考视频：第14周05--第8章排序5--8.3交换排序1--冒泡排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=163
// 参考课本：p241-p243
// 时间复杂度：O(n^2)，空间复杂度：O(1)。平均性能比 直接插入排序还差。
// 基本思路：通过交换相邻元素的方式进行排序。
// 排序稳定性：稳定。
// 性能对于顺序、无序、逆序的区别：顺序最好，O(n)；无序次之，O(n^2)；逆序最差，O(n^2)。
// 存储结构：可以用顺序存储结构，也可以用链式存储结构。

// 辅助函数swap：交换两个元素的值
void swap(RedType &a, RedType &b){
    // 交换两个元素的值
    RedType temp = a; // 用一个临时变量保存a的值
    a = b; // 将b的值赋给a
    b = temp; // 将temp的值赋给b
}

// 正式开始
void BubbleSort(SqList &L){
    // 对顺序表L进行冒泡排序
    int j; // j是循环变量
    int flag=1;// flag是标志变量，表示是否发生交换, 1表示发生交换，0表示没有发生交换，初始化为1。
    int m = L.length-1; 
    while(flag&& m>0){ // 如果发生交换，继续循环；如果没有发生交换，说明已经排好序了，退出循环。
        flag = 0; // 初始化标志变量为0
        for(j=1; j<=m; ++j){ // 从第1个元素开始，依次比较相邻的两个元素
            if(L.r[j].key > L.r[j+1].key){ // 如果前一个元素大于后一个元素，则交换位置
                swap(L.r[j], L.r[j+1]); // 交换相邻的两个元素
                flag = 1; // 发生交换，标志变量设为1
            }
        }
        --m; // 每次循环结束后，最大的元素已经排好序了，所以下次循环只需要比较[1,m-1]的元素即可。
    }
}

/* -------------- 8.3.2 快速排序（Quick Sort）--------------- */

// 【算法8.5】快速排序（Quick Sort）
// 参考视频：第14周06--第8章排序6--8.3交换排序2--快速排序1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=164
// 参考课本：p243-p245
// 时间复杂度：
//  - 最好：O(nlogn)（每趟排序都能将序列分成两个近似相等的子序列）
//  - 最坏：O(n^2)（有序或者近似有序，逆序或者顺序都是），退化到简单选择排序的水平。
//  - 平均：O(nlogn)（无序）。
// 空间复杂度： 最大递归调用次数与递归树的深度一致。
//  - 最好： O(logn)（每趟排序都能将序列分成两个近似相等的子序列）
//  - 最坏： O(n)（有序或者近似有序，逆序或者顺序都是，此时递归树变成单支树，深度为n）。
// 基本思路：是对冒泡排序的改进，采用分治法，将待排序序列分成两个子序列，分别进行排序，直到子序列的长度为1。用到双指针法。
// 排序稳定性：不稳定。
// 性能对于顺序、无序、逆序的区别：有序（顺序或者逆序）时性能最差，O(n^2)；无序时性能最好，O(nlogn)。
// 存储结构：适用于顺序存储结构，很难用于链式存储结构，因为涉及到定位表的上下界。
// 适用情况：初始记录无序，n较大的情况。
// ！！！注意：在平均情况下，快速排序是所有内部排序算法中速度最快的。

int Partition(SqList &L, int low, int high){
    // 对顺序表L的子表L.r[low]~L.r[high]进行1趟排序，并返回枢轴的下标
    // 选择第一个元素作为基准元素
    L.r[0] = L.r[low]; // 将第一个元素保存到哨兵位置
    KeyType pivotkey = L.r[low].key; // 基准元素的关键字
    while(low < high){ // 当low小于high时，继续循环。用low和high双指针反复交换元素，即从表的两端交替地向中间扫描。
        while(low < high && L.r[high].key >= pivotkey) --high; // 从右向左查找第一个小于基准元素的元素
        L.r[low] = L.r[high]; // 将该元素赋值给low位置
        while(low < high && L.r[low].key <= pivotkey) ++low; // 从左向右查找第一个大于基准元素的元素
        L.r[high] = L.r[low]; // 将该元素赋值给high位置
    } // 此处会导致排序不稳定。
    L.r[low] = L.r[0]; // 将基准元素赋值给low位置。此时low ==high，表示基准元素已经找到位置了。
    return low; // 返回基准元素的下标
}
void QSort(SqList &L, int low, int high){
    // 调用前置初始值：low=1, high=L.length
    // 对顺序表L的子表L.r[low]~L.r[high]进行快速排序,用到Partition函数
    if(low<high){
        int pivotloc = Partition(L, low, high); // 将L.r[low]~L.r[high]划分成两个子表，pivotloc是枢轴的位置。
        QSort(L, low, pivotloc-1); // 对基准元素左边的子序列进行快速排序,[low,pivotloc-1]
        QSort(L, pivotloc+1, high); // 对基准元素右边的子序列进行快速排序,[pivotloc+1,high]
    }
}
void QuickSort(SqList &L){
    // 对顺序表L进行快速排序
    QSort(L,1,L.length); // 调用递归函数Qsort进行快速排序。初始值low=1, high=L.length
}
