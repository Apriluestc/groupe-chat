/*
 * ========================================================================
 * 
 *      fileName:       dataPool.h
 *      compiler:       g++
 *      description:    对外提供的一个数据池，用于 getMessage 和 putMessage
 *                      其中，class noncopyable 是为了防止拷贝
 *
 *      对外接口模式:   void getMessage(string& inMessage);
 *                      void putMessage(string& putMessage);
 *
 *      usage:
 *          #include "dataPool.h"
 *          DataPool dataPool;
 *          dataPool.getMessage();
 *          dataPool.putMessage();
 *
 *      QQ:             1003625407
 *      email@:             13669186256@163.com
 *
 * ========================================================================
 * */

#ifndef INCLUDE_DATAPOOL_H
#define INCLUDE_DATAPOOL_H

#include <iostream>
#include <vector>
#include <semaphore.h>
#include <boost/noncopyable.hpp>

const int NUM = 256;

class DataPool : boost::noncopyable {
public:
    DataPool() {}
    
    DataPool(int size)
        :_pool(size)
        ,_capacity(size)
    {
        _productStep = 0;
        _consumeStep = 0;
        sem_init(&_blankSem, 0, size);
        sem_init(&_dataSem, 0, 0);
    }
/*
    DataPool(const DataPool& rhx) {
        this->_pool = rhx._pool;
        this->_capacity = rhx._capacity;
        this->_consumeStep = rhx._consumeStep;
        this->_productStep = rhx._productStep;
        this->_blankSem = rhx._blankSem;
        this->_dataSem = rhx._dataSem;
    }
*/
    void getMessage(std::string& inMessage) {
        sem_wait(&_dataSem);
        inMessage = _pool[_consumeStep];
        _consumeStep++;
        _consumeStep %= _capacity;
        sem_post(&_blankSem);
    }

    void putMessage(const std::string& putMessage) {
        sem_wait(&_blankSem);
        _pool[_productStep] = putMessage;
        _productStep++;
        _productStep %= _capacity;
        sem_post(&_dataSem);
    }

    ~DataPool() {
        sem_destroy(&_blankSem);
        sem_destroy(&_dataSem);
    }

private:
    std::vector<std::string> _pool;
    int _capacity;
    int _consumeStep;
    int _productStep;
    sem_t _blankSem;
    sem_t _dataSem;
};

#endif
