#include "impl/opencv_encoder.h"

#include <opencv2/imgcodecs.hpp>

std::optional<std::vector<uint8_t>> OpenCVEncoder::Encode(
    const cv::Mat& image, const std::string& format, int quality) {
  try {
    std::vector<uint8_t> buffer;
    std::vector<int> params;
    if (format == ".jpg" || format == ".jpeg") {
      params = {cv::IMWRITE_JPEG_QUALITY, quality};
    }
    if (!cv::imencode(format, image, buffer, params)) {
      return std::nullopt;
    }
    return buffer;
  } catch (const cv::Exception& e) {
    return std::nullopt;
  }
}

std::optional<cv::Mat> OpenCVEncoder::Decode(const std::vector<uint8_t>& data,
                                             int flags) {
  try {
    if (data.empty()) return std::nullopt;
    cv::Mat img = cv::imdecode(data, flags);
    if (img.empty()) return std::nullopt;
    return img;
  } catch (const cv::Exception& e) {
    return std::nullopt;
  }
}