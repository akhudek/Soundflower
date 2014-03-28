/*
  File:SoundflowerClip.cpp

	Version: 1.0.1, ma++ ingalls
    
	Copyright (c) 2004 Cycling '74

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include "SoundflowerEngine.h"
#include <IOKit/IOLib.h>


// here, instead of clipping, we save the buffer 

IOReturn SoundflowerEngine::clipOutputSamples(const void *mixBuf, void *sampleBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
    UInt32 channelCount = streamFormat->fNumChannels;
    UInt32 offset = firstSampleFrame * channelCount;
    UInt32 byteOffset = offset * sizeof(float);
    UInt32 numBytes = numSampleFrames * channelCount * sizeof(float);
    
    if (((SoundflowerDevice *)audioDevice)->mMuteIn[0])
        memset((UInt8 *)thruBuffer + byteOffset, 0, numBytes);
    else {
        memcpy((UInt8 *)thruBuffer + byteOffset, (UInt8 *)mixBuf + byteOffset, numBytes);
        float masterGain =   ((SoundflowerDevice *)audioDevice)->mGain[0]/(float)SoundflowerDevice::kGainMax;
        float masterVolume = ((SoundflowerDevice *)audioDevice)->mVolume[0]/(float)SoundflowerDevice::kVolumeMax;

        for(UInt32 channelBufferIterator = 0; channelBufferIterator < numSampleFrames; channelBufferIterator++)
            for(UInt32 channel = 0; channel < channelCount; channel++) {
                float channelGain =   ((SoundflowerDevice *)audioDevice)->mGain[channel+1]/(float)SoundflowerDevice::kGainMax;
                float channelVolume = ((SoundflowerDevice *)audioDevice)->mVolume[channel+1]/(float)SoundflowerDevice::kVolumeMax;
                            
                thruBuffer[offset + channelBufferIterator*channelCount + channel] *= masterVolume * channelVolume * masterGain * channelGain;
            }

    }
    
    return kIOReturnSuccess;
}


// This is called when client apps need input audio.  Here we give them saved audio from the clip routine.

IOReturn SoundflowerEngine::convertInputSamples(const void *sampleBuf, void *destBuf, UInt32 firstSampleFrame, UInt32 numSampleFrames, const IOAudioStreamFormat *streamFormat, IOAudioStream *audioStream)
{
    UInt32 offset;
    UInt32 frameSize = streamFormat->fNumChannels * sizeof(float);
    offset = firstSampleFrame * frameSize;
              
    if (((SoundflowerDevice *)audioDevice)->mMuteOut[0])
        memset((UInt8 *)destBuf, 0, numSampleFrames * frameSize);
    else
        memcpy((UInt8 *)destBuf, (UInt8 *)thruBuffer + offset, numSampleFrames * frameSize);

    return kIOReturnSuccess;
}
