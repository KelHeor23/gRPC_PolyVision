#pragma once
#include <grpcpp/grpcpp.h>
#include <interfaces/IImageEncoder.h>

#include <memory>

#include "interfaces/IImageProcessor.h"
#include "proto/ImageAnalysis.grpc.pb.h"

class ImageProcessingServer final
    : public ImageDetection::ImageProcessing::Service {
 public:
  ImageProcessingServer(std::unique_ptr<IImageEncoder> encoder,
                        std::unique_ptr<IImageProcessor> processor)
      : encoder_(std::move(encoder)), processor_(std::move(processor)) {}

  grpc::Status ProcessImage(
      grpc::ServerContext* context,
      grpc::ServerReaderWriter<ImageDetection::ProcessResponse,
                               ImageDetection::ProcessRequest>* stream)
      override;

 private:
  std::unique_ptr<IImageEncoder> encoder_;
  std::unique_ptr<IImageProcessor> processor_;
};