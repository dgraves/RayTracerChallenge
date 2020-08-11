/*
** Copyright(c) 2020 Dustin Graves
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this softwareand associated documentation files(the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions :
**
** The above copyright noticeand this permission notice shall be included in all
** copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
** SOFTWARE.
*/

#pragma once

#include "output_stream.h"

#include <cinttypes>
#include <vector>

namespace rtc
{
    class MemoryOutputStream : public OutputStream
    {
    public:
        virtual bool IsValid() override { return true; }

        virtual bool Fail() override { return false; }

        virtual bool Write(const char* data, size_t size) override
        {
            data_.insert(data_.end(), data, std::next(data, size));
            return true;
        }

        void Reset() { data_.clear(); }

        size_t GetSize() const { return data_.size(); }

        const uint8_t* GetData() const { return data_.data(); }

    private:
        std::vector<uint8_t> data_;
    };
}
