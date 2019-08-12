#ifndef __REALSENSE__
#define __REALSENSE__
#include <windows.h>
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

#include <string>

#ifdef WIN32
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem::v1;
#else
#include <filesystem>
namespace filesystem = std::filesystem;
#endif

using namespace cv;


class RealSense
{
private:

	//timer
	time_t timer;
	struct tm calTime;
	clock_t start, end;
	double duration;

	//Recording
	VideoWriter* ColorizedDepthWriter;
	VideoWriter* BinaryDepthWriter;
	VideoWriter* ColorWriter;
	VideoWriter* InfraredWriter;

    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;
	std::string serial_number;
	std::string friendly_name;



    // Color Buffer
    rs2::frame color_frame;
    cv::Mat color_mat;
	uint32_t color_width = 1280;
	uint32_t color_height = 720;
	//uint32_t color_width = 640;
	//uint32_t color_height = 360;
    uint32_t color_fps = 30;

    // Depth Buffer
    rs2::frame depth_frame;
    cv::Mat depth_mat;
	// Scaling
	cv::Mat scale_mat;
	cv::Mat binaryDepth_mat;
	cv::Mat colorizedDepth_mat;
	uint32_t depth_width = 1280;
	uint32_t depth_height = 720;
	//uint32_t depth_width = 640;
	//uint32_t depth_height = 360;
    uint32_t depth_fps = 30;

    // Infrared Buffer
    rs2::frame infrared_frame;
    cv::Mat infrared_mat;
	uint32_t infrared_width = 1280;
	uint32_t infrared_height = 720;
	//uint32_t infrared_width = 640;
	//uint32_t infrared_height = 360;
    uint32_t infrared_fps = 30;

    // File
    std::string file_name = "file.bag";
	filesystem::path directory;
	filesystem::path bag_file;
	char buffer[MAX_PATH];
	std::ostringstream pathname;
	std::string pathnameS;
	std::string inputPath;

public:
    // Constructor
	RealSense(const std::string serial_number, const std::string friendly_name = "");

    // Destructor
    ~RealSense();

    /////////////////// Processing////////////////////////////////

	void recordCheck();
	// Update Data
	void update();
	// Draw Data
	void draw();
	// Show Data
	void show();

	void record();


private:
    // Initialize
    void initialize();

    // Initialize Sensor
    //inline void initializeSensor();


	inline void initializeSensor();

	inline void initializeSave();

    // Finalize
    void finalize(); 

    // Update Frame
    inline void updateFrame();

    // Update Color
    inline void updateColor();

    // Update Depth
    inline void updateDepth();

    // Update Infrared
    inline void updateInfrared();


    // Draw Color
    inline void drawColor();

    // Draw Depth
    inline void drawDepth();

    // Draw Infrared
    inline void drawInfrared();


    // Show Color
    inline void showColor();

    // Show Depth
    inline void showDepth();

    // Show Infrared
    inline void showInfrared();
};

#endif // __REALSENSE__