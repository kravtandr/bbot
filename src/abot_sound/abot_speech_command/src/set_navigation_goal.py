#!/usr/bin/env python3
# coding: utf-8

import rospy
import pandas as pd
from std_msgs.msg import String

class SetNavigationGoal(object):
	def __init__(self):
		rospy.init_node('set_navigation_goal')
		rospy.on_shutdown(self.shutdown)
		rospy.Subscriber('/abot/tts/coordinates', String, self.grammarCallback)
		rospy.spin()

	def grammarCallback(self, text_msg):
		rospy.loginfo("Voice command node 'Set Navigation Goal': Start. The text message: %s", text_msg.data)

	@staticmethod
	def shutdown():
		rospy.loginfo("Navigation control node: Stop SetNavigationGoal")
		rospy.sleep(1)

if __name__ == "__main__":
	SetNavigationGoal()
