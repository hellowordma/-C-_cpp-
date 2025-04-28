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
* 8.4 选择排序
- 基本思想：每次从待排序序列中选择最小（或最大）的元素，放在已排序序列的末尾，直到所有元素都排好序。
*==============================================================*/

/* -------------- 8.4.1 简单选择排序（Simple Selection Sort，直接选择排序）--------------- */

//【算法8.6】简单选择排序（Simple Selection Sort，直接选择排序）
// 参考视频：第14周08--第8章排序8--8.4选择排序1--简单选择排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=166
// 参考课本：p246-p247
// 时间复杂度：O(n^2) （最好，最坏，平均都一样）
// 空间复杂度：O(1)。
// 基本思路：每次从待排序序列中选择最小（或最大）的元素，放在已排序序列的末尾，直到所有元素都排好序。
// 排序稳定性：不稳定。（但可以通过改进成稳定的选择排序）
// 性能对于顺序、无序、逆序的区别：顺序最好，O(n)；无序次之，O(n^2)；逆序最差，O(n^2)。 
// 存储结构：可以用顺序存储结构，也可以用链式存储结构。
// 适用情况：因为移动记录次数较少，所以当每个记录占用空间较多的时候，比直接插入排序要好。

void SelectSort(SqList &L){
    // 对顺序表L进行简单选择排序
    int i,j,k; // i是当前元素，j是前一个元素，k是最小元素的下标
    for(i=1; i<L.length; ++i){ // 从第1个元素到倒数第2个元素（因为到最后，最后一个元素已经排好序了），依次选择最小元素
        k = i; // 假设第i个元素是最小元素
        for(j=i+1; j<=L.length; ++j){ // 从第i+1个元素开始到最后一个元素，依次比较
            if(L.r[j].key < L.r[k].key) k = j; // 如果找到更小的元素，则更新最小元素的下标。k指向此次排序中关键字最小的记录
        }
        if(k != i) swap(L.r[i], L.r[k]); // 如果最小元素k不是第i个元素，则交换i和k的位置。否则不需要交换。
    }
}

/* -------------- 8.4.3 堆排序（Heap Sort）--------------- */

// 【算法8.7】筛选法调整堆
// 参考视频：第14周10--第8章排序10--8.4选择排序3--堆排序2--堆调整 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=168
// 参考课本：p251
// 思路：在交换了r[s]和r[m+1]之后，r[s+1]~r[m]已经是堆（大根堆），需要将r[s]~r[m]调整为堆（大根堆），像筛子一样，从上到下筛选，
//      让较大的元素往上走，较小的元素往下走。

void HeapAdjust(SqList &L, int s, int m){
    // 假设r[s+1]~r[m]已经是堆（大根堆），将r[s]~r[m]调整为堆（大根堆）
    // 注意：swap(L.r[s], L.r[m+1]) 是在主函数中对HeapAdjust函数的前置操作。
    RedType rc = L.r[s]; // 将当前节点保存到临时变量rc中。rc=record
    int j;
    for(j=2*s; j<=m; j*=2){ // 沿着key较大的孩子节点向下查找，直到叶子节点为止。
        if(j < m && L.r[j].key < L.r[j+1].key) ++j; // 如果右孩子存在且大于左孩子，则将j指向右孩子,否则指向左孩子
        if(rc.key >= L.r[j].key) break; // 如果当前节点rc大于等于子节点，则不需要调整，直接退出循环
        L.r[s] = L.r[j]; // 如果当前节点rc小于子节点，则将子节点的值赋值给当前节点
        s = j; // 更新当前节点s为子节点j，相当于s永远是j的双亲节点
    }
    L.r[s] = rc; // 将rc的值赋值给当前节点s，相当于将rc插入到正确的位置
}


// 【算法8.8】建初堆（大根堆）
// 参考视频：第14周11--第8章排序11--8.4选择排序4--堆排序3--建初堆 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=169
// 参考课本：p251-p252
// 思路：本质上是反复调用HeapAdjust函数的过程，从最后一个非叶子节点(n/2)开始，依次向上调整，直到根节点。叶子结点本身不需要调整。
void CreatHeap(SqList &L){
    // 把无序徐磊L.r[1]~L.r[length]调整为大根堆
    int i, n = L.length; // n是线性表的长度
    for(i=n/2; i>0; --i){ // 从最后一个非叶子节点开始，依次向上调整.这里的i是从n/2开始的，因为n/2是最后一个非叶子节点。
        HeapAdjust(L, i, n); // 调整为大根堆
    }
}

// 【算法8.9】堆排序（Heap Sort）
// 参考视频：第14周12--第8章排序12--8.4选择排序5--堆排序4--堆排序算法及分析 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=170
// 参考课本：p252
// 思路：先建初堆，然后将堆顶元素（最大元素）与最后一个元素交换，然后将剩下的元素重新调整为堆，直到所有元素都排好序。
// 时间复杂度：O(nlogn)（最好，最坏，平均都一样）
// 空间复杂度：O(1)。
// 排序稳定性：不稳定。
// 性能对于顺序、无序、逆序的区别：没有区别。
// 存储结构：顺序和链式存储结构都可以。
// 适用情况：n较大时较为高效。最坏情况下也是O(nlogn)，相比快速排序的O(n^2)还要好。

void HeapSort(SqList &L){
    // 对顺序表L进行堆排序
    int i, n = L.length; // n是线性表的长度
    CreatHeap(L); // 建初堆
    for(i=n; i>1; --i){ // 从最后一个元素开始，依次向前调整,直到第2个元素为止。注意这里是从n开始的，因为n是最后一个元素。
        swap(L.r[1], L.r[i]); // 交换堆顶元素1和 未经排序的子序列L.r[1]~L.r[i]的最后一个元素i
        HeapAdjust(L, 1, i-1); // 将L.r[1]~L.r[i-1]调整为大根堆。
    }
}