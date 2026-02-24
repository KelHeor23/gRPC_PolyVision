#include "impl/GrpcStreamClient.h"

GrpcStreamClient::GrpcStreamClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(ImageDetection::ImageProcessing::NewStub(channel)) {}

std::unique_ptr<grpc::ClientReaderWriter<ImageDetection::ProcessRequest,
                                         ImageDetection::ProcessResponse>>
GrpcStreamClient::CreateStream(grpc::ClientContext* context) {
  return stub_->ProcessImage(context);
}
