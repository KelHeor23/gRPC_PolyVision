#pragma once

#include "Config.h"
#include "interfaces/IImageProcessor.h"

class IClassMapper;
class IDetector;
class IObjectFilter;
class IDrawer;

class ObjectsDetecting : public IImageProcessor {
 public:
  explicit ObjectsDetecting(const Config& config,
                            std::shared_ptr<IClassMapper> mapper,
                            std::unique_ptr<IDetector> detector,
                            std::unique_ptr<IObjectFilter> objectFilter,
                            std::unique_ptr<IDrawer> drawer);

  void process(cv::Mat& image, std::vector<ImageDetection::Polygon>& polygons,
               const std::string& fileName) override;

  void setDrawPolygons(bool draw) { drawPolygons_ = draw; }

 private:
  Config config_;
  std::shared_ptr<IClassMapper> classMapper_;
  std::unique_ptr<IDetector> detector_;
  std::unique_ptr<IObjectFilter> objectFilter_;
  std::unique_ptr<IDrawer> drawer_;
  bool drawPolygons_ = false;
};