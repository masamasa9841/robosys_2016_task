#! /usr/bin/env python
import rospy
from std_msgs.msg import Header
from nav_msgs.msg import MapMetaData, OccupancyGrid
from geometry_msgs.msg import Pose, Point, Quaternion

class map(object):
    def __init__(self):
        rospy.init_node('map')
        self.pub = rospy.Publisher('map', OccupancyGrid, queue_size=10)
        self.header = Header()
        self.pose = Pose()
        self.point = Point()
        self.quaternion = Quaternion()
        self.info = MapMetaData()
        self.bar = OccupancyGrid()
        self.header.seq =0
    def run(self):
        while not rospy.is_shutdown():
            self.header.seq += 1
            self.header.stamp = rospy.Time.now()
            self.header.frame_id = "map"
            
            self.point.x = 0.0
            self.point.y = 0.0
            self.point.z = 0.0

            self.quaternion.x = 0.0
            self.quaternion.y = 0.0
            self.quaternion.z = 0.0
            self.quaternion.w = 1.0

            self.pose.position = self.point
            self.pose.orientation = self.quaternion

            self.info.map_load_time = rospy.Time.now()
            self.info.resolution = 0.05
            self.info.width = 100
            self.info.height = 100
            self.info.origin = self.pose

            self.bar.info = self.info
            self.a = [-1 for i in range (10000) ]
            self.data = self.a
            self.pub.publish(self.bar)

if __name__ == '__main__':
    h = map()
    h.run()
