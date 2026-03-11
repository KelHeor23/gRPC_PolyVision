#include "impl/yolo_detector.h"

#include "impl/detection.h"
#include "impl/detection_output_parser.h"
#include "impl/image_preprocessor.h"
#include "impl/inference_backend.h"

YoloDetector::YoloDetector(std::unique_ptr<IImagePreprocessor> preprocessor,
                           std::unique_ptr<IInferenceBackend> backend,
                           std::unique_ptr<IDetectionOutputParser> parser,
                           float confThreshold, float nmsThreshold)
    : preprocessor_(std::move(preprocessor)),
      backend_(std::move(backend)),
      parser_(std::move(parser)),
      confThreshold_(confThreshold),
      nmsThreshold_(nmsThreshold) {}

std::vector<Detection> YoloDetector::Detect(const cv::Mat& image) {
  auto blob = preprocessor_->preprocess(image);
  auto outputs = backend_->forward(blob);
  return parser_->parse(outputs, image.size(), confThreshold_, nmsThreshold_);
}