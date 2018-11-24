/**
 * @file Visualizer.cpp
 * @author @jonatechout
 * @brief Provides simple visualization
 */
#include "Visualizer.hpp"

Visualizer::Visualizer() : _centerPoint(400, 400),
                           _imageSize(800, 800),
                           _scale(5.0),
                           _cameraPos(0, 0)
{
  _colorChart[0] = cv::Scalar(255, 0, 0);     //blue
  _colorChart[1] = cv::Scalar(0, 255, 0);     //green
  _colorChart[2] = cv::Scalar(0, 0, 255);     //red
  _colorChart[3] = cv::Scalar(255, 255, 255); //white
  _colorChart[4] = cv::Scalar(120, 120, 120); //light grey
  _colorChart[5] = cv::Scalar(50, 50, 50);    //dark grey
}

Visualizer::~Visualizer()
{
}

void Visualizer::init(double imageSizeX, double imageSizeY, double centerX, double centerY, double scale)
{
  _imageSize = VisPoint(imageSizeX, imageSizeY);
  _centerPoint = VisPoint(centerX, centerY);
  _scale = scale;
}

void Visualizer::setCameraPosition(double x, double y)
{
  _cameraPos.x = x;
  _cameraPos.y = y;
}

void Visualizer::clearObjects()
{
  _objects.clear();
}

void Visualizer::addObject(const VisCar &obj)
{
  _objects.push_back(obj);
}

void Visualizer::addPath(const VisLine &line)
{
  _lines.push_back(line);
}

void Visualizer::getImage(cv::Mat *out_img)
{
  (*out_img) = cv::Mat::zeros(cv::Size(_imageSize.x, _imageSize.y), CV_8UC3);

  drawGrid(*out_img);

  // Draw lines
  for (const auto &line : _lines)
  {
    if (line.points.size() <= 1)
    {
      continue;
    }

    for (unsigned int i = 1; i < line.points.size(); i++)
    {
      VisPoint p = toVisCoord(line.points.at(i - 1).x, line.points.at(i - 1).y);
      VisPoint p2 = toVisCoord(line.points.at(i).x, line.points.at(i).y);

      cv::line(*out_img, p, p2, _colorChart[0]);
    }
  }

  // Draw car objects
  for (const auto &obj : _objects)
  {
    VisPoint p = toVisCoord(obj.position.x, obj.position.y);

    VisPoint p2 = toVisCoord(
        obj.position.x + 3 * cos(obj.heading),
        obj.position.y + 3 * sin(obj.heading));

    cv::circle(*out_img, p, static_cast<int>(_scale), _colorChart[1]);
    cv::line(*out_img, p, p2, _colorChart[1]);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << obj.velocity;
    cv::putText(*out_img, ss.str().c_str(), p + cv::Point2d(10, -10), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                _colorChart[1]);
  }
}

Visualizer::VisPoint Visualizer::toVisCoord(double x, double y)
{
  return VisPoint(-(y - _cameraPos.y), -(x - _cameraPos.x)) * _scale + _centerPoint;
}

Visualizer::VisPoint Visualizer::toWorldCoord(double x, double y)
{
  return VisPoint(-(y - _centerPoint.y), -(x - _centerPoint.x)) * (1.0 / _scale) - _cameraPos;
}

void Visualizer::setScale(double scale)
{
  _scale = scale;
}

void Visualizer::drawGrid(cv::Mat &img)
{
  const double gridSize = 20;

  double gridOffsetX = _cameraPos.y - floor(_cameraPos.y / gridSize) * gridSize;
  double gridOffsetY = _cameraPos.x - floor(_cameraPos.x / gridSize) * gridSize;

  for (unsigned int gridX = 0; gridX < _imageSize.x; gridX += gridSize)
  {
    cv::line(
        img,
        cv::Point2d(static_cast<double>(gridX) + gridOffsetX, 0) * _scale,
        cv::Point2d(static_cast<double>(gridX) + gridOffsetX, _imageSize.y) * _scale,
        _colorChart[5]);
  }

  for (unsigned int gridY = 0; gridY < _imageSize.y; gridY += gridSize)
  {
    cv::line(
        img,
        cv::Point2d(0, static_cast<double>(gridY) + gridOffsetY) * _scale,
        cv::Point2d(_imageSize.x, static_cast<double>(gridY) + gridOffsetY) * _scale,
        _colorChart[5]);
  }
}