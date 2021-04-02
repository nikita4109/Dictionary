#include "dictionary.h"
#include "mainwindow.h"

Dictionary::Dictionary(QObject *parent) : QObject(parent) { }

void Dictionary::SetPath(const QString &_path)
{
    if (!QFile::exists(_path)) throw QString("Поместите файл со словарем в папку с исполняемым файлом");

    file.setFileName(_path);
    Find("");
}

void Dictionary::SendBlock()
{
    emit Add(currentBlock);
    QThread::currentThread()->msleep(5);

    currentBlock = "";
    block_size = 0;
}

void Dictionary::AddWord(const QString &word)
{
    ++block_size;
    currentBlock += word;

    if (block_size == MAX_BLOCK_SIZE)
        SendBlock();
    else
        currentBlock += '\n';
}

bool Dictionary::Contains(const QString &word, const QString &query)
{
    if (m_type == SearchType::Substring) return word.contains(query);
    if (query.size() == 0) return false;

    // Проверка на содержание как подпоследовательности.
    for (int i = 0, j = 0; i < word.size(); ++i)
    {
        if (word[i] == query[j]) ++j;
        if (j == query.size()) return true;
    }

    return false;
}

void Dictionary::Find(const QString &query)
{
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QTextStream stream(&file);

    while (m_running && !stream.atEnd())
    {
        QString word = stream.readLine();
        if (Contains(word.toLower(), query))
            AddWord(word);
    }

    // Если блок не пустой, то добавляем его в textEdit
    if (block_size > 0)
        SendBlock();

    file.close();
}

bool Dictionary::running() const
{
    return m_running;
}

SearchType Dictionary::type() const
{
    return m_type;
}

void Dictionary::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(m_running);
}

void Dictionary::setType(SearchType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}
