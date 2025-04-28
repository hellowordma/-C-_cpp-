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
* 7.4 散列表的查找
 *==============================================================*/


/* -------------- 7.4.4 散列表的查找--------------- */

// 开放地址法，散列表的存储表示
#define m 20 // 散列表的表长
typedef int KeyType; // 关键字类型。暂时假设为int类型
typedef int InfoType; // 其他数据域类型。暂时假设为int类型

typedef struct HashTable{
    KeyType key; // 关键字域
    InfoType otherinfo; // 其他数据域
}HashTable[m]; // 散列表(数组)


// 【算法7.10】 散列表的查找。解决冲突的方法：开放定址法（细分探测方法：线性探测法）
// 参考视频：第13周9--7.4散列表的查找3-7.4.3处理冲突方法1--开放地址法-线性探测法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.videopod.episodes&p=155
// 参考课本：p225-226

// 辅助函数：散列函数（除留余数法）
int Hash(KeyType key){
    // 散列函数，计算散列地址
    int n = 19; // n一般取≤散列表表长m的质数
    return key % n; // 除留余数法。
}
#define NULLKEY 0 // 单元为空的标志

// 正式开始
int SearchHash(HashTable HT, KeyType key){
    // 在散列表HT中查找关键字为key的元素，若找到，则返回散列表的单元标号；否则，返回-1。
    int H0 = Hash(key); // 根据散列函数H(key)计算散列地址H0
    if (HT[H0].key == NULLKEY) return -1; //  若当前单元为空，则所查元素不存在，返回-1
    else if (HT[H0].key == key) return H0; // 若当前单元存放的正是所查元素，则返回当前单元的地址H0
    else{ // 若当前单元非空且存放的不是所查元素，则按照冲突解决方法处理        
        for (int i = 1; i < m; ++i){ // 线性探测法：Hi = (H0 + di) % m, di = 1, 2, ..., m-1
            int Hi = (H0 + i) % m; // 计算下一个散列地址Hi
            if (HT[Hi].key == NULLKEY) return -1; // 若查找开放地址序列中的单元为空，则所查元素不存在，返回-1
            else if (HT[Hi].key == key) return Hi; // 若当前单元存放的正是所查元素，则返回当前单元的地址Hi
        }
        return -1; // 若开放地址序列中所有单元都不为空，则表示散列表已满，返回-1
    }
}