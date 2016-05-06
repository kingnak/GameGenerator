#include "ggwintrasher.h"

#ifdef Q_OS_WIN
#include <QDebug>
#include <windows.h>

GGWinTrasher::GGWinTrasher()
{

}

bool GGWinTrasher::moveToTrash(const QString &file)
{
    QFileInfo fileinfo( file );
    if( !fileinfo.exists() )
        return false;

    WCHAR from[ MAX_PATH ];
    memset( from, 0, sizeof( from ));
    int l = fileinfo.absoluteFilePath().toWCharArray( from );
    Q_ASSERT( 0 <= l && l < MAX_PATH );
    from[ l ] = '\0';

    SHFILEOPSTRUCT fileop;
    memset( &fileop, 0, sizeof( fileop ) );
    fileop.wFunc = FO_DELETE;
    fileop.pFrom = from;
    fileop.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
    int rv = SHFileOperation( &fileop );
    if( 0 != rv ){
        qDebug() << rv << QString::number( rv ).toInt( 0, 8 );
        return false;
    }
    return true;
}

#endif
