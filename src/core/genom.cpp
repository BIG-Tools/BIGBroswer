#include "genom.h"
#include <QFile>
#include <QDebug>
#include <QRegularExpression>
namespace big {
namespace core {
Genom::Genom(const QString &filename)
    :mFilename(filename)
{
    QFileInfo info(filename);
    mName = info.baseName();
}

int Genom::chromosomCount()
{

}

int Genom::chromosomLength(const QString &chromosom)
{

}

Sequence Genom::sequence(const QString &chromosom, qint64 pos, qint64 length)
{
    // This function generate a Sequence object using a fai index file

    if (!hasIndex())
    {
        qDebug()<<Q_FUNC_INFO<<"fai index has not been created..";
        return Sequence();
    }

    // Load index
    // Read Index file and get param. see createIndex()
    QByteArray name;
    qint64 baseCount;
    qint64 firstOffset;
    qint64 lineBaseSize;
    qint64 lineSize ;

    QFile file(mFilename+".fai");
    if (file.open(QIODevice::ReadOnly))
    {
        // loop over index file and get params
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();

            QByteArrayList list = line.split('\t');
            name = list[0];
            // get params only for the chromosom query
            if (name == chromosom.toUtf8())
            {
                baseCount    = list[1].toInt();
                firstOffset  = list[2].toInt();
                lineBaseSize = list[3].toInt();
                lineSize     = list[4].simplified().toInt();
                break;
            }
        }

        file.close();
        // Now read Original file and make random access memory

        QFile file(mFilename);
        if (file.open(QIODevice::ReadOnly))
        {
            int lineCount = pos / lineBaseSize;
            // Formule pour récupérer la position ... Fausse! Faut corriger ca !

            file.seek((firstOffset + lineCount * lineSize) - lineCount);

            // y a encore des truc qui font chier.. les \n j'arrive faut bien les prendre en compte
            QByteArray seq = file.read(length).simplified();
            return Sequence(seq);

        }


    }

    return Sequence();


}

const QString &Genom::name() const
{
    return mName;
}

const QString &Genom::filename() const
{
    return mFilename;
}

bool Genom::isValid()
{
    return QFile::exists(mFilename);
}

bool Genom::hasIndex()
{
    return QFile::exists(mFilename+".fai");

}

bool Genom::createIndex()
{
    // The purpose of this function is to create an index file <filename.fa>.fai following the rule
    // from http://www.htslib.org/doc/faidx.html

    // This is the genome file
    QFile file(mFilename);

    // This is the index file to be generated
    QFile outputFile(mFilename+".fai");

    // Create a text stream to easily read inside the genome file
    QTextStream stream(&outputFile);

    if (file.open(QIODevice::ReadOnly))
    {
        // How many line of seq for chromosom X
        qint64 lineNumber = 0;

        // How many base of seq for chromosom x
        qint64 baseCount  = 0;

        // The name of chromosome x : eg  chr3
        QByteArray name;

        // The byte offset where the sequence start for chromosom X
        int firstOffset = 0;

        // the line size without endl (depends on platform)
        int lineBaseSize= 0;

        // the line size with endl . (depends on platform)
        int lineSize    = 0;

        //  Loop of genom file and compute for each chromosom :
        //        name	baseCount	firstOffset	lineBaseSize	lineSize
        //        ex:
        //        chr1	550	6	50	51
        //        chr2	550	573	50	51

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();

            // output line to the stream
            if ( line.startsWith(">") && lineNumber != 0)
                stream<<name<<"\t"<<baseCount<<"\t"<<firstOffset<<"\t"<<lineBaseSize<<"\t"<<lineSize<<"\n";

            // Detect line header
            //-------------------------------------
            if (line.startsWith(">"))
            {
                name        = line.right(line.size()-1).simplified();
                firstOffset = file.pos();
            }
            // Otherwise count base
            //-------------------------------------
            else {
                baseCount += line.simplified().size();
                if (!lineSize){
                    lineBaseSize = line.simplified().size();
                    lineSize     =  line.size();
                }
            }

            ++lineNumber;
        }
        // last line ...
        stream<<name<<"\t"<<baseCount<<"\t"<<firstOffset<<"\t"<<lineBaseSize<<"\t"<<lineSize<<"\n";




    }
    file.close();

    // Write results to index file
    if (outputFile.open(QIODevice::WriteOnly))
    {
        stream.flush();
        outputFile.close();

    }
    return true;
}

}}
