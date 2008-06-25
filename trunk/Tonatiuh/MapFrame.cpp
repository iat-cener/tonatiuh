/***************************************************************************
    Copyright (C) 2007 by Manuel J. Blanco, Amaia Mutuberria,             
                          Victor Martin, Javier Garcia-Barberena,         
 			   			   Inaki Perez, Inigo Pagola					   
                                    					  			 	   
    mblanco@cener.com                                                     
                                                                          
    This program is free software; you can redistribute it and/or modify  
    it under the terms of the GNU General Public License as published by  
    the Free Software Foundation; either version 3 of the License, or     
    (at your option) any later version.                                   
                                                                          
    This program is distributed in the hope that it will be useful,       
    but WITHOUT ANY WARRANTY; without even the implied warranty of        
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         
    GNU General Public License for more details.                          
                                                                          
    You should have received a copy of the GNU General Public License     
    along with this program; if not, write to the                         
    Free Software Foundation, Inc.,                                       
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.		   	  
 									  									  	 
    Acknowledgments:							   						  
 									   									  
    The development of Tonatiuh was supported from May 2004 to October    
    2006 by the Department of Energy (DOE) and the National Renewable     
    Energy Laboratory (NREL) under the Minority Research Associate (MURA) 
    Program Subcontract ACQ-4-33623-06 with the University of Texas at    
    Brownsville (UTB).							   						  
 															   			  
    Since June 2006, the development of Tonatiuh is supported by	   	   
    the Spanish National Renewable Energy Centre (CENER), which has 	   
    established a permanent Tonatiuh software development team, under 	   
    the direction of Dr. Manuel J. Blanco, Director of CENER's Solar 	   
    Thermal Energy Department, Full-Professor of UTB, and former 	   	   
    Principal Investigator of the MURA Program Subcontract ACQ-4-33623-06.
 									   									   
    Support for the validation of Tonatiuh is also provided by NREL as	   
    in-kind contribution under the framework of the Memorandum 	   	   
    of Understanding between NREL and CENER (MOU#NREL-07-117) signed on   
    February, 20, 2007.						   						   							   									   
 ***************************************************************************/
 
 #include <QLabel>
 #include <QMouseEvent>
 #include <QVBoxLayout>
 #include <QToolTip>
  
 #include "MapFrame.h"
 
 MapFrame::MapFrame( QWidget * parent )
 :QFrame( parent )
 {
 	QVBoxLayout* layout = new QVBoxLayout;
 	layout->setObjectName( QString::fromUtf8( "layout" ) );
 	layout->setSizeConstraint(QLayout::SetFixedSize);
 	layout->setContentsMargins ( 0, 0, 0, 0 );
 		
 	QLabel* imageWorldData = new QLabel( );
    imageWorldData->setObjectName(QString::fromUtf8("ImageWorldData"));
    imageWorldData->setGeometry(QRect(0, 0, 500, 250));
    //ImageWorldData->setMinimumSize ( 500, 250 ); 
    imageWorldData->setMouseTracking(true);
    imageWorldData->setPixmap(QPixmap(QString::fromUtf8(":/images/mundo.BMP")));
    
    //connect( ImageWorldData, SIGNAL( mouseMoveEvent( QMouseEvent* ) ), this, SLOT(mouseMoveEvent( QMouseEvent* ) ) );
    
    layout->addWidget( imageWorldData );
    
    setLayout( layout );
   	setMouseTracking( true );
    
    gray = new QImage(":/images/WorldMapGray.BMP");
    
    countries.append("Afghanistan");
	countries.push_back("Albania");
	countries.push_back("Algeria");
	countries.push_back("Andorra");
	countries.push_back("EMPTY");
	countries.push_back("Angola");
	countries.push_back("EMPTY");
	countries.push_back("Antarctica");
	countries.push_back("Argentina");
	countries.push_back("EMPTY");
	countries.push_back("Armenia");
	countries.push_back("EMPTY");
	countries.push_back("Australia");
	countries.push_back("Austria");
	countries.push_back("Azerbaijan");
	countries.push_back("EMPTY");
	countries.push_back("Bahrain");
	countries.push_back("Bangladesh");
	countries.push_back("Belarus");
	countries.push_back("EMPTY");
	countries.push_back("Belgium");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Belize");
	countries.push_back("Benin");
	countries.push_back("EMPTY");
	countries.push_back("Bhutan");
	countries.push_back("Bolivia");
	countries.push_back("EMPTY");
	countries.push_back("Bosnia and Herzegovina");
	countries.push_back("Botswana");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Brazil");
	countries.push_back("EMPTY");
	countries.push_back("Bulgaria");
	countries.push_back("Burkina");
	countries.push_back("Burma");
	countries.push_back("EMPTY");
	countries.push_back("Burundi");
	countries.push_back("EMPTY");
	countries.push_back("CAR");
	countries.push_back("Cambodia");
	countries.push_back("Cabinda");
	countries.push_back("Cameroon");
	countries.push_back("EMPTY");
	countries.push_back("Canada");
	countries.push_back("Chad");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Chile");
	countries.push_back("China");
	countries.push_back("Colombia");
	countries.push_back("EMPTY");
	countries.push_back("Congo");
	countries.push_back("EMPTY");
	countries.push_back("Costa Rica");
	countries.push_back("Cote d'Ivoire");
	countries.push_back("Croatia");
	countries.push_back("EMPTY");
	countries.push_back("Cuba");
	countries.push_back("Cyprus");
	countries.push_back("EMPTY");
	countries.push_back("Czech Republic");
	countries.push_back("Denmark");
	countries.push_back("Djibouti");
	countries.push_back("EMPTY");
	countries.push_back("Dominican Republic");
	countries.push_back("Ecuador");
	countries.push_back("EMPTY");
	countries.push_back("Egypt");
	countries.push_back("El Salvador");
	countries.push_back("EMPTY");
	countries.push_back("Equatorial Guinea");
	countries.push_back("Eritrea");
	countries.push_back("EMPTY");
	countries.push_back("Ethiopia");
	countries.push_back("Estonia");
	countries.push_back("Falkland Islands");
	countries.push_back("EMPTY");
	countries.push_back("Fiji");
	countries.push_back("Finland");
	countries.push_back("EMPTY");
	countries.push_back("France");
	countries.push_back("French Guiana");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Gabon");
	countries.push_back("EMPTY");
	countries.push_back("Georgia");
	countries.push_back("Germany");
	countries.push_back("Ghana");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Gibraltar");
	countries.push_back("Greece");
	countries.push_back("Greenland");
	countries.push_back("EMPTY");
	countries.push_back("Guatemala");
	countries.push_back("Guinea");
	countries.push_back("Guinea-Bissau");
	countries.push_back("EMPTY");
	countries.push_back("Guyana");
	countries.push_back("EMPTY");
	countries.push_back("Haiti");
	countries.push_back("Honduras");
	countries.push_back("Hungary");
	countries.push_back("EMPTY");
	countries.push_back("Iceland");
	countries.push_back("India");
	countries.push_back("EMPTY");
	countries.push_back("Indonesia");
	countries.push_back("Iran");
	countries.push_back("EMPTY");
	countries.push_back("Iraq");
	countries.push_back("Ireland");
	countries.push_back("Israel");
	countries.push_back("Italy");
	countries.push_back("EMPTY");
	countries.push_back("Jamaica");
	countries.push_back("EMPTY");
	countries.push_back("Japan");
	countries.push_back("Jordan");
	countries.push_back("EMPTY");
	countries.push_back("Kazakhstan");
	countries.push_back("Kenya");
	countries.push_back("EMPTY");
	countries.push_back("Kuwait");
	countries.push_back("Kyrgyzstan");
	countries.push_back("EMPTY");
	countries.push_back("Laos");
	countries.push_back("Latvia");
	countries.push_back("EMPTY");
	countries.push_back("Lebanon");
	countries.push_back("Liberia");
	countries.push_back("EMPTY");
	countries.push_back("Libya");
	countries.push_back("EMPTY");
	countries.push_back("Liechtenstein");
	countries.push_back("Lithuania");
	countries.push_back("Luxembourg");
	countries.push_back("Macedonia");
	countries.push_back("EMPTY");
	countries.push_back("Madagascar");
	countries.push_back("Malawi");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Malaysia");
	countries.push_back("Mali");
	countries.push_back("Mauritania");
	countries.push_back("Mexico");
	countries.push_back("Moldova");
	countries.push_back("EMPTY");
	countries.push_back("Monaco");
	countries.push_back("EMPTY");
	countries.push_back("Mongolia");
	countries.push_back("Morocco");
	countries.push_back("Mozambique");
	countries.push_back("EMPTY");
	countries.push_back("Namibia");
	countries.push_back("Nepal");
	countries.push_back("Netherlands");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("New Zealand");
	countries.push_back("Nicaragua");
	countries.push_back("Niger");
	countries.push_back("EMPTY");
	countries.push_back("Nigeria");
	countries.push_back("EMPTY");
	countries.push_back("North Korea");
	countries.push_back("Norway");
	countries.push_back("Oman");
	countries.push_back("EMPTY");
	countries.push_back("Pakistan");
	countries.push_back("Panama");
	countries.push_back("Papua New Guinea");
	countries.push_back("EMPTY");
	countries.push_back("Paraguay");
	countries.push_back("Peru");
	countries.push_back("EMPTY");
	countries.push_back("Philippines");
	countries.push_back("Poland");
	countries.push_back("EMPTY");
	countries.push_back("Portugal");
	countries.push_back("Puerto Rico");
	countries.push_back("EMPTY");
	countries.push_back("Qatar");
	countries.push_back("Romania");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Russia");
	countries.push_back("Rwanda");
	countries.push_back("Saudi Arabia");
	countries.push_back("San Marino");
	countries.push_back("Senegal");
	countries.push_back("EMPTY");
	countries.push_back("Serbia and Montenegro");
	countries.push_back("Sierra Leone");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Singapore");
	countries.push_back("Slovakia");
	countries.push_back("Slovenia");
	countries.push_back("Somalia");
	countries.push_back("South Africa");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Lesotho");
	countries.push_back("South Korea");
	countries.push_back("Sri Lanka");
	countries.push_back("Spain");
	countries.push_back("Sudan");
	countries.push_back("EMPTY");
	countries.push_back("Suriname");
	countries.push_back("EMPTY");
	countries.push_back("Swaziland");
	countries.push_back("EMPTY");
	countries.push_back("Sweden");
	countries.push_back("Switzerland");
	countries.push_back("Syria");
	countries.push_back("EMPTY");
	countries.push_back("Taiwan");
	countries.push_back("EMPTY");
	countries.push_back("Tajikistan");
	countries.push_back("Tanzania");
	countries.push_back("Thailand");
	countries.push_back("EMPTY");
	countries.push_back("Togo");
	countries.push_back("Tunisia");
	countries.push_back("Turkmenistan");
	countries.push_back("Turkey");
	countries.push_back("UAE");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("United Kingdom");
	countries.push_back("USA");
	countries.push_back("EMPTY");
	countries.push_back("Uganda");
	countries.push_back("Ukraine");
	countries.push_back("EMPTY");
	countries.push_back("Uruguay");
	countries.push_back("Uzbekistan");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
	countries.push_back("Venezuela");
	countries.push_back("Vietnam");
	countries.push_back("Western Sahara");
	countries.push_back("Yemen");
	countries.push_back("EMPTY");
	countries.push_back("Zaire");
	countries.push_back("EMPTY");
	countries.push_back("Zambia");
	countries.push_back("Zimbabwe");
	countries.push_back("EMPTY");
	countries.push_back("EMPTY");
 }
 
MapFrame::~MapFrame()
{
 	
}
 
void MapFrame::mouseMoveEvent( QMouseEvent* e)
{

	int x=e->x();
	int y=e->y();
	

	// latitude and longitude
	int height=frameRect().height();
	double dLatitude = int( 10 * ( ( -180.0/(height -1) ) * y + 90.0 ) )/10.0;

	int width=frameRect().width();
	double dLongitude = int( 10 * ( ( 360.0/(width -1) ) * x - 180.0 ) )/10.0;


	//Change tool tip	
	QString country= Country(x,y);
	
	if (!(country=="EMPTY"))
		QToolTip::showText(e->globalPos(),country);
	else
		QToolTip::showText(e->globalPos(),"");

	emit positionChanged(dLongitude,dLatitude);
}

void MapFrame::mousePressEvent ( QMouseEvent* e)
{

	int x=e->x();
	int y=e->y();
	
	// latitude and longitude
	int height=frameRect().height();
	double dLatitude = int( 10 * ( ( -180.0/(height -1) ) * y + 90.0 ) )/10.0;

	int width=frameRect().width();
	double dLongitude = int( 10 * ( ( 360.0/(width -1) ) * x - 180.0 ) )/10.0;



	emit positionSelected( dLongitude, dLatitude );
		
}

QString MapFrame::Country(int x, int y)
{
	QRgb color=gray->pixel( x, y );
	int gray = qGray( color );
	
	
	QString country=(QString)countries.at( gray );
	return country;
}
