#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtparser.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonParseExpression_clicked()
{
    QString inputExpression = ui->lineEditInputExpression->text();
    QByteArray inputExpressionBA = inputExpression.toLocal8Bit();
    const char *c_inputExpression = inputExpressionBA.data();
    QtParser p(c_inputExpression);

    qDebug() << "Input expression: " << inputExpression;

    auto result = QtParser::eval(p.parse());

    QString answerExpression = QString::number(result);
    ui->lineEditParseAnswer->setText(answerExpression);
}
