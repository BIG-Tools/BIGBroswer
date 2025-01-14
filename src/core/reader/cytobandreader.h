#ifndef CYTOBANDREADER_H
#define CYTOBANDREADER_H
#include "abstracttextregionreader.h"
#include <QTextStream>
namespace big {
namespace core {

/*!
 * \brief Region reader for Cytoband file
 *
 * exemple :
 * CytobandReader reader = CytobandReader("file.txt");
 * while ( reader.next())
 * {
 *  qDebug()<<reader.region().chromosom()
 * }
 */
class CytobandReader: public AbstractTextRegionReader
{
public:
    CytobandReader(const QString& filename);
    CytobandReader(QIODevice * device);



protected:
    Region parseLine(const QString &line) const;


private:
    QTextStream mStream;

};

}}
#endif // CYTOBANDREADER_H
