#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE
#define StationNum 261
#define LineNum 251
#define N 226

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QChart *m_ptrChart;
    QChartView *m_ptrChartView;
    QLineSeries * mLineSeries;

    QPaintEvent *event;

    MyGraph(QGraphicsItem *parent = Q_NULLPTR);
private slots:
   // void on_pushButton_clicked();

    void on_action1_triggered(bool checked);
    void on_action2_triggered(bool checked);

    void on_action3_triggered(bool checked);
    void on_action_1_triggered(bool checked);

    void on_action4_triggered(bool checked);

    void on_action5_triggered(bool checked);

    void on_action6_triggered(bool checked);

    void on_action7_triggered(bool checked);

    void on_action8_triggered(bool checked);

    void on_action9_triggered(bool checked);

    void on_action10_triggered(bool checked);

    bool dataProcess();
    int dijkstra(int sec, int n);
    bool buildGraph();

    void on_lineEdit1_returnPressed();

    void on_lineEdit2_returnPressed();
    void visitLine();
    void on_lineEdit3_returnPressed();

private:
    Ui::MainWindow *ui;
    //QImage image;
    QChart *mChart;    //创建图表对象
    QLabel *label;
    QList<QLineSeries *> m_series;

    QChart *chart = new QChart();
    QChart *chart2 = new QChart();
    QLineSeries *series1 = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    QLineSeries *series3 = new QLineSeries();
    QLineSeries *series4 = new QLineSeries();
    QLineSeries *series5 = new QLineSeries();
    QLineSeries *series6 = new QLineSeries();
    QLineSeries *series7 = new QLineSeries();
    QLineSeries *series8 = new QLineSeries();
    QLineSeries *series9 = new QLineSeries();
    QLineSeries *series10 = new QLineSeries();
    QLineSeries *series11 = new QLineSeries();
    QLineSeries *series12 = new QLineSeries();
    QLineSeries *series13 = new QLineSeries();
    QLineSeries *series14 = new QLineSeries();
    QLineSeries *series15 = new QLineSeries();
    QLineSeries *series16 = new QLineSeries();



    QValueAxis *xAxis = new QValueAxis();
    QValueAxis *yAxis = new QValueAxis();
    QValueAxis *xAxis2 = new QValueAxis();
    QValueAxis *yAxis2 = new QValueAxis();

    QString stationName[StationNum];
    double stationX[StationNum];
    double stationY[StationNum];
    int stationExist[StationNum]={0};
    double distance[LineNum];
    int LineName[LineNum];


    const int INF = 100000;
    int n, m;
    int g[N][N];   //利用邻接矩阵保存两个点的距离
    int dist[N];   //保存每个点到起点的最短距离
    bool st[N];    //每个点是否已经确定最短距离
    int path[N];   //每个点最短路径的前一个节点

    QString ss,ee;

protected:
   // void mousePressEvent(QMouseEvent *event);
protected:
    /*void paintEvent(QPaintEvent *event){
        QPainter painter(this);
        painter.drawImage(0,0,image);
    }*/
protected:
    //void paintEvent(QPaintEvent *event);

};

#endif // MAINWINDOW_H
