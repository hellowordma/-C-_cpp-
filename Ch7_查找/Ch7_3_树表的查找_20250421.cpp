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
* 7.3 树表的查找
 *==============================================================*/


 /* -------------- 7.3.1 二叉排序树--------------- */
 
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
