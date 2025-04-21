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
* 6.4 图的存储结构
 *==============================================================*/


 /* -------------- 6.4.1 邻接矩阵--------------- */

// 1. 图的邻接矩阵存储表示
// 参考视频：第10周03--6.4图的存储结构3-2采用邻接矩阵表示法创建无向网 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=114
// 参考课本：p154
#define MAXINT 32767 // 最大整数值，用来表示极大值。
// 2^15-1=32767，因为1个字节有8个bit，2个字节有16个bit，32767就相当于 signed short int类型的最大值(2个字节，其中1个bit用来表示符号位)。
# define MAXVNUM 100 // 最大顶点数
typedef char VertexType; // 顶点类型, 假设为字符型
typedef int ArcType; // 边的权值类型, 假设为整型
typedef struct AMGraph{ // 邻接矩阵类型
    VertexType vexs[MAXVNUM]; // 顶点表(一维数组)
    ArcType arcs[MAXVNUM][MAXVNUM]; // 邻接矩阵（二维数组）
    int vexnum, arcnum; // 图的当前顶点数和边数
} AMGraph; // AMGraph是图的邻接矩阵类型，表示图的存储结构。
// AMGraph是一个结构体，包含了顶点表、邻接矩阵、顶点数和边数。AM = Adjacency Matrix

// 【算法6.1】采用邻接矩阵表示法创建无向网
// 参考视频：第10周03--6.4图的存储结构3-2采用邻接矩阵表示法创建无向网 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=114
// 参考课本：p155
// 时间复杂度：O(n^2)。
int LocateVex(AMGraph G, VertexType u) { // 查找顶点u在图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.vexs[i] == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}

Status CreatUDN(AMGraph &G) {
    // 采用邻接矩阵表示法，创建无向网。UDN = Undirected Network
    int i,j,k; // 定义变量i、j、k。
    ArcType w; // 定义边的权值变量w，目前是int类型
    char v1, v2; // 定义字符变量v1、v2
    cin >> G.vexnum >> G.arcnum; // 输入顶点的总数和边的总数
    for(i =0; i<G.vexnum; ++i) { // 输入每个顶点值
        cin >> G.vexs[i]; 
    }
    for(i = 0; i<G.vexnum; ++i) { // 初始化邻接矩阵 vexnum * vexnum, 边的权值初始化都为MAXINT（极大值）
        for(j = 0; j<G.vexnum; ++j) {
            G.arcs[i][j] = MAXINT;   // 如果是无向图，则改成 G.arcs[i][j]  = 0
        }
    }
    for(int k = 0; k<G.arcnum; ++k){ // 在邻接矩阵中输入边的权值
        cin >> v1 >> v2 >> w; // 输入边的两个顶点和权值
        i = LocateVex(G, v1); j = LocateVex(G, v2); // 查找顶点在顶点表中的位置
        G.arcs[i][j] = w; // 邻接矩阵赋值，<v1,v2>的权值置为w
        // 如果是无向图，则令w = 1作为常数即可，也不用cin>>w。
        G.arcs[j][i] = G.arcs[i][j]; // 无向图或者无向网的邻接矩阵是对称的。如果是有向图或者有向网，则不需要这一步。
    }
}

/* -------------- 6.4.2 邻接表--------------- */


// 1. 图的邻接表存储表示
// 参考视频：第10周07--6.4图的存储结构7-邻接表表示法3-建立邻接表的算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=118
// 参考课本：p156-p157
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

// 【算法6.2】采用邻接表表示法创建无向图
// 参考视频：第10周07--6.4图的存储结构7-邻接表表示法3-建立邻接表的算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=118
// 参考课本：p157
// 时间复杂度：O(n+e)，n是顶点数，e是边数。
int LocateVex_ALG(ALGraph G, VertexType u) { // 查找顶点u在ALGraph图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.vertices[i].data == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}

Status CreatUDG(ALGraph &G) { // 采用邻接表表示法，创建无向图
    cin>>G.vexnum>>G.arcnum; // 输入顶点的总数和边的总数
    char v1, v2; // 定义字符变量v1、v2
    ArcNode *p1,*p2; // 定义边结点指针p
    int i, j,k ; // 定义下标变量i、j、k
    for(int i=0; i<G.vexnum; ++i) { // 输入每个顶点值
        cin>>G.vertices[i].data; // 输入顶点值
        G.vertices[i].firstarc = NULL; // 初始化表头结点的指针域为空
    }
    for(k=0; k<G.arcnum; ++k) { // 在邻接表中输入边
        cin >> v1 >> v2; // 输入边的两个顶点
        i = LocateVex_ALG(G, v1); j = LocateVex_ALG(G, v2); // 查找v1 v2顶点在G中的位置作为i和j
        // 将新结点p1插入到顶点i的链表中（相当于链表头插法）！！！
        p1 = new ArcNode; // 创建新结点p1
        p1->adjvex = j; // 邻接点序号为j
        p1->nextarc = G.vertices[i].firstarc; // 指向第一条依附该顶点的边的指针
        G.vertices[i].firstarc = p1; // 将新结点p1插入到顶点i的链表中
        // 将新结点p2插入到顶点j的链表中（相当于链表头插法）！！！。
        // 如果不是无向图，而是有向图，则不需要p2，只需要p1即可。
        p2 = new ArcNode; // 创建新结点p2
        p2->adjvex = i; // 邻接点序号为i
        p2->nextarc = G.vertices[j].firstarc; // 指向第一条依附该顶点的边的指针
        G.vertices[j].firstarc = p2; // 将新结点p2插入到顶点j的链表中
        // 如果不是无向图，而是无向网，则额外添加一行代码，输入边的权值，比如:
        // cin>>v1>>v2>>w; p1.info = w; p2.info = w;
        // 如果不是无向图，而是有向网，则不需要p2，只需要p1即可。
        // cin>>v1>>v2>>w; p1.info = w;
    }
    return OK; // 返回成功
    // 注意：p1 = new ArcNode;的必要性：如果不new，当函数返回时，p1 被销毁，G.vertices[i].firstarc 指向的内存变为无效。
}

/* -------------- 6.4.3 十字链表（有向图）--------------- */
// 1. 图的十字链表存储表示
// 参考视频：第10周09--6.4图的存储结构9-6.4.3十字链表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=120
// 参考课本：p159
#define MAX_VERTEX_NUM 20 // 最大顶点数
typedef struct ArcBox{ // 十字链表 的 边结点类型
    int tailvex, headvex; // 弧头和弧尾在顶点表中的位置
    struct ArcBox *hlink, *tlink; // 弧头和弧尾的下一条弧的指针
    OtherInfo info; // 其他信息，比如权值
} ArcBox; // ArcBox是十字链表的边结点类型
typedef struct VexNode{ // 十字链表的顶点结点类型
    VertexType data; // 顶点信息域，存储顶点的信息
    ArcBox *firstin, *firstout; // 指向第一条依附该顶点的边的指针
} VexNode; // VexNode是十字链表的顶点结点类型
typedef struct OLGraph{ // 图的十字链表类型
    VexNode xlist[MAX_VERTEX_NUM]; // 表头向量
    int vexnum, arcnum; // 有向图的当前顶点数和边数
} OLGraph; // OLGraph是图的十字链表类型，表示图的存储结构。OL = Orthogonal List

// 【补充算法1】采用十字链表表示法创建有向图
// 时间复杂度：O(n+e)，n是顶点数，e是边数。
int LocateVex_OLG(OLGraph G, VertexType u) { // 查找顶点u在OLGraph图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.xlist[i].data == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}
Status CreateDG(OLGraph &G) { // 采用十字链表表示法，创建有向图
    cin>>G.vexnum>>G.arcnum; // 输入顶点的总数和边的总数
    char v1, v2; // 定义字符变量v1、v2
    ArcBox *p; // 定义边结点指针p
    int i, j, k; // 定义下标变量i、j、k
    for(int i=0; i<G.vexnum; ++i) { // 输入每个顶点值
        cin>>G.xlist[i].data; // 输入顶点值
        G.xlist[i].firstin = NULL; // 初始化表头结点的指针域
        G.xlist[i].firstout = NULL; // 初始化表头结点的指针域
    }
    for(k=0; k<G.arcnum; ++k) { // 在邻接表中输入边
        cin >> v1 >> v2; // 输入边的两个顶点，有向图是<v1,v2>,v1是弧尾，v2是弧头
        i = LocateVex_OLG(G, v1); j = LocateVex_OLG(G, v2); // 查找v1 v2顶点在G中的位置作为i和j
        p = new ArcBox; // 创建新结点p
        p->tailvex = i; p->headvex = j; // 弧尾（v1）和弧头（v2）在顶点表中的位置
        p->tlink = G.xlist[i].firstout; // 将弧尾（v1）插入到顶点i的链表的出度中（相当于链表头插法）！！！
        G.xlist[i].firstout = p; 
        p->hlink = G.xlist[j].firstin; // 将弧头（v2）插入到顶点j的链表的入度中（相当于链表头插法）！！！
        G.xlist[j].firstin = p; 
        // 如果是有向网，则额外添加一行代码，输入边的权值，比如:
        // cin>>v1>>v2>>w; p.info = w;
    }
    return OK; // 返回成功
}

/* -------------- 6.4.4 邻接多重表（无向图）--------------- */
// 1. 图的多重链表存储表示
// 参考视频：第10周10--6.4图的存储结构10-6.4.4邻接多重表 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=121
// 参考课本：p161
#define MAX_VERTEX_NUM 20 // 最大顶点数
typedef int InfoType; // 边的权值类型, 假设为整型
typedef int VisitIf; // 访问标志类型，假设为整型,一般是布尔类型
typedef struct EBox{
    VisitIf mark; // 标志域，表示该边是否被访问过
    int ivex, jvex; // 边的两个顶点在顶点表中的位置
    struct EBox *ilink, *jlink; // 弧头和弧尾的下一条弧的指针
    InfoType *info; // 其他信息，比如权值
} EBox; // EBox是邻接多重表的边结点类型
typedef struct VexBox {
    VertexType data; // 顶点信息域，存储顶点的信息
    EBox *firstedge; // 指向第一条依附该顶点的边的指针
} VexBox; // VexBox是邻接多重表的顶点结点类型
typedef struct AMLGraph{ // 图的邻接多重表类型
    VexBox xlist[MAX_VERTEX_NUM]; // 表头向量
    int vexnum, arcnum; // 无向图的当前顶点数和边数
} AMLGraph; // AMLGraph是图的邻接多重表类型，表示图的存储结构。AML = Adjacency Multi List
// 【补充算法2】采用邻接多重表表示法创建无向图
// 时间复杂度：O(n+e)，n是顶点数，e是边数。
int LocateVex_AML(AMLGraph G, VertexType u) { // 查找顶点u在AMLGraph图G中的位置
    for (int i=0; i<G.vexnum; i++) { // 遍历顶点表
        if (G.xlist[i].data == u) return i; // 找到顶点v，返回其下标
    }
    return -1; // 没有找到，返回-1
}
Status CreateUDG_AML(AMLGraph &G) { // 采用邻接多重表表示法，创建无向图
    cin>>G.vexnum>>G.arcnum; // 输入顶点的总数和边的总数
    char v1, v2; // 定义字符变量v1、v2
    EBox *p; // 定义边结点指针p
    int i, j, k; // 定义下标变量i、j、k
    for(int i=0; i<G.vexnum; ++i) { // 输入每个顶点值
        cin>>G.xlist[i].data; // 输入顶点值
        G.xlist[i].firstedge = NULL; // 初始化表头结点的指针域
    }
    for(k=0; k<G.arcnum; ++k) { // 在邻接表中输入边
        cin >> v1 >> v2; // 输入边的两个顶点
        i = LocateVex_AML(G, v1); j = LocateVex_AML(G, v2); // 查找v1 v2顶点在G中的位置作为i和j
        p = new EBox; // 创建新结点p
        p->ivex = i; p->jvex = j; // 边的两个顶点在顶点表中的位置
        p->ilink = G.xlist[i].firstedge; // 将弧尾（v1）插入到顶点i的链表中（相当于链表头插法）！！！
        G.xlist[i].firstedge = p;
        p->jlink = G.xlist[j].firstedge; // 将弧头（v2）插入到顶点j的链表中（相当于链表头插法）！！！
        G.xlist[j].firstedge = p;
    }
    return OK; // 返回成功
}



/*==============================================================
* 6.5 图的遍历
 *==============================================================*/

 /* -------------- 6.5.1 深度优先搜索--------------- */
// 【算法6.3】深度优先搜索遍历连通图
// 参考课本：p162
bool visited[MVNUM] = {false}; // 访问标志数组，显式初始化所有元素为False
void DFS(Graph G, int v) { // 深度优先搜索遍历连通图
    // 从第v个顶点出发递归地深度优先遍历图G
    cout<<v;visited[v] = True; // 访问第v个顶点，并将其标记为已访问
    for (int w=FirstAdjVex(G, v); w>=0; w=NextAdjVex(G, v, w)) { 
        // 依次检查v的所有邻接点w，FirstAdjVex(G, v)表示v的第一个邻接点
        // NextAdjVex(G, v, w)表示v相对于w的下一个邻接点，w>=0表示存在邻接点。
        if (!visited[w]) { // 如果w没有被访问过，则递归地深度优先遍历图G
            DFS(G, w); 
        }
    }
}

// 【算法6.4】深度优先搜索遍历非连通图
// 参考课本：p162-p163
void DFSTraverse(Graph G) { // 深度优先搜索遍历非连通图
    for (int v=0; v<G.vexnum; v++) { // 遍历所有顶点
        visited[v] = False; // 初始化访问标志数组
    }
    for (int v=0; v<G.vexnum; v++) { // 遍历所有顶点
        if (!visited[v]) { // 如果v没有被访问过，则递归地深度优先遍历图G
            DFS(G, v); 
        }
    }
}

// 【算法6.5】采用邻接矩阵表示图的深度优先搜索遍历
// 参考视频：第10周12--6.图的遍历2-深度优先搜索遍历实现--邻接矩阵上的遍历算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=123
// 参考课本：p163
// 时间复杂度：O(n^2)，n是顶点数；空间复杂度：O(n)
void DFS_AM(AMGraph G, int v) { // 深度优先搜索遍历连通图
    // 从第v个顶点出发递归地深度优先遍历图G
    cout<<v;visited[v] = True; // 访问第v个顶点，并将其标记为已访问
    for (int w=0; w<G.vexnum; w++) { // 遍历所有顶点
        if (G.arcs[v][w] != 0 && !visited[w]) { // 如果v和w之间有边且w没有被访问过，则递归地深度优先遍历图G
            DFS_AM(G, w); 
        }
    }
}

// 【算法6.6】采用邻接表表示图的深度优先搜索遍历
// 参考课本：p163
// 时间复杂度：O(n+e)，n是顶点数，e是边数；空间复杂度：O(n)
void DFS_AL(ALGraph G, int v) { // 深度优先搜索遍历连通图
    // 从第v个顶点出发递归地深度优先遍历图G
    cout<<v;visited[v] = True; // 访问第v个顶点，并将其标记为已访问
    ArcNode *p = G.vertices[v].firstarc; // 指向第一条依附该顶点的边的指针
    while (p != NULL) { // 遍历所有邻接点
        int w = p->adjvex; // 邻接点序号,即p->adjvex,表示w是v的邻接点
        if (!w) { // 如果w没有被访问过，则递归地深度优先遍历图G
            DFS_AL(G, w); 
        }
        p = p->nextarc; // p指向下一条依附该顶点的边的指针
    }
}

 /* -------------- 6.5.2 广度优先搜索--------------- */
// 【算法6.7】广度优先搜索遍历连通图
// 参考视频：第10周14--6.图的遍历4-广度优先搜索遍历实现 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=125
// 参考课本：p164-p165

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

// 正式开始
void BFS(Graph G,int v){
    cout<<v;visited[v] = True; // 访问第v个顶点，并将其标记为已访问
    SqQueue Q; // 定义队列Q
    InitQueue(Q); // 初始化队列Q
    EnQueue(Q, v); // 将第v个顶点入队
    int u; // 定义变量u，用来保存出队的顶点
    while(!QueueEmpty(Q)){ // 当队列不空时
        DeQueue(Q, u); // 出队一个顶点,并将其赋值给u
        for(int w=FirstAdjVex(G, u); w>=0; w=NextAdjVex(G, u, w)){ // 依次检查v的所有邻接点w
            if(!visited[w]){ // 如果w没有被访问过，则访问w
                cout<<w;visited[w] = True; // 访问第w个顶点，并将其标记为已访问
                EnQueue(Q, w); // 将第w个顶点入队
            }
        }
    }
}
// 【补充算法3】广度优先搜索遍历非连通图（只需要再DFSTraverse函数中将DFS改成BFS即可）
void BFSTraverse(Graph G) { // 广度优先搜索遍历非连通图
    for (int v=0; v<G.vexnum; v++) { // 遍历所有顶点
        visited[v] = False; // 初始化访问标志数组
    }
    for (int v=0; v<G.vexnum; v++) { // 遍历所有顶点
        if (!visited[v]) { // 如果v没有被访问过，则递归地广度优先遍历图G
            BFS(G, v); 
        }
    }
}
