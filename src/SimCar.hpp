/**
 * @file SimCar.hpp
 * @author @jonatechout
 * @brief Simulates following cars motion
 *
 */
#ifndef SIMCAR_H
#define SIMCAR_H

#include "Car.hpp"
#include <math.h>
#include <algorithm>
#include <deque>
#include <memory>

/**
 * @class SimCar
 * @brief Simulates following cars motion
 */
class SimCar : public Car
{
public:
  SimCar();
  virtual ~SimCar();

  /**
   * @brief Update vehicle state.
   */
  virtual void update();

  /**
   * @brief Set the Leading Car to follow
   * @param leadingCar Pointer to a Car object to follow
   */
  void setLeadingCar(const Car *leadingCar);

protected:
  /**
   * @brief Get the index of the closest position data in _leadingCarHistory.
   *
   * @param car Reference car
   * @return int Index of history
   */
  int getClosestHitoryIndex(const Car *car);

  /**
   * @brief Get the target tyre angle, directly toward the following point.
   *
   * @param followPoint Destination point
   * @return double Tyre angle to reach for the destination point
   */
  double getTargetTyreAngle(const PositionData &followPoint);

  const Car *_leadingCar; ///< Pointer to the leading car
  std::deque<PositionData> _leadingCarHistory; ///< History of leading car position

};

#endif