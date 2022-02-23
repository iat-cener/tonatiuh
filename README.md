![http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif](http://tonatiuhdocs.googlepages.com/Logodefinitivo301x115.gif)
# News #

## Tonatiuh release 2.2.4 is now available! ##

In this release some bugs have been solved. The most relevant one is a bugs in ShapeCAD instersection algorithm only visible in some operating systems. In addition, Pillbox distribution function removed from material errors in order to avoid some simulations errors by using this distribution function
 
Find the last release following this link: https://github.com/iat-cener/tonatiuh/releases/tag/v2.2.4

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

From version 2.0.1 the format of the outputs of simulations has been changed to be more flexible for post-processing. You can find a description of the format [Tonatiuh's output files format](https://github.com/iat-cener/tonatiuh/wiki/Output-files-format) .

## Citing Tonatiuh ##

These are some of the most relevant references:
 * Les, I., Mutuberria, A., Schöttl, P., Nitz, P. (2017). New Functionalities for the Tonatiuh Ray-tracing Software. Proceedings of the 23rd SolarPACES Conference.
 * J. Cardoso, J., Mutuberria, A., Marakko, C., Schoettl, P., Osório, T., Les, I.,(2017). New Functionalities for the Tonatiuh Ray-tracing Software. Proceedings of the 23rd SolarPACES Conference.
 * Blanco, M., Mutuberria, A., Monreal, A., & Albert, R. (2011). Results of the empirical validation of Tonatiuh at Mini-Pegase CNRS-PROMES facility. Proc SolarPACES.
 * Blanco, M. J., Mutuberria, A., & Martinez, D. (2010). Experimental validation of Tonatiuh using the Plataforma Solar de Almería secondary concentrator test campaign data. In 16th annual SolarPACES symposium.
 * Blanco, M. J., Mutuberria, A., Garcia, P., Gastesi, R., & Martin, V. (2009). Preliminary validation of Tonatiuh SOLARPACES Symposium. Berlin, Germany.
 * Blanco, M. J., Amieva, J. M., & Mancillas, A. (2005, January). The Tonatiuh Software Development Project: An open source approach to the simulation of solar concentrating systems. In ASME 2005 International Mechanical Engineering Congress and Exposition (pp. 157-164). American Society of Mechanical Engineers.
 
 
<a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'><img src='http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png' alt='Creative Commons License' /></a><br />This work is licensed under a <a href='http://creativecommons.org/licenses/by-nc-sa/3.0/'>Creative Commons Attribution-Noncommercial-Share Alike 3.0 Unported License</a>.
