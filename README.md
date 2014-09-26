Sysiphus Stone
=============
Sysiphus Stone is an interactive video instalation made during the EASTN residency in Tsepelovo for the "Touching the Village" art show that was hold in the village center.

### Concept

In Greek mythology Sisyphus was a king of Ephyra (now known as Corinth) punished for chronic deceitfulness by being compelled to roll an immense boulder up a hill, only to watch it roll back down, and to repeat this action forever.

Albert Camus, in his 1942 essay The Myth of Sisyphus, saw Sisyphus as personifying the absurdity of human life, but Camus concludes "one must imagine Sisyphus happy" as "The struggle itself towards the heights is enough to fill a man's heart." [1]

By interacting with the stone a video loop that always loops back is controled.

[1] http://en.wikipedia.org/wiki/Sisyphus

### Media

http://vimeo.com/105853957


###Team

* Ovidiu - Openframeworks
* Hannes - After Efects, Mad Mapper
* Dimitri - 3DMax
* Anastasia - Rhino

Sisyphus Stone is an interactive video installation, created by Hannes Andersson, Ovidiu Cincheza, Dimitri Miller and Anastasia Pistofidou. The installation was made during the EASTN residency in Tsepelovo for the "Touching the Village" art show, presented in the village centre. As Tsepelovo lies in the Greek mountains and is surrounded by stone, we decided that Sisyphus Stone was an appropriate theme for the installation. 
In Greek mythology Sisyphus was a king of Ephyra (now known as Corinth). As a punishment for his evil deeds, he was doomed to every day role a huge boulder up on a mountain, only to watch it roll down again, when he reached the top (Hard, 2003). 
The installation functioned in a similar concept applied to a projection mapped on a stone fountain. The interaction interface consisted of a stone, which the audience interacted with through physical touch.
 

As the audience touched the stone, the projection started, and if the stone was released, it rolled back to its beginning. The same proceedings further occurred, if the stone was pushed until the end.
The interaction was achieved, measuring the presence of a user with a Kinect, which is a depth camera developed by Microsoft, as a touch less controller for videogames, intended to be used with the Microsoft Xbox. However, the device has been hacked to perform other tasks, such as using the body as interface. It is  also used as a 3D scanner with tools such as Skanect, and Reconstructme. In this particular installation the Kinect holds a rather simple function, as it only measures the absence or presence of a user. The Kinect data is received and interpreted by the Sisyphus Stone Application, which handles the video playback. This application is written in openFrameworks and functions as following: 
A 3D space is created as an environment to interpret the Kinect data. The data received is represented as a point cloud, updated in real time and placed within this environment. The relation between the amount of pixels captured and the total amount of pixel resolution defines the size of the point cloud, in relation to the environment. A point cloud could be described as a 3D object, with only the sides being seen from the angle of the camera. In difference to a 2D object, it still has geometry that can be represented in a 3D environment, where it appears more or less like an invisible object would, if it was spray-painted from one direction, rendering visible the sides touched by the paint.
A 3D object is created to act as a bounding box, to define the area of interest. The bounding box is scaled and placed. Thereby it encompasses the point cloud representation of the stones captured by the Kinect with a slight margin in all directions. Consequently data received by the camera concerning areas corresponding with the areas outside of the bounding box, will be ignored by the system.

Once the bounding box has been placed, the amount of pixels within the bounding box is defined as the idle mode, a user is not touching the stone. In order to not having to change the code every time the installation was made, a button for setting current pixel amount as the idle mode was incorporated in the interface. If something is touching the stone, the pixel number captured within the bounding box will increase, and playback of the audio-visual material is triggered. If the detected pixel amount returns to that corresponding with the idle mode, it acts as a trigger for the audio-visual material to be played backwards until it reaches frame one. 
The reading of the Kinect, definition of the bounding box, and control of the video playback are done in openFrameworks. The audio-visual material was created in After Effects, 3D Studio Max, and Rhino. 

The initial idea was for the physical spatial placement to be done within openFrameworks, as the installation would then be completely stand alone, and not depend on external software. The available tools for spatial placement within openFramework are however not very developed, compared to specialised tools for this specific purpose, we decided to do this with MadMapper, a complimentary or stand alone application for real time fitting of projected material by the use of masks and warping, created by the Swiss company GarageCube. The tool also allows for controlling stage lights using DMX protocol, and if connected to a Canon 5D camera, is able to automatically create masks corresponding with the spatial dimensions, based on an optical analysis of the physical dimensions of a space. In the latest release there has also been an integration of a basic 3D engine. The communication between Mad Mapper and openFrameworks is done via Syphon, an Open Source Mac OS technology allowing for sharing frames in-between software.

