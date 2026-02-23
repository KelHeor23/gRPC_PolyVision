#include "impl/OpencvCodec.h"

#include <opencv2/imgcodecs.hpp>

std::vector<uint8_t> OpenCVEncoder::encode(const cv::Mat& image,
                                           const std::string& format,
                                           int quality) {
  std::vector<uint8_t> buffer;
  std::vector<int> params;
  if (format == ".jpg" || format == ".jpeg") {
    params = {cv::IMWRITE_JPEG_QUALITY, quality};
  }
  cv::imencode(format, image, buffer, params);
  return buffer;
}

cv::Mat OpenCVEncoder::decode(const std::vector<uint8_t>& data, int flags) {
  return cv::imdecode(data, flags);
}