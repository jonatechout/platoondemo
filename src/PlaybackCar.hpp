/**
 * @file PlaybackCar.hpp
 * @author @jonatechout
 * @brief This class can load XY data from given data file (Oxford robotcar dataset).
 */

#ifndef PLAYBACKCAR_H
#define PLAYBACKCAR_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <opencv2/video/tracking.hpp>
#include "Car.hpp"

/**
 * @class PlaybackCar
 * @brief This class can load XY data from given data file (Oxford robotcar dataset).
 * Applies Kalman filter to it and estimates XY, velocity and heading angle.
 * First data point becomes the origin point of XY position, so the initial position of car is always (0,0).
 */
class PlaybackCar : public Car
{
public:
  PlaybackCar();
  virtual ~PlaybackCar();

  /**
   * @brief Update vehicle state.
   */
  virtual void update();

  /**
   * @brief Set the Data file path and load data.
   *
   * @param filepath
   * @return true  Data load has succeeded.
   * @return false  Data load failed.
   */
  bool setData(std::string filepath);

  /**
   * @brief Get the Whole Path
   *
   * @param out_path  Whole path loaded from csv file
   * @param interval  Minimum distance between each points
   */
  void getWholePath(std::vector<PositionData>* out_path, double interval);

  /**
   * @brief Initialize Kalman filter.
   *
   */
  void initKalman();

protected:
  std::vector<PositionData> _data;  ///< Loaded position data
  unsigned int _dataIndex;          ///< Index of playing data

  cv::KalmanFilter _kalman; ///< Kalman filter

  bool _kalmanStateIsInit; ///< True if Kalman filter's pre-state is initialized.
};

#endif