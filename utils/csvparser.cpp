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
#include "csvparser.h"

CSVParser::CSVParser(QString split):
    _split(split),
    _new_line('\n'),
    _current_line(0)
{
    append(QStringList()); // first line
}

bool CSVParser::parseFile(QString path,QString split){
    QFile file(path);
    QString data;
    QStringList datas;
    _split = split;
    clear();
    if(file.open(QIODevice::ReadOnly)){
        data=file.readAll();
        datas=data.split("\n",QString::SkipEmptyParts);
        for (int i = 0; i < datas.size(); ++i) {
            append(datas[i].split(split,QString::SkipEmptyParts));
        }
        file.close();
    } else return false ;
}

bool CSVParser::parseFile(QString path){
    return parseFile(path,_split);
}

void  CSVParser::insertion(const QString& value){
    QStringList& list = (*this)[_current_line];
    list<<value;
}

void CSVParser::nextLine(){
    _current_line++;
    if (_current_line == size())
        append(QStringList());
    else if (_current_line > size())
        qWarning()<<"CSV file current line error";
}
void CSVParser::previousLine(){
    if (_current_line > 0)
        _current_line--;
}


bool CSVParser::saveInFile(QString name, QString ext){
    QFile file;
    int i = 1;
    QChar nl = _new_line;
    QString c = _split;
    QString oldname=name;
    file.setFileName(name+"."+ext);
    // Checks if file already exists
    while (file.exists()){
        qWarning()<<"File already exists";
        name = oldname+" ("+QString::number(i)+")";
        file.setFileName(name+"."+ext);
        ++i;
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qWarning()<<"Couldn't open file "<<name;
        exit(0);
    } else {
        QTextStream stream(&file);
        for (int i = 0; i <  size(); ++i) {
            for (int j = 0; j < at(i).size(); ++j) {
                stream<<at(i).at(j)<<c;
            }
            stream<<nl;
        }
        file.close();
        qDebug()<<"File successfully written : "<<file.fileName();
    }

}
