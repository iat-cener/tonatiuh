TEMPLATE = subdirs


SUBDIRS = ComponentHeliostatField \
			MaterialAngleDependentRefractive \
			MaterialAngleDependentSpecular \
			MaterialBasicRefractive \
			MaterialOneSideSpecular \
			MaterialStandardSpecular \
            MaterialStandardRoughSpecular \
            MaterialVirtual \
			PhotonMapExportDB \
			PhotonMapExportFile \
			PhotonMapExportNull\
			RandomMersenneTwister \
			RandomRngStream \
            ShapeBezierSurface \
			ShapeCAD \
			ShapeCone \
			ShapeCylinder \
			ShapeFlatDisk \
			ShapeFlatRectangle \
            ShapeFlatTriangle \
			ShapeHyperboloid \
			ShapeParabolicDish \
			ShapeParabolicRectangle \
			ShapeSphere \
			ShapeSphericalPolygon \
            ShapeSphericalRectangle \
            ShapeTroughAsymmetricCPC \
			ShapeTroughCHC \
            ShapeTroughCPC \
            ShapeTroughHyperbola \
            ShapeTroughParabola \
            ShapeTrumpet \
            SunshapeBuie \
			SunshapePillbox \
			TrackerHeliostat \
			TrackerHeliostatWithOffsets \
            TrackerLinearFresnel \
			TrackerOneAxis \
			TrackerZAxis \
            TransmissivityATMParameters \
            TransmissivityBallestrin \
            TransmissivityDefault \
            TransmissivityMirval \
            TransmissivitySenguptaNREL \
            TransmissivityVantHull \
            TransmissivityVittitoeBiggs

plugins.target= plugins
plugins.depends = src

QMAKE_EXTRA_TARGETS += plugins