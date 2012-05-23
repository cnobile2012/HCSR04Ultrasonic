/*
 * Ultrasonic.h - Library for HC-SR04 Ultrasonic Sensing Module.
 *
 * With ideas from:
 *   Created by ITead studio. Alex, Apr 20, 2010.
 *   iteadstudio.com
 *
 * SVN Keywords
 * ----------------------------------
 * $Author$
 * $Date$
 * $Revision$
 * ----------------------------------
 *
 * Thank you to Rowan Simms for pointing out the change in header name with
 * Arduino version 1.0 and up.
 *
 */

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stddef.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

// Undefine COMPILE_STD_DEV if you don't want Standard Deviation.
#define COMPILE_STD_DEV

// Hard-coded maximum distance in CM (manual for HC-SR04 says 500cm).
#define MAX_CM_DISTANCE 500


typedef struct bufferCtl
    {
    float *pBegin;
    float *pIndex;
    size_t length;
    bool filled;
    } BufCtl;

class Ultrasonic
    {
    public:
    Ultrasonic(int tp, int ep, int md);
    Ultrasonic(int tp, int ep);
    long timing();
    float convert(long microsec, int metric);
    void setDivisor(float value, int metric);
    static const int IN = 0;
    static const int CM = 1;

#ifdef COMPILE_STD_DEV
    bool sampleCreate(size_t size, ...);
    void sampleClear();
    float unbiasedStdDev(float value, size_t bufNum);
#endif // COMPILE_STD_DEV

    private:
    int _trigPin;
    int _echoPin;
    int _maxCMDistance;
    int _maxINDistance;
    unsigned int _pingTimeout;
    float _cmDivisor;
    float _inDivisor;

#ifdef COMPILE_STD_DEV
    size_t _numBufs;
    BufCtl *_pBuffers;
    void _sampleUpdate(BufCtl *buf, float msec);
    void _freeBuffers();
#endif // COMPILE_STD_DEV
    };

#endif // ULTRASONIC_H
