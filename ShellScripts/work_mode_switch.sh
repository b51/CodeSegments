#########################################################################
#
#              Author: b51
#                Mail: b51live@gmail.com
#            FileName: work_mode_switch.sh
#
#          Created On: Wed 07 Mar 2018 12:19:29 PM CST
#
#########################################################################

#!/bin/bash

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

while(true)
do
  switch_mode 0
  sleep 2
  switch_mode 1
  sleep 2
  switch_mode 2
  sleep 2
done
