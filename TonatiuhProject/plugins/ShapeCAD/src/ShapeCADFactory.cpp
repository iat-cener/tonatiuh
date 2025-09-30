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


Acknowledgments:

The development of Tonatiuh was started on 2004 by Dr. Manuel J. Blanco,
then Chair of the Department of Engineering of the University of Texas at
Brownsville. From May 2004 to July 2008, it was supported by the Department
of Energy (DOE) and the National Renewable Energy Laboratory (NREL) under
the Minority Research Associate (MURA) Program Subcontract ACQ-4-33623-06.
During 2007, NREL also contributed to the validation of Tonatiuh under the
framework of the Memorandum of Understanding signed with the Spanish
National Renewable Energy Centre (CENER) on February, 20, 2007 (MOU#NREL-07-117).
Since June 2006, the development of Tonatiuh is being led by the CENER, under the
direction of Dr. Blanco, now Director of CENER Solar Thermal Energy Department.

Developers: Manuel J. Blanco (mblanco@cener.com), Amaia Mutuberria, Victor Martin.

Contributors: Javier Garcia-Barberena, Inaki Perez, Inigo Pagola,  Gilda Jimenez,
Juana Amieva, Azael Mancillas, Cesar Cantu, Iñigo Les.
***************************************************************************/

#include <fstream>
#include <sstream>

#include <QFileDialog>
#include <QIcon>
#include <QSettings>
#include <QString>

#include "ShapeCADFactory.h"

QString ShapeCADFactory::TShapeName() const
{
	return QString("CAD_Shape");
}

QIcon ShapeCADFactory::TShapeIcon() const
{
	return QIcon( ":/icons/ShapeCAD.png" );
}

ShapeCAD* ShapeCADFactory::CreateTShape( ) const
{
	static bool firstTime = true;
	if ( firstTime )
	{
		ShapeCAD::initClass();
	    firstTime = false;

	    return ( 0 );
	}

	QSettings settings;
	QString directoryPath = settings.value( QLatin1String("ShapeCAD.dirname" ) ,QLatin1String( "/home") ).toString();


	QString fileName = QFileDialog::getOpenFileName( 0, tr( "Open File"),
			directoryPath,
            tr("Stereolithography files (*.stl);;STEP files(*.step)") );
	if( fileName.isEmpty() )	return ( 0 );

	QFileInfo shapecadFileInfo( fileName );
	if( !shapecadFileInfo.exists() )	return ( 0 );
	settings.setValue( QLatin1String("ShapeCAD.dirname"), shapecadFileInfo.absolutePath() );

	std::vector< Triangle* > facetList;
	bool readOK = ReadSTLFile( fileName, &facetList );
	if( !readOK )
	{
		for( unsigned int f = 0; f < facetList.size(); f++ )
			delete facetList[f];
		facetList.clear();
		return ( 0 );
	}

	ShapeCAD* newShape = new ShapeCAD;
	newShape->SetFacetList( facetList );

	return ( newShape );
}

ShapeCAD* ShapeCADFactory::CreateTShape( int numberOfParameters, QVariant parameters ) const
{

	if( numberOfParameters !=  1 )	return ( 0 );
	QList<QVariant> parametersList = parameters.toList();
	if( parametersList.size() != 1  )	return ( 0 );
	

	QString fileName = parametersList[0].toString();
	if( fileName.isEmpty() )	return( 0 );

	QFileInfo shapecadFileInfo( fileName );
	if( !shapecadFileInfo.exists() )	return ( 0 );

	std::vector< Triangle* > facetList;
	bool readOK = ReadSTLFile( fileName, &facetList );
	if( !readOK )
	{
		for( unsigned int f = 0; f < facetList.size(); f++ )
			delete facetList[f];
		facetList.clear();
		return ( 0 );
	}

	ShapeCAD* newShape = new ShapeCAD;
	newShape->SetFacetList( facetList );

	return ( newShape );
}

bool ShapeCADFactory::ReadSTLFile( QString filename, std::vector< Triangle* >* facetList ) const
{

	int isfacet = 0 ;
	int issolid = 0 ;

	//Check if file is binary or ASCII 
	//If 'solid' and 'facet' are found it is ascii file.
	
	std::ifstream checkIfASCII;
	checkIfASCII.open (filename.toStdString().c_str() );
	while( !checkIfASCII.eof() )
	{
		std::string input;
		getline (checkIfASCII, input);
		
		int facet = input.find("facet", 0);
		int solid = input.find("solid", 0);


		if( facet >=0 )	isfacet = 1;
		if( solid >=0 ) issolid = 1;
	}
	checkIfASCII.close();

	//Binary file
	if( isfacet == 0 || issolid == 0 )
	{
		// bytes: header (char[80])
		char header[80] ; 

		std::ifstream binaryRead;
		binaryRead.open( filename.toStdString().c_str(), std::ios::in|std::ios::binary );
		binaryRead.read(header, 80 );
		
		 //bytes: number of facets (unsigned int)
		unsigned int nOfFacetsL;
		binaryRead.read(reinterpret_cast<char*>(&nOfFacetsL), sizeof(unsigned int));
		int nOfFacets = (int)nOfFacetsL;
		
		char txt[4] ;
		for( int i=0; i<nOfFacets; i++ )
		{
			 // read Normal vectors per facet

			 NormalVector facetNormal;

			 binaryRead.read(txt,4);
			 facetNormal.x= *( (float*) &txt );

			 binaryRead.read(txt,4) ;
			 facetNormal.y= *( (float*) &txt );

			 binaryRead.read(txt,4) ;
			 facetNormal.z= *( (float*) &txt );

			 //read Vertex (x, y, z) coordinates per facet
			 Point3D v1;
		     binaryRead.read( txt, 4 );
		     v1.x = *((float*)&txt);

		  	 binaryRead.read( txt, 4 ) ;
		  	 v1.y = *((float*)&txt);

		  	 binaryRead.read( txt, 4 ) ;
		  	 v1.z = *((float*)&txt);

			 Point3D v2;
		  	 binaryRead.read( txt, 4 ) ;
		  	 v2.x = *((float*)&txt);

		  	 binaryRead.read( txt, 4 ) ;
		  	 v2.y =  *( (float*) &txt );

		  	 binaryRead.read( txt, 4 );
		  	 v2.z = *((float*) &txt );

			 Point3D v3;
		  	 binaryRead.read( txt, 4 );
		  	 v3.x = *( (float*) &txt );

		  	 binaryRead.read( txt, 4 ) ;
		  	 v3.y =  *( (float*) &txt );

		  	 binaryRead.read(txt,4) ;
		  	 v3.z = *( ( float* ) &txt );


			 Triangle*  facet = new Triangle( v1, v2, v3, facetNormal );

		  	 binaryRead.seekg(2, std::ios::cur); // move 2 bytes into the file
		  	 facetList->push_back( facet );
		 }


		 binaryRead.close(); // end of while for binaryRead
	}
	else
	{
		std::string value;

		//A first reading for count the number of facetssa
		std::ifstream first_reading;
		first_reading.open(filename.toStdString().c_str() );


		int nFacets = 0; 
		int nVertex = 0;
		while (!first_reading.eof())
		{
			std::string textline;
			getline( first_reading, textline);
			int ind = textline.find("normal", 0);
			int ind_vrx = textline.find("vertex", 0);

			if( ind >= 0 )	nFacets++;
			if( ind_vrx >= 0 )	nVertex++ ;

		}
		first_reading.close();

		//Read normals and coordinates of vertex
		std::ifstream reader;
		reader.open(filename.toStdString().c_str() );

		int counter = 0 ; // must be set equal to 0 not as  int counter; because it gives weird no.
		
		std::vector<float> normalsVector;		
		std::vector<float> vertexVector;

		while(!reader.eof())
		{
			std::string textline;
			getline(reader, textline);
			counter++;

			int iNormal = textline.find("normal", 0);  // index of 'n' character for a 'normal' in the line
			if( iNormal >= 0 ) 
		 	{
				//For normal line
				std::string normalCoordString = textline.erase(0,iNormal+7); //'normal ' size
				char seperator = ' ';
				
				//Find the first space. After that space the coordinate vaules start
				std::string::size_type prev_pos = 0, pos = 0;
				
				while( ( pos = normalCoordString.find(seperator, pos) ) != std::string::npos )
				{
					std::string substring( normalCoordString.substr(prev_pos, pos-prev_pos) );
					double value;
					std::istringstream is{substring};
					is >> value;
					
					normalsVector.push_back( value );
					prev_pos = ++pos;
				}
				
				double value;
				std::istringstream is{normalCoordString.substr(prev_pos, pos-prev_pos )};
				is >> value;
				normalsVector.push_back( value );
				

		 	} 
			else
			{
				int iVertex = textline.find( "vertex", 0 );  // index of 'v' for any 'vertex' line
				if( iVertex >= 0 ) 
				{
					//For vertex line
					std::string vertexCoordString = textline.erase(0,iVertex+7); //'normal ' size
					char seperator = ' ';
					
					//Find the first space. After that space the coordinate vaules start
					std::string::size_type prev_pos = 0, pos = 0;

					while( ( pos = vertexCoordString.find(seperator, pos) ) != std::string::npos )
					{
						std::string substring( vertexCoordString.substr(prev_pos, pos-prev_pos) );
						
						double value;
						std::istringstream is{substring};
						is >> value;
						
						vertexVector.push_back( value );
						prev_pos = ++pos;
					}
					
					double value;
					std::istringstream is{vertexCoordString.substr(prev_pos, pos-prev_pos )};
					is >> value;
					vertexVector.push_back( value );
					
				} 

			} 
		} // end of while for filereader


		// RESHAPING NORMAL AND VERTEX ARRAYS
		for( int ifacet =0; ifacet < nFacets; ifacet++ ) 
		{
				
			int normalsCoordIndex = ifacet * 3;
			NormalVector normal;
			normal.x = normalsVector[normalsCoordIndex];
			normal.y = normalsVector[normalsCoordIndex+1];
			normal.z = normalsVector[normalsCoordIndex+2];

			int facetCoordIndex = ifacet * 3 * 3;
			Point3D v1;
			v1.x = vertexVector[facetCoordIndex];
			v1.y = vertexVector[facetCoordIndex+1];
			v1.z = vertexVector[facetCoordIndex+2];

			Point3D v2;
			v2.x = vertexVector[facetCoordIndex+3];
			v2.y = vertexVector[facetCoordIndex+4];
			v2.z = vertexVector[facetCoordIndex+5];

			Point3D v3;
			v3.x = vertexVector[facetCoordIndex+6];
			v3.y = vertexVector[facetCoordIndex+7];
			v3.z = vertexVector[facetCoordIndex+8];
			
			Triangle* facet = new Triangle( v1, v2, v3, normal );
		  	facetList->push_back( facet );
		}

		normalsVector.clear();
		vertexVector.clear();
	}

	return ( true );

}

#if QT_VERSION < 0x050000 // pre Qt 5
	Q_EXPORT_PLUGIN2(ShapeCAD, ShapeCADFactory)
#endif

