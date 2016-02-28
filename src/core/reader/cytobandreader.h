#ifndef CYTOBANDREADER_H
#define CYTOBANDREADER_H
#include "abstractregionreader.h"
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
class CytobandReader: public AbstractRegionReader
{
public:
    CytobandReader(const QString& filename);
    virtual bool next() Q_DECL_OVERRIDE;



private:
    QTextStream mStream;

};

}}
#endif // CYTOBANDREADER_H
