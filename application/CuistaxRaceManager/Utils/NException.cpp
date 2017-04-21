//
//  NException.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 21/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#include "NException.hpp"

NException::NException(QString const& message, int code) :
    QException(), _message(message), _code(code)
{
    // Nothing to do here ...
}

NException::NException(NException const& copy) :
    QException(), _message(copy._message), _code(copy._code)
{
    // Nothing to do here ...
}

NException& NException::operator=(NException const& copy)
{
    if (this != &copy)
    {
        this->_message = copy._message;
        this->_code = copy._code;
    }

    return *this;
}

NException::~NException(void) _NOEXCEPT
{
    // Nothing to do here ...
    // No pointer to manage ...
}

void NException::raise(void) const
{
    throw *this;
}

NException* NException::clone(void) const
{
    return new NException(*this);
}

const char* NException::what(void) const _NOEXCEPT
{
    return this->_message.toStdString().c_str();
}

QString NException::message() const _NOEXCEPT
{
    return this->_message;
}

int NException::code(void) const _NOEXCEPT
{
    return this->_code;
}
