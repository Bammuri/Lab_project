#include "realsense.h"

#include <windows.h>
#include <string>

#define RECORD


// Constructor
RealSense::RealSense(const std::string serial_number, const std::string friendly_name)
	: serial_number(serial_number)
	, friendly_name(friendly_name)
{
	// Initialize
	initialize();
}

// Destructor
RealSense::~RealSense()
{
    // Finalize
    finalize();
}


// Initialize
void RealSense::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Sensor
    initializeSensor();

}

std::string replace_all(
	__in const std::string &message,
	__in const std::string &pattern,
	__in const std::string &replace
) {

	std::string result = message;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != std::string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}




// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
	config.enable_device(serial_number);
#ifdef RECORD
    config.enable_stream( rs2_stream::RS2_STREAM_COLOR, color_width, color_height, rs2_format::RS2_FORMAT_BGR8, color_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_DEPTH, depth_width, depth_height, rs2_format::RS2_FORMAT_Z16, depth_fps );
    config.enable_stream( rs2_stream::RS2_STREAM_INFRARED, infrared_width, infrared_height, rs2_format::RS2_FORMAT_Y8, infrared_fps );
	

#else
    // Set Play File
    config.enable_device_from_file( file_name );
#endif

    // Start Pipeline
    pipeline_profile = pipeline.start( config );

	//initialize save
	initializeSave();
	
	// Create Save Directory, File Name and Initialize record
	timer = time(NULL);
	localtime_s(&calTime, &timer);
	
	 
	pathname << directory.generic_string() << "/Depth/";
	pathname << "ColorizedDepth"<<"-"<< serial_number << "-" <<calTime.tm_year + 1900<<"-"<<calTime.tm_mon + 1<< "-" << calTime.tm_mday<<"_" << calTime.tm_hour<< "-"<< calTime.tm_min<< "-" << calTime.tm_sec<<".avi";
	pathnameS = pathname.str();
	
	pathnameS =replace_all(pathnameS, "/", "\\");


	ColorizedDepthWriter = new VideoWriter;
	if (!ColorizedDepthWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(depth_width, depth_height), true)) {
		delete ColorizedDepthWriter;
		return;
	}

	pathname.str("");
	pathname.clear();
	pathnameS.clear();

	pathname << directory.generic_string() << "/Depth/";
	pathname << "BinaryDepth" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec << ".avi";
	pathnameS = pathname.str();

	pathnameS = replace_all(pathnameS, "/", "\\");

	BinaryDepthWriter = new VideoWriter;
	if (!BinaryDepthWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(depth_width, depth_height), false)) {
		delete BinaryDepthWriter;
		return;
	}

	pathname.str("");
	pathname.clear();
	pathnameS.clear();

	pathname << directory.generic_string() << "/Color/";
	pathname << "Color" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec <<  ".avi";
	pathnameS = pathname.str();
	
	pathnameS = replace_all(pathnameS, "/", "\\");

	ColorWriter = new VideoWriter;
	if (!ColorWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(color_width, color_height), true)) {
		delete ColorWriter;
		return;
	}

	pathname.str("");
	pathname.clear();
	pathnameS.clear();

	pathname << directory.generic_string() << "/Infrared 2/";
	pathname << "Infrared" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec <<  ".avi";
	pathnameS = pathname.str();
	
	pathnameS = replace_all(pathnameS, "/", "\\");

	InfraredWriter = new VideoWriter;
	if (!InfraredWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(infrared_width, infrared_height), false)) {
		delete InfraredWriter;
		return;
	}

	pathname.str("");
	pathname.clear();
	pathnameS.clear();

	// start timer
	start = clock();

	//set  Laser MAX power
	
	rs2::device selected_device = pipeline_profile.get_device();
	auto depth_sensor = selected_device.first<rs2::depth_sensor>();

	if (depth_sensor.supports(RS2_OPTION_EMITTER_ENABLED))
	{
		depth_sensor.set_option(RS2_OPTION_EMITTER_ENABLED, 1.f); // Enable emitter
	}
	if (depth_sensor.supports(RS2_OPTION_LASER_POWER))
	{
		// Query min and max values:
		auto range = depth_sensor.get_option_range(RS2_OPTION_LASER_POWER);
		

		depth_sensor.set_option(RS2_OPTION_LASER_POWER, range.max); // Set max power
	}

}




// Initialize Save
inline void RealSense::initializeSave()
{
	timer = time(NULL);
	localtime_s(&calTime, &timer);
	std::ostringstream oss;
	oss << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << " " << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec;
	

	GetModuleFileName(NULL, buffer, MAX_PATH);
	inputPath = buffer;
	bag_file = inputPath;

	// Create Root Directory (Bag File Name)
	directory = bag_file.parent_path().generic_string() + "/" + bag_file.stem().string();
	filesystem::create_directories(directory);

	// Create Sub Directory for Each Streams (Stream Name)
	const std::vector<rs2::stream_profile> stream_profiles = pipeline_profile.get_streams();
	for (const rs2::stream_profile stream_profile : stream_profiles) {
		filesystem::path sub_directory = directory.generic_string() + "/" + stream_profile.stream_name();
		filesystem::create_directories(sub_directory);
	}
}

// Finalize
void RealSense::finalize()
{
    //Delete Record
	delete ColorizedDepthWriter;
	delete BinaryDepthWriter;
	delete ColorWriter;
	delete InfraredWriter;
	
	// Close Windows
    cv::destroyAllWindows();

    // Stop Pipline
    pipeline.stop();
}

void RealSense::recordCheck()
{
	timer = time(NULL);
	localtime_s(&calTime, &timer);
	std::ostringstream oss;
	oss <<calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << " " << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec << "S";
	system("cls");
	std::cout << oss.str() << std::endl;


	end = clock();
    duration = (double)(end - start) / CLOCKS_PER_SEC;

	//check duration whether over 10min making new file
	if (duration >= 600) {

		
		pathname.str("");
		pathname.clear();
		pathnameS.clear();
		delete ColorizedDepthWriter;
		pathname << directory.generic_string() << "/Depth/";
		pathname << "ColorizedDepth" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec << ".avi";
		pathnameS = pathname.str();

		pathnameS = replace_all(pathnameS, "/", "\\");

		ColorizedDepthWriter = new VideoWriter;
		if (!ColorizedDepthWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(depth_width, depth_height), true)) {
			delete ColorizedDepthWriter;
			return;
		}

		pathname.str("");
		pathname.clear();
		pathnameS.clear();

		delete BinaryDepthWriter;
		pathname << directory.generic_string() << "/Depth/";
		pathname << "BinaryDepth" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec << ".avi";
		pathnameS = pathname.str();

		pathnameS = replace_all(pathnameS, "/", "\\");

		BinaryDepthWriter = new VideoWriter;
		if (!BinaryDepthWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(depth_width, depth_height), false)) {
			delete BinaryDepthWriter;
			return;
		}

		pathname.str("");
		pathname.clear();
		pathnameS.clear();



		delete ColorWriter;
		pathname << directory.generic_string() << "/Color/";
		pathname << "Color" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec <<  ".avi";
		pathnameS = pathname.str();

		pathnameS = replace_all(pathnameS, "/", "\\");

		ColorWriter = new VideoWriter;
		if (!ColorWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(color_width, color_height), true)) {
			delete ColorWriter;
			return;
		}

		pathname.str("");
		pathname.clear();
		pathnameS.clear();


		delete InfraredWriter;
		pathname << directory.generic_string() << "/Infrared 2/";
		pathname << "Infrared" << "-" << serial_number << "-" << calTime.tm_year + 1900 << "-" << calTime.tm_mon + 1 << "-" << calTime.tm_mday << "_" << calTime.tm_hour << "-" << calTime.tm_min << "-" << calTime.tm_sec <<  ".avi";
		pathnameS = pathname.str();

		pathnameS = replace_all(pathnameS, "/", "\\");

		InfraredWriter = new VideoWriter;
		if (!InfraredWriter->open(pathnameS, VideoWriter::fourcc('X', 'V', 'I', 'D'), 30, Size(infrared_width, infrared_height), false)) {
			delete InfraredWriter;
			return;
		}

		pathname.str("");
		pathname.clear();
		pathnameS.clear();


		//initializeClock
		start = clock();
	}
	

}




// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Color
    updateColor();

    // Update Depth
    updateDepth();

    // Update Infrared
    updateInfrared();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Color
inline void RealSense::updateColor()
{
    // Retrieve Color Flame
    color_frame = frameset.get_color_frame();

    // Retrive Frame Size
    color_width = color_frame.as<rs2::video_frame>().get_width();
    color_height = color_frame.as<rs2::video_frame>().get_height();
}

// Update Depth
inline void RealSense::updateDepth()
{
    // Retrieve Depth Flame
    depth_frame = frameset.get_depth_frame();

    // Retrive Frame Size
    depth_width = depth_frame.as<rs2::video_frame>().get_width();
    depth_height = depth_frame.as<rs2::video_frame>().get_height();
}

// Update Infrared
inline void RealSense::updateInfrared()
{
    // Retrieve Infrared Flame
    infrared_frame = frameset.first( rs2_stream::RS2_STREAM_INFRARED );

    // Retrive Frame Size
    infrared_width = infrared_frame.as<rs2::video_frame>().get_width();
    infrared_height = infrared_frame.as<rs2::video_frame>().get_height();
}

// Draw Data
void RealSense::draw()
{
    // Draw Color
    drawColor();

    // Draw Depth
    drawDepth();

    // Draw Infrared
    drawInfrared();
}

// Draw Color
inline void RealSense::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) );
}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16SC1, const_cast<void*>( depth_frame.get_data() ) );

}

// Draw Infrared
inline void RealSense::drawInfrared()
{
    // Create cv::Mat form Infrared Frame
    infrared_mat = cv::Mat( infrared_height, infrared_width, CV_8UC1, const_cast<void*>( infrared_frame.get_data() ) );
}

// Show Data
void RealSense::show()
{
	
    // Show Color
    showColor();

    // Show Depth
    showDepth();

    // Show Infrared
    showInfrared();

}

// Show Color
inline void RealSense::showColor()
{
	//Capture
    if( color_mat.empty() ){
        return;
    }

    // Show Color Image
	*ColorWriter << color_mat;

	cv::imshow("Color - " + friendly_name + " (" + serial_number + ")", color_mat);


}

// Show Depth
inline void RealSense::showDepth()
{

    if( depth_mat.empty() ){
        return;
    }
 
    depth_mat.convertTo( scale_mat, CV_8U, -700 / 10000.0, 255.0 ); // 0-10000 -> 255(white)-0(black)
    //depth_mat.convertTo( scale_mat, CV_8U, 255.0 / 10000.0, 0.0 ); // 0-10000 -> 0(black)-255(white)

    // Apply False Colour
	binaryDepth_mat = scale_mat;
    cv::applyColorMap( scale_mat, colorizedDepth_mat, cv::COLORMAP_JET);

    // Show Depth Image
	*BinaryDepthWriter << binaryDepth_mat;
	*ColorizedDepthWriter << colorizedDepth_mat;

	cv::imshow("colorizedDepth - " + friendly_name + " (" + serial_number + " )", colorizedDepth_mat);
	cv::imshow("binaryDepth - " + friendly_name + " (" + serial_number + " )", binaryDepth_mat);

	
	
}

// Show Infrared
inline void RealSense::showInfrared()
{
    if( infrared_mat.empty() ){
        return;
    }

	*InfraredWriter << infrared_mat;
    // Show Infrared Image
	cv::imshow("Infrared - " + friendly_name + " (" + serial_number + " )", infrared_mat);
	


}

void RealSense::record()
{

	//Record Infrared
	//*ColorWriter << color_mat;

	//Record Depth
//	*BinaryDepthWriter << binaryDepth_mat;

	//*ColorizedDepthWriter << colorizedDepth_mat;

	

	//Record Infrared
	//*InfraredWriter << infrared_mat;
}