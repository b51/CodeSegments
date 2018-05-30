#set -ex
############# functions #################
time_name="`date +%Y-%m-%d_%H:%M:%S`"
camera_log="/home/ubuntu/log/$time_name"_camera.log
alg_log="/home/ubuntu/log/$time_name"_alg.log
auto_log="/home/ubuntu/log/$time_name"_test.log


function is_exist() {
	#if [ ! -e "/dev/fov_front" ];
	if [ ! -e $1 ];
	then
		echo "$1 not exist! "
	else
		echo "$1 ok!"
	fi
}

function not_empty(){
	filesize=`ls -l $1 | awk '{ print $5 }'`
	if [ $filesize -gt 1000 ]
	then
		echo "$1 ok,filesize=$filesize"
	else
		echo "$1 error,filesize=$filesize"
	fi
}

function switch_mode(){
	rosservice call /work_mode_switch "roi: {x_offset: 0, y_offset: 0, height: 0, width: 0, do_rectify: false}
work_mode: $1"
	if [[ $? -eq 0 ]]
	then
		echo "switch mode to $1 ok"
	else
		echo "switch mode to $1 error "
	fi
}

function kface() {
    cnt=3
    while [ $cnt -ge 1 ]
    do
        killall rostopic 2>/dev/null
        killall FaceDetectNode 2>/dev/null
        killall roslaunch 2>/dev/null
        sleep 0.5
        cnt=`ps -ef|grep  -E "rostopic|roslaunch|FaceDetectNode"|grep -v grep|wc -l`
    done
}
#########################################

############# check usb #################
echo -e "\033[31m checking usb \033[0m"
is_exist "/dev/fov_front"
is_exist "/dev/fov_back"
#########################################

############# check camera ##############

echo -e "\033[31m checking camera \033[0m"
source ~/RobotCoreCtl/devel/setup.bash
#roslaunch face_detect multi_astra.launch > $camera_log &
roslaunch face_detect astra.launch > $camera_log &
sleep 10
echo -e "\033[31m checking camera \033[0m"
cat $camera_log |grep "Failed to set USB interface!" >> /dev/null
if [[ $? -eq 0 ]]
then
	echo "astra camera open error" >> $auto_log 2>&1
else
	echo "astra camera open ok" >> $auto_log 2>&1
fi
:<<!
filename=/tmp/front.depth
rostopic echo /camera_front/depth/image_raw > $filename&
sleep 2
not_empty $filename
killall rostopic
rm $filename
filename=/tmp/back.depth
rostopic echo /camera_front/depth/image_raw > $filename &
sleep 2
not_empty $filename
killall rostopic
rm $filename
sleep 1
filename=/tmp/front.mjpeg
rostopic echo /camera_front/rgb/image_raw > $filename &
sleep 5
sync
not_empty $filename
killall rostopic
rm $filename
sleep 1
filename=/tmp/back.mjpeg
rostopic echo /camera_back/rgb/image_raw > $filename &
sleep 5
sync
not_empty $filename
killall rostopic
rm $filename
!
#########################################

############# check alg #################
echo -e "\033[31m checking alg \033[0m"
filename=/tmp/faces_front
#roslaunch face_detect face_detect_node.launch > $alg_log &
roslaunch face_detect face_detect_node.launch &
sleep 8
rostopic echo /faces > $filename &
sleep 5
not_empty $filename
killall rostopic
rm $filename


#########################################

############# check service #############
echo -e "\033[31m checking service \033[0m"
while(true)
do
	rosservice call /camera_switch  "camera: 'camera_back'"
	sleep 1
	filename=/tmp/faces_back
	rostopic echo /faces > $filename &
	sleep 5
	not_empty $filename
	cat $filename |grep "test" >> /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "switch to back ok"
	else
		echo "switch to back error / please check the picture on the back , or is it registed to name test ?"
	fi
	killall rostopic
	rm $filename
	sync
	switch_mode 0
	sleep 1
	switch_mode 1
	sleep 1
	switch_mode 2

	rosservice call /camera_switch  "camera: 'camera_front'"
	sleep 1
	filename=/tmp/faces_front
	rostopic echo /faces > $filename &
	sleep 5
	not_empty $filename
	cat $filename |grep "test" >> /dev/null
	if [[ $? -eq 0 ]]
	then
		echo "switch to front error / please check the picture on the back , or is it registed to name test ?"
	else
		echo "switch to front ok"
	fi
	killall rostopic
	rm $filename

	switch_mode 0
	sleep 1
	switch_mode 1
	sleep 1
	switch_mode 2
done
#########################################

kface()
