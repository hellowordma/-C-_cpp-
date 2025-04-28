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
* 8.5 归并排序
- 基本思路：将两个或两个以上的有序表合并成一个新的有序表。将两个有序表合并成一个有序表的过程称为2-路归并。
         - 具体来说：假设初始序列含有n个记录，则可看成是n个有序的子序列，每个子序列的长度为1，然后两两归并，得到[n/2个](进一法）长度为2或1的有序子序列;再两两归并，……，如此重复，直至得到个长度为n的有序序列为止。
*==============================================================*/

//【算法8.10】相邻两个有序子序列的归并
// 参考视频：第14周13--第8章排13--8.5归并排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=171
// 参考课本：p255

void Merge(RedType R[], RedType T[], int low, int mid, int high){
    // 将有序表R[low]~R[mid]和R[mid+1]~R[high]归并成一个有序表T[low]~T[high]
    int i = low, j = mid + 1, k = low; // i是第一个有序表的起始位置，j是第二个有序表的起始位置，k是T的起始位置
    while(i <= mid && j <= high){ // 将R中的记录由小到大地并入T中
        if(R[i].key <= R[j].key){ // 如果第一个有序表的当前元素小于等于第二个有序表的当前元素
            T[k++] = R[i++]; // 将第一个有序表的当前元素放入T中，并移动i和k
        }
        else T[k++] = R[j++]; // 否则将第二个有序表的当前元素放入T中，并移动j和k
    }
    while(i <= mid) T[k++] = R[i++]; // 如果有剩余，将第一个有序表中剩余的元素放入T中
    while(j <= high) T[k++] = R[j++]; // 如果有剩余，将第二个有序表中剩余的元素放入T中
}

// 【算法8.11】归并排序（Merge Sort）
// 参考视频：第14周13--第8章排13--8.5归并排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=171
// 参考课本：p255-256
// 思路：将待排序序列分割成若干个子序列，每个子序列是有序的。然后对子序列进行合并，得到最终有序序列。
// 时间复杂度：O(nlogn)（最好，最坏，平均都一样）。当有n个元素是，需要进行[log2 n](进一法) 趟归并，每趟关键字比较次数不超过n，元素移动次数都是n，所以时间复杂度是O(nlogn)。
// 空间复杂度：O(n).因为 需要一个与原序列等长的辅助数组T来存放归并结果。
// 性能对于顺序、无序、逆序的区别：没有区别。
// 排序稳定性：稳定。
// 适用性：适用于顺序表和链表。链表不需要附加存储空间，但递归调用时需要开辟相应的递归工作栈空间。

void MSort(RedType R[], RedType T[], int low, int high){
    // 将R[low]~R[high]归并排序，结果放入T[low]~T[high]
    if(low==high) T[low] = R[low]; // 如果只有一个元素，直接将其放入T中
    else{
        int mid = low + (high-low) / 2; // 计算中间位置mid,将当前序列一分为二，mid作为分裂点
        RedType S[MAXSIZE]; // 定义一个临时数组S，用于存放合并后的结果
        MSort(R, S, low, mid); // 对左半部分进行归并排序，结果放入到S[low]~S[mid
        MSort(R, S, mid + 1, high); // 对右半部分进行归并排序,结果放入到S[mid+1]~S[high]
        Merge(S, T, low, mid, high); // 将两个有序子序列合并成一个有序序列,即将S[low]~S[mid]和S[mid+1]~S[high]合并成T[low]~T[high]
    }
}

void MergeSort(SqList &L){
    // 对顺序表R[1]~R[n]进行归并排序
    MSort(L.r, L.r, 1, L.length); // 将L.R[1]~L.R[n]归并排序，结果放入到L.R[1]~L.R[n]
}