#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try
    {
        dictionary.SetPath(PATH);
    }
    catch (const QString &exception)
    {
        QMessageBox::information(0, "error", exception);
        exit(0);
    }

    connect(&dictionary, SIGNAL(Add(QString)), this, SLOT(AddWord(QString)));

    this->setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &word)
{
    Search(word);
}

void MainWindow::on_checkBox_stateChanged(int)
{
    if (ui->checkBox->isChecked())
        dictionary.setType(SearchType::Subsequence);
    else
        dictionary.setType(SearchType::Substring);

    Search(ui->lineEdit->text());
}

void MainWindow::AddWord(const QString &word)
{
    ui->textEdit->insertPlainText(word);
}

void MainWindow::Search(const QString &word)
{
    dictionary.setRunning(false);
    future.waitForFinished();
    ui->textEdit->clear();

    if (word == "") return;

    dictionary.setRunning(true);

    try
    {
        future = QtConcurrent::run([this, word] { dictionary.Find(word.toLower()); });
    }
    catch (const QException &exception)
    {
        QMessageBox::information(0, "error", exception.what());
    }
}


void MainWindow::closeEvent(QCloseEvent*)
{
    dictionary.setRunning(false);
    future.waitForFinished();
}
