/*
 * ShapeCADFactory.cpp
 *
 *  Created on: 30/03/2015
 *      Author: amutuberria
 */


#include <string> // library for string handleing
#include <fstream> // library for file handling e.g. reading and writing
#include <stdlib.h> // to include *atof*, string to double

#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QSettings>
#include <QString>

#include "ShapeCADFactory.h"

#include <Inventor/nodes/SoShapeHints.h>
//#include "nodekits/SoSubKitP.h"
//#include "steel.h"

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
            tr("Stereolithography files (*.stl);;STEP files(*.step)")/*,
            QString( QLatin1String("*.stl") )*/ );
	if( fileName.isEmpty() )	return ( 0 );

	QFileInfo shapecadFileInfo( fileName );
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

bool ShapeCADFactory::ReadSTLFile( QString filename, std::vector< Triangle* >* facetList ) const
{
	 std::string input;
	 int facet = 0;
	 int solid = 0 ;
	 int isfacet = 0 ;
	 int issolid = 0 ;

	 // ESTABLISH IF FILE IS BINARY OR ASCII BY LOOKING FOR 'solid' and 'facet' (conatined in STL FILE)

	std::ifstream checkifasciiRead;
	checkifasciiRead.open (filename.toStdString().c_str() );
	while( !checkifasciiRead.eof() )
	{
		getline (checkifasciiRead, input);
		facet = input.find("facet", 0);
		solid = input.find("solid", 0);

		//    cout << facet << " " << solid << endl ;

		if (facet >=0)
		{
			isfacet = 1 ; //cout << "facet found"<< facet<< endl;
		}

		if (solid >=0)
		{
			issolid = 1 ; //cout << "solid found"<< solid << endl;
		}
	}
	checkifasciiRead.close();


    //std::cout << " isfacet: " << isfacet << " issolid: " << issolid << std::endl ;
	 // ENTER IF STATEMENT TO RUN BINARY OR ASCII READ MODE
	if (isfacet == 0 || issolid == 0) // if this condition is met then file is binary stl
	{
		//////////////////// BEGINNING OF BINARY STL READ MODE /////////////////////////

		char header[80] ; // int x;
		char txt[4] ;// char spc[4];

		std::ifstream binaryRead;
		binaryRead.open( filename.toStdString().c_str(), std::ios::in|std::ios::binary );
		binaryRead.read(header, 80 );
		binaryRead.read( txt, 4 );
		unsigned long nOfFacetsL = *((unsigned long*)&txt);

		int nOfFacets = (int)nOfFacetsL;


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
		std::string textline;
		std::string value;

		// FIRST STL READING TO COUNT NUMBER OF FACETS, FACET NORMALS AND COORDINATES
		std::ifstream first_reading;
		first_reading.open(filename.toStdString().c_str() );

		int no_of_facets = 0;
		int no_of_facet_Normals = 0;
		int no_of_facet_xyzCrds = 0;
		int COUNTER=0; int COUNTER_vrx=0;
		while (!first_reading.eof())
		{
			getline (first_reading, textline);
			int ind = textline.find("normal", 0);
			int ind_vrx = textline.find("vertex", 0);

			if (ind>=0)		COUNTER++;
			if (ind_vrx>=0)	COUNTER_vrx++ ;


			no_of_facets = COUNTER ;
			no_of_facet_Normals = no_of_facets*3 ; // WHEN READER IS 100% READY BEST TO COUNT VECTORS TO ESTABLISH STL
			no_of_facet_xyzCrds = COUNTER_vrx*3 ; // VALIDITY

		}
		first_reading.close();
		//      cout << no_of_facets << " facet(s) counted. " << endl ;
		//        cout << no_of_facet_Normals << " facet normals counted. " << endl ;
		//cout << no_of_facet_xyzCrds << " vertex coords counted. " << endl ;


		std::ifstream reader;
		reader.open(filename.toStdString().c_str() );

		int counter = 0 ; // must be set equal to 0 not as  int counter; because it gives weird no.

		int spc_ind[3] ;
		float N_vec[no_of_facet_Normals] ;       // (one-dim) array for Normal vectors of facets
		float Coord_vec [no_of_facet_xyzCrds] ;  // (one-dim) array for Vertex coords of facets
		// Defining Facet normal vectors N_i, N_j and N_k set as float number:
		//float N_i[no_of_facets];
		//float N_j[no_of_facets];
		//float N_k[no_of_facets];

		// Defining Facet vertex points x, y, z set as float numbers
		//  	(3 points/facet or 9 x,y,z ponts/facet):
		//float Vertex_x1[no_of_facets]; float Vertex_x2[no_of_facets]; float Vertex_x3[no_of_facets];
		//float Vertex_y1[no_of_facets]; float Vertex_y2[no_of_facets]; float Vertex_y3[no_of_facets];
		//float Vertex_z1[no_of_facets]; float Vertex_z2[no_of_facets]; float Vertex_z3[no_of_facets];

		//   int spc_step[3] ;
		int n=0;
		int n_vrx=0;

		while(!reader.eof())
		{
			getline(reader, textline);
			counter++;
			//cout << textline << endl;// << " " << linesize << endl ;

			int ind = textline.find("normal", 0);    // get index of 'n' for any 'normal' in the line
			int ind_vrx = textline.find("vertex", 0);  // get index of 'v' for any 'vertex' in the line

			////////////////////////////////////////////////
			if (ind>=0) // if line is for the facet normal coords
		 	{
				int spcv=0;  int no=0;
				int sz = textline.size() ;
				for (int i=ind; i<=sz; i++)
				{
					int ind_space = textline.find(" ",i++) ; // find indices of spaces in line facet normal starting
		                                                        // search from the index of 'n' in 'normal'--> see -for-statement above

					if (ind_space>0 && spcv!=ind_space) // spcv!=ind_space is used to ensure that spc
					{	                           // consists of (three) different indices, one for each
		           		                              // space in between the normal vec (or coordinate) values
						spcv = ind_space ;
						no++ ;
						spc_ind[no-1] = ind_space;  // array for space indices
					}

				}

				std::string value;
				for (int i=0; i<=2; i++)  // for loop to read three numbers N_i, N_j and N_k
				{
					if (i==1)	value.assign(textline, spc_ind[i]+1,sz);
					else	value.assign(textline, spc_ind[i]+1,spc_ind[i+1]-1) ;

					N_vec[n] = atof(value.c_str()) ;
					n++ ; // cout << "n: "<< n << endl ;
				}

		 	} // end of if-line-contains '...normal...'

		/////////////////////////////////////////////////////

		 	  if (ind_vrx>=0) // if line is for vertex coords
		 	  {
		 	   	     int spcv=0;  int no=0;
		 	   		 int sz = textline.size() ;


		 	           for (int i=ind_vrx; i<=sz; i++)
		 	   		   {
		 	   		       int ind_space = textline.find(" ",i++) ; // find indices of spaces in line facet normal starting
		 	                                                          // search from the index of 'n' in 'normal'--> see -for-statement above
		//cout <<"check value: " << sz << endl ;
		 	  // 		                cout << "i_spcv: " << spcv << ", i_ind_space: " << ind_space << endl;

		 	             	if (ind_space>0 && spcv!=ind_space) // spcv!=ind_space is used to ensure that spc
		 	                 	{	                           // consists of (three) different indices, one for each
		 	             		                              // space in between the normal vec (or coordinate) values
		 	   		            spcv = ind_space ;
		 	  // 		                  cout << "spcv: " << spcv << endl;
		 	   		             no++ ;
		 	   		            spc_ind[no-1] = ind_space;  // array for space indices
		 	   		       }

		 	             }

		 	           std::string value ;
		 	           for (int i=0; i<=2; i++)  // for loop to read nine vertex points per facet
		 	           {
		 	        	   if (i==1)	 value.assign(textline, spc_ind[i]+1,sz);
		 	        	   else	value.assign(textline, spc_ind[i]+1,spc_ind[i+1]-1) ;
		 	        	   Coord_vec[n_vrx] = atof(value.c_str()) ;
		 	        	   n_vrx++ ;  // cout << "n_vrx: "<< n_vrx << endl ;

		 	             }
		 	  } // end of if-line-contains '...vertex...'
		 	  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		} // end of while for filereader


		// RESHAPING NORMAL AND VERTEX ARRAYS
		int m = 0;
		int j = 0;
		for( int i=0; i < no_of_facets; i++ ) // reshaping one-dim normal array for Nvec to a
		{
			// two dimensional array: rows:No of Facets, columns: Ni Nj Nk i.e. 3 columns
			NormalVector normal;
			normal.x = N_vec[m];
			normal.y = N_vec[m+1];
			normal.z = N_vec[m+2];
			m = m+3;


			Point3D v1;
			v1.x = Coord_vec[j];
			v1.y = Coord_vec[j+1];
			v1.z = Coord_vec[j+2];

			Point3D v2;
			v2.x = Coord_vec[j+3];
			v2.y = Coord_vec[j+4];
			v2.z = Coord_vec[j+5];

			Point3D v3;
			v3.x = Coord_vec[j+6];
			v3.y = Coord_vec[j+7];
			v3.z = Coord_vec[j+8];
			j = j+9;


			Triangle* facet = new Triangle( v1, v2, v3, normal );
		  	facetList->push_back( facet );
		}
	}


	return ( true );

}

#if QT_VERSION < 0x050000 // pre Qt 5
	Q_EXPORT_PLUGIN2(ShapeCAD, ShapeCADFactory)
#endif

