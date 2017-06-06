#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include <string>
#include <sstream>
#include <iostream>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/cloud_viewer.h>

using namespace std;
using namespace pcl;

	pcl::VoxelGrid<pcl::PointXYZ> voxel_grid;
	pcl::PassThrough<pcl::PointXYZ> pass_through;
	pcl::visualization::CloudViewer cloud_viewer("Cloud Viewer");

void functionCB(const sensor_msgs::PointCloud2::ConstPtr& msg){	

	PointCloud<pcl::PointXYZ>::Ptr cloud_ptr = PointCloud<pcl::PointXYZ>::Ptr(new PointCloud<PointXYZ> ());

	pcl::fromROSMsg(*msg, *cloud_ptr);
	
	pass_through.setInputCloud(cloud_ptr);
	pass_through.setFilterLimits(0.5, 10);
	pass_through.setFilterFieldName("z");
	pass_through.filter(*cloud_ptr);

	voxel_grid.setInputCloud(cloud_ptr);
	voxel_grid.setLeafSize(0.1, 0.1, 0.1);
	voxel_grid.filter(*cloud_ptr);

	cloud_viewer.showCloud(cloud_ptr);

}

int main(int argc, char **argv) {
	
	ros::init(argc, argv, "pclnode");
	ros::NodeHandle n;  
	
	ros::Publisher pub = n.advertise<std_msgs::String>("postopic", 100);
	ros::Subscriber sub = n.subscribe<sensor_msgs::PointCloud2>("camera/depth/points", 1, functionCB);

	ros::spin();
	return 0;
}
