/***************************************************************************
Copyright (C) 2008 by the Tonatiuh Software Development Team.

This file is part of Tonatiuh.

Tonatiuh program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <QObject>

class QString;
class SoSelection;
class SoSeparator;
class TSceneKit;
class TShapeKit;

//!  Document class stores the application scene.
/*!
 * This class is not yet documented.
*/

class Document : public QObject
{
    Q_OBJECT

public:
    Document();
    ~Document();
    void SetDocumentModified( bool status );
    void New();
    bool ReadFile( const QString& fileName );
    bool WriteFile( const QString& fileName );

    bool IsModified( );
    TSceneKit* GetSceneKit() const;

signals:
    void Warning( QString message );

private:
    TSceneKit* GetSceneKitFromFile( const QString& fileName );
    void InitializeScene();
    void ClearScene();

    TSceneKit* m_scene;
    bool m_isModified;


};

#endif /*DOCUMENT_H_*/
