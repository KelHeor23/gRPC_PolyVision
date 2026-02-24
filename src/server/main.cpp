#include <grpcpp/grpcpp.h>
#include <impl/OpencvCodec.h>

#include "impl/ImageProcessor.h"
#include "include/processing/ImageProcessingServer.h"

void RunServer() {
  std::string server_address("0.0.0.0:50051");

  ImageProcessingServer service(std::move(std::make_unique<OpenCVEncoder>()),
                                std::move(std::make_unique<ImageProcessor>()));

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}
int main(int argc, char** argv) {
  RunServer();
  return 0;
}