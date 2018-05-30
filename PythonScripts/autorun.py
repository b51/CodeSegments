# -*- coding: utf-8 -*-

import traceback
import json
import os
import sys
import errno
import urllib
import thread
import time

g_node_1_launch = "gnome-terminal -x bash -c 'source /home/user/catkin_ws/devel/setup.bash;roslaunch node1 node1.launch;exec bash;'"
g_node_2_launch = "gnome-terminal -x bash -c 'source /home/user/catkin_ws/devel/setup.bash;roslaunch node2 node2.launch;exec bash;'"

def log(text):
    print(text)
    sys.stdout.flush()

def startup():
    if  os.path.isdir('/home/user/.config/autostart') == False:
        os.system('mkdir ~/.config/autostart')
    os.chdir('/home/user/.config/autostart')
    if os.path.isfile('autorun.py.desktop') == False:
        f = open('autorun.py.desktop','w')
        f.write('[Desktop Entry]\nType=Application\nExec=/usr/bin/python /home/user/autorun.py\nHidden=false\n \
                 NoDisplay=false\nX-GNOME-Autostart-enabled=true\nName[en_US]=autorun\nName=autorun\nComment[en_US]=\nComment=')
        f.close()
import subprocess

def mypass():
    mypass = 'pass'
    return mypass

def run_sth():
    echo = subprocess.Popen(['echo',mypass()],
                        stdout=subprocess.PIPE,
                        )

    sudo = subprocess.Popen(['sudo','-S','sh','/home/user/sth.sh'],
                        stdin=echo.stdout,
                        stdout=subprocess.PIPE,
                        )

    end_of_pipe = sudo.stdout

    log("do sth")

def run():
    global g_node_launch

    os.environ['LD_LIBRARY_PATH']= '/usr/local/lib:/opt/ros/kinetic/lib:/usr/local/cuda-8.0/lib64:$LD_LIBRARY_PATH'
    os.environ['ROS_MASTER_URI']='http://localhost:11311'
    os.environ['ROS_HOSTNAME']='localhost'
    os.system("~/startvnc.sh");
    log("node 1 launch")
    os.system(g_node_1_launch)
    time.sleep(3)
    log("node 2 launch")
    os.system(g_node_2_launch)
    time.sleep(3);
    log("run sth")
    run_sth()

if __name__ == '__main__':
    startup()
    os.chdir('/home/user')
    run()
