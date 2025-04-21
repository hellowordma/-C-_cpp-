/***********************************************************
 * 第2章 线性表
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
typedef int Status; // Status是函数的类型，其值是函数结果状态代码


/*==================================
* 2.4 线性表的顺序存储结构
 *==================================*/

 /* ---------- 2.4.1 顺序表的顺序存储表示---------- */
// 参考视频： 第02周05--2.4线性表的顺序表示和实现2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=14
// 课本范围：p23-p25
// 代码实现

// 1. 定义线性表的顺序存储结构SqList
# define LIST_INIT_SIZE 100 // 线性表存储空间的初始分配量
typedef struct SqList{
    // ElemType elem[LIST_INIT_SIZE]; // 这行是书上的，但是会报错，所以改成这行。ELemType是元素类型，可以是int、float、char等任意类型。
    // int elem[LIST_INIT_SIZE]; //当前是静态分配内存的方式。 也可以考虑改成 int *elem;  这是动态分配内存的方式
    int *elem; // 动态分配内存的方式。动态分配内存会到堆区，需要手动释放内存；静态分配内存会到栈区，系统自动释放内存。
    int length; // 当前长度
}SqList;

// 2. 例子1：多项式的顺序存储结构
# define MAX_SIZE 100
typedef struct Polynomial{
    float coef[MAX_SIZE]; // 系数
    int expn[MAX_SIZE]; // 指数
    int length; // 当前长度
}Polynomial; // 多项式定义

typedef struct SqList_Poly{
    Polynomial *elem;
    int length;
}SqList_Poly; // 多项式的顺序表定义

// 3. 例子2： 图书表的顺序存储结构
# define BOOK_MAX_SIZE 100
typedef struct Book{
    char no[10]; // 书号
    char name[20]; // 书名
    float price; // 价格
}Book;

typedef struct SqList_Book{
    Book elem[BOOK_MAX_SIZE];
    int length;
}SqList_Book; // 图书表的顺序表定义


 /* ---------- 2.4.2 线性表的顺序表示的基本操作的实现---------- */
// 参考视频： 第02周05--2.4线性表的顺序表示和实现3 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=17
// 课本范围：p25-p29

// 1. 【算法2.1】初始化 线性表的顺序表示。（见课本《数据结构(严蔚敏)》）
Status InitList(SqList &L){
    L.elem = new int[LIST_INIT_SIZE]; // 分配内存。
    if(!L.elem) return OVERFLOW; // 存储空间分配失败
    L.length = 0; // 空表长度为0
    return OK;
}

// 2. 【算法2.2】销毁 线性表的顺序表示
void DestroyList(SqList &L){
    if (L.elem) delete L.elem; // 释放内存
}

// 3. 清空 线性表的顺序表示 
void ClearList(SqList &L){
    L.length = 0; // 空表长度为0
}

// 4. 求长度： 线性表的顺序表示 的长度 
int ListLength(SqList L){
    return L.length;
}

// 5. 判断空表： 线性表的顺序表示 是否为空
int ListEmpty(SqList L){
    if (L.length == 0) return 1; // 空表
    else return 0; // 非空表
}

// 6. 【算法2.2】取值： 线性表的顺序表示 的第i个元素。
// 时间复杂度：O(1)，因为是随机存取
int GetElem(SqList L, int i, int &e){
    if (i < 1 || i > L.length) return ERROR; // i值不合法
    e = L.elem[i-1]; // 取第i个元素的值
    return OK;
}

// 7. 【算法2.3】查找： 线性表的顺序表示 中的元素e。（见课本《数据结构(严蔚敏)》）
// 参考视频：第02周09--2.4线性表的顺序表示和实现4--查找算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=18
// 时间复杂度：O(n)，因为是顺序查找
int LocateElem(SqList L, int e){
// 在顺序线性表L中查找第1个值与e相等的元素, 如果找到, 则返回其位序, 否则返回0
    for (int i = 0; i < L.length; i++){
        if (L.elem[i] == e) return i+1; // 找到元素e，返回其位序, 位序从1开始而不是0，所以要+1
    }
    return 0; // 未找到元素e
}

// 8. 【算法2.4】插入： 在 线性表的顺序表示 的第i个位置插入元素e。（见课本《数据结构(严蔚敏)》）
// 参考视频：第02周10--2.4线性表的顺序表示和实现5--插入算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=19
// 时间复杂度：O(n)，因为是要挪动元素。
Status ListInsert_Sq(SqList &L, int i, int e){ // 注意，插入第i个元素，i从1开始而不是0，应用到数组时i要-1
    if (i < 1 || i > L.length+1) return ERROR; // i值不合法。 i应该在1到L.length+1之间
    if (L.length >= LIST_INIT_SIZE) return ERROR; // 当前存储空间已满
    for (int j = L.length; j >= i; j--){
        L.elem[j] = L.elem[j-1]; // 插入位置及之后的元素后移
    }
    L.elem[i-1] = e; // 插入e。注意是i-1而不是i
    L.length++; // 表长增1
    return OK;
}

// 9. 【算法2.5】删除： 删除 线性表的顺序表示 的第i个位置的元素，并用e返回其值。（见课本《数据结构(严蔚敏)》）
// 参考视频：第02周11--2.4线性表的顺序表示和实现6--删除算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=20
// 时间复杂度：O(n)，因为是要挪动元素
Status ListDelete_Sq(SqList &L, int i){ // 注意，删除第i个元素，i从1开始而不是0,应用到数组时i要-1
    if (i < 1 || i > L.length) return ERROR; // i值不合法。 i应该在1到L.length之间
    // e = L.elem[i-1]; // 被删除的元素赋值给e
    for (int j = i; j < L.length; j++){
        L.elem[j-1] = L.elem[j]; // 被删除位置之后的元素前移
    }
    L.length--; // 表长减1
    return OK;
}

int main(){
    SqList L;
    L.length = 0;
    cout << "线性表的长度为：" << L.length << endl;
    return 0;
}