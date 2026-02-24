#pragma once
#include <grpcpp/grpcpp.h>

#include <memory>

#include "interfaces/IGrpcStreamClient.h"

class GrpcStreamClient : public IGrpcStreamClient {
 public:
  explicit GrpcStreamClient(std::shared_ptr<grpc::Channel> channel);
  std::unique_ptr<grpc::ClientReaderWriter<ImageDetection::ProcessRequest,
                                           ImageDetection::ProcessResponse>>
  CreateStream(grpc::ClientContext* context) override;

 private:
  std::unique_ptr<ImageDetection::ImageProcessing::Stub> stub_;
};