#pragma once
#include <grpcpp/grpcpp.h>

#include "proto/ImageAnalysis.grpc.pb.h"

class IGrpcStreamClient {
 public:
  virtual ~IGrpcStreamClient() = default;
  virtual std::unique_ptr<grpc::ClientReaderWriter<
      ImageDetection::ProcessRequest, ImageDetection::ProcessResponse>>
  CreateStream(grpc::ClientContext* context) = 0;
};