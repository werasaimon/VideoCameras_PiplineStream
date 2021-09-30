#ifndef BLOCKREADER_H
#define BLOCKREADER_H

#include <QBuffer>
#include <QDataStream>
#include <QIODevice>
#include "order.h"

class BlockReader
{
public:

    BlockReader(QIODevice *io);

    QDataStream& stream()
    {
        return _stream;
    }

private:
    // Blocking reads data from socket until buffer size becomes exactly n. No
    // additional data is read from the socket.
    void readMax(QIODevice *io, int n);

    QBuffer buffer;
    QDataStream _stream;
};



#endif // BLOCKREADER_H
