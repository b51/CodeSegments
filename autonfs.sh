#########################################################################
#
#              Author: b51
#                Mail: b51live@gmail.com
#            FileName: autonfs.sh
#
#          Created On: Mon 05 Mar 2018 03:15:42 PM CST
#
#########################################################################

#!/bin/bash

sshfs -o allow_other username@ip_addr:folder ~/nfs
