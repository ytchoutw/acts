#include "ActsExamples/EventData/IndexSourceLink.hpp"
#include "ActsExamples/EventData/Measurement.hpp"
#include "ActsExamples/EventData/SimSpacePoint.hpp"
#include "ActsExamples/Io/Csv/CsvMeasurementReader.hpp"
#include "ActsExamples/TrackFinding/SpacePointMaker.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace Acts;
using namespace ActsExamples;

int main() {
  std::string inputDir =
      "/workspace/exatrkx-acts-demonstrator/output/train_all/";
  std::string measurement_path =
      "/workspace/exatrkx-acts-demonstrator/output/train_all/"
      "event000000000-measurement.csv";
  //   std::cout << "Reading measurements from " << measurement_path <<
  //   std::endl;
  using Measurement = ::Acts::BoundVariantMeasurement;
  CsvMeasurementReader::Config measReaderCfg;
  measReaderCfg.inputDir =
      "/workspace/exatrkx-acts-demonstrator/output/train_all/";
  measReaderCfg.outputMeasurements = "measurements";
  measReaderCfg.outputMeasurementSimHitsMap = "measurement-simhit-map";
  measReaderCfg.outputSourceLinks = "sourcelinks";
  measReaderCfg.outputClusters = "clusters";

  CsvMeasurementReader measReader(measReaderCfg, Acts::Logging::INFO);

  size_t eventNumber = 0;
  GeometryIdMultimap<Measurement> orderedMeasurements;
  IndexMultimap<Index> measurementSimHitsMap;
  IndexSourceLinkContainer sourceLinks;
  MeasurementContainer measurements;
  std::cout << "measReaderCfg.inputDir" << measReaderCfg.inputDir << std::endl;
  std::cout << "eventNumber: " << eventNumber << std::endl;
  measReader.readMeasurement(inputDir, eventNumber, orderedMeasurements,
                             measurementSimHitsMap, sourceLinks, measurements);

  std::cout << "Number of measurements: " << measurements.size() << std::endl;

  return 0;
}