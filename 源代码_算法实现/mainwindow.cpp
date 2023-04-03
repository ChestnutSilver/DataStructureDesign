#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QStack>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //dataProcess();
}

MainWindow::~MainWindow()
{
    delete ui;
}

typedef int Status;
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAX_VERTEX_NUM 10010 //最大顶点数
#define MAXQSIZE 10010
typedef int VertexType;
typedef int AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];   //邻接矩阵类型


int MainWindow::LocateVex(ALGraph G, int u)
{
    int i;
    for (i = 0; i < G.vexnum; i++)
    {
        if (u == G.vertices[i].data)
            return i;
    }
    if (i == G.vexnum)
    {
        return -1;
    }
    return 0;
}

void MainWindow::CreateALGraph(ALGraph& G)
{
    int i, j, k, w;
    int v1, v2;
    ArcNode* p, * p2;
    //G.vexnum=3;
    //G.arcnum=2;       //输入顶点数、边数
    for (i = 0; i < G.vexnum; i++)     //输入头结点（顶点）
    {
        G.vertices[i].data = i;        //输入顶点值
        G.vertices[i].firstarc = NULL; //初始化表头结点的指针域
    }

    //int lis[4]={0,1,0,2};
    int place=0;

    for (k = 0; k < G.arcnum; k++)     //输入各边，构造邻接表
    {
        //cin >> v1 >> v2;               //输入一条边依附的两个顶点
        i = LocateVex(G, strList2.at(place).toInt());
        place++;
        j = LocateVex(G, strList2.at(place).toInt());
        place++;

        p = (ArcNode*)malloc(sizeof(ArcNode));  //生成一个新的边结点*p
        p->adjvex = j;                  //邻接点序号为j
        p->nextarc = NULL;
        p2 = G.vertices[i].firstarc;    //【尾插法】插入邻接表
        if (p2 == NULL)                 //第FPS个顶点的边表结点为空
            G.vertices[i].firstarc = p; //实现两点相连
        else
        {
            while (p2->nextarc != NULL)
                p2 = p2->nextarc;
            p2->nextarc = p;
        }

        p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = i;
        p->nextarc = NULL;
        p2 = G.vertices[j].firstarc;     //【尾插法】插入邻接表
        if (p2 == NULL)                  //第FPS个顶点的边表结点为空
            G.vertices[j].firstarc = p;  //实现两点相连
        else
        {
            while (p2->nextarc != NULL)
                p2 = p2->nextarc;
            p2->nextarc = p;
        }
    }
    return;
}

void MainWindow::PrintALGraph(ALGraph G)
{
    QString AlGraph="";
    int i;
    for (i = 0; i < G.vexnum; i++)
    {
        ArcNode* p = new ArcNode;
        AlGraph += QString::number(G.vertices[i].data);
        AlGraph += "-->";

        for (p = G.vertices[i].firstarc; p; p = p->nextarc)
        {
            AlGraph += QString::number(p->adjvex);
            AlGraph += " ";
        }
        if (p == NULL)
            AlGraph += "\n";
    }

    ui->label3->setText(AlGraph);
    ui->label3->setWordWrap(true);
    QFont ft3;
    ft3.setPointSize(12);
    ui->label3->setFont(ft3);
}

void MainWindow::DFS(ALGraph G, int v)     //从第v个顶点出发DFS
{
    ArcNode* p;
    dfsResult += QString::number(G.vertices[v].data);    //打印当前访问的结点
    visited[v] = 1;                //访问过该结点后，visited立刻标记为1
    stackView+="顶点入栈";
    stackView+="\n";
    p = G.vertices[v].firstarc;
    while (p)
    {
        if (!visited[p->adjvex])
        {
            dfsResult += " ";
            stackView+="顶点入栈";
            stackView+="\n";
            DFS(G, p->adjvex);     //如果该结点还有未访问的相邻结点，就去DFS该结点
                                   /* 注：这是在递归调用DFS；当一个结点所有相邻结点都被访问过时，上一次的DFS就会继续开始执行，相当于“回退” */
        }
        stackView+="栈顶元素出栈";
        stackView+="\n";
        p = p->nextarc;
    }

}
void MainWindow::DFSTraverse(ALGraph G)       /* 注：对于连通图，从一个顶点出发，调用DFS函数即可将所有顶点都遍历到；对于非连通图，需要设计DFSTraverse函数 */
{
    int v;
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = 0;
    for (v = 0; v < G.vexnum; ++v)
    {
        if (!visited[v])           //若此时图中尚有顶点未被访问到，则另选一个未被访问过的顶点作起始点
        {
            dfsResult += "{";
            DFS(G, v);
            dfsResult += "}";
        }
    }
    dfsResult += "\n";
}


Status MainWindow::InitQueue(SqQueue& Q)              //队列初始化
{
    Q.base = (QElemType*)malloc(MAXQSIZE * sizeof(QElemType));
    if (!Q.base)
        exit(OVERFLOW);
    Q.front = Q.rear = 0;
    return OK;
}

void MainWindow::BFS(ALGraph G, int v)
{
    ArcNode* p;
    InitQueue(Q);
    bfsResult += QString::number(G.vertices[v].data);         //打印当前访问的结点
    visited[v] = 1;                     //访问过该结点后，visited立刻标记为1
    Q.base[Q.rear] = v;
    Q.rear = (Q.rear + 1) % MAXQSIZE;   //将这一层访问过的结点加入队列
    printQueue(&Q);
    while (Q.front != Q.rear)           //队列中存在结点（非空）；后面在访问时，除了移出以外，还会随着BFS层次深入加入新的结点，直至队列为空，即图的遍历完成
    {
        v = Q.base[Q.front];
        Q.front = (Q.front + 1) % MAXQSIZE;         //已经访问过的结点移出队列
        printQueue(&Q);
        p = G.vertices[v].firstarc;
        while (p)
        {
            if (!visited[p->adjvex])
            {
                bfsResult += " ";
                bfsResult += QString::number(G.vertices[p->adjvex].data);
                visited[p->adjvex] = 1;
                Q.base[Q.rear] = p->adjvex;
                Q.rear = (Q.rear + 1) % MAXQSIZE;   //将这一层访问过的结点加入队列
                printQueue(&Q);
            }
            p = p->nextarc;
        }
    }
}

void MainWindow::printQueue(SqQueue* t)
{
    //队列为空时首尾相等，尾指针指向的区域不存放元素。
    if (t->front == t->rear) {
        queueView += "队列为空！";
    }
    else {
        int p = t->front;
        //循环次数取自队列元素个数
        for (int i = 0; i < (t->rear - t->front + MAXQSIZE) % MAXQSIZE; i++) {
            queueView += QString::number(t->base[(p + i) % MAXQSIZE]);
        }
    }
    queueView += "\n";
    return;
}


void MainWindow::BFSTraverse(ALGraph G)   /* 注：对于连通图，从一个顶点出发，调用BFS函数即可将所有顶点都遍历到；对于非连通图，需要设计BFSTraverse函数 */
{
    int v;
    for (v = 0; v < G.vexnum; ++v)
        visited[v] = 0;
    for (v = 0; v < G.vexnum; ++v)          //若此时图中尚有顶点未被访问到，则另选一个未被访问过的顶点作起始点
    {
        if (!visited[v])
        {
            bfsResult += "{";
            BFS(G, v);
            bfsResult += "}";
        }
    }
    bfsResult += "\n";
}

void MainWindow::dataProcess()
{
    CreateALGraph(G);
    PrintALGraph(G);
    DFSTraverse(G);
    BFSTraverse(G);

    ui->label4->setText(stackView);
    ui->label4->setWordWrap(true);
    QFont ft4;
    ft4.setPointSize(12);
    ui->label4->setFont(ft4);

    ui->label5->setText(queueView);
    ui->label5->setWordWrap(true);
    QFont ft5;
    ft5.setPointSize(12);
    ui->label5->setFont(ft5);
}

void MainWindow::on_pushButton_clicked()
{
     ui->label->setText(dfsResult);
     ui->label->setWordWrap(true);
     QFont ft;
     ft.setPointSize(12);
     ui->label->setFont(ft);
}

void MainWindow::on_pushButton2_clicked()
{
    ui->label2->setText(bfsResult);
    ui->label2->setWordWrap(true);
    QFont ft2;
    ft2.setPointSize(12);
    ui->label2->setFont(ft2);
}

void MainWindow::on_lineEdit1_returnPressed()
{
    QStringList strList1;
    le1=ui->lineEdit1->text();
    strList1=le1.split(QChar(','));
    G.vexnum=strList1.first().toInt();
    G.arcnum=strList1.last().toInt();
}

void MainWindow::on_lineEdit2_returnPressed()
{
    le2=ui->lineEdit2->text();
    strList2=le2.split(QChar(','));

    dataProcess();
}
