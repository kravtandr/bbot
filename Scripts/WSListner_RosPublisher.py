#!/usr/bin/env python
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that listens to std_msgs/Strings published
## to the 'chatter' topic

import rospy
import json
import websocket
from turtlesim.msg import Pose
from std_msgs.msg import Float64
from types import SimpleNamespace

# ws = websocket.WebSocket()
# ws.connect("ws://localhost:8000/ws/robot/testroom/")




import websocket
import _thread
import time
import rel

def on_message(ws, message):
    print(message)
    cmd = json.loads(message, object_hook=lambda d: SimpleNamespace(**d))
    

    if cmd.data.command == "stop":
        stop()
    else:
        if cmd.data.params.dir == "forward":
            forward()
        elif cmd.data.params.dir == "backward":
            backward()
        elif cmd.data.params.dir == "left":
            left()
        elif cmd.data.params.dir == "right":
            right()
    rate = rospy.Rate(10)
    rate.sleep()

def forward():
    pubr = rospy.Publisher('/abot/right_wheel/pwm', Float64, queue_size=10)
    publ = rospy.Publisher('/abot/left_wheel/pwm', Float64, queue_size=10)
    pubr.publish(-5.0)
    publ.publish(-5.0)
def backward():
    pubr = rospy.Publisher('/abot/right_wheel/pwm', Float64, queue_size=10)
    publ = rospy.Publisher('/abot/left_wheel/pwm', Float64, queue_size=10)
    pubr.publish(2.0)
    publ.publish(2.0)
def left():
    pubr = rospy.Publisher('/abot/right_wheel/pwm', Float64, queue_size=10)
    publ = rospy.Publisher('/abot/left_wheel/pwm', Float64, queue_size=10)
    pubr.publish(2.0)
    publ.publish(-2.0)
def right():
    pubr = rospy.Publisher('/abot/right_wheel/pwm', Float64, queue_size=10)
    publ = rospy.Publisher('/abot/left_wheel/pwm', Float64, queue_size=10)
    pubr.publish(-2.0)
    publ.publish(2.0)
def stop():
    pubr = rospy.Publisher('/abot/right_wheel/pwm', Float64, queue_size=10)
    publ = rospy.Publisher('/abot/left_wheel/pwm', Float64, queue_size=10)
    pubr.publish(0.0)
    publ.publish(0.0)

def on_error(ws, error):
    print("WS ERROR")
    print(error)

def on_close(ws, close_status_code, close_msg):
    print("### closed ###")

def on_open(ws):
    print("Opened connection")

if __name__ == "__main__":
    print("START WS LISTNER")
    rospy.init_node('talker', anonymous=True)
    # websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://127.0.0.1:8080/ws/robot/testroom/",
                              on_open=on_open,
                              on_message=on_message,
                              on_error=on_error,
                              on_close=on_close)

    ws.run_forever(dispatcher=rel)  # Set dispatcher to automatic reconnection
    rel.signal(2, rel.abort)  # Keyboard Interrupt
    rel.dispatch()




# def callback(data):
#     global ws
#     rospy.loginfo(rospy.get_caller_id() + 'x: %s', data.x)
#     position = {
#         'x': data.x,
#         'y': data.y,
#         'theta': data.theta
#     }
#     message = json.dumps({ 'message' : position })
#     ws.send('%s' % message)


# def listener():

#     # In ROS, nodes are uniquely named. If two nodes with the same
#     # name are launched, the previous one is kicked off. The
#     # anonymous=True flag means that rospy will choose a unique
#     # name for our 'listener' node so that multiple listeners can
#     # run simultaneously.
#     print("listen")
#     rospy.init_node('robotDispatcher', anonymous=True)
#     rospy.Subscriber('turtle1/pose', Pose, callback)

#     # spin() simply keeps python from exiting until this node is stopped
#     rospy.spin()


# if __name__ == '__main__':
#     listener()
