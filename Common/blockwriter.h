#ifndef BLOCKWRITER_H
#define BLOCKWRITER_H

#include <QBuffer>
#include <QDataStream>
#include <QIODevice>


class BlockWriter
{
public:

     BlockWriter(QIODevice *io);
    ~BlockWriter();

    QDataStream &stream()
    {
        return _stream;
    }

private:
    QBuffer buffer;
    QDataStream _stream;
    QIODevice *io;
};




#endif // BLOCKWRITER_H
