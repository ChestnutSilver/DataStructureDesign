#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindlg.h"
#include <qpushbutton.h>
#include <QMessageBox>
#include <QValueAxis>
#include <QTime>
#include "qchartview.h"
#include "qgraphicsitem.h"
#include <QPointF>
#include <QString>
#include <QTextEdit>
#include <QFile>
#include <algorithm>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //image = QImage(1000,600,QImage::Format_RGB32);  //画布的初始化大小设为600*500，使用32位颜色
    //QColor backColor = qRgb(255,255,255);    //画布初始化背景色使用白色
    //image.fill(backColor);//对画布进行填充

    chart->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    //chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    chart->setLocalizeNumbers(true);//数字是否本地化

    //chart->createDefaultAxes();//创建默认轴
    // 添加轴
    //QValueAxis *xAxis = new QValueAxis();
    //QValueAxis *yAxis = new QValueAxis();
    // 坐标轴整体
    //xAxis->setRange(0, 200);
    //yAxis->setRange(0, 200);
    xAxis->setRange(121.15,121.85);
    yAxis->setRange(30.95,31.45);

    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);




    chart->setTitle("Simple line chart example");//设置标题
    chart->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
    chart->setTitleFont(QFont("微软雅黑"));//设置标题字体

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
    //chart->legend()->detachFromChart();
    chart->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart->legend()->setVisible(true);//设置是否可视
    chart->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart->legend()->font();
    font.setItalic(!font.italic());
    chart->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(12);
    chart->legend()->setFont(font);//设置字体大小
    chart->legend()->setFont(QFont("微软雅黑"));//设置字体类型

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);


    //将界面上按钮的显示文本更改为“新窗口”
    ui->pushButton->setText(tr("新窗口"));

    m_series.append(series11);
    dataProcess();
    visitLine();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//讲解二方法二（转到槽，可以不用）
/*void MainWindow::on_pushButton_clicked()
{
    QDialog*dlg = new QDialog(this);

    dlg->show();
}
*/
bool MainWindow::dataProcess()
{
        QFile file("C:\\Users\\lenovo\\Desktop\\station.txt");
        if(!file.open(QIODevice::ReadOnly))
            return false;
        QTextStream in(&file);
        int i=0;
        int j=0;
        int flag=0;
        /*经统计共226个车站*/
        while(!in.atEnd())
        {
            QString id;
            for (i=0; !in.atEnd()&&i<StationNum; ++i)
            {
                QString longlat;
                QStringList strList;
                QString nameTemp;
                in>>id>>nameTemp>>longlat;
                flag=0;
                for(j=0;j<i;j++)
                {
                    if(nameTemp==stationName[j])
                    {
                        i--;
                        flag=1;
                        break;
                    }
                }
                if(flag==1)
                    continue;
                stationName[i]=nameTemp;
                strList=longlat.split(QChar(','));
                stationX[i]=strList.first().toDouble();
                stationY[i]=strList.last().toDouble();
            }
        }

        return true;
}

int MainWindow::dijkstra(int sec, int n)
{
    memset(dist, 0x3f, sizeof dist); //初始化所有距离为最大
    dist[sec] = 0;                   //初始化起点距离为0
    for (int i = 0; i < n - 1; i++)  //循环找到起点最短的且未确定最短距离的点t
    {
        int t = -1;
        for (int j = 0; j <= n; j++)
            if (!st[j] && (t == -1 || dist[t] > dist[j])) {
                t = j;
            }

        for (int j = 0; j <= n; j++) {
            if (dist[j] > dist[t] + g[t][j]) {
                path[j] = t; //若从起点到点j的距离被从起点到点t，再从点t到点j更新，
                             //那么t就是j的前驱节点，保存到path[j]里
            }
            if(dist[j] < dist[t] + g[t][j])
                dist[j] = dist[j];//更新最短路
            else {
                dist[j] = dist[t] + g[t][j];
            }
        }
        st[t] = true;   //点t的最短路径已经确定
    }

    if (dist[n] > 0x3f-1) return -1;
    return dist[n];
}


bool MainWindow::buildGraph()
{
    memset(g, 0x3f3f3f3f, sizeof g);     //初始化邻接矩阵距离无穷大
       memset(path, -1, sizeof path); //初始化每个点的前驱为-1，表示没有前驱
       n=226;
       m=251;

       QFile file("C:\\Users\\lenovo\\Desktop\\station.txt");
       if(!file.open(QIODevice::ReadOnly))
           return false;
       QTextStream in(&file);

       int a, b;
       int c;

       QString id;
       QString longlat;
       QString nameTemp;
       QStringList strList;
       in>>id>>nameTemp>>longlat;
       for(int m=0;m<261-1;m++)//开始逐一构建无向图中的线
       {
           QString id2;
           QString longlat2;
           QString nameTemp2;
           QStringList strList2;
           in>>id2>>nameTemp2>>longlat2;
           if(id2=="1")
               continue;
           else
           {
               int j,l;
               for(j=0;j<226;j++)
               {
                   if(nameTemp==stationName[j])
                   {
                       break;
                   }
               }
               for(l=0;l<226;l++)
               {
                   if(nameTemp2==stationName[l])
                   {
                       break;
                   }
               }

               a=j;
               b=l;
               c=int(abs((stationX[j]*1000.0-stationX[l]*1000.0))+abs((stationY[j]*1000.0-stationY[l]*1000.0)));
               //c=int((stationX[j]-stationX[l])*(stationX[j]-stationX[l])+(stationY[j]-stationY[l])*(stationY[j]-stationY[l]));
               g[a][b] = c;
               g[b][a] = c;

               id=id2;
               nameTemp=nameTemp2;
               longlat=longlat2;
           }
       }

       int s,e;
       for(s=0;s<226;s++)
       {
           if(ss==stationName[s])
           {
               break;
           }
       }
       for(e=0;e<226;e++)
       {
           if(ee==stationName[e])
           {
               break;
           }
       }


       //int s=10, e=52;  // s为起点，e为终点

       int ret = dijkstra(s, 226 - 1); //传入起点，和点的个数，由于点从0开始，所以应该传入n-1

       QStack<int> stk;  //根据path特点，我们用一个栈保存，最后弹栈输出时，即为正序
           int k = e;
           while (true) {
               if (k == s) break;
               stk.push(path[k]);
               k = path[k];
           }
           QString shortline="起点站：";
           while (!stk.empty())
           {
               if(stationExist[stk.top()]==1){
               shortline+=stationName[stk.top()];
               shortline+="\n";
               }
               //series11->append(stationX[stk.top()],stationY[stk.top()]);

               qDebug() << stationName[stk.top()];
                static QLabel *plabel =new QLabel(this);
                if(1)
                {
                    plabel->move(1200,300);
                    plabel->setVisible(true);
                    plabel->setMinimumWidth(200);//设置最小宽度，使自动调整时最小值为200
                    plabel->setWordWrap(true);//设置自动换行
                    plabel->adjustSize();//自动调整

                }
                plabel->setText(shortline);  //设置文本
                QString str=plabel->text();   //返回文本
               stk.pop();
           }
           static QLabel *p2label =new QLabel(this);
           if(1)
           {
              QString dst = "终点站："+ee;
               p2label->move(1200,550);
               p2label->setVisible(true);
               p2label->setMinimumWidth(200);//设置最小宽度，使自动调整时最小值为200
               p2label->setWordWrap(true);//设置自动换行
               p2label->adjustSize();//自动调整
               p2label->setText(dst);  //设置文本
               QString str=p2label->text();   //返回文本
           }

       /*static QLabel *plabel =new QLabel(this);
       if(1)
       {
           plabel->move(250,450);
           plabel->setVisible(true);
           plabel->clear();   //清空
           plabel->setText(QString::number(g[2][3]));  //设置文本
           QString str=plabel->text();   //返回文本
       }
*/


    return true;


}

void MainWindow::on_action1_triggered(bool checked)
{ 
    if(checked)
    {
        //QLineSeries *series1 = new QLineSeries();//实例化一个QLineSeries对象
        m_series.append(series1);
        //设置线条名称
        series1->setName(QString("line " + QString::number(1)));
        //设置线条颜色，如果不设置会给默认颜色
        series1->setColor(QColor(QRgb(0xE70012)));
        //设置是否线条可视
        series1->setVisible(true);
        //点标签是否可视
        series1->setPointLabelsVisible(false);
        //点标签颜色
        series1->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series1->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series1->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series1->setPointLabelsClipping(false);
        //设置点标签是否可视
        series1->setPointsVisible(true);

        //添加坐标点
        *series1 << QPointF(121.424661,31.39226) <<
                QPointF(121.427726,31.381227) <<
                QPointF(121.430975,31.369369) <<
                QPointF(121.433936,31.355004) <<
                QPointF(121.437711,31.339703) <<
                QPointF(121.441498,31.331094) <<
                QPointF(121.447455,31.318642) <<
                QPointF(121.448141,31.305883) <<
                QPointF(121.449858,31.292242) <<
                QPointF(121.452004,31.279846) <<
                QPointF(121.455265,31.271409) <<
                QPointF(121.459128,31.258717) <<
                QPointF(121.455523,31.249692) <<
                QPointF(121.459128,31.2414) <<
                QPointF(121.467882,31.238538) <<
                QPointF(121.472989,31.232814) <<
                QPointF(121.47329,31.222502) <<
                QPointF(121.460115,31.217107) <<
                QPointF(121.451059,31.213033) <<
                QPointF(121.446725,31.20426) <<
                QPointF(121.438056,31.191522) <<
                QPointF(121.436511,31.181903) <<
                QPointF(121.43458,31.168097) <<
                QPointF(121.43046,31.154693) <<
                QPointF(121.414324,31.142059) <<
                QPointF(121.402994,31.130929) <<
                QPointF(121.393167,31.120936) <<
                QPointF(121.385184,31.111237);

        chart->addSeries(series1);//添加系列到QChart上;
    }
    else
    {
        series1->setVisible(false);
        series1->clear();
        chart->removeSeries(series1);
        m_series.removeOne(series1);
    }
    series1->attachAxis(xAxis);
    series1->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}


//地铁二号线
void MainWindow::on_action2_triggered(bool checked)
{
    if(checked)
    {
        //QLineSeries *series2 = new QLineSeries();//实例化一个QLineSeries对象
        m_series.append(series2);
        //设置线条名称
        series2->setName(QString("line " + QString::number(2)));
        //设置线条颜色，如果不设置会给默认颜色
        series2->setColor(QColor(QRgb(0x008000)));
        //设置是否线条可视
        series2->setVisible(true);
        //点标签是否可视
        series2->setPointLabelsVisible(false);
        //点标签颜色
        series2->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series2->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series2->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series2->setPointLabelsClipping(false);
        //设置点标签是否可视
        series2->setPointsVisible(true);

        //添加坐标点
        *series2 << QPointF(121.806436,31.149596) <<
                QPointF(121.796878,31.168459) <<
                QPointF(121.755346,31.199485) <<
                QPointF(121.723791,31.192826) <<
                QPointF(121.69821,31.186741) <<
                QPointF(121.681098,31.196553) <<
                QPointF(121.673133,31.214327) <<
                QPointF(121.656269,31.214107) <<
                QPointF(121.619354,31.211042) <<
                QPointF(121.601662,31.204405) <<
                QPointF(121.587687,31.201832) <<
                QPointF(121.557884,31.204102) <<
                QPointF(121.552556,31.215723) <<
                QPointF(121.544439,31.21882) <<
                QPointF(121.527221,31.228764) <<
                QPointF(121.515556,31.23327) <<
                QPointF(121.502937,31.237991) <<
                QPointF(121.475137,31.232781) <<
                QPointF(121.472894,31.232943) <<
                QPointF(121.459971,31.229853) <<
                QPointF(121.445314,31.223493) <<
                QPointF(121.430635,31.220408) <<
                QPointF(121.416649,31.219879) <<
                QPointF(121.404058,31.211158) <<
                QPointF(121.387285,31.21487) <<
                QPointF(121.373994,31.216357) <<
                QPointF(121.359588,31.218212) <<
                QPointF(121.327625,31.194014) <<
                QPointF(121.318911,31.193987) <<
                QPointF(121.299204,31.188367);

        chart->addSeries(series2);//添加系列到QChart上;
    }
    else
    {
        series2->setVisible(false);
        series2->clear();
        chart->removeSeries(series2);
        m_series.removeOne(series2);
    }
    series2->attachAxis(xAxis);
    series2->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action3_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series3);
        series3->setName(QString("line " + QString::number(3)));
        //设置线条颜色，如果不设置会给默认颜色
        series3->setColor(QColor(QRgb(0xFFD100)));
        //设置是否线条可视
        series3->setVisible(true);
        //点标签是否可视
        series3->setPointLabelsVisible(false);
        //点标签颜色
        series3->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series3->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series3->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series3->setPointLabelsClipping(false);
        //设置点标签是否可视
        series3->setPointsVisible(true);

        //添加坐标点
        *series3 << QPointF(121.430041,31.154579) <<
                QPointF(121.443205,31.157949) <<
                QPointF(121.444346,31.16961) <<
                QPointF(121.43841,31.176746) <<
                QPointF(121.427817,31.186164) <<
                QPointF(121.420814,31.197524) <<
                QPointF(121.417062,31.209616) <<
                QPointF(121.415683,31.218026) <<
                QPointF(121.41342,31.232338) <<
                QPointF(121.417454,31.238943) <<
                QPointF(121.429856,31.246135) <<
                QPointF(121.441143,31.254316) <<
                QPointF(121.458052,31.24929) <<
                QPointF(121.476076,31.251491) <<
                QPointF(121.480024,31.259782) <<
                QPointF(121.479252,31.269613) <<
                QPointF(121.482427,31.280984) <<
                QPointF(121.483286,31.289273) <<
                QPointF(121.485174,31.305518) <<
                QPointF(121.484616,31.319927) <<
                QPointF(121.491182,31.331878) <<
                QPointF(121.500409,31.344817) <<
                QPointF(121.498521,31.3579) <<
                QPointF(121.492556,31.371019) <<
                QPointF(121.488178,31.381095) <<
                QPointF(121.479466,31.395126) <<
                QPointF(121.475904,31.403844) <<
                QPointF(121.460798,31.407947) <<
                QPointF(121.439984,31.407617);

        chart->addSeries(series3);//添加系列到QChart上;
    }
    else
    {
        series3->setVisible(false);
        series3->clear();
        chart->removeSeries(series3);
        m_series.removeOne(series3);
    }
    series3->attachAxis(xAxis);
    series3->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}


void MainWindow::on_action_1_triggered(bool checked)
{


}



void MainWindow::on_action4_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series4);
        series4->setName(QString("line " + QString::number(4)));
        //设置线条颜色，如果不设置会给默认颜色
        series4->setColor(QColor(QRgb(0x471E86)));
        //设置是否线条可视
        series4->setVisible(true);
        //点标签是否可视
        series4->setPointLabelsVisible(false);
        //点标签颜色
        series4->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series4->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series4->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series4->setPointLabelsClipping(false);
        //设置点标签是否可视
        series4->setPointsVisible(true);

        //添加坐标点
        *series4 << QPointF(121.427194,31.186717) <<
                QPointF(121.436721,31.182311) <<
                QPointF(121.443588,31.184955) <<
                QPointF(121.454917,31.190021) <<
                QPointF(121.463844,31.194353) <<
                QPointF(121.474487,31.198685) <<
                QPointF(121.489507,31.201621) <<
                QPointF(121.499635,31.207935) <<
                QPointF(121.51869,31.209623) <<
                QPointF(121.527627,31.211672) <<
                QPointF(121.532044,31.222246) <<
                QPointF(121.527444,31.228561) <<
                QPointF(121.51929,31.239423) <<
                QPointF(121.51723,31.251898) <<
                QPointF(121.51354,31.257474) <<
                QPointF(121.500923,31.260189) <<
                QPointF(121.488735,31.258868) <<
                QPointF(121.47689,31.251237) <<
                QPointF(121.455947,31.249476) <<
                QPointF(121.441184,31.254319) <<
                QPointF(121.431829,31.246321) <<
                QPointF(121.417667,31.237735) <<
                QPointF(121.411315,31.230836) <<
                QPointF(121.415607,31.217551) <<
                QPointF(121.41698,31.20955) <<
                QPointF(121.422216,31.195968);

        chart->addSeries(series4);//添加系列到QChart上;
    }
    else
    {
        series4->setVisible(false);
        series4->clear();
        chart->removeSeries(series4);
        m_series.removeOne(series4);
    }
    series4->attachAxis(xAxis);
    series4->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action5_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series5);
        series5->setName(QString("line " + QString::number(5)));
        //设置线条颜色，如果不设置会给默认颜色
        series5->setColor(QColor(QRgb(0x9253B2)));
        //设置是否线条可视
        series5->setVisible(true);
        //点标签是否可视
        series5->setPointLabelsVisible(false);
        //点标签颜色
        series5->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series5->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series5->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series5->setPointLabelsClipping(false);
        //设置点标签是否可视
        series5->setPointsVisible(true);

        //添加坐标点
        *series5 << QPointF(121.385379,31.111193) <<
                QPointF(121.385937,31.098112) <<
                QPointF(121.390143,31.089365) <<
                QPointF(121.401944,31.066945) <<
                QPointF(121.409755,31.045033) <<
                QPointF(121.416578,31.026243) <<
                QPointF(121.420012,31.018225) <<
                QPointF(121.410313,31.011237) <<
                QPointF(121.395292,31.007265) <<
                QPointF(121.38083,31.003366) <<
                QPointF(121.369715,31.000423);

        chart->addSeries(series5);//添加系列到QChart上;
    }
    else
    {
        series5->setVisible(false);
        series5->clear();
        chart->removeSeries(series5);
        m_series.removeOne(series5);
    }
    series5->attachAxis(xAxis);
    series5->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action6_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series6);
        series6->setName(QString("line " + QString::number(6)));
        //设置线条颜色，如果不设置会给默认颜色
        series6->setColor(QColor(QRgb(0xD7006C)));
        //设置是否线条可视
        series6->setVisible(true);
        //点标签是否可视
        series6->setPointLabelsVisible(false);
        //点标签颜色
        series6->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series6->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series6->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series6->setPointLabelsClipping(false);
        //设置点标签是否可视
        series6->setPointsVisible(true);

        //添加坐标点
        *series6 << QPointF(121.574752,31.353005) <<
                QPointF(121.586768,31.347764) <<
                QPointF(121.593892,31.33532) <<
                QPointF(121.601874,31.321427) <<
                QPointF(121.589386,31.312261) <<
                QPointF(121.589129,31.302618) <<
                QPointF(121.588528,31.290736) <<
                QPointF(121.588399,31.280651) <<
                QPointF(121.588013,31.271775) <<
                QPointF(121.586768,31.263154) <<
                QPointF(121.581533,31.256955) <<
                QPointF(121.572778,31.250057) <<
                QPointF(121.564324,31.245324) <<
                QPointF(121.551921,31.238867) <<
                QPointF(121.543638,31.235454) <<
                QPointF(121.534798,31.232775) <<
                QPointF(121.527373,31.228298) <<
                QPointF(121.529104,31.22008) <<
                QPointF(121.527846,31.211526) <<
                QPointF(121.523297,31.203194) <<
                QPointF(121.516516,31.193356) <<
                QPointF(121.509778,31.185793) <<
                QPointF(121.51098,31.172465) <<
                QPointF(121.515658,31.159466) <<
                QPointF(121.514327,31.14977) <<
                QPointF(121.50613,31.148779) <<
                QPointF(121.495316,31.148558) <<
                QPointF(121.480209,31.153223) ;

        chart->addSeries(series6);//添加系列到QChart上;
    }
    else
    {
        series6->setVisible(false);
        series6->clear();
        chart->removeSeries(series6);
        m_series.removeOne(series6);
    }
    series6->attachAxis(xAxis);
    series6->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action7_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series7);
        series7->setName(QString("line " + QString::number(7)));
        //设置线条颜色，如果不设置会给默认颜色
        series7->setColor(QColor(QRgb(0xFF7F00)));
        //设置是否线条可视
        series7->setVisible(true);
        //点标签是否可视
        series7->setPointLabelsVisible(false);
        //点标签颜色
        series7->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series7->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series7->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series7->setPointLabelsClipping(false);
        //设置点标签是否可视
        series7->setPointsVisible(true);

        //添加坐标点
        *series7 << QPointF(121.562754,31.211212) <<
                QPointF(121.557304,31.20321) <<
                QPointF(121.550094,31.193005) <<
                QPointF(121.53988,31.187278) <<
                QPointF(121.525074,31.187278) <<
                QPointF(121.509711,31.185846) <<
                QPointF(121.500527,31.181991) <<
                QPointF(121.494054,31.182001) <<
                QPointF(121.486117,31.174611) <<
                QPointF(121.473748,31.171967) <<
                QPointF(121.456968,31.183864) <<
                QPointF(121.45465,31.190399) <<
                QPointF(121.450273,31.199393) <<
                QPointF(121.449028,31.213121) <<
                QPointF(121.44847,31.222736) <<
                QPointF(121.442806,31.233305) <<
                QPointF(121.4383,31.240497) <<
                QPointF(121.431776,31.246258) <<
                QPointF(121.422035,31.256164) <<
                QPointF(121.422721,31.263684) <<
                QPointF(121.423022,31.273955) <<
                QPointF(121.421348,31.284885) <<
                QPointF(121.416541,31.293026) <<
                QPointF(121.413537,31.303404) <<
                QPointF(121.408559,31.314807) <<
                QPointF(121.398817,31.321296) <<
                QPointF(121.389076,31.320416) <<
                QPointF(121.373626,31.322066) <<
                QPointF(121.373025,31.344463) <<
                QPointF(121.362425,31.3574) <<
                QPointF(121.355988,31.364107) <<
                QPointF(121.35749,31.388838) <<
                QPointF(121.350237,31.401623);

        chart->addSeries(series7);//添加系列到QChart上;
    }
    else
    {
        series7->setVisible(false);
        series7->clear();
        chart->removeSeries(series7);
        m_series.removeOne(series7);
    }
    series7->attachAxis(xAxis);
    series7->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action8_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series8);
        series8->setName(QString("line " + QString::number(8)));
        //设置线条颜色，如果不设置会给默认颜色
        series8->setColor(QColor(QRgb(0x0000FF)));
        //设置是否线条可视
        series8->setVisible(true);
        //点标签是否可视
        series8->setPointLabelsVisible(false);
        //点标签颜色
        series8->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series8->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series8->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series8->setPointLabelsClipping(false);
        //设置点标签是否可视
        series8->setPointsVisible(true);

        //添加坐标点
        *series8 << QPointF(121.512272,31.061427) <<
                QPointF(121.510737,31.07297) <<
                QPointF(121.508076,31.084143) <<
                QPointF(121.505844,31.096345) <<
                QPointF(121.497862,31.119126) <<
                QPointF(121.489451,31.14102) <<
                QPointF(121.480181,31.152847) <<
                QPointF(121.493399,31.16078) <<
                QPointF(121.496146,31.170474) <<
                QPointF(121.494515,31.178038) <<
                QPointF(121.493571,31.184941) <<
                QPointF(121.489537,31.20146) <<
                QPointF(121.486447,31.211151) <<
                QPointF(121.483271,31.218638) <<
                QPointF(121.479409,31.227006) <<
                QPointF(121.474774,31.232437) <<
                QPointF(121.470654,31.241977) <<
                QPointF(121.469281,31.252911) <<
                QPointF(121.469023,31.263183) <<
                QPointF(121.47855,31.271693) <<
                QPointF(121.490996,31.276168) <<
                QPointF(121.501467,31.274847) <<
                QPointF(121.509878,31.27338) <<
                QPointF(121.518118,31.27448) <<
                QPointF(121.528504,31.278442) <<
                QPointF(121.534684,31.288344) <<
                QPointF(121.53331,31.295239) <<
                QPointF(121.532109,31.304699) <<
                QPointF(121.531508,31.314526) <<
                QPointF(121.531765,31.322445);

        chart->addSeries(series8);//添加系列到QChart上;
    }
    else
    {
        series8->setVisible(false);
        series8->clear();
        chart->removeSeries(series8);
        m_series.removeOne(series8);
    }
    series8->attachAxis(xAxis);
    series8->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action9_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series9);
        series9->setName(QString("line " + QString::number(9)));
        //设置线条颜色，如果不设置会给默认颜色
        series9->setColor(QColor(QRgb(0x7ac8ee)));
        //设置是否线条可视
        series9->setVisible(true);
        //点标签是否可视
        series9->setPointLabelsVisible(false);
        //点标签颜色
        series9->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series9->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series9->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series9->setPointLabelsClipping(false);
        //设置点标签是否可视
        series9->setPointsVisible(true);

        //添加坐标点
        *series9 << QPointF(121.548664,31.22751) <<
                QPointF(121.527221,31.228764) <<
                QPointF(121.516428,31.23014) <<
                QPointF(121.498446,31.216837) <<
                QPointF(121.486215,31.21179) <<
                QPointF(121.477182,31.209514) <<
                QPointF(121.468792,31.206211) <<
                QPointF(121.460659,31.202614) <<
                QPointF(121.450317,31.199218) <<
                QPointF(121.436884,31.195199) <<
                QPointF(121.427314,31.184387) <<
                QPointF(121.417916,31.174767) <<
                QPointF(121.397853,31.170508) <<
                QPointF(121.384828,31.166469) <<
                QPointF(121.368971,31.157986) <<
                QPointF(121.34923,31.15525) <<
                QPointF(121.337642,31.149373) <<
                QPointF(121.319296,31.137252) <<
                QPointF(121.260288,31.117562) <<
                QPointF(121.229732,31.103599) <<
                QPointF(121.23059,31.084269) <<
                QPointF(121.231706,31.053539) <<
                QPointF(121.230676,31.0303) <<
                QPointF(121.230333,31.015884) <<
                QPointF(121.229361,31.001125) <<
                QPointF(121.229903,30.982482);

        chart->addSeries(series9);//添加系列到QChart上;
    }
    else
    {
        series9->setVisible(false);
        series9->clear();
        chart->removeSeries(series9);
        m_series.removeOne(series9);
    }
    series9->attachAxis(xAxis);
    series9->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}

void MainWindow::on_action10_triggered(bool checked)
{
    if(checked)
    {
        m_series.append(series10);
        series10->setName(QString("line " + QString::number(10)));
        //设置线条颜色，如果不设置会给默认颜色
        series10->setColor(QColor(QRgb(0xC7AFD3)));
        //设置是否线条可视
        series10->setVisible(true);
        //点标签是否可视
        series10->setPointLabelsVisible(false);
        //点标签颜色
        series10->setPointLabelsColor(QColor(255,255,255));
        //点标签字体
        series10->setPointLabelsFont(QFont("微软雅黑"));
        //设置点标签显示格式
        series10->setPointLabelsFormat("(@xPoint,@yPoint)");
        //是否切割边缘点标签，默认为true
        series10->setPointLabelsClipping(false);
        //设置点标签是否可视
        series10->setPointsVisible(true);

        //添加坐标点
        *series10 << QPointF(121.506951,31.328496) <<
                QPointF(121.506779,31.321714) <<
                QPointF(121.508453,31.313245) <<
                QPointF(121.514247,31.302648) <<
                QPointF(121.514805,31.297955) <<
                QPointF(121.510084,31.289373) <<
                QPointF(121.506393,31.282075) <<
                QPointF(121.501501,31.27474) <<
                QPointF(121.49442,31.268247) <<
                QPointF(121.488583,31.259186) <<
                QPointF(121.484163,31.251886) <<
                QPointF(121.482489,31.243814) <<
                QPointF(121.484592,31.238163) <<
                QPointF(121.487296,31.227705) <<
                QPointF(121.48382,31.219081) <<
                QPointF(121.475365,31.216181) <<
                QPointF(121.458542,31.215007) <<
                QPointF(121.444338,31.207923) <<
                QPointF(121.435282,31.202013) <<
                QPointF(121.422622,31.19625) <<
                QPointF(121.411936,31.19647) <<
                QPointF(121.403868,31.19871) <<
                QPointF(121.392195,31.19926) <<
                QPointF(121.38005,31.194231) <<
                QPointF(121.367905,31.190119) <<
                QPointF(121.347263,31.191363) <<
                QPointF(121.326385,31.194226) <<
                QPointF(121.319025,31.193919);

        chart->addSeries(series10);//添加系列到QChart上;
    }
    else
    {
        series10->setVisible(false);
        series10->clear();
        chart->removeSeries(series10);
        m_series.removeOne(series10);
    }
    series10->attachAxis(xAxis);
    series10->attachAxis(yAxis);

    ui->graphicsView->setChart(chart);  //histogramView--ui中已添加的QChartView
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVisible(true);
}


void MainWindow::on_lineEdit1_returnPressed()
{
     ss=ui->lineEdit1->text();
}

void MainWindow::on_lineEdit2_returnPressed()
{
     ee=ui->lineEdit2->text();
     buildGraph();


     if(1)
     {
         series11->setName(QString("line " + QString::number(11)));
         //设置线条颜色，如果不设置会给默认颜色
         series11->setColor(QColor(QRgb(0xC7AFD3)));
         //设置是否线条可视
         series11->setVisible(true);
         //点标签是否可视
         series11->setPointLabelsVisible(false);
         //点标签颜色
         series11->setPointLabelsColor(QColor(255,255,255));
         //点标签字体
         series11->setPointLabelsFont(QFont("微软雅黑"));
         //设置点标签显示格式
         series11->setPointLabelsFormat("(@xPoint,@yPoint)");
         //是否切割边缘点标签，默认为true
         series11->setPointLabelsClipping(false);
         //设置点标签是否可视
         series11->setPointsVisible(true);

         int s,e;
         for(s=0;s<226;s++)
         {
             if(ss==stationName[s])
             {
                 break;
             }
         }
         for(e=0;e<226;e++)
         {
             if(ee==stationName[e])
             {
                 break;
             }
         }

         //添加坐标点
         QStack<int> stk;  //根据path特点，我们用一个栈保存，最后弹栈输出时，即为正序
             int k = e;
             while (true) {
                 if (k == s) break;
                 stk.push(path[k]);
                 k = path[k];
             }

             while (!stk.empty())
             {

                 series11->append(stationX[stk.top()],stationY[stk.top()]);
                 stk.pop();
             }

         chart2->addSeries(series11);//添加系列到QChart上;
     }
     else
     {
         series11->setVisible(false);
         series11->clear();
         chart2->removeSeries(series11);
         m_series.removeOne(series11);
     }
     series11->attachAxis(xAxis2);
     series11->attachAxis(yAxis2);

     ui->graphicsView2->setChart(chart2);  //histogramView--ui中已添加的QChartView
     ui->graphicsView2->setRenderHint(QPainter::Antialiasing);
     ui->graphicsView2->setVisible(true);
}

void MainWindow::visitLine()
{

    chart2->setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    //chart->setAnimationOptions(QChart::AllAnimations);//设置启用或禁用动画
    chart2->setLocalizeNumbers(true);//数字是否本地化
    chart2->addAxis(xAxis2, Qt::AlignBottom);
    chart2->addAxis(yAxis2, Qt::AlignLeft);


    chart2->setTitle("换乘路线示意图");//设置标题
    chart2->setTitleBrush(QBrush(QColor(255,170,255)));//设置标题Brush
    chart2->setTitleFont(QFont("微软雅黑"));//设置标题字体

    chart2->legend()->setVisible(false);
    chart2->legend()->setAlignment(Qt::AlignBottom);//底部对齐
    //chart->legend()->detachFromChart();
    chart2->legend()->setBackgroundVisible(true);//设置背景是否可视
    chart2->legend()->setLabelColor(QColor(255,128,255));//设置标签颜色
    chart2->legend()->setBorderColor(QColor(255,255,170,185));//设置边框颜色
    QFont font = chart2->legend()->font();
    font.setItalic(!font.italic());
    chart2->legend()->setFont(font);//设置字体为斜体
    font.setPointSizeF(12);
    chart2->legend()->setFont(font);//设置字体大小
    chart2->legend()->setFont(QFont("微软雅黑"));//设置字体类型

    ui->graphicsView2->setChart(chart2);  //histogramView--ui中已添加的QChartView
    ui->graphicsView2->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView2->setVisible(true);
}

void MainWindow::on_lineEdit3_returnPressed()
{
    QString addstation = ui->lineEdit3->text();
    if(addstation=="一键添加")
    {
        for(int j=0;j<226;j++)
        {
            stationExist[j]=1;
        }
    }
    else
    {
        for(int j=0;j<226;j++)
        {
            if(addstation==stationName[j])
            {
                stationExist[j]=1;
                break;
            }
        }
    }

}
