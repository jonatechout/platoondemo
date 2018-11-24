/**
 * @file Car.cpp
 * @author @jonatechout
 * @brief Base class of car. This is an abstract class.
 */

#include "Car.hpp"

Car::Car() : _periodTime(0.02),
             _currentTime(0),
             _x(0),
             _y(0),
             _velocity(0),
             _heading(0)
{
}

Car::~Car()
{
}

void Car::init(double x, double y, double velocity, double heading)
{
  this->_currentTime = 0.0;

  this->_x = x;
  this->_y = y;
  this->_velocity = velocity;
  this->_heading = heading;
}

void Car::setPeriod(double period)
{
  this->_periodTime = period;
}

double Car::distanceTo(const Car& target) const
{
  double distSq =
    (this->_x - target.x()) * (this->_x - target.x()) +
    (this->_y - target.y()) * (this->_y - target.y());

  return sqrt(distSq);
}

double Car::distanceTo(const PositionData& target) const
{
  double distSq =
    (this->_x - target.x) * (this->_x - target.x) +
    (this->_y - target.y) * (this->_y - target.y);

  return sqrt(distSq);
}