#include "impl/OpenCVEncoder.h"

#include <opencv2/imgcodecs.hpp>

std::optional<std::vector<uint8_t>> OpenCVEncoder::encode(
    const cv::Mat& image, const std::string& format, int quality) {
  std::vector<uint8_t> buffer;
  std::vector<int> params;
  if (format == ".jpg" || format == ".jpeg") {
    params = {cv::IMWRITE_JPEG_QUALITY, quality};
  }
  if (!cv::imencode(format, image, buffer, params)) {
    return std::nullopt;
  }
  return buffer;
}

std::optional<cv::Mat> OpenCVEncoder::decode(const std::vector<uint8_t>& data,
                                             int flags) {
  cv::Mat img = cv::imdecode(data, flags);
  if (img.empty()) return std::nullopt;
  return img;
}