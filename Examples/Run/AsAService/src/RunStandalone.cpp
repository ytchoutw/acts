#include "Acts/Geometry/GeometryContext.hpp"
#include "ActsExamples/DD4hepDetector/DD4hepDetector.hpp"
#include "ActsExamples/DD4hepDetector/DD4hepGeometryService.hpp"
#include "ActsExamples/EventData/IndexSourceLink.hpp"
#include "ActsExamples/EventData/Measurement.hpp"
#include "ActsExamples/EventData/SimSpacePoint.hpp"
#include "ActsExamples/Io/Csv/CsvMeasurementReader.hpp"
#include "ActsExamples/Io/Json/JsonGeometryList.hpp"
#include "ActsExamples/Io/Root/RootMaterialDecorator.hpp"
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

  std::string oddDir = "/workspace/acts/thirdparty/OpenDataDetector/";
  std::string oddMaterialMap = oddDir + "data/odd-material-maps.root";
  std::string geoSelectionExaTrkX =
      "/workspace/exatrkx-acts-demonstrator/detector/"
      "odd-geo-selection-whole-detector.json";

  // Read measurements
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

  // Read material map
  // DD4hepDetector::Config dd4hepCfg;
  ActsExamples::DD4hep::DD4hepGeometryService::Config dd4hepCfg;
  dd4hepCfg.xmlFileNames = {oddDir + "xml/OpenDataDetector.xml"};

  // Create the DD4hepGeometryService
  ActsExamples::DD4hep::DD4hepDetector dd4hepDetector;
  dd4hepDetector.geometryService =
      std::make_shared<ActsExamples::DD4hep::DD4hepGeometryService>(dd4hepCfg);

  ActsExamples::RootMaterialDecorator::Config rootMatDecConfig;
  rootMatDecConfig.fileName = oddMaterialMap;
  auto mdecorator = std::make_shared<const ActsExamples::RootMaterialDecorator>(
      rootMatDecConfig, Acts::Logging::INFO);

  auto result = dd4hepDetector.finalize(dd4hepCfg, mdecorator);
  // std::pair<TrackingGeometryPtr, ContextDecorators>
  // Access the result values if needed
  auto trackingGeometry = result.first;
  auto contextDecorators = result.second;

  // Acts::GeometryContext geoContext;
  SimSpacePointContainer spacePoints;

  SpacePointMaker::Config spmCfg;
  spmCfg.inputMeasurements = "measurements";
  spmCfg.inputSourceLinks = "sourcelinks";
  spmCfg.outputSpacePoints = "spacepoints";
  spmCfg.trackingGeometry = trackingGeometry;

  // Create the geometry context
  // Not sure if it's correct.
  Acts::GeometryContext geoContext(trackingGeometry);

  std::vector<Acts::GeometryIdentifier> geoIds =
      readJsonGeometryList(geoSelectionExaTrkX);
  spmCfg.geometrySelection = geoIds;
  SpacePointMaker spm(spmCfg, Acts::Logging::INFO);
  // TODO: geoContext should be correctly set
  spm.executeSpacePointsMaker(measurements, sourceLinks, geoContext,
                              spacePoints);

  return 0;
}
