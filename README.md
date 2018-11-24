Platooning vehicles demo
====

This software provides these features:
- Reading INS data from Oxford RobotCar dataset and estimating the state of ego car by Kalman filter.
- Adding some following cars which follow leading car's path, and dynamically control the speed to keep inter-vehicle distance like simple platooning.
- Visualizing the objects above.

![screenshot](https://user-images.githubusercontent.com/36672072/48968770-a5c7c380-f037-11e8-9c5e-2399ce1ca558.png)

## System
 Tested on Linux mint 17.3 and Ubuntu 18.04.

## Requirement
 g++ and OpenCV is required.
 ```apt-get install g++ libopencv-dev```

 You need Oxford RobotCar dataset: http://robotcar-dataset.robots.ox.ac.uk/downloads/
 INS file format must match the sample data on the web page.

## Build
 ```
 cd platoondemo
 make
 ```

## Simple usage
 1.Run  
 
 ```./platoondemo ./sample_data/ins_cut.csv``` 
     
 2.Visualizer window will open and starts demo automatically.
 
 3.Ctrl+C to exit.

## Detail
 ```platoondemo "INS file name" ["# of followers"]```

INS file name:
 path to the INS file of INS file.

\# of followers
 Number of following cars. (Optional)

## Visualization
 - Oriented circles are cars. (First car is from playback data, others are simulated ones.)
 - Numbers shown near cars are velocity.
 - Blue line is the whole path of playback data.
