#include <grpcpp/grpcpp.h>
#include <impl/OpencvCodec.h>

#include "impl/ClassMapper.h"
#include "impl/Drawer.h"
#include "impl/ObjectFilterByPolygon.h"
#include "impl/ObjectsDetecting.h"
#include "impl/PolygonProcessor.h"
#include "impl/YoloDetector.h"
#include "include/processing/ImageProcessingServer.h"

void RunServer() {
  std::string server_address("0.0.0.0:50051");

  Config conf;
  conf.modelWeights = "../../../YOLO/yolov4.weights";
  conf.modelConfig = "../../../YOLO/yolov4.cfg";
  conf.classesFile = "../../../YOLO/coco.names";

  auto classMapper = std::make_shared<ClassMapper>(conf.classesFile);

  auto objectDetector = std::make_unique<ObjectsDetecting>(
      conf, classMapper, std::make_unique<YoloDetector>(conf, classMapper),
      std::make_unique<PolygonProcessor>(),
      std::make_unique<ObjectFilterByPolygon>(), std::make_unique<Drawer>());

  // Включаем отрисовку полигонов для отладки
  objectDetector->setDrawPolygons(true);

  ImageProcessingServer service(std::move(std::make_unique<OpenCVEncoder>()),
                                std::move(objectDetector));

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