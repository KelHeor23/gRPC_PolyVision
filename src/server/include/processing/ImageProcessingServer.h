#pragma once
#include <grpcpp/grpcpp.h>
#include <interfaces/IImageEncoder.h>

#include <memory>

#include "interfaces/IDrawer.h"
#include "interfaces/IPolygonProcessor.h"
#include "proto/ImageAnalysis.grpc.pb.h"

class ImageProcessingServer final
    : public ImageDetection::ImageProcessing::Service {
 public:
  ImageProcessingServer(std::unique_ptr<IImageEncoder> encoder,
                        std::unique_ptr<IDrawer> drawer,
                        std::unique_ptr<IPolygonProcessor> processor)
      : encoder_(std::move(encoder)),
        drawer_(std::move(drawer)),
        processor_(std::move(processor)) {}

  grpc::Status ProcessImage(
      grpc::ServerContext* context,
      grpc::ServerReaderWriter<ImageDetection::ProcessResponse,
                               ImageDetection::ProcessRequest>* stream)
      override;

 private:
  std::unique_ptr<IImageEncoder> encoder_;
  std::unique_ptr<IDrawer> drawer_;
  std::unique_ptr<IPolygonProcessor> processor_;
};