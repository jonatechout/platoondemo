/**
 * @file Car.hpp
 * @author @jonatechout
 * @brief Base class of car. This is an abstract class.
 */
#ifndef CAR_H
#define CAR_H

#include <math.h>

/**
 * @class Car
 * @brief Base class of car. This is an abstract class.
 */
class Car
{
public:
  struct PositionData
  {
    double timestamp;
    double x;
    double y;
  };

  Car();
  virtual ~Car();

  /**
   * @brief Initialize the state.
   *
   * @param x
   * @param y
   * @param velocity
   * @param heading
   */
  void init(double x, double y, double velocity, double heading);

  /**
   * @brief Set the update period
   *
   * @param period [sec]
   */
  void setPeriod(double period);

  /**
   * @brief Returns distance to given car object.
   *
   * @param target
   * @return double Distance
   */
  double distanceTo(const Car& target) const;

  /**
   * @brief Returns distance to given position data.
   *
   * @param target
   * @return double Distance
   */
  double distanceTo(const PositionData& target) const;

  virtual void update() = 0;

protected:
  double _periodTime;   ///< Update period [s]
  double _currentTime;  ///< Current simulation time [s]
  double _x;            ///< X [m] (world coordinate)
  double _y;            ///< Y [m] (world coordinate)
  double _velocity;     ///< Velocity [m/s]
  double _heading;      ///< Heading angle [rad] (world coordinate, east is 0)

  // Getter
public:
  const double x() const { return _x; }
  const double y() const { return _y; }
  const double velocity() const { return _velocity; }
  const double heading() const { return _heading; }
};

#endif