#########################################################################
#
#              Author: b51
#                Mail: b51live@gmail.com
#            FileName: scan_dist_cal.py
#
#     Licensed under The MIT License [see LICENSE for details]
#
#########################################################################

#!/usr/bin/env python

import sys
import os
import math
import traceback
import time
import numpy

import roslib
import rospy

from std_msgs.msg import *
from sensor_msgs.msg import *

class LaserDistCal(object):

    def __init__(self):
        laser_sub = rospy.Subscriber("/scan", LaserScan, self.LaserCB);
        self.index = 0;
        self.inited = False;
        self.f = open("scan_zero_angle.txt", 'w');
        self.counter = 0;
        self.dist = [];

    def LaserCB(self, _laser_msg):
        if (not self.inited):
            self.angle_min = _laser_msg.angle_min;
            self.angle_increment = _laser_msg.angle_increment;
            print("angle min: %0.5f, angle increment: %0.5f" % (self.angle_min, self.angle_increment));
            for i, point_range in enumerate(_laser_msg.ranges):
                if (self.angle_min + self.angle_increment * i) == 0.0:
                    self.index = i
                    print("angle zero index: %d" % self.index);
                    self.inited = True;
                    break;
        angle = self.angle_min + self.angle_increment * self.index;
        dist = _laser_msg.ranges[self.index]
        if (dist == float("inf")):
            rospy.logwarn("Get inf data");
            return
        print >> self.f, "%0.2f(rad) %04d count distance: %0.5f" % (angle, self.counter+1, dist);
        self.dist.append(dist);

        if self.counter % 1000 == 999:
            dist_array = numpy.array(self.dist);
            dist_max = dist_array.max();
            dist_min = dist_array.min();
            dist_mean = dist_array.mean();
            dist_var = dist_array.var();
            print "%0.2f(rad) %04d count dist max: %0.5f, min:%0.5f, mean: %0.5f, var: %0.8f"\
                    % (angle, self.counter + 1, dist_max, dist_min, dist_mean, dist_var);
            print >> self.f, "---------------------------------------"
            print >> self.f, "%0.2f(rad) %04d count dist max: %0.5f, min:%0.5f, mean: %0.5f, var: %0.8f"\
                            % (angle, self.counter + 1, dist_max, dist_min, dist_mean, dist_var);
            print >> self.f, ""
            if self.counter > 9999:
                self.dist = [];
                self.counter = 0;

        self.counter += 1;

    def spin(self):
        while not rospy.is_shutdown():
            rospy.spin();
        self.f.close();

def main(argv = None):
    if argv == None:
        argv = sys.argv;
    rospy.init_node("dist_cal")
    dist_cal = LaserDistCal();
    dist_cal.spin()
    pass

if __name__ == "__main__":
    main();
