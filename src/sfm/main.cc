#include "colmap/exe/sfm.h"

#include "colmap/controllers/automatic_reconstruction.h"
#include "colmap/controllers/bundle_adjustment.h"
#include "colmap/controllers/hierarchical_mapper.h"
#include "colmap/controllers/option_manager.h"
#include "colmap/exe/gui.h"
#include "colmap/scene/reconstruction.h"
#include "colmap/util/misc.h"
#include "colmap/util/opengl_utils.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

// Simple example that reads and writes a reconstruction.
int main(int argc, char** argv) {
  colmap::AutomaticReconstructionController::Options reconstruction_options;
  std::string data_type = "individual";
  std::string quality = "high";
  std::string mesher = "poisson";

  colmap::OptionManager options;
  options.AddRequiredOption("workspace_path",
                            &reconstruction_options.workspace_path);
  options.AddRequiredOption("image_path", &reconstruction_options.image_path);
  options.AddDefaultOption("mask_path", &reconstruction_options.mask_path);
  options.AddDefaultOption("vocab_tree_path",
                           &reconstruction_options.vocab_tree_path);
  options.AddDefaultOption(
      "data_type", &data_type, "{individual, video, internet}");
  options.AddDefaultOption("quality", &quality, "{low, medium, high, extreme}");
  options.AddDefaultOption("camera_model",
                           &reconstruction_options.camera_model);
  options.AddDefaultOption("single_camera",
                           &reconstruction_options.single_camera);
  options.AddDefaultOption("camera_params",
                           &reconstruction_options.camera_params);
  options.AddDefaultOption("sparse", &reconstruction_options.sparse);
  options.AddDefaultOption("dense", &reconstruction_options.dense);
  options.AddDefaultOption("mesher", &mesher, "{poisson, delaunay}");
  options.AddDefaultOption("num_threads", &reconstruction_options.num_threads);
  options.AddDefaultOption("use_gpu", &reconstruction_options.use_gpu);
  options.AddDefaultOption("gpu_index", &reconstruction_options.gpu_index);
  options.Parse(argc, argv);

  colmap::StringToLower(&data_type);
  if (data_type == "individual") {
    reconstruction_options.data_type =
        colmap::AutomaticReconstructionController::DataType::INDIVIDUAL;
  } else if (data_type == "video") {
    reconstruction_options.data_type =
        colmap::AutomaticReconstructionController::DataType::VIDEO;
  } else if (data_type == "internet") {
    reconstruction_options.data_type =
        colmap::AutomaticReconstructionController::DataType::INTERNET;
  } else {
    LOG(FATAL) << "Invalid data type provided";
  }

  colmap::StringToLower(&quality);
  if (quality == "low") {
    reconstruction_options.quality =
        colmap::AutomaticReconstructionController::Quality::LOW;
  } else if (quality == "medium") {
    reconstruction_options.quality =
        colmap::AutomaticReconstructionController::Quality::MEDIUM;
  } else if (quality == "high") {
    reconstruction_options.quality =
        colmap::AutomaticReconstructionController::Quality::HIGH;
  } else if (quality == "extreme") {
    reconstruction_options.quality =
        colmap::AutomaticReconstructionController::Quality::EXTREME;
  } else {
    LOG(FATAL) << "Invalid quality provided";
  }

  colmap::StringToLower(&mesher);
  if (mesher == "poisson") {
    reconstruction_options.mesher =
        colmap::AutomaticReconstructionController::Mesher::POISSON;
  } else if (mesher == "delaunay") {
    reconstruction_options.mesher =
        colmap::AutomaticReconstructionController::Mesher::DELAUNAY;
  } else {
    LOG(FATAL) << "Invalid mesher provided";
  }

  auto reconstruction_manager = std::make_shared<colmap::ReconstructionManager>();

  if (reconstruction_options.use_gpu && colmap::kUseOpenGL) {
    QApplication app(argc, argv);
    colmap::AutomaticReconstructionController controller(reconstruction_options,
                                                 reconstruction_manager);
    RunThreadWithOpenGLContext(&controller);
  } else {
    colmap::AutomaticReconstructionController controller(reconstruction_options,
                                                 reconstruction_manager);
    controller.Start();
    controller.Wait();
  }

  return EXIT_SUCCESS;
}