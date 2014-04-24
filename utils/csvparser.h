/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

//! class offering methods to parse a csv file
/*!
        This class is used to simply parse and use CSV files. Since CSV have a quite simple structure, this class provides simple access it. \n
        After you call the parsefile function, you can access each line of the file just like you would do with a list, and each element using the element access method on the line
    */
class CSVParser : public QList<QStringList>
{
public:
    explicit CSVParser(QString split=",");
    //! Parses the file given in parameter
    /*!
        \param  path : the path of the CSV file
        \param  split : the separation character betwenn elements
        \return false if the parsing has failed
    */
    bool parseFile(QString path, QString split);
    bool parseFile(QString path);


    CSVParser& operator<<(const QString& value){
        insertion(value);
        return *this;
    }
    CSVParser& operator<<(float value){
        insertion(QString::number(value));
        return *this;
    }

    void nextLine();
    void previousLine();
    bool saveInFile(QString name, QString ext="csv");

private:
    void insertion(const QString& value);
    QString _split;
    QChar _new_line;
    int _current_line;
};

#endif // CSVPARSER_H
