# !/bin/sh
# kill bin in linux
ps -ef | grep "EXECUTE_NAME" | awk '{print $2}' | xargs kill -9
