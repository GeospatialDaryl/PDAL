/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#ifndef INCLUDED_ITERATOR_HPP
#define INCLUDED_ITERATOR_HPP

#include <libpc/Stage.hpp>

namespace libpc
{

class LIBPC_DLL Iterator
{
public:
    Iterator(const Stage& stage);

    const Stage& getStage() const;

    // This reads a set of points at the current position in the file.
    //
    // The schema of the PointData buffer we are given here might
    // not match our own header's schema.  That's okay, though: all
    // that matters is that the buffer we are given has the fields
    // we need to write into.
    //
    // This is NOT virtual.  Derived classes should override the 
    // readBuffer function below, not this one.
    //
    // Returns the number of valid points read.
    boost::uint32_t read(PointData&);

    // advance (or retreat) to the Nth point in the file (absolute, 
    // not relative).  In some cases, this might be a very slow, painful
    // function to call.
    virtual void seekToPoint(boost::uint64_t pointNum) = 0;

    // Returns the current point number.  The first point is 0.
    // If this number if > getNumPoints(), then no more points
    // may be read (and atEnd() should be true).
    boost::uint64_t getCurrentPointIndex() const;

    // returns true after we've read all the points available to this stage
    // (actually a convenience function that compares getCurrentPointIndex and getNumPoints)
    bool atEnd() const;

protected:
    // Implement this to do the actual work to fill in a buffer of points.
    virtual boost::uint32_t readBuffer(PointData&) = 0;

    // Each concrete stage is repsonsible for managing its own current
    // point index when a read or seek occurs.  Call this function to set
    // the value.
    void setCurrentPointIndex(boost::uint64_t delta);

    // this is easier than saying setCurrentPointIndex(getCurrentPointIndex()+n)
    void incrementCurrentPointIndex(boost::uint64_t currentPointDelta);

private:
    const Stage& m_stage;
    boost::uint64_t m_currentPointIndex;

    Iterator& operator=(const Iterator&); // not implemented
    Iterator(const Iterator&); // not implemented
};

} // namespace libpc

#endif