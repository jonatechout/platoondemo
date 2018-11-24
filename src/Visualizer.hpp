/**
 * @file Visualizer.hpp
 * @author @jonatechout
 * @brief Provides simple visualization
 */
#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <vector>
#include <string>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include "Car.hpp"

/**
 * @class Visualizer
 * @brief Simple visualization
 *
 */
class Visualizer
{
public:
  typedef cv::Point2d VisPoint;

  struct VisCar
  {
    VisPoint position;
    double heading;
    double velocity;
  };

  struct VisLine
  {
    std::vector<VisPoint> points;
  };

  Visualizer();
  virtual ~Visualizer();

  /**
   * @brief Initialize class.
   * @param imageSizeX image size x [pix]
   * @param imageSizeY image size y [pix]
   * @param centerX Origin point x on the image [pix]
   * @param centerY Origin point y on the image [pix]
   * @param scale Scale factor
   */
  void init(double imageSizeX, double imageSizeY, double centerX, double centerY, double scale);

  /**
   * @brief Set the Camera position on 2D.
   *
   * @param x X position [m] (world coordinate)
   * @param y Y position [m] (world coordinate)
   */
  void setCameraPosition(double x, double y);

  /**
   * @brief Get visualizer image.
   * @param out_img output image
   */
  void getImage(cv::Mat *out_img);

  /**
   * @brief Clear all the objects.
   */
  void clearObjects();

  /**
   * @brief Add object to Visualizer.
   * @param obj Car object to visualize
   */
  void addObject(const VisCar &obj);

  /**
   * @brief Add line to Visualizer
   * @param line Line object to visualize
   */
  void addPath(const VisLine &line);

  /**
   * @brief Set the Scale factor
   * @param scale
   */
  void setScale(double scale);

protected:
  /**
   * @brief Convert world coordinate into visualizer coordinarte
   * @param x X [m] on world coodinate
   * @param y Y [m] on world coodinate
   * @return Point on visualizer coordinate
   */
  VisPoint toVisCoord(double x, double y);

  /**
   * @brief Convert visualizer coordinarte into world coordinate
   * @param x X on visualizer coodinate
   * @param y Y on visualizer coodinate
   * @return Point on world coordinate
   */
  VisPoint toWorldCoord(double x, double y);

  /**
   * @brief Draw grid on given image
   * @param img
   */
  void drawGrid(cv::Mat &img);

  std::vector<VisCar> _objects; ///< List of ofjects to show on Visualizer
  std::vector<VisLine> _lines;  ///< List of lines to show on Visualizer
  VisPoint _centerPoint;        ///< center point of image [pix]
  VisPoint _imageSize;          ///< image size [pix]
  double _scale;                ///< scale factor
  VisPoint _cameraPos;          ///< Position of camera on world coordinate [m](2D)

  std::map<int, cv::Scalar> _colorChart; ///< Pre-defined color (0: blue, 1: green, 2: red)
};

#endif
