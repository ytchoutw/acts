#pragma once
#include "ActsExamples/TrackFindingExaTrkXTriton/ExaTrkXTriton.hpp"

#include <memory>
#include <string>
#include <vector>

class ExaTrkXTrackFindingTriton
{
public:
 struct TritonConfig {
   std::string modelName;
   std::string url;
   bool verbose = false;

   // hyperparameters in the pipeline.
   int64_t spacepointFeatures = 3;
   int embeddingDim = 8;
   float rVal = 1.6;
   int knnVal = 500;
   float filterCut = 0.21;
 };

 ExaTrkXTrackFindingTriton(const TritonConfig& config);
 virtual ~ExaTrkXTrackFindingTriton() {}

 std::vector<std::vector<int>> getTracks(std::vector<float>& inputValues,
                                         std::vector<int>& spacepointIDs) const;

 const TritonConfig& config() const { return m_cfg; }

private:
 TritonConfig m_cfg;
 std::unique_ptr<ExaTrkXTriton> m_client;
};
