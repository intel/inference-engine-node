#ifndef IE_NODE_UTILS_H
#define IE_NODE_UTILS_H

#include "inference_engine.hpp"

namespace ienodejs {

namespace utils {

bool IsValidLayoutName(const std::string& name);
InferenceEngine::Layout GetLayoutByName(const std::string& name);
std::string GetNameOfLayout(const InferenceEngine::Layout& layout);

bool IsValidPrecisionName(const std::string& name);
InferenceEngine::Precision GetPrecisionByName(const std::string& name);
std::string GetNameOfPrecision(const InferenceEngine::Precision& precision);

}

}

#endif  // IE_NODE_UTILS_H