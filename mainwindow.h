#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QProcess>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectMenuEvent();
    void changeWindowTitle();
public slots:
    void showAboutDialog();
    void showHelpDialog();
    void openFile();
    bool saveFile(bool a = false);
    void newFile();
    void showStatus();
    void looper();
private:
    Ui::MainWindow *ui;
    QTextEdit * edit = new QTextEdit(this);
    QString filePath;
    bool fileSaved = true;
    bool pressedCtrl = false;

protected:
    void closeEvent(QCloseEvent * event){
        if(!fileSaved&&edit->toPlainText().length()>0){
            QMessageBox::StandardButton btn = QMessageBox::warning(this, tr("退出"), tr("当前文件未保存，请问是否需要先保存后再退出？"), QMessageBox::Cancel|QMessageBox::Save|QMessageBox::No);
            if(btn == QMessageBox::Save){
                if(!this->saveFile()){
                    event->ignore();
                    return;
                }
            }
            else if(btn == QMessageBox::Cancel){
                event->ignore();
                return;
            }
        }
        event->accept();
    }
    void keyPressEvent(QKeyEvent *event = nullptr);
    void keyReleaseEvent(QKeyEvent *event = nullptr);
};
#endif // MAINWINDOW_H
