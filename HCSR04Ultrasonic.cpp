/*
 * Ultrasonic.cpp - Library for HC-SR04 Ultrasonic Sensing Module.
 *
 * With ideas from:
 *   Created by ITead studio. Apr 20, 2010.
 *   iteadstudio.com
 *
 * SVN Keywords
 * ----------------------------------
 * $Author$
 * $Date$
 * $Revision$
 * ----------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include <HCSR04Ultrasonic.h>


Ultrasonic::Ultrasonic(int tp, int ep)
    {
    pinMode(tp, OUTPUT);
    pinMode(ep, INPUT);
    _trigPin = tp;
    _echoPin = ep;
    _multiplier = MULTIPLIER_DEFAULT;
    /* Don't allow a maximum distance farther than the sensor supports. */
    _maxDistance = _MAX_CM_DISTANCE;
    _pingTimeout = _maxDistance * _PING_OVERHEAD;
    _temp = _TEMP_CELSIUS_DEFAULT;
    }

long Ultrasonic::timing(void)
    {
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);
    return pulseIn(_echoPin, HIGH, _pingTimeout);
    }

float Ultrasonic::convert(long microsec, int metric)
    {
    // 0.5 * 331.29 * (1 + _temp/273)^0.5 * (microsec * 0.0001) * _multiplier
    float dist = min(0.0165645 * pow(1 + _temp/273, 0.5)
                     * microsec * _multiplier, _maxDistance);

    if(metric) return dist;
    else return dist * 0.3937;
    }

void Ultrasonic::setMultiplier(float value)
    {
    _multiplier = value;
    }

void Ultrasonic::setTemperature(float value)
    {
    _temp = value;
    }

void Ultrasonic::setMaxDistance(int value)
    {
    _maxDistance = min(value, _MAX_CM_DISTANCE);
    _pingTimeout = _maxDistance * _PING_OVERHEAD;
    }

#ifdef COMPILE_STD_DEV
bool Ultrasonic::sampleCreate(size_t numBufs, ...)
    {
    bool result = false;
    va_list ap;
    _numBufs = numBufs;

    if((_pBuffers = (BufCtl *) calloc(numBufs, sizeof(BufCtl))) != NULL)
        {
        va_start(ap, numBufs);
        BufCtl *buf;
        size_t smpSize;

        for(size_t i = 0; i < _numBufs; i++)
            {
            buf = &_pBuffers[i];
            smpSize = va_arg(ap, size_t);

            if((buf->pBegin = (float *) calloc(smpSize, sizeof(float))) != NULL)
                {
                buf->pIndex = buf->pBegin;
                buf->length = smpSize;
                buf->filled = false;
                result = true;
                }
            else
                {
                result = false;
                break;
                }
            }

        va_end(ap);
        }

    if(!result) _freeBuffers();
    return result;
    }

void Ultrasonic::sampleClear()
    {
    if(_pBuffers)
        {
        BufCtl *buf;

        for(size_t i = 0; i < _numBufs; i++)
            {
            buf = &_pBuffers[i];
            memset(buf, '\0', sizeof(float) * buf->length);
            buf->pIndex = buf->pBegin;
            buf->filled = false;
            }
        }
    }

float Ultrasonic::unbiasedStdDev(float value, size_t bufNum)
    {
    float result = 0.0;

    if(_pBuffers)
        {
        BufCtl *buf = &_pBuffers[bufNum];

        if(buf->length > 1)
            {
            _sampleUpdate(buf, float(value));

            if(buf->filled)
                {
                float sum = 0.0, mean, tmp;

                for(size_t i = 0; i < buf->length; i++)
                    sum += buf->pBegin[i];

                mean = sum / buf->length;
                sum = 0.0;

                for(size_t i = 0; i < buf->length; i++)
                    {
                    tmp = buf->pBegin[i] - mean;
                    sum += (tmp * tmp);
                    }

                result = sqrt(sum / (buf->length - 1));
                //Serial.print(bufNum);
                //Serial.print(" : ");
                //Serial.println(result);
                }
            }
        }

    return result;
    }

void Ultrasonic::_sampleUpdate(BufCtl *buf, float msec)
    {
    if(buf->pIndex >= (buf->pBegin + buf->length))
        {
        buf->pIndex = buf->pBegin;
        buf->filled = true;
        }

    *(buf->pIndex++) = msec;
    }

void Ultrasonic::_freeBuffers()
    {
    if(_pBuffers)
        {
        BufCtl *buf;

        for(size_t i = 0; i < _numBufs; i++)
            {
            buf = &_pBuffers[i];
            free(buf->pBegin);
            }

        free(_pBuffers);
        }
    }
#endif // COMPILE_STD_DEV
