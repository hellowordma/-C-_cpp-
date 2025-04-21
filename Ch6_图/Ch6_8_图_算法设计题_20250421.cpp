/***********************************************************
 * 第6章 图的存储结构
 ***********************************************************/

// 参考来源：
// 1. 课本：《数据结构 C语言第二版》by 严蔚敏 p190
// 2. 习题：《数据结构习题解析与实验指导》by 李冬梅。 p129-p136。

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

// 定义顺序栈 SqStack
#define MAXSIZE 100 // 最大栈长度
typedef struct SqStack{
    int *base; // 栈底指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int *top; // 栈顶指针. int本来应该是SElemType，这是元素类型，可以是int、float、char等任意类型。
    int stacksize; // 栈的大小
}SqStack;
Status InitStack(SqStack &S){ 
    S.base = new int[MAXSIZE]; // 分配MAXSIZE个元素的数组空间。如果SqStack的元素类型是int，则分配int类型的数组空间；如果SqStack的元素类型是char，则分配char类型的数组空间。
    if(!S.base) return OVERFLOW; // 存储空间分配失败
    S.top = S.base; // 栈顶指针指向栈底，作为初始化（栈空）
    S.stacksize = MAXSIZE; // 栈的大小
    return OK;
}
Status StackEmpty(SqStack S){ // 判断栈空
    if (S.top == S.base) return 1; // 栈空
    else return 0; // 栈非空
}
Status Push(SqStack &S, int e){ // 入栈操作。int本质上是SElemType
    if (S.top - S.base == S.stacksize) 
        return ERROR; // 栈满
    *S.top++ = e; // 将元素e压入栈顶，栈顶指针上移。相当于 *S.top = e; S.top++;
    return OK;
}
Status Pop(SqStack &S, int &e){ // 出栈操作
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *--S.top; // 将栈顶元素赋值给e，栈顶指针下移。相当于 --S.top; e = *S.top;。
    // 注意不是 *S.top--，因为栈顶元素的值需要top指针下移后才能获取。
    return OK;
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

//  1. 分别以邻接矩阵和邻接表作为存储结构，实现以下图的基本操作
// （1）增加一个新顶点，InsertVex(G,v)
// （2）删除一个顶点，DeleteVex(G,v)
// （3）增加一条边<v,w>，InsertArc(G,v,w)
// （4）删除一条边<v,w>，DeleteArc(G,v,w)

//  假设是无向无权图
// !!! 注意：AMGraph的G.vexs数组的下标范围是[0,vexnum-1]; ALGraph的G.vertices数组下标范围也是[0,vexnum-1]
//     所以++G.vexnum和--G.vexnum的操作是放到最后而不是最前面的。
// !!! 注意：代码可能有点混乱，v，w可能指的是顶点的索引（int), 也可能是指的是顶点本身（但如果设置为int，则VertexType也要暂时改成int）。

// 先做邻接矩阵的部分。
Status InsertVex_AMGraph(AMGraph &G, VertexType v) {
    // 增加一个新顶点v（用邻接矩阵）
    if (G.vexnum >= MAXVNUM) return ERROR; // 顶点数已达到最大值，不能插入，返回ERROR
    G.vexs[G.vexnum] = v; // 将新顶点v加入顶点表
    for(int i = 0; i <=G.vexnum; ++i) { // 将新顶点与其他顶点之间添加边, 包含自身
        G.arcs[i][G.vexnum] = 0;G.arcs[G.vexnum][i] = 0; // 新顶点与其他顶点之间无边
    }
    ++G.vexnum; // 顶点数加1
}
Status DeleteVex_AMGraph(AMGraph &G, VertexType v) {
    // 删除一个顶点v（用邻接矩阵）。采用交换的方法，避免多次移动元素
    int i,temp; 
    int n = G.vexnum; // 顶点数n
    int k = LocateVex(G,v); // 顶点v的索引k
    if(k== -1) return ERROR; // 顶点v不存在,返回ERROR
    // 将待删除顶点交换到最后一个顶点。
    VertexType tmp = G.vexs[n-1];
    G.vexs[n-1] = G.vexs[k];
    G.vexs[k] = tmp;
    for(i = 0; i < n; ++i) { // 将边的关系也随之交换
        temp = G.arcs[n-1][i];
        G.arcs[n-1][i] = G.arcs[k][i];
        G.arcs[k][i] = temp;
        temp = G.arcs[i][n-1]; // 其实可以在无向图中省略这一行，因为 G.arcs[i][n-1] = G.arcs[n-1][i]
        G.arcs[i][n-1] = G.arcs[i][k];
        G.arcs[i][k] = temp;
    }
    --G.vexnum; // 顶点数减1，自然把最后一个顶点以及对应的边删除了（类似栈的出栈操作，即 --S.top)
    return OK;
}
Status InsertArc_AMGraph(AMGraph &G, VertexType v, VertexType w) {
    // 在图G中添加一条边(v,w)（用邻接矩阵）
    int i = LocateVex(G,v), j = LocateVex(G,w); // 查找顶点v和w在图G中的位置,i和j
    if(i == -1 || j == -1) return ERROR; // 顶点v或w不存在,返回ERROR
    G.arcs[i][j] = G.arcs[j][i] = 1; // 添加边(v,w)，无向图
    return OK;
}
Status DeleteArc_AMGraph(AMGraph &G, VertexType v, VertexType w) {
    // 在图G中删除一条边(v,w)（用邻接矩阵）
    int i = LocateVex(G,v), j = LocateVex(G,w); // 查找顶点v和w在图G中的位置,i和j
    if(i == -1 || j == -1) return ERROR; // 顶点v或w不存在,返回ERROR
    G.arcs[i][j] = G.arcs[j][i] = 0; // 删除边(v,w)，无向图
    return OK;
}
// 再做邻接表的部分
Status InsertVex_ALGraph(ALGraph &G, VertexType v) {
    // 在图G中插入顶点v（邻接表）
    if(G.vexnum>=MAXVNUM) return ERROR; // 顶点数已达到最大值,不能再插入，返回ERROR
    G.vertices[G.vexnum].data = v; // 将新顶点v加入顶点表
    G.vertices[G.vexnum].firstarc = NULL; // 新顶点的第一条边指针置空
    ++G.vexnum; // 顶点数加1
    return OK;
}
Status DeleteVex_ALGraph(ALGraph &G, VertexType v) {
    // 在图G中删除顶点v（用邻接表）
    int i, k = LocateVex_ALG(G,v); // 查找顶点v在图G中的位置
    if(k == -1) return ERROR; // 顶点v不存在,返回ERROR
    ArcNode *p, *q; //  定义边结点指针p和q
    for(i=k; i<G.vexnum-1; ++i) // 顶点v后面的顶点依次前移
        G.vertices[i] = G.vertices[i+1];
    --G.vexnum; // 顶点数减1。此时相当于同时把顶点v的边表也删除了，之后只修要把其他顶点的边表中指向顶点v的边删除即可
    for(i=0; i<G.vexnum; ++i) { // 遍历所有顶点，依次删除顶点v的所有边（即其他顶点的边表中指向顶点v的边）
        p = q = G.vertices[i].firstarc; // p和q都指向顶点i的第一条边。
        // 之后让p指向当前结点的前驱，q指向当前结点。
        if(p->adjvex==k) { // 找到以顶点v为弧头的弧, 删除（采用链表的方法）。k是顶点v在图G中的位置
            p = p->nextarc; // p指向当前边结点的后继，即删除当前边结点
            --G.arcnum; // 边数减1
            delete q; // 释放结点q（当前结点）
            continue; // 继续for循环，跳过后面的q的部分。
        }
        q = q->nextarc; // q指向当前结点的后继。继续比较后面的边结点
        while(q){ 
            if(q->adjvex==k) { // 找到以顶点v为弧头的弧, 删除（采用链表的方法）。k是顶点v在图G中的位置
                p= q->nextarc; // p指向当前边结点的后继，即删除当前边结点
                --G.arcnum; // 边数减1
                delete q; // 释放结点q（当前结点）
                break; // 跳出while循环
            }
            p = q; // p指向当前结点
            q = q->nextarc; // 没找到与v相关联的边，q指针后移
        }
    }
    return OK;
}
Status InsertArc_ALGraph(ALGraph &G, VertexType v, VertexType w) {
    // 在有向图G中插入边(v,w)（用邻接表）
    // 思路：链表的头插法。
    int i = LocateVex_ALG(G,v), j = LocateVex_ALG(G,w); // 查找顶点v和w在图G中的位置
    if(i == -1 || j == -1) return ERROR; // 顶点v或w不存在,返回ERROR
    ArcNode *p1 = new ArcNode; // 定义新结点p1
    ArcNode *p2 = new ArcNode; // 定义新结点p2
    // 先插入边(v,w)，用到头插法
    p1->adjvex = j; // p1指向顶点w
    p1->nextarc = G.vertices[i].firstarc; // p1的下一条边指向顶点i的第一条边
    G.vertices[i].firstarc = p1; // 顶点i的第一条边指向p1
    // 再插入边(w,v)，用到头插法
    p2->adjvex = i; // p2指向顶点v
    p2->nextarc = G.vertices[j].firstarc; // p2的下一条边指向顶点j的第一条边
    G.vertices[j].firstarc = p2; // 顶点j的第一条边指向p2
    return OK;
}
Status DeleteArc_ALGraph(ALGraph &G, int v, int w) {
    // 在有向图G中删除边(v,w)（用邻接表）
    int i = LocateVex_ALG(G,v), j = LocateVex_ALG(G,w); // 查找顶点v和w在图G中的位置
    if(i == -1 || j == -1) return ERROR; // 顶点v或w不存在,返回ERROR
    ArcNode *p1, *p2; // 定义两个指针
    // 先删除边(v,w)
    if(G.vertices[i].firstarc->adjvex == j){ // 如果顶点v的第一条边就是顶点w,则直接删除
        p1 = G.vertices[i].firstarc;
        G.vertices[i].firstarc = G.vertices[i].firstarc->nextarc;
        delete p1;
        --G.arcnum; // 边数减1
    } //if
    else{ // 否则，遍历顶点v的边表，找到顶点w的边
        p1 = G.vertices[i].firstarc->nextarc; // p1指向顶点v的第一条边的下一条边
        while(p1){ // 遍历顶点v的边表
            if(p1->adjvex == j){ // 找到顶点w的边
                p2 = p1; // 定义新结点p2,指向当前结点p1的前驱结点
                p1 = p1->nextarc; // p1指向当前结点的下一条边
                G.vertices[i].firstarc->nextarc = p1; 
                delete p2;
                --G.arcnum; // 边数减1
                break;
            } //if
            else p1 = p1->nextarc; // p1指向当前结点的下一条边
        } //while
    } //else
    // 再删除边(w,v)。 代码基本上和删除边(v,w)一样，只需要i和j互换即可。
    if(G.vertices[j].firstarc->adjvex == i){ // 如果顶点w的第一条边就是顶点v,则直接删除
        p1 = G.vertices[j].firstarc;
        G.vertices[j].firstarc = G.vertices[j].firstarc->nextarc;
        delete p1;
        --G.arcnum; // 边数减1
    } //if
    else{ // 否则，遍历顶点w的边表，找到顶点v的边
        p1 = G.vertices[j].firstarc->nextarc; // p1指向顶点w的第一条边的下一条边
        while(p1){ // 遍历顶点w的边表
            if(p1->adjvex == i){ // 找到顶点v的边
                p2 = p1; // 定义新结点p2,指向当前结点p1的前驱结点
                p1 = p1->nextarc; // p1指向当前结点的下一条边
                G.vertices[j].firstarc->nextarc = p1; 
                delete p2;
                --G.arcnum; // 边数减1
                break;
            } //if
            else p1 = p1->nextarc; // p1指向当前结点的下一条边
        } //while
    } //else
    return OK;
}

// 2. 一个连通图采用邻接表作为存储结构，设计一个算法，实现从顶点v出发的深度优先遍历的非递归过程，
// 思路：栈
// - 参考习题：p133-p134

bool visited[MVNUM] = {false}; // 访问标志数组，显式初始化所有元素为False

// 正式开始
void DFS(ALGraph G, int v) { 
    // 从顶点v出发，非递归地深度优先遍历图G。
    SqStack S; // 定义栈S
    int k; // 定义变量k
    ArcNode *p; // 定义边结点指针p
    int w; // 定义顶点类型w
    InitStack(S); // 初始化栈S
    Push(S, v); // 将顶点v入栈
    while(!StackEmpty(S)) { // 当栈不空时
        Pop(S, k); // 出栈一个顶点,并将其赋值给k
        if(!visited[k]) { // 如果k没有被访问过，则访问k
            cout << k; // 访问第k个顶点
            visited[k] = True; // 将k标记为已访问
            p = G.vertices[k].firstarc; // p指向第k个顶点的第一条边
            while(p) { // 遍历所有边
                w = p->adjvex; // p的邻接点为w
                if(!visited[w]) Push(S, w); // 如果w没有被访问过，则将w入栈
                p = p->nextarc; // p指向顶点k的下一条边
            } //while
        } // if
    } // while
}

// 3. 设计一个算法，求图G中距离顶点v的最短路径长度最大的一个顶点，设v可达其余各个顶点。
// 思路：借助Dijkstra算法得到D[i]数组，然后求出D[i]的最大值的下标m即可。
// 参考习题：p134-p135

// 辅助数组S：存储已知最短路径的顶点
int S[MAXVNUM]; // S[i] = True表示第i个顶点在集合S中，False表示尚未确定
// 辅助数组Path：存储最短路径的前驱顶点
int Path[MAXVNUM]; // Path[i]表示第i个顶点的前驱顶点
// 辅助数组D：存储从源点到各个顶点的最短路径
int D[MAXVNUM]; // D[i]表示从源点到第i个顶点的最短路径

// 正式开始
int ShortestPathMax(AMGraph G,int v0){
    // 求图G中距离顶点v0的最短路径长度最大的一个顶点。
    // 先把ShortestPath_DIJ函数的代码复制过来，然后额外增加取最大值的操作即可。
    int i,v,w,min_; // 定义变量i,v,w,min_
    int n = G.vexnum; // 顶点数
    for(v = 0; v < n; ++v){
        S[v] = False; // 初始化S数组，为空集合
        D[v] = G.arcs[v0][v]; // 初始化D数组，将v0到各个终点v的最短路径长度初始化为弧上的权值
        if(D[v] < MAXINT) Path[v] = v0; // 如果v0到v之间有弧，则Path[v]初始化为v0，表示v的前驱顶点是v0
        else Path[v] = -1; // 否则，Path[v]初始化为-1，即v的前驱顶点不存在
    }
    S[v0] = True; // 将源点v0加入集合S
    D[v0] = 0; // 源点到源点的最短路径长度为0
    /*初始化结束，开始主循环，每次求得v0到某个顶点v的最短路径，将v加到S集合*/
    for(i=1; i<n; ++i) { // 循环n-1次, 对其余n-1个顶点，依次计算。
        min_ = MAXINT; // 初始化最小值为极大值
        for(w=0; w<n; ++w) { // 遍历所有顶点（集合V-S上）, 选择一条当前的最短路径，终点为w。相当于遍历找最小值。
            if(!S[w] && D[w] < min_) { // 如果w不在集合S中且D[w]小于当前最小值，则更新最小值和索引
                v = w; // 更新索引
                min_ = D[v]; // 更新最小值. 改成min_= D[w]也可以。
            }
        }
        S[v] = True; // for循环结束后，再将顶点v加入集合S
        for(w=0; w<n; ++w) { // 遍历所有顶点（集合V-S上），更新从v0出发到集合V-S上的所有顶点的最短路径长度
            if(!S[w] && (min_ + G.arcs[v][w] < D[w])) { // 如果w不在集合S中，且从v到w的路径长度小于D[w]
                // min_=D[v], min_ + G.arcs[v][w]相当于v加入到S集合，作为中转点之后，查看(v0,v,w)是否比之前的(v0,w)路径更短
                D[w] = min_ + G.arcs[v][w]; // 更新D[w]
                Path[w] = v; // 更新Path[w]，也就是更新w的前驱为v
            }
        }
    }
    // 取最大值的索引
    int maxIndex = v0; // 初始化最大值索引为源点v0
    for (i = 0; i < n; i++) { // 遍历所有顶点, 找到最大值索引
        if (D[i] > D[maxIndex]) {
            maxIndex = i;
        }
    }
    return G.vexs[maxIndex]; // 返回最大值索引对应的顶点
}

// 4. 基于图的DFS写一个算法，判别以邻接表方式存储的有向图中是否存在由顶点v_i到顶点v_j的路径。(i!=j)
// 思路：DFS, 引入level记录递归的深度
// 参考习题：p135

int level = 1; // 记录递归的深度
bool visited[MVNUM] = {false}; // 访问标志数组，显式初始化所有元素为False
int PathDFS(ALGraph G, int i, int j) { // 判断从顶点i到顶点j是否存在路径
    // 从顶点i出发，判断是否存在路径到达顶点j
    if (i == j) return 1; // 递归结束条件：首尾相接，存在路径
    ArcNode *p; // 定义边结点指针p
    int k; // 定义顶点类型k
    visited[i] = True; // 标记顶点i为已访问
    for(p=G.vertices[i].firstarc; p; p=p->nextarc,--level) { 
        // 遍历顶点i的所有邻接点, level--是因为每次递归深度加1，而本for循环内的p都是在同一个level的，所以需要减1回到原来的level
        ++level; // 递归深度加1
        k = p->adjvex; // k为顶点i的邻接点
        if (!visited[k]&& PathDFS(G, k, j)) { // 如果k没有被访问过且从k到j存在路径
            return 1; // 存在路径
        }
    }
    if(level == 1) return 0; // 如果level为1，表示没有路径, 返回0（因为level=1是初始值）
    // 此时相当于遍历完了顶点i的所有邻接点去做DFS，但都没有找到路径，所以返回0
}

// 5. 采用邻接表存储结构，编写算法，判别无向图中任意给定的两个顶点之间是否存在一条长度为k的简单路径。
// 思路：DFS, 递归，记得回溯visited, 每调用一次算法k-1。因为使用了k-1所以b
// 参考习题：p135-p136

bool visited[MVNUM] = {false}; // 访问标志数组，显式初始化所有元素为False
int PathLenK(ALGraph G, int i, int j, int k) { // 判断从顶点i到顶点j是否存在长度为k的路径
    // 从顶点i出发，判断是否存在长度为k的路径到达顶点j
    if (i == j && k == 0) return 1; // 递归结束条件：首尾相接，且k长度为0，存在路径
    else if(k>0){
        ArcNode *p; // 定义边结点指针p
        int v; // 定义顶点类型v
        visited[i] = True; // 标记顶点i为已访问
        for(p=G.vertices[i].firstarc; p; p=p->nextarc) { 
            // 遍历顶点i的所有邻接点
            v = p->adjvex; // v为顶点i的邻接点
            if (!visited[v] && PathLenK(G, v, j, k-1)) { // 如果v没有被访问过且从v到j存在长度为k-1的路径
                return 1; // 存在路径
            }
        }
        visited[i] = False; // 回溯，标记顶点i为未访问，因为允许曾经被访问过的结点出现在另一条路径上
        // 可以看出在for循环内，也就是i的所有邻接点遍历期间，visited[i]是True的状态；当for循环结束后，也就是i的所有邻接点遍历结束，visited[i]重新变回False的状态
    }
    return 0; // 不存在路径。
}