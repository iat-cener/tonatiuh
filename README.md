![http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif](http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif)
# News #

## Tonatiuh release 2.2.0 is now available!##
This is a minor relase of Tonatiuh. It introduces a new tool inside Tonatiuh to analyse the flux distribution. Besides two new materials have been implemented. The e new functionalities indroduced to the tool that have been developed in activities that are being performed in subtask 12.2.1 WP12 of STAGE STAE (STAGE-STE project, http://stage-ste.eu/, has received funding from the European Union´s Horizon FP7 under grant agreement nº 609837).

Details about the new functionalities:

  * Flux Distribution: this tool add the capability to analyse the flux distribution in a selected surface. Current version only works in some surfaces: cylinder, flat disk and flat rectangle. Read the about how to use this utility in the following  [link] (https://github.com/iat-cener/tonatiuh/wiki/Flux%20Distribution%20Tool).

  * Angle-Dependent_Refractive_Material and Angle-Dependent_Specular_Material: a new specular and refractive material that include angle dependency for the optic properties.


## First workshop on Tonatiuh ##

First TONATIUH workshop, with title “An introduction to TONATIUH”, has been successfully delivered by Amaia Mutuberria, Senior researcher of CENER Solar Thermal Energy Department.
The course was organized within the activities that are being performed in WP12 of STAGE STAE (STAGE-STE project, http://stage-ste.eu/, has received funding from the European Union´s Horizon FP7 under grant agreement nº 609837).

The workshop was online and took place on 16th and 17th of September, 3 hours course per day. It was a success with a mass participation: more than 20 attenders of 10 different countries.

The main aim of this workshop was to explain how to design and analyze different CSP technologies using TONATIUH. In this workshop R programming language was used for the post-processing of TONATIUH's simulation output files, in order to transform raw photon data into flux distributions estimates, and to present those fluxes estimates as graphs.

Advanced level workshop will take place coming soon (second week of November expected). The main aim of this webinar will be to introduce the users in the development of the source code. After these sessions the attendants will be able to modify the code of the tool and they will be able to develop new plug-ins for TONATIUH.

Mrs. Amaia Mutuberria is a CENER senior researcher with more than 8 years of experience in the field of design and development of software for CSP systems simulations.

Amaia had the cooperation of Mr. Juan Manuel Quintanilla, Researcher of CENER Solar Thermal Energy Department, making ready the examples done during the presentation.

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

From version 2.0.1 the format of the outputs of simulations has been changed to be more flexible for post-processing. You can find a description of the format [Tonatiuh's output files format] (Output files format).


<a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'><img src='http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png' alt='Creative Commons License' /></a><br />This work is licensed under a <a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'>Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License</a>.
