#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

enum SearchType { Substring, Subsequence };

class MainWindow;

class Dictionary : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(SearchType type READ type WRITE setType NOTIFY typeChanged)

private:
    // Добавляю в textEdit по блокам в MAX_BLOCK_SIZE слов, потому что добавление текста долгое.

    /// Максимальное количество слов в блоке.
    const size_t MAX_BLOCK_SIZE = 1000;

    /// Блок для добавления в textEdit.
    QString currentBlock;

    /// Количество слов в текущем блоке.
    size_t block_size;

    /// Файл из которого считываю словарь.
    QFile file;

    /// Тип поиска.
    SearchType m_type;

    /// Флаг, который отображает идет поиск или нет.
    bool m_running = false;


    /// Добавляю слово в блок.
    void AddWord(const QString&);

    /// Отправляю блок для отображения в textEdit.
    void SendBlock();

    /// Проверка на содержание в строке.
    bool Contains(const QString&, const QString&);

public:
    explicit Dictionary(QObject *parent = nullptr);

    /// Установка пути до текстового файла со словарем.
    void SetPath(const QString&);

    /// Поиск в словаре по подстроке.
    void Find(const QString&);

    bool running() const;

    SearchType type() const;

public slots:
    void setRunning(bool running);
    void setType(SearchType type);

signals:
    /// Добавление блока в textEdit.
    void Add(const QString&);

    void runningChanged(bool running);
    void typeChanged(SearchType type);
};

#endif // DICTIONARY_H
