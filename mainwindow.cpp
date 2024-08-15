#include "mainwindow.h"
#include "ui_mainwindow.h"

//主窗口构造函数
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(edit);
    this->showStatus();
    //时间循环
    QTimer * timer = new QTimer;
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, &MainWindow::looper);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete edit;
}

//绑定事件
void MainWindow::connectMenuEvent(){
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(ui->action_help, &QAction::triggered, this, &MainWindow::showHelpDialog);
    connect(ui->action_exit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->action_open, &QAction::triggered, this, &MainWindow::openFile);
    connect(edit, &QTextEdit::textChanged, this, [=](){
        fileSaved = false;
        this->changeWindowTitle();
    });
    connect(ui->action_save, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->action_save_a, &QAction::triggered, this, [=](){
        this->saveFile(true);
    });
    connect(ui->action_new, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->action_undo, &QAction::triggered, this, [=](){
        edit->undo();
    });
    connect(ui->action_redo, &QAction::triggered, edit, &QTextEdit::redo);
    connect(ui->action_copy, &QAction::triggered, edit, &QTextEdit::copy);
    connect(ui->action_paste, &QAction::triggered, edit, &QTextEdit::paste);
    connect(ui->action_selectAll, &QAction::triggered, edit,&QTextEdit::selectAll);
    //connect(edit, &QTextEdit::cursorPositionChanged, this, &MainWindow::showStatus);
}

void MainWindow::looper(){
    this->showStatus();
}

//按键事件绑定
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control)pressedCtrl = true;
    if(pressedCtrl)
        switch(event->key()){
        case Qt::Key_S :
            this->saveFile();
            break;
        case Qt::Key_N :
            this->newFile();
            break;
        case Qt::Key_O :
            this->openFile();
            break;
        default:
            break;
        };
}

void MainWindow::keyReleaseEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Control)pressedCtrl = false;
}

void MainWindow::showAboutDialog(){
    QMessageBox::information(this, tr("关于"), tr("这是一款基于Qt C++开发的一款文本编辑器\n版本号：V.0.0.1"));
}

void MainWindow::showHelpDialog(){
    QMessageBox::question(this, tr("帮助"), "暂无帮助...", QMessageBox::Yes);
}

void MainWindow::openFile(){
    if(!fileSaved&&edit->toPlainText().length() > 0){
        QMessageBox::StandardButton btn = QMessageBox::question(this, tr("打开文件"), tr("当前文件未保存，请问是否保存后再打开文件，打开文件后编辑器将被清空"), QMessageBox::No|QMessageBox::Save);
        if(btn == QMessageBox::Save)this->saveFile();
    }
    filePath = QFileDialog::getOpenFileName(this, tr("打开文件"));
    if(!filePath.isEmpty()){
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, tr("打开文件"), tr("文件打开错误%1").arg(file.errorString()));
            return;
        }
        QTextStream in(&file);
        edit->setText(in.readAll());
        file.close();
        fileSaved = true;
        this->changeWindowTitle();
    }else QMessageBox::critical(this, tr("打开文件"), tr("请选择文件！"));
}

bool MainWindow::saveFile(bool a){
    if(filePath.isEmpty()||a){
        filePath = QFileDialog::getSaveFileName(this, tr("保存文件"), tr("*.txt"));
        if(filePath.isEmpty())return 0;
    }
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this, tr("保存文件"), tr("文件保存错误%1").arg(file.errorString()));
        return 0;
    }
    QTextStream out(&file);
    out << edit->toPlainText();
    file.close();
    fileSaved = true;
    this->changeWindowTitle();
    QMessageBox::information(this, tr("保存文件"), tr("文件保存成功！"));
    return 1;
}

void MainWindow::newFile(){
    QProcess * p = new QProcess;
    p->start(tr("./textEditor"));
}

void MainWindow::changeWindowTitle(){
    this->setWindowTitle(tr("文本编辑器-").append(filePath.isEmpty()?"未命名":filePath).append(fileSaved?"":"*"));
}

void MainWindow::showStatus(){
    QString t = edit->toPlainText();
    int p = edit->textCursor().position();
    t.remove(p, edit->toPlainText().length()-p+1);
    ui->statusbar->showMessage(tr("行：%1         |  列：%2        |  位置：%3").arg(t.count("\n")+1).arg(edit->textCursor().positionInBlock()+1).arg(p));
}
