#ifndef __ASTREAM_H
#define __ASTREAM_H

#include "APrint.h"

class Stream : public Print {
public:
    virtual ~Stream() = default;
    virtual bool available() = 0;
    virtual int read() = 0;
};

#endif //__ASTREAM_H
