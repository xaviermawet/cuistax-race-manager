//
//  NException.hpp
//  CuistaxRaceManager
//
//  Created by Xavier Mawet on 21/04/2017.
//  Copyright © 2017 Nakim. All rights reserved.
//

#ifndef __NEXCEPTION_HPP__
#define __NEXCEPTION_HPP__

#include <QException>

/*!
 * \brief The NException class provides a base class for exceptions that can
 *        transferred across threads.
 */
class NException : public QException
{
    public:

        /*!
         * \brief NException: default constructor
         * \param message: error message
         * \param code: error code
         */
        explicit NException(QString const& message, int code = -1);

        /*!
         * \brief NException: copy constructor
         * \param copy: NException object to copy
         */
        NException(NException const& copy);

        /*!
         * \brief assignation operator overload
         * \param copy : NException object to copy
         */
        virtual NException& operator=(NException const& copy);

        /*!
         * \brief ~NException : virtual destructor
         */
        virtual ~NException(void) _NOEXCEPT;

        /*!
         * \brief raise: supports throwing and catching exceptions across thread boundaries
         */
        void raise(void) const;

        /*!
         * \brief clone: supports throwing and catching exceptions across thread boundaries
         * \return the exception cloned
         */
        NException* clone(void) const;

        /*!
         * \brief what : getter for error message
         * \return : error message
         */
        virtual const char* what(void) const _NOEXCEPT;

        /*!
         * \brief what : getter for error message
         * \return : error message
         */
        virtual QString message(void) const _NOEXCEPT;

        /*!
         * \brief code : getter for error code
         * \return : error code
         */
        virtual int code(void) const _NOEXCEPT;

    protected:

        QString _message;
        int     _code;
};

#endif /* __NEXCEPTION_HPP__ */
