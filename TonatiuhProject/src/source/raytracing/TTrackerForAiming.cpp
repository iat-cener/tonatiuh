

#include "TTrackerForAiming.h"
#include "TSceneKit.h"
#include "Transform.h"
#include "trf.h"


SO_NODEENGINE_ABSTRACT_SOURCE( TTrackerForAiming );

void TTrackerForAiming::initClass()
{
	SO_NODEENGINE_INIT_ABSTRACT_CLASS( TTrackerForAiming, TTracker, "TTracker" );
}

TTrackerForAiming::TTrackerForAiming()
{
	SO_NODEENGINE_CONSTRUCTOR( TTrackerForAiming );

	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Absolute );
	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Relative );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfAimingPoint, AimingPointType );
	SO_NODE_ADD_FIELD( typeOfAimingPoint, (Absolute) );
	SoFieldSensor* m_infoDisplayed = new SoFieldSensor( updateTypeOfAimingPoint, this );
	m_infoDisplayed->attach( &typeOfAimingPoint );
}
/*
inline void TTrackerForAiming::InitAimingPointField()
{
	SO_NODE_ADD_FIELD( aimingPoint, ( 0.0, 0.0, 0.0 ) );
	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Absolute );
	SO_NODE_DEFINE_ENUM_VALUE( AimingPointType, Relative );
	SO_NODE_SET_SF_ENUM_TYPE( typeOfAimingPoint, AimingPointType );
	SO_NODE_ADD_FIELD( typeOfAimingPoint, (Absolute) );
	SoFieldSensor* m_infoDisplayed = new SoFieldSensor( updateTypeOfAimingPoint, this );
	m_infoDisplayed->attach( &typeOfAimingPoint );
}*/


TTrackerForAiming::~TTrackerForAiming()
{

}

void TTrackerForAiming::SetAimingPointRelativity(bool relative)
{
	if ((typeOfAimingPoint.getValue() == 1) != relative)
	{		
		if (relative)
		{
			typeOfAimingPoint.setValue(1);
		}
		else
		{
			typeOfAimingPoint.setValue(0);
		}
	}
}


void TTrackerForAiming::updateTypeOfAimingPoint( void* data, SoSensor* )
{
	TTrackerForAiming* tracker = (TTrackerForAiming *) data;
	tracker->SwitchAimingPointType();
}
