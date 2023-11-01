#include "ActsExamples/TrackFindingExaTrkXTriton/ExaTrkXTrackFindingTriton.hpp"

#include "ActsExamples/TrackFindingExaTrkXTriton/ExaTrkXTriton.hpp"

#include <chrono>

#include "grpc_client.h"
#include "grpc_service.pb.h"
namespace tc = triton::client;

ExaTrkXTrackFindingTriton::ExaTrkXTrackFindingTriton(
    const ExaTrkXTrackFindingTriton::TritonConfig& config)
    : m_cfg(config) {
  bool verbose = false;
  uint32_t client_timeout = 0;
  std::string model_version = "";
  m_client = std::make_unique<ExaTrkXTriton>(
      m_cfg.modelName, m_cfg.url, model_version, client_timeout, verbose);
}

// The main function that runs the Exa.TrkX pipeline
// Be care of sharpe corners.
std::vector<std::vector<int>> ExaTrkXTrackFindingTriton::getTracks(
    std::vector<float>& inputValues, std::vector<int>& spacepointIDs) const {

  std::vector<std::vector<int>> trackCandidates;

  // ************
  // TrackFinding
  // ************
  int64_t numSpacepoints = inputValues.size() / m_cfg.spacepointFeatures;
  std::vector<int64_t> embedInputShape{numSpacepoints,
                                       m_cfg.spacepointFeatures};

  bool status_check = true;

  status_check = m_client->ClearInput();
  // std::cout << "ClearInput Status " << status_check << std::endl;

  std::chrono::steady_clock::time_point t_begin_input =
      std::chrono::steady_clock::now();
  status_check = m_client->AddInput<float>("FEATURES", embedInputShape,
                                        inputValues);

  std::chrono::steady_clock::time_point t_end_input =
      std::chrono::steady_clock::now();
  // std::cout << "AddInput Status " << status_check << std::endl;

  std::cout << "Time difference (Only sent)= "
            << std::chrono::duration_cast<std::chrono::microseconds>(
                   t_end_input - t_begin_input)
                   .count()
            << "[ms]" << std::endl;

  // std::cout << "Number of Spacepoints " << numSpacepoints << std::endl;

  std::vector<int64_t> trackLabels;
  std::vector<int64_t> trackLabelsShape{numSpacepoints, 1};

  std::chrono::steady_clock::time_point t_begin =
      std::chrono::steady_clock::now();

  status_check = m_client->GetOutput<int64_t>("LABELS", trackLabels,
                                           trackLabelsShape);

  std::chrono::steady_clock::time_point t_end =
      std::chrono::steady_clock::now();

  std::cout << "Time difference (Only recieve)= " << std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_begin).count() << "[ms]" << std::endl;

  // std::cout << "GetOutput Status " << status_check << std::endl;

  // std::cout << "size of components: " << trackLabels.size() << std::endl;
//   if (trackLabels.size() == 0)
//     return trackCandidates;
//   trackCandidates.clear();

  int existTrkIdx = 0;
  // map labeling from MCC to customized track id.
  std::map<int32_t, int32_t> trackLableToIds;

  for (int32_t idx = 0; idx < numSpacepoints; ++idx) {
    int32_t trackLabel = trackLabels[idx];
    int spacepointID = spacepointIDs[idx];

    int trkId;
    if (trackLableToIds.find(trackLabel) != trackLableToIds.end()) {
      trkId = trackLableToIds[trackLabel];
      trackCandidates[trkId].push_back(spacepointID);
    } else {
      // a new track, assign the track id
      // and create a vector
      trkId = existTrkIdx;
      trackCandidates.push_back(std::vector<int>{spacepointID});
      trackLableToIds[trackLabel] = trkId;
      existTrkIdx++;
    }
  }

  return trackCandidates;
}
