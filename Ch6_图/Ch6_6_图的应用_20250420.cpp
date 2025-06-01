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
* 6.6 图的应用
 *==============================================================*/


 /* -------------- 6.6.1 最小生成树--------------- */

 /* ---------- Start of 从Ch6_4and5 导入图的存储结构---------- */
//  图的邻接矩阵存储表示 AMGraph
#define MAXINT 32767 // 最大整数值，用来表示极大值。
# define MAXVNUM 100 // 最大顶点数
typedef char VertexType; // 顶点类型, 假设为字符型
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
    OtherInfo weight; // 跟边相关的信息，比如权值
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
 /* ---------- End of 从Ch6_4and5 导入图的存储结构---------- */


// 【算法6.8】 Prim(普里姆）算法求最小生成树
// 参考视频：第11周04--6.6图的应用4-6.6.1最小生成树4--构造最小生成树1--Prim算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=129
// 参考课本：p163
// 时间复杂度：O(n^2)，n是顶点数。适合稠密图。
// 思路：贪心算法

// 构造辅助数组closedge，记录从U到V-U的最小权值边
typedef struct Closedge{
    VertexType adjvex; // 最小边在U中的顶点u
    ArcType lowcost; // 最小边的权值
} Closedge[MVNUM]; // 数组下标k为V-U的顶点在G中的位置,即v= G.vexs[k]；而closedge[k].adjvex是U中的顶点，即u = closedge[k].adjvex

// 辅助函数Min：找到closedge中lowcost最小的顶点索引。时间复杂度O(n)
int Min(Closedge closedge, int vexnum) {
    int minIndex = -1; // 初始化最小索引为-1
    ArcType minCost = MAXINT; // 初始化最小权值为极大值
    for (int i = 0; i < vexnum; ++i) { // 遍历closedge数组
        // 如果lowcost不为0且小于当前最小权值，则更新最小权值和索引
        if (closedge[i].lowcost != 0 && closedge[i].lowcost < minCost) {
            minCost = closedge[i].lowcost;
            minIndex = i;
        }
    }
    return minIndex;
}

// 正式开始
void MiniSpanTree_Prim(AMGraph G, VertexType u){
    // 无向网G以邻接矩阵存储，从顶点u出发，构造G的最小生成树T，输出T的各条边
    int i, j, k; // 定义变量i、j、k。
    VertexType u0, v0; // 定义变量u0、v0，表示最小边的两个顶点
    Closedge closedge; // 辅助数组，记录从U到V-U的最小权值边
    k = LocateVex(G, u); // 查找顶点u在图G中的位置
    // 初始，U={u}
    closedge[k].lowcost = 0; //  将顶点u的权值置为0，表示该顶点已加入U。
    // 之所以置0，是因为在网的邻接矩阵中，如果G.arcs[k][j] = MAXINT，表示第k个顶点到第j个顶点没有边，二者可以区分开来
    for(j=0; j<G.vexnum; ++j) { // 对于V-U(目前U只有一个顶点u）中的每个顶点v，初始化closedge[j]的adjvex为u， lowcost为u到v的边的权值，也就是G.arcs[k][j]
        if(j!= k) closedge[j] = {u, G.arcs[k][j]}; // 用集合赋值，相当于 closedge[j].adjvex = u; closedge[j].lowcost = G.arcs[k][j];
    }
    // 利用MST的性质，选择其余n-1个顶点，生成n-1条边（n=G.vexnum）。
    for(i=1; i<G.vexnum; ++i) { // 注意i从1而不是0开始，循环n-1次。
        // 选择其余n-1个顶点，生成n-1条边（n=G.vexnum）。
        k = Min(closedge, G.vexnum); // 此k非彼k。求出T的下一个结点：第k个顶点，closedge[k]中存有当前的最小边
        u0=closedge[k].adjvex; // u0为最小边的顶点，u0∈U。第一次循环的时候u0=u。
        v0=G.vexs[k]; // v0为最小边的另一个顶点，v0∈V-U
        cout<<u0<<"-->"<<v0<<endl; // 输出最小边
        closedge[k].lowcost = 0; // 将该边的权值置为0，表示第k个顶点v0已加入U
        for(j=0; j<G.vexnum; ++j) { // 新顶点（索引k）并入U后，重新选择最小边（遍历整个V，其中也就包含了V-U）
            if( G.arcs[k][j] < closedge[j].lowcost) { // 如果第k个顶点到第j个顶点的权值小于closedge[j]中的权值
                closedge[j] = {G.vexs[k], G.arcs[k][j]}; // 更新closedge[j]。注意v0=G.vexs[k]
            } 
            // 这里是通过closedge[k].lowcost = 0以及 判断 G.arcs[k][j] < closedge[j].lowcost来实现V-U的遍历的。
            // 因为U中的顶点必然满足closedge[k].lowcost = 0，所以G.arcs[k][j] < closedge[j].lowcost是不可能的，肯定不会更新closedge[j]
        }
    }   
}

// 【算法6.9】 Kruskal（克鲁斯卡尔）算法求最小生成树
// 参考视频：第11周05--6.6图的应用5-6.6.1最小生成树5--构造最小生成树2--Kruskal算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=130
// 参考课本：p169-p170
// 时间复杂度：O(e*log(e))，e是边数。适合稀疏图。

// 辅助数组Edge：存储边的信息，包括权值和两个顶点
#define MAXARCNUM 10000 // 最大边数（暂时设置为MAXVNUM的平方）
typedef struct Edge{
    VertexType Head, Tail; // 边的始点和终点
    ArcType lowcost; // 边的权值
} Edge[MAXARCNUM]; // 
// 辅助数组Vexset：标识各个顶点所属的连通分量。初始时Vexset[i] = i，表示每个顶点都各自成为一个连通分量
int Vexset[MAXVNUM]; // 辅助数组，存储每个顶点所在的集合
// 辅助函数Sort：对边集数组进行排序（升序）
void Sort(Edge edge, int arcnum){ //相当于冒泡排序，时间复杂度O(e²)。之后可以考虑替换成O(e*log(e))的排序算法比如第8章的堆排序
    Edge temp; // 边集数组的临时变量, 用于交换
    for (int i = 0; i < arcnum - 1; i++) { // 注意要-1（因为冒泡排序涉及到j和j+1的比较）
        for (int j = 0; j < arcnum - 1 - i; j++) { // 注意要-1-i
            if (edge[j].lowcost > edge[j + 1].lowcost) { // 升序排列
                // 交换边的权值
                temp[0] = edge[j];
                edge[j] = edge[j + 1];
                edge[j + 1] = temp[0];
            }
        }
    }
}

// 正式开始
void MinSpanTree_Kruskal(AMGraph G){
    // 无向网G以邻接矩阵存储，采用Kruskal算法求最小生成树T，输出T的各条边
    int i, j ; // 定义变量i、j
    int v1, v2, vs1, vs2; // 定义变量v1、v2、vs1、vs2，表示边的两个顶点和它们所在的连通分量
    Edge edge; // 边集数组
    int arc_count = 0; // 边数初始化为0
    for(i=0; i<G.vexnum; ++i) { // 遍历邻接矩阵, 对edge数组赋值。
        for(j=i; j<G.vexnum; ++j) { // 注意j从i到G.vexnum，相当于遍历上三角矩阵（对称矩阵），包含主对角线上的元素。
            // 这里是为了避免重复添加边，比如(1,2)和(2,1)是同一条边
            if(G.arcs[i][j] != MAXINT) { // 如果存在边(MAXINT针对网，0针对图)
                edge[arc_count].Head = G.vexs[i]; // 边的始点
                edge[arc_count].Tail = G.vexs[j]; // 边的终点
                edge[arc_count].lowcost = G.arcs[i][j]; // 边的权值
                arc_count++; // 边数加1
            }
        }
    }
    Sort(edge, G.arcnum); // 边集按权值升序排列
    for(i = 0; i < G.vexnum; ++i)  Vexset[i] = i; // 初始化Vexset数组，每个顶点都各自成为一个连通分量
    for(i=0; i<G.arcnum; ++i) { // 遍历边集数组
        v1 = LocateVex(G, edge[i].Head); // v1为边的始点Head在图G中的位置
        v2 = LocateVex(G, edge[i].Tail); // v2为边的终点Tail在图G中的位置
        vs1 = Vexset[v1]; // vs1为边edge[i]的始点v1所在的连通分量
        vs2 = Vexset[v2]; // vs2为边edge[i]的终点v2所在的连通分量
        if(vs1 != vs2) { // 如果vs1和vs2不相等，表示edge[i]的两个顶点不在同一个连通分量。否则会形成回路，跳过。
            cout<<edge[i].Head<<"-->"<<edge[i].Tail<<endl; // 输出最小边
            for(j=0; j<G.vexnum; ++j) { // 合并连通分量：将vs2所在的连通分量并入vs1所在的连通分量
                if(Vexset[j] == vs2) Vexset[j] = vs1; 
            }
        }
    }
}


 /* -------------- 6.6.2 最短路径--------------- */
// 【算法6.10】 Dijkstra（迪杰斯特拉）算法求单源最短路径
// 参考视频：第11周07--6.6图的应用6-6.6.2最短路径2--Dijkstra算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=132
// 参考课本：p173-p174
// 时间复杂度：O(n²)，n是顶点数。适合稠密图。

// 辅助数组S：存储已知最短路径的顶点。相当于bool flag数组，表示顶点是否在集合S中。
int S[MAXVNUM]; // S[i] = True表示第i个顶点在集合S中，False表示尚未确定
// 辅助数组Path：存储最短路径的前驱顶点。相当于记录弧尾点的数组。
int Path[MAXVNUM]; // Path[i]表示第i个顶点的前驱顶点。如果Path[i] = -1，表示第i个顶点没有前驱顶点，即不存在从源点到第i个顶点的路径；如果Path[i] = v0，表示第i个顶点的前驱顶点是源点v0。
// 辅助数组D：存储从源点到各个顶点的最短路径。
int D[MAXVNUM]; // D[i]表示从源点到第i个顶点的最短路径。初值为：如果有弧，则为弧上的权值；如果没有弧，则为极大值MAXINT（相当于正无穷大）。

// 正式开始
void ShortestPath_DIJ(AMGraph G, int v0){
    // 用Dijkstra算法求有向网G从顶点v0到其余各顶点的最短路径。用到3个辅助数组S、Path、D
    int i,v,w,min_; // 定义变量i,v,w,min_
    int n = G.vexnum; // 顶点数
    for(v = 0; v < n; ++v){ // 对于所有顶点，初始化S、Path、D数组
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
    // // 输出最短路径
    // for(i=0; i<n; ++i) { // 遍历所有顶点
    //     cout<<v0<<"到"<<i<<"的最短路径长度为："<<D[i]<<endl; // 输出最短路径长度
    //     cout<<v0<<"到"<<i<<"的最短路径为："; // 输出最短路径
    //     w = i; // 从终点开始
    //     while(w != v0) { // 如果w不是源点，则输出前驱顶点
    //         cout<<w<<"<-"; // 输出前驱顶点
    //         w = Path[w]; // 更新w为前驱顶点
    //     }
    //     if(w == -1) cout<<"不存在"<<endl; // 如果w为-1，表示不存在前驱顶点
    //     else cout<<v0<<endl; // 输出源点
    // }
}

// 【算法6.11】 Floyd（弗洛伊德）算法求每一对顶点间的最短路径
// 参考视频：第11周08--6.6图的应用7-6.6.2最短路径3--Floyd算法 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=133
// 参考课本：p174-p175
// 时间复杂度：O(n³)，n是顶点数。
//  求每一对顶点间的最短路径还可以调用n次Dijkstra算法，时间复杂度O(n²*n)=O(n³)，与Floyd算法相同。

int D_matrix[MAXVNUM][MAXVNUM]; // 辅助数组D，存储任意两顶点间的最短路径长度
int Path_matrix[MAXVNUM][MAXVNUM]; // 辅助数组Path，存储最短路径的前驱顶点

// 正式开始
void ShortestPath_Floyd(AMGraph G){
    // 用Floyd算法求有向网G中每一对顶点间的最短路径。用到2个二维数组D_matrix和Path_matrix
    int i, j, k; // 定义变量i、j、k
    int n = G.vexnum; // 顶点数
    for(i=0; i<n; ++i)// 初始化D数组，将D[i][j]初始化为G.arcs[i][j]
        for(j=0; j<n; ++j) {
            D_matrix[i][j] = G.arcs[i][j]; // D[i][j]表示从顶点i到顶点j的最短路径长度
            if(D_matrix[i][j] < MAXINT &&i!=j) Path_matrix[i][j] = i; // 如果i到j之间有弧，则Path[i][j]初始化为i，表示j的前驱顶点是i
            // 注意&&i!=j，表示不考虑自环边
            else Path_matrix[i][j] = -1; // 如果i到j之间没有弧，则Path[i][j]初始化为-1，即j的前驱顶点不存在
        }
    for(k=0; k<n; ++k)  // 遍历所有顶点k，作为中转点
        for(i=0; i<n; ++i)  // 遍历所有顶点i，作为起点
            for(j=0; j<n; ++j) // 遍历所有顶点j，作为终点
                if(D_matrix[i][k] + D_matrix[k][j] < D_matrix[i][j]) { // 如果从i到k再到j的路径长度小于从i到j的路径长度，则更新D[i][j]
                    D_matrix[i][j] = D_matrix[i][k] + D_matrix[k][j]; // 更新D[i][j]
                    Path_matrix[i][j] = k; // 更新Path[i][j]，也就是更新i到j的前驱为k
                }
    // // 输出最短路径
    // for(i=0; i<n; ++i) { // 遍历所有顶点
    //     for(j=0; j<n; ++j) { // 遍历所有顶点
    //         cout<<i<<"到"<<j<<"的最短路径长度为："<<D_matrix[i][j]<<endl; // 输出最短路径长度
    //         cout<<i<<"到"<<j<<"的最短路径为："; // 输出最短路径
    //         int w = j; // 从终点开始
    //         while(w != i) { // 如果w不是源点，则输出前驱顶点
    //             cout<<w<<"<-"; // 输出前驱顶点
    //             w = Path_matrix[i][w]; // 更新w为前驱顶点
    //         }
    //         if (w == -1) cout<<"不存在"<<endl; // 如果w为-1，表示不存在前驱顶点
    //         else cout<<i<<endl; // 输出源点
    //     }
    //     cout << endl;
    // }
}


 /* -------------- 6.6.3 拓扑排序（DAG(有向无环图)中的AOV-网,无权) --------------- */

// 【算法6.12】 拓扑排序
// 参考视频：第11周10--6.6图的应用9-6.6.3拓扑排序 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=134
// 参考课本：p178
// 时间复杂度：O(n+e)，n是顶点数，e是边数。

// 辅助数组indegree：存储各个顶点的入度
int indegree[MAXVNUM]; // 辅助数组，存储各个顶点的入度
// 辅助栈S_toposort：暂时存储所有入度为0的顶点

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
int GetTop(SqStack S, int &e){ // 获取栈顶元素。int本质上是SElemType
    if (S.top == S.base) 
        return ERROR; // 栈空
    e = *(S.top - 1); // 栈顶元素的值
    return OK;
}

SqStack S_toposort; // 定义栈S_toposort

void FindInDegree(ALGraph G, int indegree[]){ // 因为indegree是数组，所以不用加&符号
    // 求有向图G的各个顶点的入度
    int i, j; // 定义变量i、j
    for(i=0; i<G.vexnum; ++i) { // 遍历所有顶点
        indegree[i] = 0; // 初始化入度为0
    }
    for(i=0; i<G.vexnum; ++i) { // 遍历邻接表
        ArcNode *p = G.vertices[i].firstarc; // p指向第i个顶点的第一条边
        while(p) { // 遍历所有边
            j = p->adjvex; // p的邻接点为j
            indegree[j]++; // j的入度加1
            p = p->nextarc; // p指向顶点i的下一条边
        }
    }
}

// 正式开始
Status TopologicalSort(ALGraph G,int topo[]){
    // 用拓扑排序算法对有向无环图G进行拓扑排序，返回拓扑序列。用到的数据结构是图的邻接表。
    // topo[]是拓扑序列的存储数组
    int i, k, count = 0; // 定义变量i、j、k、count.对输出顶点计数，初始为0
    ArcNode *p; // 定义指针p，指向邻接表中的边
    InitStack(S_toposort); // 初始化栈S_toposort（存储入度为0的顶点的下标）
    FindInDegree(G, indegree); // 求入度数组indegree（存储各个顶点的入度）
    for(i=0; i<G.vexnum; ++i) 
        if(!indegree[i]) Push(S_toposort, i); // 将入度为0的顶点的下标入栈
    while(!StackEmpty(S_toposort)) { // 栈不空
        Pop(S_toposort, i); // 出栈, vi为出栈顶点
        topo[count++] = i; // 将出栈顶点vi存入拓扑序列, 并且计数加1
        p = G.vertices[i].firstarc; // p指向第i个顶点vi的第一个邻接点
        while(p){ // 遍历vi所有邻接点
            k = p->adjvex; // vi（p)的邻接点设为vk
            --indegree[k]; // vi的所有邻接点的入度减1
            if(!indegree[k]) Push(S_toposort, k); // 如果入度为0，则k入栈
            p = p->nextarc; // p指向vi的下一个邻接点vk
        }
    }
    if (count < G.vexnum) return ERROR; // 如果输出顶点数小于顶点数，则有回路
    else return OK; // 否则返回成功
}


 /* -------------- 6.6.4 关键路径(DAG(有向无环图)中的AOV-网,有权) --------------- */
// 【算法6.13】 关键路径
// 参考视频：第11周11--6.6图的应用10-6.6.4关键路径 https://www.bilibili.com/video/BV1nJ411V7bd?vd_source=b53bd15b89ae068fd295b2d36a386a97&spm_id_from=333.788.player.switch&p=135
// 参考课本：p182-p183
// 时间复杂度：O(n+e)，n是顶点数，e是边数。
// 思路：用到拓扑排序和逆拓扑排序。

// 辅助数组ve、vl：存储各个顶点的最早发生时间和最迟发生时间
int ve[MAXVNUM]; // ve[i]表示第i个顶点的最早发生时间
int vl[MAXVNUM]; // vl[i]表示第i个顶点的最迟发生时间
// 辅助数组topo：存储拓扑序列

Status CriticalPath(ALGraph G){
    // 求有向网G的关键路径。用到的数据结构是图的邻接表。
    int topo[MAXVNUM]; // topo[i]表示第i个顶点在拓扑序列中的位置
    if(!TopologicalSort(G, topo)) return ERROR; // 拓扑排序失败，返回错误
    // 通过拓扑排序得到的拓扑序列topo，如果有回路，则返回错误

    int n = G.vexnum; // 顶点数
    int i, j, k, e, l; // 定义变量i、j、k、e、l。
    ArcNode *p; // 定义指针p，指向邻接表中的边
    for(int i=0; i<n; ++i) { // 初始化ve数组所有元素都为0，顺便实现ve[0] = 0
        ve[i] = 0; // 相当于初始化最大值为0。
    }
    /*------按照拓扑排序，求每个事件的最早发生时间ve------*/
    for(i=0; i<n; ++i) {
        k = topo[i]; // k为拓扑序列中的第i个顶点
        p= G.vertices[k].firstarc; // p指向第k个顶点的第一条边
        while(p) { // 依次更新k的所有邻接点的最早发生时间ve
            j = p->adjvex; // vk的邻接点为vj
            if(ve[j] < ve[k] + p->weight) // 更新ve[j]，如果ve[j]小于ve[k] + p->weight。p->weight是边<vk,vj>的权值
                ve[j] = ve[k] + p->weight; 
            p = p->nextarc; // 指向k的下一个邻接点
        }
    }
    for(i=0; i<n; ++i) { // 初始化vl数组所有元素为ve[n-1]，顺便实现vl[n-1] = ve[n-1]
        vl[i] = ve[n-1]; // 相当于初始化最小值为ve[n-1]
    }
    /*------按照逆拓扑排序，求每个事件的最迟发生时间vl（格式非常类似拓扑排序更新ve）------*/
    for(i = n - 1; i >= 0; --i) { // 注意i从n-1到0, 逆拓扑排序
        k = topo[i];
        p = G.vertices[k].firstarc;
        while(p) { // 依次更新k的所有邻接点的最迟发生时间vl
            j = p->adjvex; // vk的邻接点为vj
            if(vl[k] > vl[j] - p->weight) // 更新vl[k]，如果vl[k]大于vl[j] - p->weight。p->weight是边<vk,vj>的权值
                vl[k] = vl[j] - p->weight; 
            p = p->nextarc; // 指向k的下一个邻接点
        }
    }
    /*------判断每一活动是否为关键活动（关键路径上的活动称为关键活动）------*/
    // 求出e(i)和l(i)，如果e(i) = l(i)，则该活动为关键活动
    for(i = 0; i < n; ++i){
        p= G.vertices[i].firstarc; // p指向第i个顶点的第一条边
        while(p) { // 遍历第i个顶点的所有邻接点
            j = p->adjvex; // p的邻接点为j
            e = ve[i]; // e为活动<vi,vj>的最早发生时间,即课本上的e(i) = ve(j), 其中a_i=<v_j,v_i>
            l = vl[j] - p->weight; // l为活动<vi,vj>的最迟发生时间,即课本上的l(i) = vl(k) - w_j,k, 其中a_i=<v_j,v_i>
            if(e == l) { // 如果e等于l，则说明该活动为关键活动
                cout<<G.vertices[i].data<<"-->"<<G.vertices[j].data<<endl; // 输出关键活动
            }
            p = p->nextarc; // p指向第i个顶点的下一个邻接点
        }
    }
}


/*==============================================================
* 6.7 案例分析与实现
 *==============================================================*/

//  【算法6.14】 六度空间理论的验证(用BFS，也可以用Dijkstra或者Floyd算法)
// 参考课本：p185
void SixDegree_BFS(Graph G, int Start){
    // 验证六度空间理论,通过BFS算法求出从Start出发的最短路径
    int Visit_num = 0;
    int i,len,u;
    int visited[MAXVNUM]; // 访问标记数组
    int level[6]; // 层数数组, 最多6层
    visited[Start] = True; // 将起始顶点标记为已访问
    SqQueue Q; // 定义队列Q
    InitQueue(Q); // 初始化队列Q
    EnQueue(Q, Start); // 将起始顶点入队
    level[0] = 1; // 初始化level[0]为1，表示第0层的顶点数为1
    for(len = 1;len<6&&!QueueEmpty(Q); ++len) { // 遍历6层
        for(i = 0; i < level[len - 1]; ++i) { // 遍历第len-1层的所有顶点
            DeQueue(Q, u); // 出队
            for(w=FirstAdjVex(G, u); w>=0; w=NextAdjVex(G, u, w)) { // 遍历u的所有邻接点
                if(!visited[w]) { // 如果w未被访问
                    visited[w] = True; // 标记为已访问
                    EnQueue(Q, w); // 入队
                    Visit_num++;level[len]++; // 访问顶点数加1，第len层的顶点数加1
                }
            }
        }
    }
    cout<<100*Visit_num/G.vexnum<<endl; // 计算访问顶点数占总顶点数的百分比
}
