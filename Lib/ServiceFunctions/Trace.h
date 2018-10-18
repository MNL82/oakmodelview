/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "Assert.h"

#ifdef WIN32

#include <Windows.h>
#include <stdio.h>

struct Tracer
{
    char const * m_fileName;
    unsigned m_line;

    Tracer(const char * fileName, const unsigned line)
        : m_fileName { fileName },
          m_line { line }
    {
    }

    template <typename... Args>
    void operator() (const wchar_t * format, Args... args) const
    {
        if (blocked()) { return; }
        wchar_t buffer [256];

        auto count = swprintf_s(buffer,
                                L"%S(%d): ",
                                m_fileName,
                                m_line);

        ASSERT(-1 != count);

        ASSERT(-1 != _snwprintf_s(buffer + count,
                                  _countof(buffer) - count,
                                  _countof(buffer) - count - 1,
                                  format,
                                  args...));

        OutputDebugString(buffer);
    }

    static bool &blocked()
    {
        static bool blocked = false;
        return blocked;
    }
};



#else
// Todo: Implement linux/mac tracer function here!
#endif

#ifdef _DEBUG
#ifdef WIN32
#define TRACE Tracer(__FILE__, __LINE__)
#define TRACE_BLOCK Tracer::blocked()
#else
// Todo: Implement linux/mac TRACE macro here!
#define TRACE __noop
#define TRACE_BLOCK __noop
#endif
#else
#define TRACE __noop
#define TRACE_BLOCK __noop
#endif
