#include "blockreader.h"

BlockReader::BlockReader(QIODevice *io)
{
     buffer.open(QIODevice::ReadWrite);
    _stream.setVersion(QDataStream::Qt_4_8);
    _stream.setDevice(&buffer);

    quint64 blockSize;

    // Read the size.
    readMax(io, sizeof(blockSize));
    buffer.seek(0);
    _stream >> blockSize;

    // Read the rest of the data.
    readMax(io, blockSize);
    buffer.seek(sizeof(blockSize));
}

void BlockReader::readMax(QIODevice *io, int n)
{
    while (buffer.size() < n)
    {
        if (!io->bytesAvailable())
        {
            io->waitForReadyRead(30000);
        }
        buffer.write(io->read(n - buffer.size()));
    }
}
