/***********************************************************
 * 第6章 图的存储结构
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
* 6.7 案例分析与实现
 *==============================================================*/

 /* ---------- Start of 定义 ---------- */
 //  主要参考：D:\MacBook迁移\2025互联网准备\C++\数据结构_王卓_20250320\Ch6_图\Ch6_4_and5_图的存储结构and图的遍历_20250419.cpp

// 图的邻接矩阵存储表示 AMGraph
 #define MAXINT 32767 // 最大整数值，用来表示极大值。
// 2^15-1=32767，因为1个字节有8个bit，2个字节有16个bit，32767就相当于 signed short int类型的最大值(2个字节，其中1个bit用来表示符号位)。
# define MAXVNUM 100 // 最大顶点数
typedef int VertexType; // 顶点类型, 假设为字符型->暂时改为int
typedef int ArcType; // 边的权值类型, 假设为整型
typedef struct AMGraph{ // 邻接矩阵类型
    VertexType vexs[MAXVNUM]; // 顶点表(一维数组)
    ArcType arcs[MAXVNUM][MAXVNUM]; // 邻接矩阵（二维数组）
    int vexnum, arcnum; // 图的当前顶点数和边数
} AMGraph; // AMGraph是图的邻接矩阵类型，表示图的存储结构。
// AMGraph是一个结构体，包含了顶点表、邻接矩阵、顶点数和边数。AM = Adjacency Matrix
int LocateVex(AMGraph G, VertexType u) { // 查找顶点u在图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.vexs[i] == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}
// 图的邻接表存储表示 ALGraph
typedef int OtherInfo; // 其他信息类型，主要是边的信息比如权值,暂定为int类型 
#define MVNUM 100 // 最大顶点数
typedef struct ArcNode{ // 边结点类型
    int adjvex; // 邻接点域，存储该边所指向的顶点在顶点数组中的下标
    struct ArcNode *nextarc; // 指向下一条边的指针
    OtherInfo info; // 跟边相关的信息，比如权值
} ArcNode; // ArcNode是边结点类型

typedef struct VNode{ // 表头结点类型
    VertexType data; // 顶点信息域，存储顶点的信息
    ArcNode *firstarc; // 指向第一条依附该顶点的边的指针
} VNode, AdjList[MVNUM]; // VNode是顶点结点类型，AdjList是邻接表类型，表示图的存储结构。例如AdjList v;相当于 VNode v[MVNUM];
// AdjList[MVNUM]是一个数组，表示图的邻接表，MVNUM是最大顶点数。
typedef struct ALGraph{ // 图的邻接表类型
    AdjList vertices; // 邻接表 。相当于 VNode vertices[MVNUM]
    int vexnum, arcnum; // 图的当前顶点数和边数
} ALGraph; // ALGraph是图的邻接表类型，表示图的存储结构。AL = Adjacency List

int LocateVex_ALG(ALGraph G, VertexType u) { // 查找顶点u在ALGraph图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.vertices[i].data == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}


// 先引入队列的定义
#define MAXQSIZE 100 // 最大队列长度
typedef struct SqQueue{
    int *base; // 初始化的动态分配存储空间。int本来应该是QElemType，这是元素类型，可以是int、float、char等任意类型。
    int front; // 头指针，若队列不空，指向队列头元素
    int rear;  // 尾指针，若队列不空，指向队列尾元素的下一个位置
}SqQueue; 
Status InitQueue(SqQueue &Q){
    Q.base = new int[MAXQSIZE]; // 分配MAXQSIZE个元素的数组空间。int本来应该是QElemType，这是元素类型，可以是int、float、char等任意类型。
    if(!Q.base) return OVERFLOW; // 存储空间分配失败
    Q.front = Q.rear = 0; // 头指针和尾指针置为0，表示队列为空
    return OK;
}
Status EnQueue(SqQueue &Q, int e){  // int本来应该是QElemType
    if((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR; // 队列已满
    Q.base[Q.rear] = e; // 新元素插入队尾
    Q.rear = (Q.rear + 1) % MAXQSIZE; // 队尾指针+1，若到最后则转到数组头部。因此不能直接++Q.rear。
    return OK;
}
Status DeQueue(SqQueue &Q, int &e){ // int本来应该是QElemType
    if(Q.front == Q.rear) return ERROR; // 队列为空
    e = Q.base[Q.front]; // 保存队头元素
    Q.front = (Q.front + 1) % MAXQSIZE; // 队头指针+1，若到最后则转到数组头部
    return OK;
}
Status QueueEmpty(SqQueue Q){ // 判断队列是否为空
    if(Q.front == Q.rear) return True; // 队列为空
    else return False; // 队列不空
}
 /* ---------- End of 定义 ---------- */

// 【算法6.1x】六度空间理论验证(用BFS)
// 参考课本：p185

bool visited[MVNUM] = {false}; // 访问标志数组，显式初始化所有元素为False
void SixDegree_BFS(Graph G, int Start){
    // 通过BFS算法遍历G来验证六度空间理论，Start为起始顶点
    int Visit_Num = 0; // 记录路径长度不超过7的顶点数
    visited[Start] = True; // 标记起始顶点为已访问
    SqQueue Q; // 定义队列Q
    InitQueue(Q); // 初始化队列Q
    EnQueue(Q, Start); // 将起始顶点入队
    int level[6]; // 定义一个数组，用于记录每一层的顶点数。没用递归，所以不需要把level放到函数外面作为全局变量
    level[0] = 1; // 第一层入队的顶点数为初始化为1
    int i, w,u,len; // 定义变量i, j, k, w,u,len
    for(len =1;len<=6 && !QueueEmpty(Q);++len){ // 统计路径长度不超过6的顶点数。队列
        for(i=0;i<level[len-1]; ++i){ // 遍历上一层的顶点，得到本层的定点数。相当于从level[0] 到level[5]。
            DeQueue(Q, u); // 出队
            for(w= FirstAdjVex(G, u); w>=0; w= NextAdjVex(G, u, w)){ // 遍历顶点u的所有邻接点
            // FirstAdjVex(G, u)返回顶点u的第一个邻接点w，NextAdjVex(G, u, w)返回顶点u的邻接点w的下一个邻接点,w>=0表示存在临界点
                if(!visited[w]){ // 如果顶点w没有被访问过
                    visited[w] = True; // 标记顶点w为已访问
                    ++Visit_Num; // 统计路径长度不超过6的顶点数 +1
                    level[len]++; // 统计第len层的顶点数 +1（注意不是level[len-1]而是level[len]）
                    EnQueue(Q, w); // 将顶点w入队
                } //if
            } // for w
        } // for i    
    } // for len
    cout << 100 * Visit_Num / G.vexnum << "%" << endl; 
    // 输出从顶点Start出发的路径长度不超过6的顶点数，占总顶点数的百分比
}

void SixDegree_BFS_ALGraph(ALGraph G, int Start){
    // 通过BFS算法遍历G来验证六度空间理论，Start为起始顶点。这里用到邻接表存储图。
    int Visit_Num = 0; // 定义变量Visit_Num，用于统计路径长度不超过6的顶点数
    int i,u,w,len; // 定义变量i, j, k, w,u,len
    ArcNode *p; // 定义指针p，用于遍历顶点u的所有邻接点
    int level[6]; // 定义一个数组，用于记录每一层的顶点数。没用递归，所以不需要把level放到函数外面作为全局变量
    level[0] = 1; // 初始化第一层的顶点数为1
    SqQueue Q;
    InitQueue(Q);
    visited[Start] = True;
    EnQueue(Q, Start);
    for(len=1;len<=6&&!QueueEmpty(Q);++len){ 
        for(i=0;i<level[len-1];++i){
            DeQueue(Q, u); // 出队
            p = G.vertices[u].firstarc; // 获取顶点u的第一个邻接点
            while(p){ // 遍历顶点u的所有邻接点
                if(!visited[w]){ // 如果顶点w未被访问，则加入访问列表，并增加Visit_Num、level[len]的值，并将顶点w入队
                    visited[w] = True;
                    ++Visit_Num;
                    level[len]++;
                    EnQueue(Q, w);
                }
                p = p->nextarc; // 获取顶点u的下一个邻接点
            } 
        }
    }
    cout<< 100* Visit_Num/G.vexnum << "%" << endl;
    // 输出从顶点Start出发的路径长度不超过6的顶点数，占总顶点数的百分比
}