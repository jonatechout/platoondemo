/**
 * @file main.cpp
 * @author @jonatechout
 * @brief Platooning vehicles demo
 *
 * This software loads INS XY data (from Oxford robotcar dataset) and estimates XY, velocity and heading with Kalman filter.
 * Also generates some simulated cars which follows thier leading cars. Following cars refers positons of leading cars and
 * tries to follow the same path.
 *
 */

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Visualizer.hpp"
#include "PlaybackCar.hpp"
#include "SimCar.hpp"
#include "Car.hpp"

using namespace std;
using namespace std::chrono;

/**
 * @brief Converts Car object to visualizer position
 *
 * @param car
 * @return Visualizer::VisCar
 */
Visualizer::VisCar convertCarToVisCar(const Car &car)
{
  Visualizer::VisCar viscar;
  viscar.position.x = car.x();
  viscar.position.y = car.y();
  viscar.velocity = car.velocity();
  viscar.heading = car.heading();

  return viscar;
}

/**
 * @brief Converts Car position data vector to visualizer line object
 *
 * @param path
 * @return Visualizer::VisLine
 */
Visualizer::VisLine convertPathToVisLine(const vector<PlaybackCar::PositionData> &path)
{
  Visualizer::VisLine visline;

  for (auto &&it = path.begin(); it != path.end(); it++)
  {
    Visualizer::VisPoint pnt;
    pnt.x = it->x;
    pnt.y = it->y;

    visline.points.push_back(pnt);
  }

  return visline;
}

/**
 * @brief Main function
 * INS data file path must be provided as 1st argument.
 * Number of following cars can be provided as 2nd argument. If not, default value is 2.
 */
int main(int argc, char *argv[])
{
  const double period = 0.02;

  if (argc < 2)
  {
    cout << argv[0] << " <INS file name> [<# of followers>]" << endl;
    return -1;
  }

  const char* dataFileName = argv[1];
  int followerNum = 2;
  if (argc >= 3)
  {
    followerNum = atoi(argv[2]);

    if (followerNum < 0)
    {
      cout << "# of followers must be positive." << endl;
      return -1;
    }
  }

  // Ego car loads INS data from data file
  PlaybackCar egoCar;
  if (!egoCar.setData(dataFileName))
  {
    cout << "Failed to load data." << endl;
    return -1;
  }

  egoCar.setPeriod(period);
  egoCar.initKalman();

  // Get a whole path data (thined out) for visualization
  vector<PlaybackCar::PositionData> pathData;
  egoCar.getWholePath(&pathData, 2.0);

  // Initialize following cars
  vector<SimCar> simCarVec(followerNum);
  for (unsigned int i = 0; i < simCarVec.size(); i++)
  {
    simCarVec.at(i).init(pathData.at(0).x, pathData.at(0).y, 0, 0);
    simCarVec.at(i).setPeriod(period);

    if (i == 0)
    {
      // First one follows ego car
      simCarVec.at(i).setLeadingCar(&egoCar);
    }
    else
    {
      // Follows the previous following car
      simCarVec.at(i).setLeadingCar(&simCarVec.at(i - 1));
    }
  }

  // Initialize visualization
  Visualizer vis;
  vis.init(1000, 800, 500, 400, 5.0);
  vis.addPath(convertPathToVisLine(pathData));

  cv::namedWindow("platoondemo", CV_WINDOW_AUTOSIZE);

  int loopCycleMSec = static_cast<int>(period * 1000);
  steady_clock::time_point nextTime = steady_clock::now() + milliseconds(loopCycleMSec);

  // Main loop
  while (1)
  {
    vis.clearObjects();

    // Update the state of ego car
    egoCar.update();

    // Add current object to visualizer
    vis.addObject(convertCarToVisCar(egoCar));

    // Update the state of following car and add them to visualizer
    for (auto&& car = simCarVec.begin(); car != simCarVec.end(); car++)
    {
      car->update();
      vis.addObject(convertCarToVisCar(*car));
    }

    // Set ego car position to Visualizer's center position
    vis.setCameraPosition(egoCar.x(), egoCar.y());

    // Generate visualizaion image
    cv::Mat image;
    vis.getImage(&image);

    cv::imshow("platoondemo", image);
    cv::waitKey(1);

    // Sleep until next time step
    this_thread::sleep_until(nextTime);
    nextTime += milliseconds(loopCycleMSec);
  }
}
