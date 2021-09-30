#include "blockwriter.h"

BlockWriter::BlockWriter(QIODevice *io)
{
    buffer.open(QIODevice::WriteOnly);
    this->io = io;
    _stream.setVersion(QDataStream::Qt_4_8);
    _stream.setDevice(&buffer);

    // Placeholder for the size. We will get the value
    // at the end.
    _stream << quint64(0);
}

BlockWriter::~BlockWriter()
{
    // Write the real size.
    _stream.device()->seek(0);
    _stream << (quint64) buffer.size();

    // Flush to the device.
    io->write(buffer.buffer());
}
