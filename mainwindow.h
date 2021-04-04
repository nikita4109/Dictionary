#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dictionary.h"
#include <QtConcurrent/QtConcurrent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    /// Добавление слов в textEdit.
    void AddWord(const QString&);

private slots:

    /// Обработка изменения текста в lineEdit.
    void on_lineEdit_textChanged(const QString&);

    /// Изменение типа поиска.
    void on_checkBox_stateChanged(int);

private:
    const QString PATH = "dictionary.txt";

    Ui::MainWindow *ui;

    QFuture<void> future;

    /// Словарь.
    Dictionary dictionary;

    /// Поиск в словаре.
    void Search(const QString&);

    virtual void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
