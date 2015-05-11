![http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif](http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif)
# News #

## Tonatiuh release 2.0.1 is now available! ##

This a major release of the program. It introduces major modifications on the usage of Tonatiuh from previous versions of the program. In this new release, the user must decide what ray tracing results to save and how to save them before starting the ray tracing. The results saving options are:

  * _Not export._ Selecting this option instructs Tonatiuh not to save any ray tracing results. This option could be of interest to verify that an optical system has been properly modelled before using the program to trace a large number of rays.
  * _Binary data files._ Selecting this option instructs Tonatiuh to save the ray tracing results into one or several binary files. The format of the binary files vary depending on the information the user wants to store from every photon. The specific format of the binary files produce by Tonatiuh is detailed in an associated ASCII file.
  * _SQL Database._ Selecting this option instructs Tonatiuh to save the ray tracing results in a SQL Database. Different database's tables are use to store different pieces of information. The table entitled "Surfaces" stores the basic identification information of the surfaces the photons are intersecting. The table entitled "Photons" stores the intersection-related information of each photon intersecting the concentrating geometry. Finally, the table entitled "WPhoton" stores the power associated to each photon.

A wiki page will be created in the Wiki section of this website to explain in detail how to use the new features of Tonatiuh version 2.0.1. It is recommended not to switch to this version until obtaining a good understanding of how this new features work.

# Overview #
_The Tonatiuh project aims to create an open source, cutting-edge, accurate, and easy to use Monte Carlo ray tracer for the optical simulation of solar concentrating systems. It intends to advance the state-of-the-art of the simulation tools available for the design and analysis of solar concentrating systems, and to make those tools freely available to anyone interested in using and improving them._
Some of the most relevant design goals of Tonatiuh are:
  * To develop a robust theoretical foundation that will facilitate the optical simulation of almost any type of solar concentrating systems.
  * To exhibit a clean and flexible software architecture, that will allow the user to adapt, expand, increase, and modify its functionalities with ease.
  * To achieve operating system independence at source level, and run on all major platforms with none, or minor, modifications to its source code.
  * To provide the users with an advanced and easy-of-use Graphic User Interface (GUI).
Additional information on the rationale for this open source project, and on the goals, general characteristics, and current status of Tonatiuh is given in the two following videos. The first video is based on a [Pecha Kucha presentation](http://en.wikipedia.org/wiki/Pecha_Kucha) given by Dr. Manuel J. Blanco at the University of Seville in September 2008. Although, the audio track is in Spanish, the video is closed-captioned in Spanih, and subtitled in English. Information on how to use the close-caption/subtitle features of YouTube videos can be found [here](http://help.youtube.com/support/youtube/bin/answer.py?answer=100078). The second video is based on an oral presentation of the paper entitled "Preliminary validation of Tonatiuh" given by Dr. Manuel J. Blanco on September 17th, at the 2009 International Energy Agency's SolarPACES Symposium, celebrated in Berlin, Germany.

| <a href='http://www.youtube.com/watch?feature=player_embedded&v=90FP79pGM_o' target='_blank'><img src='http://img.youtube.com/vi/90FP79pGM_o/0.jpg' width='425' height=344 /></a> |  <a href='http://www.youtube.com/watch?feature=player_embedded&v=DUCgpTnapew' target='_blank'><img src='http://img.youtube.com/vi/DUCgpTnapew/0.jpg' width='425' height=344 /></a>|
|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

## Features ##
The use of extended Open Inventor files to represent the "scene" (i.e. the solar concentrating system, the sunlight model, etc.)
An advance and easy-to-use GUI providing:
  * 3D and tree views of the "scene" to simulate.
  * Handlers and manipulators to modify and query scene objects using 3D views.
  * Interface elements to manage the undo and redo of user actions.
  * Interface elements to define de type of Monte Carlo ray tracing to execute.
A pervasive plugin architecture which allos the user to:
  * Add new sunlight models.
  * Add new geometric surfaces.
  * Add new reflective materials.
  * (planned) Add new refractive materials.
  * (planned) Add new photon map and other results analyzers, and post-processors.
  * (planned) Add new spectrum models.
## Requirements ##
As any other ambitious open source program, Tonatiuh uses and leverages on several existing open source libraries, and tools. The principal open source resources used by Tonatiuh are:
  * Digia Qt for the Graphic User Interface (GUI).
  * Coin3D Toolkit for 3D Graphics Visualization.
  * Marble generic geographical map widget and framework.
  * CPPUnit for testing the code.
All these tools are used for developing Tonatiuh within the Eclipse IDE in a standard development environment used by the entire developing team.
## Tonatiuh's output files format ##
Tonatiuh’s output file is a binary file whose format is independent of the host computer's operating system. The file stores real values one after the other in a block, with no spaces, separators, carriage returns, or line feeds separating the values.
The binary format used to store the values is [Real64](http://en.wikipedia.org/wiki/Double_precision_floating-point_format#IEEE_754_double_precision_binary_floating-point_format:_binary64) with [big-endian byte ordering](http://en.wikipedia.org/wiki/Endianness#Big-endian). The first value stored in the file is the power per photon in watts (W). This real value is then followed by seven-tuples of real values, one per photon .
Every seven-tuple holds the following values in the specific order given:
  * PhotonID
  * x-coordinate
  * y-coordinate
  * z-coordinate
  * surface side ( 0 if is back side , 1 if is front side )
  * Previous PhotonID (0 if the photon is the first of a given ray)
  * Next PhotonID (0 is the photon is the last of a given ray)
Tonatiuh does not provide means for post-processing and representing the results of the ray tracing it carries out.

At [CENER](http://www.cener.com/es/index.asp) we use either Mathematica or R applications for post-processing of Tonatiuh's output binary files, in order to transform raw photon data into flux distributions estimates, and to present those fluxes estimates as graphs.

A typical Mathematica snippet for reading a Tonatiuh output file could be:
``` mathematica
(* Function to Read Tonatiuh's binary output file *)
ReadTonatiuhResults[filename_] := BinaryReadList[ filename, "Real64",  ByteOrdering -> +1];

(* Read Tonatiuh's ouput file and store content in fileData *)
rawData = ReadTonatiuhResults["fileName.dat"];

(* Store the power per photon in powerPerPhoton *)
powerPerPhoton = rawData[[1]]
 
(* Create a matrix storing all the photon seven-tuples at a row per seven-tuple *) 
photonMap = Partition[fileData[[2;;Length[fileData]]],7];

numberOfPhotons = Length[photonMap]
 
....
```
A typical R snippet for reading a Tonatiuh output file could be:
``` r
#Open binary data file.
fileName<- file("filePath/fileName.dat", "rb")

#Store the power per photon in poerPerPhoton variable.
powerPerPhoton <- readBin(fileName, what="numeric", n=1, endian="big")

#Create a vector with photons all the information.
endOfFile<-FALSE
photonData<-vector( mode="numeric")
while( !endOfFile )
{
   photon<-readBin(fileName, what="numeric", n=7, endian="big")
   if( length( photon ) < 7 ) endOfFile = TRUE else photonData<-append( photonData, photon )
}
#Close the data file.
close(fileName)

#Compute the number of photons in the vector. all the photon sextuples at a row per #sextuple 
nPhotons<-length( photonData ) / 7
photonMap<-matrix(  photonData , nrow= nPhotons, ncol=7, byrow=TRUE )

...
```
<a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'><img src='http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png' alt='Creative Commons License' /></a><br />This work is licensed under a <a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'>Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License</a>.
