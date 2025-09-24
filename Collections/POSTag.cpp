#include "POSTag.h"

template<>
bool ZLibFile::write<POSTag>(const POSTag& t)
{
    return write(t.POS) && write(t.features);
}

template<>
bool ZLibFile::read<POSTag>(POSTag& t)
{
    return read(t.POS) && read(t.features);
}
