/**
 * @file PlaybackCar.cpp
 * @author @jonatechout
 */
#include "PlaybackCar.hpp"

using namespace std;
using namespace cv;

PlaybackCar::PlaybackCar() : _dataIndex(0),
                             _kalman(4, 2),
                             _kalmanStateIsInit(false)
{
}

PlaybackCar::~PlaybackCar()
{
}

void PlaybackCar::update()
{
  if (_data.size() == 0 || _dataIndex >= _data.size() - 1)
  {
    return;
  }

  _currentTime += _periodTime;

  // If next data time is already passed, search for the latest data.
  if (_data.at(_dataIndex + 1).timestamp <= _currentTime)
  {
    while (_dataIndex < _data.size() - 1 && _data.at(_dataIndex + 1).timestamp <= _currentTime)
    {
      _dataIndex++;
    }

    // Kalman filter measurement
    Mat measurement = Mat::zeros(2, 1, CV_32F);
    measurement.at<float>(0) = _data.at(_dataIndex).x;
    measurement.at<float>(1) = _data.at(_dataIndex).y;

    // If Kalman filter pre-state is not initialized, put the current measurement.
    if (!_kalmanStateIsInit)
    {
      _kalman.statePre.at<float>(0) = measurement.at<float>(0);
      _kalman.statePre.at<float>(1) = measurement.at<float>(1);
      _kalman.statePre.at<float>(2) = 0.0;
      _kalman.statePre.at<float>(3) = 0.0;

      _kalmanStateIsInit = true;
    }

    // Kalman filter update measurement
    _kalman.correct(measurement);
  }

  if (_kalmanStateIsInit)
  {
    // Kalman filter prediction
    Mat estimatedState = _kalman.predict();

    // Update state using predicted state
    _x = estimatedState.at<float>(0);
    _y = estimatedState.at<float>(1);

    double vx = estimatedState.at<float>(2);
    double vy = estimatedState.at<float>(3);

    _velocity = sqrt(vx * vx + vy * vy);

    // Heading should change only when car is moving.
    if (_velocity > 0.5)
    {
      // Assuming moving angle matches heading anble. (This is not strictly correct, but good enough for low speed.)
      _heading = atan2(vy, vx);
    }
  }
}

bool PlaybackCar::setData(string filepath)
{
  _data.clear();

  ifstream ifs(filepath.c_str());

  if (ifs.fail())
  {
    cout << "Failed to open file: " << filepath << endl;
    return false;
  }

  string line;
  getline(ifs, line); // Ignore the header line

  long firstTimestamp = 0;
  double firstX = 0.0;
  double firstY = 0.0;

  // Parse CSV file
  while (getline(ifs, line))
  {
    std::string item;
    std::istringstream linestream(line);

    int col = 0;
    PositionData recdata;

    while (getline(linestream, item, ','))
    {
      if (col == 0)
      {
        if (_data.empty())
        {
          firstTimestamp = stol(item); //First data
        }

        long timestamp = stol(item);

        //Calculate time difference from first timestamp
        recdata.timestamp = static_cast<double>(timestamp - firstTimestamp) * 1e-6; //[sec]
      }
      else if (col == 5)
      {
        if (_data.empty())
        {
          firstY = stod(item); // First data
        }

        recdata.y = stod(item) - firstY; //Northing
      }
      else if (col == 6)
      {
        if (_data.empty())
        {
          firstX = stod(item); // First data
        }

        recdata.x = stod(item) - firstX; //Easting
      }

      col++;
    }

    // Number of columns is wrong
    if (col != 15)
    {
      cout << "Wrong file format: " << filepath << endl;
      _data.clear();

      return false;
    }

    _data.push_back(recdata);
  }

  return true;
}

void PlaybackCar::getWholePath(vector<PositionData> *out_path, double interval)
{
  out_path->clear();

  if (_data.size() == 0)
  {
    return;
  }

  out_path->push_back(_data.at(0));

  // Search all the loaded points
  for (unsigned int i = 0; i < _data.size(); i++)
  {
    PositionData currentPoint = _data.at(i);
    PositionData prevPoint = out_path->back();

    double distSq =
        (currentPoint.x - prevPoint.x) * (currentPoint.x - prevPoint.x) +
        (currentPoint.y - prevPoint.y) * (currentPoint.y - prevPoint.y);

    // If the distance between current point and last point is bigger than interval, add the point to path.
    if (distSq > interval * interval)
    {
      out_path->push_back(currentPoint);
    }
  }
}

void PlaybackCar::initKalman()
{
  _kalmanStateIsInit = false;

  _kalman.transitionMatrix =
    (Mat_<float>(4, 4) <<
      1,      0,      _periodTime,  0,
      0,      1,      0,            _periodTime,
      0,      0,      1,            0,
      0,      0,      0,            1);

  setIdentity(_kalman.measurementMatrix);
  setIdentity(_kalman.processNoiseCov, Scalar::all(_periodTime * 0.1));
  setIdentity(_kalman.measurementNoiseCov, Scalar::all(0.5));
  setIdentity(_kalman.errorCovPost, Scalar::all(0.1));
}