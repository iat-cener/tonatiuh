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

#ifndef TSEPARATORKIT_H_
#define TSEPARATORKIT_H_

#include <Inventor/nodekits/SoSeparatorKit.h>

/*!
  \class TSeparatorKit TSeparatorKit.h TSeparatorKit.h
  \brief The TSeparatorKit class provides a set of others TSeparatorKit and TShapeKit nodes  with a transform and a tracker.


  The TSeparatorKit structure:
  \verbatim
  CLASS TSeparatorKitt
  -->"this"
        "callbackList"
  -->   "topSeparator"
  -->      "pickStyle"
  -->      "appearance"
  -->      "units"
  -->      "transform"
  -->      "texture2Transform"
  -->      "childList"
  -->"tracker"
  \endverbatim

  Detailed information on catalog parts:
  \verbatim
  CLASS SoSeparatorKit
  PVT   "this",  SoSeparatorKit  ---
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ]
  PVT   "topSeparator",  SoSeparator  ---
        "pickStyle",  SoPickStyle  ---
        "appearance",  SoAppearanceKit  ---
        "units",  SoUnits  ---
        "transform",  SoTransform  ---
        "texture2Transform",  SoTexture2Transform  ---
        "childList",  SoNodeKitListPart [ SoShapeKit, SoSeparatorKit ]
        "tracker", TTacker  --- , (default type = TDefaultTracker)
  \endverbatim

*/

class QString;

class TSeparatorKit : public SoSeparatorKit
{
	typedef SoSeparatorKit inherited;

    SO_KIT_HEADER( TSeparatorKit );
 	SO_KIT_CATALOG_ENTRY_HEADER( tracker );

public:
    TSeparatorKit();
    static void initClass();

    virtual SbBool  setPart( const SbName& partname, SoNode* from );
    virtual SoNode* getPart( const SbName &partname, SbBool makeifneeded );

    QString getIcon();

protected:
    virtual ~TSeparatorKit();

};

#endif /*TSEPARATORKIT_H_*/
