/**
 * @file SimCar.cpp
 * @author @jonatechout
 * @brief Simulates following cars motion
 */
#include "SimCar.hpp"

using namespace std;

SimCar::SimCar() : _leadingCar(nullptr),
                   _leadingCarHistory()
{
}

SimCar::~SimCar()
{
}

void SimCar::update()
{
  const double historyInterval = 0.5;   //Minimum distance between history points
  const int historySize = 100;          //History size
  const double distToFollowPoint = 5.0; //Distance to the point to follow on path
  const double wheelBase = 2.5;         //Wheel base
  const double interVehicleTime = 3.0;  //Target inter-vehicle time to calculate the target distance to leading car
  const double stopDistance = 5.0;      //Distance to stop before leading car
  const double accCoeffDist = 0.05;     //Parameter of acceleration calculation
  const double accCoeffVel = 0.4;       //Parameter of acceleration calculation

  _currentTime += _periodTime;

  if (_leadingCar != nullptr &&
      (_leadingCarHistory.empty() || _leadingCar->distanceTo(_leadingCarHistory.back()) > historyInterval))
  {
    //Store the leading car's position in history data
    PositionData hist;
    hist.timestamp = _currentTime;
    hist.x = _leadingCar->x();
    hist.y = _leadingCar->y();

    _leadingCarHistory.push_back(hist);

    if (_leadingCarHistory.size() > historySize)
    {
      _leadingCarHistory.pop_front();
    }
  }

  if (_leadingCarHistory.empty())
  {
    return;
  }

  // Get the index of closest point in leading car history
  int closestIndex = getClosestHitoryIndex(this);

  // Get the index of point to aim
  int followPointIndex = min(
    static_cast<int>(distToFollowPoint / historyInterval) + closestIndex,
    static_cast<int>(_leadingCarHistory.size()) - 1);

  // Calculate tyre angle to go to the following point
  double tyreAngle = getTargetTyreAngle(_leadingCarHistory.at(followPointIndex));
  double yawrate = _velocity * tan(tyreAngle) / wheelBase;

  // Calculate the distance and speed of leading car
  int leaderClosestIndex = getClosestHitoryIndex(_leadingCar);
  double distToLeader = max((leaderClosestIndex - closestIndex) * historyInterval, 0.0);
  double leaderVel = _leadingCar->velocity();

  double targetRange = _velocity * interVehicleTime + stopDistance;

  // Target acceleration
  double targetAccel = accCoeffDist * (distToLeader - targetRange) + accCoeffVel * (leaderVel - _velocity);

  if (distToLeader < stopDistance)
  {
    // If the leading car is very close, decelerate strongly
    targetAccel = -5.0;
  }

  // Update vehicle state
  _velocity += targetAccel * _periodTime;
  _velocity = max(_velocity, 0.0);

  _x += _velocity * cos(_heading) * _periodTime;
  _y += _velocity * sin(_heading) * _periodTime;

  _heading += yawrate * _periodTime;
}

void SimCar::setLeadingCar(const Car *leadingCar)
{
  if (leadingCar == nullptr)
  {
    return;
  }

  _leadingCarHistory.clear();
  _leadingCar = leadingCar;
}

double SimCar::getTargetTyreAngle(const PositionData& followPoint)
{
  const double tyreAngleLimit = 30.0; //[deg]

  // Follow point on car coordinate
  double followX_c = cos(_heading) * (followPoint.x - _x) + sin(_heading) * (followPoint.y - _y);
  double followY_c = -sin(_heading) * (followPoint.x - _x) + cos(_heading) * (followPoint.y - _y);

  // Tyre angle is directly toward the follwPoint
  double tyreAngle = atan2(followY_c, followX_c);
  tyreAngle = min(tyreAngle, tyreAngleLimit * M_PI / 180.0);
  tyreAngle = max(tyreAngle, -tyreAngleLimit * M_PI / 180.0);

  return tyreAngle;
}

int SimCar::getClosestHitoryIndex(const Car* car)
{
  int closestIndex = -1;
  double minDist = 1e+10;
  for (unsigned int i = 0; i < _leadingCarHistory.size(); i++)
  {
    double currentDist = car->distanceTo(_leadingCarHistory.at(i));
    if (currentDist < minDist)
    {
      closestIndex = i;
      minDist = currentDist;
    }
  }

  return closestIndex;
}