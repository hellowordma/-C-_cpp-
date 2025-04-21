/***********************************************************
 * 第2章 线性表
 ***********************************************************/

//导入相关的头文件
# include <stdio.h> // 标准输入输出流
# include <iostream> // 输入输出流
#include <cstring> // 包含 strcpy 函数的头文件

using namespace std; // 使用标准命名空间   

// Status定义
#define OK 1
#define ERROR 0 
#define OVERFLOW -2
typedef int Status; // Status是函数的类型，其值是函数结果状态代码

/*==================================
* 第02周 06 和 07 课程内容——类C语言的补充
 *==================================*/

// C++的参数传递方式
// 参考链接：第02周07--类C语言有关操作补充2 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=16 
// 1. 值传递（不改变实参的值）
// 2. 地址传递（改变实参的值）
//  2.1. 指针传递（参数为指针类型，*p）
//  2.2. 引用传递（参数为引用类型, &）
//  2.3. 数组传递（参数为数组名, []）

// 代码实现
// 1. 值传递
void swap1(int m, int n){
    int temp = m;
    m = n;
    n = temp;
}
// int main(){ // 注意：C++的main函数返回值类型必须是int，所以main不能是void
//     int a = 3, b = 5;
//     swap1(a, b);
//     cout << "a = " << a << ", b = " << b << endl; // a = 3, b = 5
//     return 0;
// }

// 2. 地址传递
// 2.1. 指针传递
void swap2(int *m, int *n){
    int temp;
    temp = *m; // *m 不是定义的时候，所以此处是指取m指针指向的内容，相当于解引用
    *m = *n;
    *n = temp;
}
// int main(){
//     int a = 3, b = 5, *p1 = &a, *p2 = &b;
//     cin >> a >> b;
//     swap2(p1, p2);
//     cout << "a = " << a << ", b = " << b << endl; // a = 5, b = 3。此时p1和p2作为指针，并没有变化，但是a和b的值变化了。
//     return 0;
// }

// 2.2. 引用传递
void swap3(int &m, int &n){
    int temp;
    temp = m;
    m = n;
    n = temp;
}
// int main(){
//     int a = 3, b = 5;
//     swap3(a, b);
//     cout << "a = " << a << ", b = " << b << endl; // a = 5, b = 3
//     return 0;
// }

// 2.3. 数组传递。数组之所以能传递，是因为数组名是数组首元素的地址。
void sub(char b[]) {
    // b[] = 'world'; // 错误，数组名不能赋值
    // 使用 strcpy 将 "world" 复制到数组 b 中
    strcpy(b, "world");
}
// int main(){
//     char a[10] = "world";
//     sub(a);
//     cout << a << endl; // hello
//     return 0;
// }

 int main(){
    //1. 动态分配内存(两种语言的方法)
    // 参考链接：第02周06--类C语言有关操作补充1 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=15
    # define MAX_SIZE 100 // 线性表存储空间的初始分配量

    // 方法1：C语言的方法，用malloc函数分配内存，然后用free函数释放内存（malloc和free是对应的）
    int *p1 = (int *)malloc(MAX_SIZE * sizeof(int)); // C语言的方法，动态分配内存，分配100个int类型的内存空间。int可以抽象成ElemType
    cout << "分配内存成功" << endl;
    free(p1); // 释放内存，C语言
    cout << "释放内存成功" << endl;
    // 方法2：C++的方法，用new运算符分配内存，然后用delete运算符释放内存（new和delete是对应的）
    int *p2 = new int; // C++的方法，动态分配内存，分配一个int类型的内存空间。注意：只有指针才能被new分配内存，其他类型比如int、float、char等都不行。
    cout << "分配内存成功" << endl;
    delete p2; // 释放内存，C++
    cout << "释放内存成功" << endl;



    return 0;
}