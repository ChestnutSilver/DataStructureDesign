#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
typedef int Status;
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define MAX_VERTEX_NUM 10010 //最大顶点数
#define MAXQSIZE 10010
typedef int VertexType;
typedef int AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];   //邻接矩阵类型


typedef struct ArcNode   //表结点（边结点）
{
    int adjvex;              //该弧所指向的顶点的位置
    struct ArcNode* nextarc; //指向下一条弧的指针
    int info;
}ArcNode; //边结点类型

typedef struct VNode //头结点
{
    VertexType data;   //顶点信息
    ArcNode* firstarc; //指向第一条依附该顶点的弧的指针
}VNode, AdjList[MAX_VERTEX_NUM];

typedef struct
{
    AdjList vertices;   //邻接表；存放各个顶点的数组
    int vexnum, arcnum; //图的当前顶点数和边数
}ALGraph;

typedef int QElemType;
typedef struct
{
    QElemType* base;
    int front;
    int rear;
}SqQueue;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void dataProcess();
    int LocateVex(ALGraph G, int u);
    void CreateALGraph(ALGraph& G);
    void DFS(ALGraph G, int v);
    void DFSTraverse(ALGraph G);
    Status InitQueue(SqQueue& Q);
    void BFS(ALGraph G, int v);
    void BFSTraverse(ALGraph G);
    //void CreateNewALGraph(ALGraph& alG);
    void PrintALGraph(ALGraph G);


private slots:
    void on_pushButton_clicked();

    void on_pushButton2_clicked();


    void on_lineEdit1_returnPressed();

    void on_lineEdit2_returnPressed();
    void printQueue(SqQueue* t);

private:
    Ui::MainWindow *ui;
    ALGraph G;
    SqQueue Q;
    QString dfsResult="";
    QString bfsResult="";
    QString le1="";
    QString le2="";
    QStringList strList2;
    QString queueView="";
    QString stackView="";

    int visited[MAX_VERTEX_NUM];   //已经访问过的结点数组




};

#endif // MAINWINDOW_H
